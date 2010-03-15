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
    void setModel(QAbstractItemModel *model){
        TimelineModel* m;
        if(this->model()){
            m=this->model();
            disconnect(m,0,this,0);
        }
        QTreeView::setModel(model);
        if(model){
            m = (TimelineModel*)model;
            connect(m,SIGNAL(unreadCountChanged(int)),
                    this,SLOT(OnUnreadCountChanged(int)));
        }
    }

    int myId(){ return m_myId; }
    void setMyId(int id){
        m_myId = id;
        this->model()->setMyId(id);
    }
    void favorited(quint64 id, bool faved){
        TimelineModel* m = model();
        for(int i=0;i<m->rowCount(QModelIndex());i++){
             Twitter::TwitterItem item = m->itemAt(i);
             if(item.id() == id){
                 item.setFav(faved);
                 break;
             }
        }
    }
    bool match(Twitter::TwitterItem &item, bool *copy);
    bool match(Twitter::TwitterItem &item, const ForwardingRule &rule);

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
        dataChanged(indexAt(QPoint(0,0)),indexAt(QPoint(width(),height())));
    }

signals:
    void itemSelected(const Twitter::TwitterItem& item);
    void unreadCountChanged(int count);
    void favorite();
    void reply();

public slots:
    void jumpToUnread();
    void OnUnreadCountChanged(int count){
        emit unreadCountChanged(count);
    }
protected:
    void keyPressEvent(QKeyEvent *event);

private:
    bool isRelatedPost(int idx, int baseIdx);
    int m_myId;
    QString m_type;
    QString m_title;
};

#endif // TIMELINEVIEW_H
