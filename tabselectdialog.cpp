#include "tabselectdialog.h"
#include "ui_tabselectdialog.h"

TabSelectDialog::TabSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TabSelectDialog)
{
    ui->setupUi(this);
}

TabSelectDialog::~TabSelectDialog()
{
    delete ui;
}

void TabSelectDialog::changeEvent(QEvent *e)
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
