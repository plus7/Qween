#ifndef SIMPLEURISHORTENER_H
#define SIMPLEURISHORTENER_H

#include <QObject>
#include "abstracturishortener.h"
#include <QtNetwork>

class SimpleUriShortener : public AbstractUriShortener
{
Q_OBJECT
public:
    explicit SimpleUriShortener(QObject *parent = 0);

    virtual QString serviceName() = 0;
    virtual QString shortenUri() = 0;
    virtual QString shortenUriParam() = 0;
    virtual int thresholdLength() = 0;
    virtual QString prefix() = 0;
    virtual QString postData(const QString& uri) { return ""; }
    virtual void shortenAsync(const QString& uri);
    virtual void resolveAsync(const QString& uri);

signals:
private:
    QMap<QString, QNetworkReply*> m_buffer;
    QNetworkAccessManager* m_manager;
private slots:
    void replyFinished(QNetworkReply*);
};

#endif // SIMPLEURISHORTENER_H
