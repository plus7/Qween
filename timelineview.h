#ifndef TIMELINEVIEW_H
#define TIMELINEVIEW_H

#include <QTreeView>
#include "twitter.h"
#include "timelinemodel.h"

class TimelineView : public QTreeView
{
Q_OBJECT
public:
    explicit TimelineView(QWidget *parent = 0);
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

};

#endif // TIMELINEVIEW_H
