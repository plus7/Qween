#ifndef QWEENTABCTRL_H
#define QWEENTABCTRL_H

#include <QTabWidget>
#include "twitter.h"
#include "QTwitLib.h"
class TabInfo;
class QweenTabCtrl : public QTabWidget
{
Q_OBJECT
public:
    explicit QweenTabCtrl(QWidget *parent = 0);
    void initWithTabInfo(TabInfo *info);
    quint64 getNewestTimelineId()const { return m_newestTLID; }
    quint64 myId() const { return m_myID; }
    void setMyId(quint64 myid);
//    void addItems(QList<Twitter::TwitterItem> items);
    void addItem(Twitter::TwitterItem item);
    Twitter::TwitterItem currentItem();

signals:
    void itemSelected(const Twitter::TwitterItem& item);
    void tabSelected(const QString& tabName, int tabType);

public slots:
    void OnItemSelected(const Twitter::TwitterItem& item);

private:
    quint64 m_newestTLID;
    quint64 m_newestDMID;
    quint64 m_newestReplyID;
    quint64 m_myID;
    Twitter::TwitterItem m_curItem;
};

#endif // QWEENTABCTRL_H
