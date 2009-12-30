#include <QTabWidget>
#include <QListWidget>
#include <QTableView>
#include <QHeaderView>
#include "qweentabctrl.h"
#include "qweenapplication.h"
#include "tabinfo.h"
#include "timelinemodel.h"
#include "timelineview.h"
#include "timelineitemdelegate.h"
QweenTabCtrl::QweenTabCtrl(QWidget *parent) :
    QTabWidget(parent),m_newestTLID(0)
{

}

void QweenTabCtrl::initWithTabInfo(TabInfo *info){
    this->addTab(new TimelineView(this), QIcon(), "Home");
    this->addTab(new QListWidget(this), QIcon(), "Reply");
    this->addTab(new QListWidget(this), QIcon(), "DM");
    this->addTab(new QListWidget(this), QIcon(), "Favorites");
    TimelineView *view = (TimelineView*)(this->widget(0));
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setRootIsDecorated(false);
    view->setItemDelegate(new TimelineItemDelegate(QweenApplication::iconManager()));
    view->setAlternatingRowColors(true);
    view->setModel(new TimelineModel(QweenApplication::iconManager(),this));
    connect(view, SIGNAL(itemSelected(Twitter::TwitterItem)),
            this, SLOT(OnItemSelected(Twitter::TwitterItem)));

}

void QweenTabCtrl::addItem(Twitter::TwitterItem item){
    QTreeView *view = (QTreeView*)(this->widget(0));
    TimelineModel *model = (TimelineModel*)view->model();
    if(m_newestTLID < item.id() && item.origin()!=Returnables::NEW_STATUS) m_newestTLID = item.id();
    model->appendItem(item);
}

Twitter::TwitterItem QweenTabCtrl::currentItem(){
    return m_curItem;
}

void QweenTabCtrl::setMyId(quint64 myid){
    //TODO: 各ViewにMyIDをセット
    m_myID = myid;
}

void QweenTabCtrl::OnItemSelected(const Twitter::TwitterItem& item){
    m_curItem = item;
    emit itemSelected(item);
}
