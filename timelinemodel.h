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

    void appendItem(Twitter::TwitterItem item);
    void insertItem(int index, Twitter::TwitterItem item);
    Twitter::TwitterItem removeItem(int index);
    Twitter::TwitterItem itemAt(int index);
    void setRead(int index, bool read=true);
    void setUserId(quint64 id);
    quint64 userId(){ return m_userId; }
    void setBaseIndex(int i){ m_baseIndex = i; }
    int baseIndex(){ return m_baseIndex; }
//    void setData(int index, Returnables::StatusElement *newData);

signals:

public slots:
    void OnIconDownloaded(quint64 userid, const QIcon& icon);

private:
    IconManager *m_iconMgr;
    int m_baseIndex;
    quint64 m_userId;
    QList<Twitter::TwitterItem*> m_itemList;
};

#endif // TIMELINEMODEL_H
