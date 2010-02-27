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
