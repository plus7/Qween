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
