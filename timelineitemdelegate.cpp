#include "timelineitemdelegate.h"
#include "timelinemodel.h"
#include "twitter.h"
#include "iconmanager.h"
#include <QtGui>

TimelineItemDelegate::TimelineItemDelegate(IconManager *iconMgr, QWidget *parent) :
    QStyledItemDelegate(parent), m_iconMgr(iconMgr)
{
}

void TimelineItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Twitter::TwitterItem *item = (Twitter::TwitterItem *)index.internalPointer();
    QStyleOptionViewItem myOption = option;
    if(!item->read())
        myOption.font.setBold(true);
    if(index.row() == item->parent()->baseIndex()){
        myOption.decorationSize.setHeight(16);
        myOption.decorationSize.setWidth(16);
        //myOption.palette.setColor(QPalette::Highlight, QColor(255,255,0));
    }
    QStyledItemDelegate::paint(painter, myOption, index);
}

//TODO: F‚Ìİ’è‚ğÀ‘•
