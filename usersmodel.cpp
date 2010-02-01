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

#include "usersmodel.h"
#include "iconmanager.h"
#include "twitter.h"
#include "QTwitLib.h"
UsersModel::UsersModel(IconManager *iconMgr, QObject *parent) :
    QAbstractItemModel(parent), m_iconMgr(iconMgr)
{
    connect(m_iconMgr, SIGNAL(iconDownloaded(quint64,QIcon)),
            this, SLOT(OnIconDownloaded(quint64,QIcon)));
}

int UsersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_itemList.size();
}

int UsersModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant UsersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_itemList.size() || index.row() < 0)
        return QVariant();

    Twitter::TwitterItem *item = m_itemList.at(index.row());
    if (role == Qt::DisplayRole || role == Qt::EditRole) {

        switch(index.column()){
        case 0:
            return item->screenName();
            break;
        default:
            return QVariant();
        }
    }else if(role == Qt::DecorationRole){
        switch(index.column()){
        case 0:
            if(m_iconMgr->isIconAvailable(item->userId())){
                return m_iconMgr->getIcon(item->userId());
            }else{
                m_iconMgr->fetchIcon(item->userId(), item->iconUri());
                return QIcon();
            }
        default:
            return QIcon();
        }
    }
    return QVariant();
}

QVariant UsersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("ID");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

void UsersModel::appendItem(Twitter::TwitterItem item)//, bool ignoreId)
{
    int index;
    index = m_itemList.count();
    beginInsertRows(QModelIndex(), index, index);
    Twitter::TwitterItem *internalItem = new Twitter::TwitterItem(item);
    //internalItem->setParent(this);
    m_itemList.append(internalItem);
    endInsertRows();
}

Twitter::TwitterItem UsersModel::removeItem(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    Twitter::TwitterItem *p = m_itemList.takeAt(index);
    Twitter::TwitterItem rv(*p);
    rv.setParent(NULL);
    delete p;
    endRemoveRows();
    return rv;
}

Twitter::TwitterItem UsersModel::itemAt(int index)
{
    Twitter::TwitterItem *p = m_itemList.at(index);
    //Twitter::TwitterItem rv(*p);
    return *p;
}

bool UsersModel::userExists(quint64 id){
    Twitter::TwitterItem *item;
    foreach(item, m_itemList){
        if(item->userId() == id){
            return true;
        }
    }
    return false;
}

bool UsersModel::userExists(const QString& screenName){
    Twitter::TwitterItem *item;
    foreach(item, m_itemList){
        if(item->screenName() == screenName){
            return true;
        }
    }
    return false;
}

bool UsersModel::hasChildren(const QModelIndex &parent) const
{
    if(parent.isValid())
        return false;
    else
        return true;
}

QModelIndex UsersModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid())
        return createIndex(row, column, m_itemList.at(row));
    else
        return QModelIndex(); //TLモデルでは子供はアイテムを持たないから
}

QModelIndex UsersModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

Qt::ItemFlags UsersModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index);// | Qt::ItemIsEditable;
}

void UsersModel::OnIconDownloaded(quint64 userid, const QIcon &icon)
{
    for(int i=0;i<m_itemList.count();i++){
        Twitter::TwitterItem *item = m_itemList.at(i);
        if(item->userId() == userid){
            QModelIndex idx = index(i,0,QModelIndex());
            emit dataChanged(idx, idx);
        }
    }
}
