#include "forwardruledialog.h"
#include "ui_forwardruledialog.h"

ForwardRuleDialog::ForwardRuleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForwardRuleDialog)
{
    ui->setupUi(this);
}

ForwardRuleDialog::~ForwardRuleDialog()
{
    delete ui;
}

void ForwardRuleDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
