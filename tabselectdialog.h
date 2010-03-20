#ifndef TABSELECTDIALOG_H
#define TABSELECTDIALOG_H

#include <QDialog>

namespace Ui {
    class TabSelectDialog;
}

class TabSelectDialog : public QDialog {
    Q_OBJECT
public:
    TabSelectDialog(const QStringList& items, QWidget *parent = 0);
    ~TabSelectDialog();

    int index();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TabSelectDialog *ui;
};

#endif // TABSELECTDIALOG_H
