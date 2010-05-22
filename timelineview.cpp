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
#include "twitter.h"
#include <QtXml>
#include <QKeyEvent>
#include <QRegExp>
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

bool TimelineView::isRelatedPost(int idx, int baseIdx){
    Twitter::TwitterItem baseItem = model()->itemAt(baseIdx);
    Twitter::TwitterItem item = model()->itemAt(idx);
    if(item.id() == baseItem.inReplyToId() ||
       baseItem.userId() == item.userId() ||
       //item.replyToList().indexOf(QweenSettings::globalSettings()->userid())>=0 ||
       baseItem.replyToList().indexOf(item.screenName())>=0 ||
       item.replyToList().indexOf(baseItem.screenName())>=0 ){
        return true;
    }
    return false;
}
void TimelineView::jumpToUnread(){
    int i;
    int next = -1;
    for(i=model()->baseIndex()+1; i<model()->count(); i++){
        if(!model()->itemAt(i).read()){
            next = i;
            break;
        }
    }
    if(next>=0) setCurrentIndex(model()->index(next,currentIndex().column()));
}

void TimelineView::setReadAll(){
    model()->setReadAll();
}

void TimelineView::keyPressEvent(QKeyEvent *event){
    int i, next;
    if(!event->modifiers().testFlag(Qt::ShiftModifier) &&
       !event->modifiers().testFlag(Qt::ControlModifier) &&
       !event->modifiers().testFlag(Qt::AltModifier)){
        switch(event->key()){
        case Qt::Key_M:
            next = -1;
            for(i=model()->baseIndex()-1; i>=0; i--){
                if(isRelatedPost(i, model()->baseIndex())){
                    next = i;
                    break;
                }
            }
            if(next>=0) setCurrentIndex(model()->index(next,currentIndex().column()));
            return;
        case Qt::Key_N:
            next = -1;
            for(i=model()->baseIndex()+1; i<model()->count(); i++){
                if(isRelatedPost(i, model()->baseIndex())){
                    next = i;
                    break;
                }
            }
            if(next>=0) setCurrentIndex(model()->index(next,currentIndex().column()));
            return;
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
            if(next>=0) setCurrentIndex(model()->index(next,currentIndex().column()));
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
            if(next>=0) setCurrentIndex(model()->index(next,currentIndex().column()));
            return;
        case Qt::Key_Space:
            jumpToUnread();
            return;
        case Qt::Key_Return:
            emit reply();
            return;
        default:
            break;
        }
    }
    if(event->modifiers().testFlag(Qt::ShiftModifier) &&
       !event->modifiers().testFlag(Qt::ControlModifier) &&
       !event->modifiers().testFlag(Qt::AltModifier)){
        switch(event->key()){
       case Qt::Key_M:
           next = -1;
           for(i=model()->baseIndex()-1; i>=0; i--){
               if(model()->itemAt(i).favorited()){
                   next = i;
                   break;
               }
           }
           if(next>=0) setCurrentIndex(model()->index(next,currentIndex().column()));
           return;
       case Qt::Key_N:
           next = -1;
           for(i=model()->baseIndex()+1; i<model()->count(); i++){
               if(model()->itemAt(i).favorited()){
                   next = i;
                   break;
               }
           }
           if(next>=0) setCurrentIndex(model()->index(next,currentIndex().column()));
           return;
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
    elm.appendChild(forward);
    //TODO: 振り分け設定の書き出し
    for(int i=0;i<forwardingRule.count();i++){
        QDomElement ruleElm = doc.createElement("rule");
        ForwardingRule rule = forwardingRule.at(i);

        ruleElm.setAttribute("move", rule.moveFromRecent);

        ruleElm.setAttribute("name", rule.name);
        ruleElm.setAttribute("content", rule.content);
        ruleElm.setAttribute("nameOrContent", rule.nameOrContent);
        ruleElm.setAttribute("uri", rule.searchUri);
        ruleElm.setAttribute("cs", rule.caseSensitive);
        ruleElm.setAttribute("complex", rule.complex);
        ruleElm.setAttribute("userx", rule.useRegex);
        ruleElm.setAttribute("rt", rule.retweet);

        ruleElm.setAttribute("exname", rule.exName);
        ruleElm.setAttribute("excontent", rule.exContent);
        ruleElm.setAttribute("exNameOrContent", rule.exNameOrContent);
        ruleElm.setAttribute("exuri", rule.exSearchUri);
        ruleElm.setAttribute("excs", rule.exCaseSensitive);
        ruleElm.setAttribute("excomplex", rule.exComplex);
        ruleElm.setAttribute("exuserx", rule.exUseRegex);
        ruleElm.setAttribute("exrt", rule.exRetweet);
        forward.appendChild(ruleElm);
    }
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
    QDomElement forward = element.firstChildElement("forward");
    if(forward.isNull()) return;
    QDomElement ruleElm = forward.firstChildElement("rule");
    while (!ruleElm.isNull()) {
        ForwardingRule rule;
        rule.moveFromRecent = (ruleElm.attribute("move", "0") == "1");

        rule.name = ruleElm.attribute("name", "");
        rule.content = ruleElm.attribute("content", "");
        rule.nameOrContent = ruleElm.attribute("nameOrContent", "");
        rule.searchUri = (ruleElm.attribute("uri", "0") == "1");
        rule.caseSensitive = (ruleElm.attribute("cs", "0") == "1");
        rule.complex = (ruleElm.attribute("complex", "0") == "1");
        rule.useRegex = (ruleElm.attribute("userx", "0") == "1");
        rule.retweet = (ruleElm.attribute("rt", "0") == "1");

        rule.exName = ruleElm.attribute("exname", "");
        rule.exContent = ruleElm.attribute("excontent", "");
        rule.exNameOrContent = ruleElm.attribute("exNameOrContent", "");
        rule.exSearchUri = (ruleElm.attribute("exuri", "0") == "1");
        rule.exCaseSensitive = (ruleElm.attribute("excs", "0") == "1");
        rule.exComplex = (ruleElm.attribute("excomplex", "0") == "1");
        rule.exUseRegex = (ruleElm.attribute("exuserx", "0") == "1");
        rule.exRetweet = (ruleElm.attribute("exrt", "0") == "1");

        forwardingRule.append(rule);

        ruleElm = ruleElm.nextSiblingElement("rule");
    }
}

bool TimelineView::match(Twitter::TwitterItem &item, bool *copy){
    for(int i=0;i<forwardingRule.count();i++){
        if(match(item, forwardingRule.at(i))){
            *copy = !forwardingRule.at(i).moveFromRecent;
            return true;
        }
    }
    *copy = false;
    return false;
}

bool TimelineView::match(Twitter::TwitterItem &item, const ForwardingRule &rule){
    QRegExp::PatternSyntax pat;
    Qt::CaseSensitivity cs;

    if(rule.useRegex) pat = QRegExp::RegExp;
    else pat = QRegExp::FixedString;

    if(rule.caseSensitive) cs = Qt::CaseSensitive;
    else cs = Qt::CaseInsensitive;

    QRegExp namerx(rule.name, cs, pat);
    QRegExp contrx(rule.content, cs, pat);
    QRegExp bothrx(rule.nameOrContent, cs, pat);

    bool matched = false;
    if(rule.complex){
        if((!rule.name.isEmpty()    && namerx.indexIn(item.screenName(),0) != -1) &&
           (!rule.content.isEmpty() && contrx.indexIn(item.status(),0) != -1)){
            matched = true;
        }
    }else{
        if(!rule.nameOrContent.isEmpty() &&
           (bothrx.indexIn(item.screenName(),0) != -1 ||
            bothrx.indexIn(item.status(),0) != -1)){
            matched = true;
        }
    }
    if(!matched) return false;

    if(rule.exUseRegex) pat = QRegExp::RegExp;
    else pat = QRegExp::FixedString;

    if(rule.exCaseSensitive) cs = Qt::CaseSensitive;
    else cs = Qt::CaseInsensitive;

    QRegExp exnamerx(rule.exName, cs, pat);
    QRegExp excontrx(rule.exContent, cs, pat);
    QRegExp exbothrx(rule.exNameOrContent, cs, pat);
    bool exmatched = false;
    if(rule.exComplex){
        if((!rule.exName.isEmpty() && exnamerx.indexIn(item.screenName(),0) != -1) &&
           (!rule.exContent.isEmpty() && excontrx.indexIn(item.status(),0) != -1)){
            exmatched = true;
        }
    }else{
        if(!rule.exNameOrContent.isEmpty() &&
           (exbothrx.indexIn(item.screenName(),0) != -1 ||
            exbothrx.indexIn(item.status(),0) != -1)){
            exmatched = true;
        }
    }
    return !exmatched;
}
