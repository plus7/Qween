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

#ifndef USERSMODEL_H
#define USERSMODEL_H

#include <QAbstractItemModel>
#include "twitter.h"
class IconManager;
class UsersModel : public QAbstractItemModel
{
Q_OBJECT
public:
    explicit UsersModel(IconManager *iconMgr, QObject *parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QModelIndex parent(const QModelIndex &child) const;
    bool hasChildren ( const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void appendItem(Twitter::TwitterItem item);
    Twitter::TwitterItem removeItem(int index);
    Twitter::TwitterItem removeItemById(quint64 id);
    Twitter::TwitterItem itemAt(int index);
    bool userExists(quint64 id);
    bool userExists(const QString& screenName);
signals:

public slots:
    void OnIconDownloaded(quint64 userid, const QIcon& icon);

private:
    IconManager *m_iconMgr;
    QList<Twitter::TwitterItem*> m_itemList;
};

#endif // USERSMODEL_H
