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
