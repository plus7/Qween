#ifndef BITLY_H
#define BITLY_H

#include <QObject>
#include "abstracturishortener.h"
#include <QtNetwork>

class Bitly : public AbstractUriShortener
{
Q_OBJECT
public:
    explicit Bitly(QObject *parent = 0);

    QString serviceName() { return "bitly"; }
    void shortenAsync(const QString& uri);
    void resolveAsync(const QString& uri);

signals:
private:
    QMap<QString, QNetworkReply*> m_buffer;
    QNetworkAccessManager* m_manager;
    QString m_id;
    QString m_key;
private slots:
    void replyFinished(QNetworkReply*);
};

#endif // BITLY_H
