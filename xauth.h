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
                         QUrl url);

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
