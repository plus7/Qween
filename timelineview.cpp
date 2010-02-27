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

#include "timelineview.h"
#include "qweensettings.h"
#include <QtXml>
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
        ruleElm.setAttribute("exbody", rule.exbody);
        ruleElm.setAttribute("name", rule.name);
        ruleElm.setAttribute("exname", rule.exname);
        ruleElm.setAttribute("cs", rule.caseSensitive);
        ruleElm.setAttribute("excs", rule.exCaseSensitive);
        /*
        bool useBoth;
        bool exUseBoth;
        bool moveFromRecent;
        bool setMark;
        bool searchUrl;
        bool exSearchUrl;
        bool caseSensitive;
        bool exCaseSensitive;
        bool useRegex;
        bool exUseRegex;*/
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

