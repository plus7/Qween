#include "statusbrowser.h"
#include <QtGui>
StatusBrowser::StatusBrowser(QWidget *parent) :
    QTextBrowser(parent)
{
}

void StatusBrowser::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();
    m_selectedName.clear();
    if(!event->pos().isNull()){
        QString tmp = anchorAt(event->pos());
        QRegExp rx("^https?://twitter\\.com/([a-zA-Z0-9_]+)$");
        if(tmp.indexOf(rx)>=0){
            m_selectedName = rx.cap(1);
        }
    }
    if(!m_selectedName.isEmpty()){
        menu->addSeparator();
        menu->addAction(tr("Follow"),this, SLOT(followClicked()));
        menu->addAction(tr("Remove"),this, SLOT(removeClicked()));
        menu->addAction(tr("ShowFriendship"),this, SLOT(friendshipClicked()));
    }
    menu->exec(event->globalPos());
    delete menu;
}

void StatusBrowser::followClicked(){
    emit followCommand(m_selectedName);
}

void StatusBrowser::removeClicked(){
    emit removeCommand(m_selectedName);
}

void StatusBrowser::friendshipClicked(){
    emit friendshipCommand(m_selectedName);
}
