#include "tabsettingsdialog.h"
#include "ui_tabsettingsdialog.h"
#include "qweentabctrl.h"
#include "timelineview.h"
#include "forwardingrule.h"
TabSettingsDialog::TabSettingsDialog(QweenTabCtrl* tabCtrl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TabSettingsDialog),
    m_tabCtrl(tabCtrl)
{
    ui->setupUi(this);
    makeConnections();
    for(int i=0;i<tabCtrl->count();i++){
        ui->tabList->addItem(m_tabCtrl->timelineView(i)->title());
    }
}

void TabSettingsDialog::makeConnections(){
    connect(ui->tabList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(OnTabListItemChanged(QListWidgetItem*,QListWidgetItem*)));
}

void TabSettingsDialog::OnTabListItemChanged(QListWidgetItem* current,QListWidgetItem* previous){
    ui->ruleList->clear();
    TimelineView *view = m_tabCtrl->timelineView(ui->tabList->row(current));
    for(int i=0;i<view->forwardingRule.count();i++){
        ForwardingRule rule = view->forwardingRule.at(i);
    }
}

TabSettingsDialog::~TabSettingsDialog()
{
    delete ui;
}

void TabSettingsDialog::changeEvent(QEvent *e)
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
