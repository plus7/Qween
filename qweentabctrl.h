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

  In addition, as a special exception, NOSE Takafumi
  gives permission to link the code of its release of Qween with the
  OpenSSL project's "OpenSSL" library (or with modified versions of it
  that use the same license as the "OpenSSL" library), and distribute
  the linked executables.  You must obey the GNU General Public License
  in all respects for all of the code used other than "OpenSSL".  If you
  modify this file, you may extend this exception to your version of the
  file, but you are not obligated to do so.  If you do not wish to do
  so, delete this exception statement from your version.
*/

#ifndef QWEENTABCTRL_H
#define QWEENTABCTRL_H

#include <QTabWidget>
#include "twitter.h"
class TabInfo;
class TimelineView;
class QweenSettings;
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
    void setManageUnread(bool val);
    bool manageUnread() const { return m_manageUnread; }
    TimelineView* homeView(){ return m_homeView; }
    TimelineView* replyView(){ return m_replyView; }
    TimelineView* dmView(){ return m_dmView; }
    TimelineView* favView(){ return m_favView; }

signals:
    void itemSelected(const Twitter::TwitterItem& item);
    void tabSelected(const QString& tabName, int tabType);
    void favorite();
    void reply();
    void dm();

public slots:
    void OnItemSelected(const Twitter::TwitterItem& item);
    void favorited(quint64 id, bool faved);
    void OnUnreadCountChanged(int count);
    void OnFavorite();
    void OnReply();
private:
    void makeConnection(TimelineView *view);
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
    bool m_manageUnread;
    QweenSettings *settings;
};

#endif // QWEENTABCTRL_H
