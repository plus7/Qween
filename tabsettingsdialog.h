#ifndef TABSETTINGSDIALOG_H
#define TABSETTINGSDIALOG_H

#include <QDialog>
#include <QtGui>

namespace Ui {
    class TabSettingsDialog;
}
class QweenTabCtrl;
class TabSettingsDialog : public QDialog {
    Q_OBJECT
public:
    TabSettingsDialog(QweenTabCtrl* tabCtrl, QWidget *parent = 0);
    ~TabSettingsDialog();

    void makeConnections();
public slots:
    void OnTabListItemChanged(QListWidgetItem*,QListWidgetItem*);


protected:
    void changeEvent(QEvent *e);

private:
    Ui::TabSettingsDialog *ui;
    QweenTabCtrl *m_tabCtrl;
};

#endif // TABSETTINGSDIALOG_H
