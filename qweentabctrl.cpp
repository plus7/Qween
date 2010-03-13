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

#include <QTabWidget>
#include <QListWidget>
#include <QTableView>
#include <QHeaderView>
#include "qweentabctrl.h"
#include "qweenapplication.h"
#include "qweensettings.h"
#include "tabinfo.h"
#include "timelinemodel.h"
#include "timelineview.h"
#include "timelineitemdelegate.h"

//XXX: だんだん汚くなってきた…

QweenTabCtrl::QweenTabCtrl(QWidget *parent) :
    QTabWidget(parent),m_homeView(NULL),m_replyView(NULL),
    m_dmView(NULL),m_favView(NULL),settings(QweenSettings::globalSettings())
{
    setMovable(true);
}

void QweenTabCtrl::saveState(QIODevice* device)
{
    const int IndentSize = 2;
    QTextStream out(device);
    QDomDocument doc;
    QDomElement root = doc.createElement("tabs");
    doc.appendChild(root);
    for(int i=0;i<this->count();i++){
        TimelineView *view = (TimelineView*)(this->widget(i));
        QDomElement elm = view->saveToElement(doc);
        root.appendChild(elm);
    }
    doc.save(out, IndentSize);
}

void QweenTabCtrl::restoreState(QIODevice* device){
    if(!device){
        fixLackingTabs();
        return;
    }
    QDomDocument doc;
    QString errorStr;
    int errorLine, errorColumn;
    if (!doc.setContent(device, true, &errorStr, &errorLine,
                                &errorColumn)) {
        qDebug() << tr("Parse error at line %1, column %2: %3").arg(errorLine).arg(errorColumn).arg(errorStr);
        fixLackingTabs();
        return;
    }
    QDomElement root = doc.documentElement();
    if (root.tagName() != "tabs") {
        qDebug() << tr("invalid root element");
        fixLackingTabs();
        return;
    }
    QDomElement tabelm = root.firstChildElement("tab");
    while (!tabelm.isNull()) {
        //XXX: 振り分け設定とかのデータをViewが持ってるのが気持ち悪い
        QString type = tabelm.attribute("type", "userdefined");
        QString title = tabelm.attribute("title", "Untitled");
        TimelineView* view = new TimelineView(this);
        if(type == "home"){
            if(!m_homeView) m_homeView = view;
            else type = "userdefined";
        }else if(type == "reply"){
            if(!m_replyView) m_replyView = view;
            else type = "userdefined";
        }else if(type == "dm"){
            if(!m_dmView) m_dmView = view;
            else type = "userdefined";
        }else if(type == "fav"){
            if(!m_favView) m_favView = view;
            else type = "userdefined";
        }
        view->setType(type);
        view->setTitle(title);
        view->restoreFromElement(tabelm);
        view->setItemDelegate(new TimelineItemDelegate(QweenApplication::iconManager()));
        view->setModel(new TimelineModel(QweenApplication::iconManager(),this));
        makeConnection(view);
        this->addTab(view, QIcon(), view->title());
        tabelm = tabelm.nextSiblingElement("tab");
    }
    fixLackingTabs();
}


void QweenTabCtrl::fixLackingTabs(){
    TimelineView* view;
    //XXX: コピペ撲滅
    if(!m_homeView){
        view = new TimelineView(this);
        view->setType("home");
        view->setTitle("Home");
        view->setItemDelegate(new TimelineItemDelegate(QweenApplication::iconManager()));
        view->setModel(new TimelineModel(QweenApplication::iconManager(),this));
        view->header()->resizeSection(1, 400);
        makeConnection(view);
        this->addTab(view,QIcon(),view->title());
        m_homeView = view;
    }

    if(!m_replyView){
        view = new TimelineView(this);
        view->setType("reply");
        view->setTitle("Reply");
        view->setItemDelegate(new TimelineItemDelegate(QweenApplication::iconManager()));
        view->setModel(new TimelineModel(QweenApplication::iconManager(),this));
        view->header()->resizeSection(1, 400);
        makeConnection(view);
        this->addTab(view,QIcon(),view->title());
        m_replyView = view;
    }

    if(!m_dmView){
        view = new TimelineView(this);
        view->setType("dm");
        view->setTitle("DM");
        view->setItemDelegate(new TimelineItemDelegate(QweenApplication::iconManager()));
        view->setModel(new TimelineModel(QweenApplication::iconManager(),this));
        view->header()->resizeSection(1, 400);
        makeConnection(view);
        this->addTab(view,QIcon(),view->title());
        m_dmView = view;
    }

    if(!m_favView){
        view = new TimelineView(this);
        view->setType("fav");
        view->setTitle("Favorites");
        view->setItemDelegate(new TimelineItemDelegate(QweenApplication::iconManager()));
        view->setModel(new TimelineModel(QweenApplication::iconManager(),this));
        view->header()->resizeSection(1, 400);
        makeConnection(view);
        this->addTab(view,QIcon(),view->title());
        m_favView = view;
    }
}

void QweenTabCtrl::addItem(Twitter::TwitterItem item){
    if(settings->setReadMyPost() &&
       item.screenName() == settings->userid()) item.setRead(true);
    switch(item.origin()){
    case HOME_TIMELINE:
        if(m_homeView) {
            m_homeView->model()->appendItem(item);
        }
        break;
    case MENTIONS:
        if(m_replyView) m_replyView->model()->appendItem(item);
        break;
    case DIRECT_MESSAGES:
        if(m_dmView) m_dmView->model()->appendItem(item);
        break;
    case SENT_DIRECT_MESSAGES:
        if(m_dmView) m_dmView->model()->appendItem(item);
        break;
    case FAVORITES:
        if(m_favView) m_favView->model()->appendItem(item);
        break;
    case UPDATE:
        if(m_homeView) m_homeView->model()->appendItem(item);
        break;
    default:
        break;
    }
}

