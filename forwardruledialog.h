#ifndef FORWARDRULEDIALOG_H
#define FORWARDRULEDIALOG_H

#include <QDialog>

namespace Ui {
    class ForwardRuleDialog;
}

class ForwardRuleDialog : public QDialog {
    Q_OBJECT
public:
    ForwardRuleDialog(QWidget *parent = 0);
    ~ForwardRuleDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ForwardRuleDialog *ui;
};

#endif // FORWARDRULEDIALOG_H
