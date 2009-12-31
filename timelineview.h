#ifndef TIMELINEVIEW_H
#define TIMELINEVIEW_H

#include <QTreeView>
#include "twitter.h"
#include "timelinemodel.h"
#include "forwardingrule.h"
#include <QtXml>
class TimelineView : public QTreeView
{
Q_OBJECT
public:
    explicit TimelineView(QWidget *parent = 0);
    QDomElement saveToElement(QDomDocument& doc);
    void restoreFromElement(const QDomElement& element);
    QList<ForwardingRule> forwardingRule;

    QString type(){return m_type;}
    void setType(const QString& val){m_type = val;}
    QString title(){return m_title;}
    void setTitle(const QString& val){m_title = val;}
    TimelineModel* model(){ return (TimelineModel*)QTreeView::model(); }

protected:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected){
        QTreeView::selectionChanged(selected, deselected);
        if(selected.indexes().count() > 0){
            TimelineModel *model = (TimelineModel *)(this->model());
            model->setBaseIndex(selected.indexes().at(0).row());
            model->setRead(selected.indexes().at(0).row(),true);
            Twitter::TwitterItem item = model->itemAt(selected.indexes().at(0).row());
            emit itemSelected(item);
        }
    }

signals:
    void itemSelected(const Twitter::TwitterItem& item);

public slots:
private:
    QString m_type;
    QString m_title;
};

#endif // TIMELINEVIEW_H