Twitter::TwitterItem QweenTabCtrl::currentItem(){
    return m_curItem;
}

TimelineView *QweenTabCtrl::currentTimelineView(){
    return (TimelineView*)this->currentWidget();
}

TimelineView *QweenTabCtrl::timelineView(int index){
    return (TimelineView*)this->widget(index);
}

TimelineView *QweenTabCtrl::addTimelineView(const QString &title){
    return insertTimelineView(count(), title);
}

TimelineView *QweenTabCtrl::insertTimelineView(int index, const QString& title){
    TimelineView* view = new TimelineView(this);
    view->setType("userdefined");
    view->setTitle(title);
    view->setItemDelegate(new TimelineItemDelegate(QweenApplication::iconManager()));
    view->setModel(new TimelineModel(QweenApplication::iconManager(),this));
    view->header()->resizeSection(1, 400);
    makeConnection(view);
    this->insertTab(index, view, QIcon(), view->title());
    return view;
}

void QweenTabCtrl::removeTimelineView(int index){
    TimelineView *view = timelineView(index);
    if(view == m_homeView || view == m_dmView || view == m_favView || view || m_replyView) return;
    delete view;
}

void QweenTabCtrl::favorited(quint64 id, bool faved){
//case CREATE_FAVORITE:
    for(int i=0;i<this->count();i++){
        this->timelineView(i)->favorited(id,faved);
    }
    //break;
}

void QweenTabCtrl::OnUnreadCountChanged(int count){
    TimelineView* view = qobject_cast<TimelineView*>(sender());
    int idx=indexOf(view);
    if(count==0 || !settings->manageUnread()){
        setTabText(idx, view->title());
        setTabIcon(idx, QIcon());
    }else{
        setTabText(idx, QString("%0(%1)").arg(view->title()).arg(count));
        if(settings->showUnreadIconInTab()){
            setTabIcon(idx, QIcon(":/res/unread.png"));
        }else{
            setTabIcon(idx, QIcon());
        }
    }
}

void QweenTabCtrl::moveTimelineView(int before, int after){
    TimelineView *view = timelineView(before);
    this->removeTab(before);
    if(before > after){
        this->insertTab(after - 1, view, QIcon(), view->title());
    }else{
        this->insertTab(after, view, QIcon(), view->title());
    }
}

void QweenTabCtrl::setMyId(quint64 myid){
    //TODO: 各ModelにMyIDをセット
    m_myID = myid;
    for(int i=0;i<this->count();i++){
        ((TimelineView*)(this->widget(i)))->setMyId(myid);
    }
}

quint64 QweenTabCtrl::getNewestHomeId() const {
    if(!m_homeView) return 0;
    return m_homeView->model()->newestId();
}

quint64 QweenTabCtrl::getNewestDMId() const {
    if(!m_dmView) return 0;
    return m_dmView->model()->newestId();
}

quint64 QweenTabCtrl::getNewestReplyId() const{
    if(!m_replyView) return 0;
    return m_replyView->model()->newestId();
}

quint64 QweenTabCtrl::getNewestFavId() const{
    if(!m_favView) return 0;
    return m_favView->model()->newestId();
}

void QweenTabCtrl::OnItemSelected(const Twitter::TwitterItem& item){
    m_curItem = item;
    emit itemSelected(item);
}

void QweenTabCtrl::setManageUnread(bool val){
    if(m_manageUnread != val){
        m_manageUnread = val;
        if(m_manageUnread){
            for(int i=0;i<count();i++){
                TimelineView* view = timelineView(i);
                int count = view->model()->unreadCount();
                if(count==0){
                    setTabText(i, view->title());
                    setTabIcon(i, QIcon());
                }else{
                    setTabText(i, QString("%0(%1)").arg(view->title()).arg(count));
                    if(settings->showUnreadIconInTab()){
                        setTabIcon(i, QIcon(":/res/unread.png"));
                    }else{
                        setTabIcon(i, QIcon());
                    }
                }
            }
        }else{
            for(int i=0;i<count();i++){
                TimelineView* view = timelineView(i);
                setTabText(i, view->title());
                setTabIcon(i, QIcon());
            }
        }
    }
}

void QweenTabCtrl::OnFavorite(){
    emit favorite();
}

void QweenTabCtrl::OnReply(){
    TimelineView* v = qobject_cast<TimelineView*>(sender());
    if(v == m_dmView){
        emit dm();
    }else{
        emit reply();
    }
}

void QweenTabCtrl::jumpToUnread(){
    TimelineView* v = currentTimelineView();
    if(v)
        v->jumpToUnread();
}

void QweenTabCtrl::makeConnection(TimelineView *view){
    connect(view, SIGNAL(itemSelected(Twitter::TwitterItem)),
            this, SLOT(OnItemSelected(Twitter::TwitterItem)));
    connect(view, SIGNAL(unreadCountChanged(int)),
            this, SLOT(OnUnreadCountChanged(int)));
    connect(view, SIGNAL(favorite()),
            this, SLOT(OnFavorite()));
    connect(view, SIGNAL(reply()),
            this, SLOT(OnReply()));
}
