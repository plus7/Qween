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
