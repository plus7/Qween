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

#include "timelineview.h"
#include "qweensettings.h"
#include <QtXml>
#include <QKeyEvent>
TimelineView::TimelineView(QWidget *parent) :
    QTreeView(parent),m_myId(0)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setRootIsDecorated(false);
    setAlternatingRowColors(true);
    int size = QweenSettings::globalSettings()->iconSize()*8 + 8;
    if(size==8) size=0;
    setIconSize(QSize(size,size));
}

void TimelineView::keyPressEvent(QKeyEvent *event){
    if(!event->modifiers().testFlag(Qt::ShiftModifier) &&
       !event->modifiers().testFlag(Qt::ControlModifier) &&
       !event->modifiers().testFlag(Qt::AltModifier)){
        int i, next;
        switch(event->key()){
        case Qt::Key_I:
            emit favorite();
            return;
        case Qt::Key_H:
            next = -1;
            for(i=model()->baseIndex()+1; i<model()->count(); i++){
                if(model()->itemAt(i).userId() == model()->itemAt(model()->baseIndex()).userId()){
                    next = i;
                    break;
                }
            }
            if(next>0) setCurrentIndex(model()->index(next,currentIndex().column()));
            return;
        case Qt::Key_J:
            QApplication::sendEvent(this, &QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier));
            return;
        case Qt::Key_K:
            QApplication::sendEvent(this, &QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier));
            return;
        case Qt::Key_L:
            next = -1;
            for(i=model()->baseIndex()-1; i>=0; i--){
                if(model()->itemAt(i).userId() == model()->itemAt(model()->baseIndex()).userId()){
                    next = i;
                    break;
                }
            }
            if(next>0) setCurrentIndex(model()->index(next,currentIndex().column()));
            return;
        case Qt::Key_Space:
            next = -1;
            for(i=model()->baseIndex()+1; i<model()->count(); i++){
                if(!model()->itemAt(i).read()){
                    next = i;
                    break;
                }
            }
            if(next>0) setCurrentIndex(model()->index(next,currentIndex().column()));
            return;
        case Qt::Key_Enter:
            emit reply();
            return;
        default:
            break;
        }
    }
    QTreeView::keyPressEvent(event);
}

QDomElement TimelineView::saveToElement(QDomDocument& doc){
    QDomElement elm = doc.createElement("tab");
    elm.setAttribute("title", title());
    elm.setAttribute("type", type());
    QDomElement header = doc.createElement("header");
    QString peHeaderState(this->header()->saveState().toPercentEncoding());
    header.setAttribute("state", peHeaderState);
    elm.appendChild(header);
    QDomElement forward = doc.createElement("forward");
    //TODO: 振り分け設定の書き出し
    for(int i;i<forwardingRule.count();i++){
        QDomElement ruleElm = doc.createElement("rule");
        ForwardingRule rule = forwardingRule.at(i);
        ruleElm.setAttribute("body", rule.body);
        ruleElm.setAttribute("name", rule.name);
        ruleElm.setAttribute("cs", rule.caseSensitive);
        ruleElm.setAttribute("useboth", rule.useBoth);
        ruleElm.setAttribute("userx", rule.useRegex);
        ruleElm.setAttribute("exbody", rule.exbody);
        ruleElm.setAttribute("exname", rule.exname);
        ruleElm.setAttribute("excs", rule.exCaseSensitive);
        ruleElm.setAttribute("exuseboth", rule.exUseBoth);
        ruleElm.setAttribute("exuserx", rule.exUseRegex);
        /*
        bool moveFromRecent;
        bool setMark;
        bool searchUrl;
        bool exSearchUrl;*/
    }
    elm.appendChild(forward);
    return elm;
}

void TimelineView::restoreFromElement(const QDomElement& element){
    QDomElement header = element.firstChildElement("header");
    QString headerStateStr = header.attribute("state", "");
    if(!headerStateStr.isEmpty()){
        QByteArray arr;
        this->header()->restoreState(arr.fromPercentEncoding(headerStateStr.toAscii()));
    }
    //TODO: 振り分け設定の読み込み
}

