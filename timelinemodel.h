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

#ifndef TIMELINEMODEL_H
#define TIMELINEMODEL_H

#include "twitter.h"
#include <QtGui>
class IconManager;
class TimelineModel : public QAbstractItemModel
{
Q_OBJECT
public:
    TimelineModel(IconManager *iconMgr, QObject *parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QModelIndex parent(const QModelIndex &child) const;
    bool hasChildren ( const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void appendItem(Twitter::TwitterItem item);//, bool ignoreId = false);
    //void insertItem(int index, Twitter::TwitterItem item, bool ignoreId = false);
    Twitter::TwitterItem removeItem(int index);
    Twitter::TwitterItem removeItemById(quint64 id);
    Twitter::TwitterItem itemAt(int index) const;
    void setFav(int index, bool fav=true);
    void setRead(int index, bool read=true);
    void setMyId(quint64 id);//{ m_myId = id; }
    quint64 myId() const { return m_myId; }
    void setNewestId(quint64 id) { m_newestId = id; }
    quint64 newestId() const { return m_newestId; }
    void setBaseIndex(int i){
        m_baseIndex = i;
        /*Twitter::TwitterItem *baseItem = m_itemList.at(m_baseIndex);
        for(int j=0;j<m_itemList.count();j++){
            Twitter::TwitterItem *item = m_itemList.at(j);
            if(item->userId() == baseItem->userId()){
                QModelIndex idx = index(j,0,QModelIndex());
                emit dataChanged(idx, idx);
            }
        }*/
    }
    int baseIndex() const { return m_baseIndex; }
//    void setData(int index, Returnables::StatusElement *newData);

signals:

public slots:
    void OnIconDownloaded(quint64 userid, const QIcon& icon);

private:
    IconManager *m_iconMgr;
    int m_baseIndex;
    quint64 m_myId;
    quint64 m_newestId;
    QList<Twitter::TwitterItem*> m_itemList;
};

#endif // TIMELINEMODEL_H
