#include "timelineview.h"
#include <QtXml>
TimelineView::TimelineView(QWidget *parent) :
    QTreeView(parent),m_id(0)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setRootIsDecorated(false);
    setAlternatingRowColors(true);
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
    //TODO: U‚è•ª‚¯İ’è‚Ì‘‚«o‚µ
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
    //TODO: U‚è•ª‚¯İ’è‚Ì“Ç‚İ‚İ
}
