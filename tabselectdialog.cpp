#include "tabselectdialog.h"
#include "ui_tabselectdialog.h"

TabSelectDialog::TabSelectDialog(const QStringList& items, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TabSelectDialog)
{
    ui->setupUi(this);
    ui->listWidget->addItems(items);
}

TabSelectDialog::~TabSelectDialog()
{
    delete ui;
}

int TabSelectDialog::index()
{
    return ui->listWidget->currentIndex().row();
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
