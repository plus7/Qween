#ifndef QWEENTABCTRL_H
#define QWEENTABCTRL_H

#include <QTabWidget>
#include "twitter.h"
#include "QTwitLib.h"
class TabInfo;
class TimelineView;
class QweenTabCtrl : public QTabWidget
{
Q_OBJECT
public:
    explicit QweenTabCtrl(QWidget *parent = 0);
    quint64 getNewestHomeId() const;
    quint64 getNewestDMId() const;
    quint64 getNewestReplyId() const;
    quint64 getNewestFavId() const;
    quint64 myId() const { return m_myID; }
    void setMyId(quint64 myid);
    void addItem(Twitter::TwitterItem item);
    Twitter::TwitterItem currentItem();
    TimelineView *currentTimelineView();
    TimelineView *timelineView(int index);
    TimelineView *addTimelineView(const QString& title);
    TimelineView *insertTimelineView(int index,const QString& title);
    void removeTimelineView(int index);
    void moveTimelineView(int before, int after);
    void saveState(QIODevice*);
    void restoreState(QIODevice*);
    void fixLackingTabs();

signals:
    void itemSelected(const Twitter::TwitterItem& item);
    void tabSelected(const QString& tabName, int tabType);

public slots:
    void OnItemSelected(const Twitter::TwitterItem& item);

private:
    TimelineView* m_homeView;
    TimelineView* m_replyView;
    TimelineView* m_dmView;
    TimelineView* m_favView;
    quint64 m_newestHomeID;
    quint64 m_newestDMID;
    quint64 m_newestReplyID;
    quint64 m_newestFavID;
    quint64 m_myID;
    Twitter::TwitterItem m_curItem;
};

#endif // QWEENTABCTRL_H
