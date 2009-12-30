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
private:
    IconManager *m_iconMgr;
};

#endif // TIMELINEITEMDELEGATE_H
