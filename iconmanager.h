#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QObject>
#include <QtGui>
#include <QtNetwork>

class IconManager : public QObject
{
Q_OBJECT
public:
    explicit IconManager(QObject *parent = 0);
    void addCache(quint64 userid, const QIcon& icon);
    bool isIconAvailable(quint64 userid);
    QIcon getIcon(quint64 id);
    void fetchIcon(quint64 userid, const QString& uri);
    //QFile getIconFile(quint64 userid);
    QString getIconFilePath(quint64 userid);

signals:
    void iconDownloaded(quint64 userid, const QIcon& icon);

public slots:


private:
    QNetworkAccessManager* m_manager;
    QMap<quint64, QNetworkReply*> m_connections;
    QMap<quint64, QIcon> m_iconCache;
    int m_iconCacheMaxCount;
private slots:
    void replyFinished(QNetworkReply*);
};

#endif // ICONMANAGER_H
