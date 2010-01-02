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
