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

#ifndef TIMELINEITEMDELEGATE_H
#define TIMELINEITEMDELEGATE_H

#include <QStyledItemDelegate>
class IconManager;
class TimelineItemDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
    explicit TimelineItemDelegate(IconManager *iconMgr, QWidget *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
/*    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
signals:
*/
/*private slots:
    void commitAndCloseEditor();*/
    quint64 myId() { return m_myid; }
    void setMyId(quint64 id){ m_myid = id; }
private:
    IconManager *m_iconMgr;
    quint64 m_myid;
};

#endif // TIMELINEITEMDELEGATE_H
