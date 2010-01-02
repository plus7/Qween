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

#include "iconmanager.h"
#include "qweenapplication.h"
#include <QtCore>

//TODO: ‚à‚Á‚ÆŒ«‚¢ƒLƒƒƒbƒVƒ…
IconManager::IconManager(QObject *parent) :
    QObject(parent), m_iconCacheMaxCount(50)
{
    m_manager = new QNetworkAccessManager( this );
    connect(m_manager,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

void IconManager::addCache(quint64 userid, const QIcon& icon){
    if(m_iconCache.count() > m_iconCacheMaxCount){
        m_iconCache.erase(m_iconCache.begin());
    }
    m_iconCache.insert(userid, icon);
}

QString IconManager::getIconFilePath(quint64 userid){
    QDir icondir(QweenApplication::profileDir() + "/icons");
    QString fileName = QString("%1").arg(userid,0,16);
    return icondir.path() + "/" + fileName;
}

bool IconManager::isIconAvailable(quint64 userid)
{
    if(m_iconCache.find(userid) != m_iconCache.end()){
        return true;
    }
    else return QFile(getIconFilePath(userid)).exists();
}

QIcon IconManager::getIcon(quint64 userid)
{
    if(m_iconCache.find(userid) != m_iconCache.end())
        return m_iconCache.value(userid, QIcon());
    else if(isIconAvailable(userid)){
        QIcon icon(getIconFilePath(userid));
        addCache(userid, icon);
        return icon;
    }
    else
        return QIcon();
}

void IconManager::fetchIcon(quint64 userid, const QString& uri)
{
    if(m_connections.contains(userid)) return;
    QNetworkReply *rep = m_manager->get(QNetworkRequest(uri));
    m_connections.insert(userid, rep);
}

void IconManager::replyFinished(QNetworkReply *reply){
    quint64 userid = m_connections.key(reply);
    if(userid == 0) {
        return;
    }
    m_connections.remove(userid);
    if(reply->error() != QNetworkReply::NoError){
        reply->deleteLater();
        return;
    }
    QFile iconFile(getIconFilePath(userid));
    iconFile.open(QFile::WriteOnly);
    iconFile.write(reply->readAll());
    iconFile.close();
    emit iconDownloaded(userid, getIcon(userid));
    reply->deleteLater();
}
