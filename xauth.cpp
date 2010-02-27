/*
  This file is part of Qween.
  Copyright (C) 2009-2010 NOSE Takafumi <ahya365@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, NOSE Takafumi
  gives permission to link the code of its release of Qween with the
  OpenSSL project's "OpenSSL" library (or with modified versions of it
  that use the same license as the "OpenSSL" library), and distribute
  the linked executables.  You must obey the GNU General Public License
  in all respects for all of the code used other than "OpenSSL".  If you
  modify this file, you may extend this exception to your version of the
  file, but you are not obligated to do so.  If you do not wish to do
  so, delete this exception statement from your version.
*/
#include "xauth.h"
#include "const.h"
#include "util.h"
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <QtGui>
#include <QtNetwork>
//TODO: 多分、Qweenの中で一番汚い場所…。
//TODO: 変数名がunkなので、あとでOAuthの仕様書を読んで適切な物にする

XAuth::XAuth(const QString& consumer_key,
             const QString& consumer_secret,
             QObject *parent) :
    QObject(parent),m_consumer_key(consumer_key),m_consumer_secret(consumer_secret),m_reply(NULL)
{
    QDateTime now = QDateTime::currentDateTime();
    qsrand(now.toTime_t());
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(OnFinished(QNetworkReply*)));
}

XAuth::~XAuth(){
    if(m_reply) m_reply->abort();
}

void XAuth::getAccessToken(const QString& userid, const QString& pass){
    //timestamp
    QDateTime now = QDateTime::currentDateTime();
    QString timestamp(QString::number(now.toTime_t(),10));
    //nonce
    QString nonce;
    for(int i=0;i<8;i++){
        nonce.append(QString::number(qrand() % 0xFF,10));
    }
    //NAZO1
    QByteArray key(QString(m_consumer_secret + "&").toAscii());
    //NAZO2
    QString encodedAccessUrl = QUrl::toPercentEncoding(XAUTH_ACCESS_URL);
    //NAZO3
    QString NAZO3 = QString("oauth_consumer_key=%0&"
                            "oauth_nonce=%1&"
                            "oauth_signature_method=HMAC-SHA1&"
                            "oauth_timestamp=%2&"
                            "oauth_version=1.0&"
                            "x_auth_mode=client_auth&"
                            "x_auth_password=%3&"
                            "x_auth_username=%4")
                    .arg(m_consumer_key, nonce, timestamp, pass, userid);
    //NAZO4
    QByteArray data = QString("POST&"+
                              encodedAccessUrl+"&"+
                              QUrl::toPercentEncoding(NAZO3)).toAscii();

    //========== digest ==========
    unsigned int  reslen;
    unsigned char    res[SHA_DIGEST_LENGTH + 1];

    if(!HMAC (EVP_sha1(),  (unsigned char*)key.data(),    key.length(),
              (unsigned char*)data.data(), data.length(), res, &reslen) )
      return;

    QByteArray digest((char *)res, reslen);

    QString params = NAZO3+"&oauth_signature="+QUrl::toPercentEncoding(digest.toBase64());
    QNetworkRequest req(QUrl(XAUTH_ACCESS_URL));
    req.setAttribute(QNetworkRequest::User,1);
    m_reply = m_manager->post(req, params.toAscii());
}

void XAuth::OnFinished(QNetworkReply* r){
    QByteArray arr(r->readAll());
    m_reply = 0;
    if(r->request().attribute(QNetworkRequest::User) != 1) {
        r->deleteLater();
        return;
    }

    QUrl dummy("http://example.com/");
    dummy.setEncodedQuery(arr);//たぶん安全なencodedQueryの使い方
    m_token_key = dummy.queryItemValue("oauth_token");
    m_token_secret = dummy.queryItemValue("oauth_token_secret");
    emit accessTokenReceived();
    //testPost();
    r->deleteLater();
}

void XAuth::OnSslError(QNetworkReply* r){

}

bool LessThan(const QPair<QString,QString> &p1, const QPair<QString,QString> &p2)
{
    return p1.first < p2.first;
}

QString XAuth::getSignature(const QString& method,
                            const QString& url,
                            QList<QPair<QString, QString> > queryItems)
{
    QList<QPair<QString,QString> > xauthItems;
    QDateTime now = QDateTime::currentDateTime();

    QString nonce;
    for(int i=0;i<8;i++){
        nonce.append(QString::number(qrand() % 0xFF,10));
    }

    xauthItems << QPair<QString,QString>(QString("oauth_consumer_key"),m_consumer_key);
    xauthItems << QPair<QString,QString>(QString("oauth_signature_method"),QString("HMAC-SHA1"));
    xauthItems << QPair<QString,QString>(QString("oauth_timestamp"),QString::number(now.toTime_t(),10));
    xauthItems << QPair<QString,QString>(QString("oauth_nonce"),nonce);
    xauthItems << QPair<QString,QString>(QString("oauth_version"),QString("1.0"));
    xauthItems << QPair<QString,QString>(QString("oauth_token"),m_token_key);
    QList<QPair<QString,QString> > mergedList(xauthItems);
    mergedList.append(queryItems);

    //sort mergedList
    qSort(mergedList.begin(), mergedList.end(), LessThan);

    QByteArray key(QString(m_consumer_secret + "&" + m_token_secret).toAscii());

    QByteArray params(util::encodeQuery(mergedList).toAscii());

    qDebug() << "PARAMS";
    qDebug() << params;

    QStringList dataList;
    dataList << method << QUrl::toPercentEncoding(url) << QUrl::toPercentEncoding(params);

    QByteArray data(dataList.join("&").toAscii());

    //========== digest ==========
    unsigned int  reslen;
    unsigned char    res[SHA_DIGEST_LENGTH + 1];
    qDebug() << "key and data";
    qDebug() << key;
    qDebug() << data;
    if(!HMAC (EVP_sha1(),  (unsigned char*)key.data(),    key.length(),
              (unsigned char*)data.data(), data.length(), res, &reslen) )
      return "";
    QByteArray digest = QByteArray::fromRawData((char *)res, reslen);

    xauthItems.append(QPair<QString,QString>(QString("oauth_signature"), QString(digest.toBase64())));

    QStringList sigList;
    for(int i=0;i<xauthItems.count();i++){
        sigList << QString("%0=\"%1\"").arg(xauthItems.at(i).first, QString(QUrl::toPercentEncoding(xauthItems.at(i).second)));
    }
    return sigList.join(", ");
}
