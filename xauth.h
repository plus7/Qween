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

#ifndef XAUTH_H
#define XAUTH_H

#include <QtNetwork>

#define XAUTH_CONSUMER_KEY "kDPLdq36x3kXIDWmRdPqKg"
#define XAUTH_CONSUMER_SECRET "UNxXNOrLVRU8JlNDrSx2msECr9cOqKD8YEfJPdWfVs"
#define XAUTH_ACCESS_URL "https://api.twitter.com/oauth/access_token"

class XAuth : public QObject
{
Q_OBJECT
public:
    explicit XAuth(const QString& consumer_key,
                   const QString& consumer_secret,
                   QObject *parent = 0);
    virtual ~XAuth();
    void getAccessToken(const QString& userid, const QString& pass);
    QString getSignature(const QString& method,
                         const QString& url,
                         QList<QPair<QString, QString> > queryItems);

    QString consumerKey() const { return m_consumer_key; }
    void setConsumerKey(const QString& key){ m_consumer_key = key; }
    QString consumerSecret() const { return m_consumer_secret; }
    void setConsumerSecret(const QString& secret){ m_consumer_secret = secret; }
    QString tokenSecret() const { return m_token_secret; }
    void setTokenSecret(const QString& secret){ m_token_secret = secret; }
    QString token() const { return m_token_key; }
    void setToken(const QString& key){ m_token_key = key; }

signals:
    void accessTokenReceived();
private slots:
    void OnFinished(QNetworkReply* r);
    void OnSslError(QNetworkReply* r);
private:
    QString m_consumer_key;
    QString m_consumer_secret;
    QString m_token_key;
    QString m_token_secret;
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
};

#endif // XAUTH_H
