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
