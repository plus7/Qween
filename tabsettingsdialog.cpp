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

  In addition, as a special exception, NOSE Takafumi
  gives permission to link the code of its release of Qween with the
  OpenSSL project's "OpenSSL" library (or with modified versions of it
  that use the same license as the "OpenSSL" library), and distribute
  the linked executables.  You must obey the GNU General Public License
  in all respects for all of the code used other than "OpenSSL".  If you
  modify this file, you may extend this exception to your version of the
  file, but you are not obligated to do so.  If you do not wish to do
  so, delete this exception statement from your version.
*/

#include "tabsettingsdialog.h"
#include "ui_tabsettingsdialog.h"
#include "qweentabctrl.h"
#include "timelineview.h"
#include "forwardingrule.h"
#include "forwardruledialog.h"
TabSettingsDialog::TabSettingsDialog(QweenTabCtrl* tabCtrl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TabSettingsDialog),
    m_tabCtrl(tabCtrl),
    m_view(NULL)
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
    m_view = view;
    if(!view || view->type() != "userdefined"){
        ui->btnDeleteRule->setEnabled(false);
        ui->btnEditRule->setEnabled(false);
        ui->btnNewRule->setEnabled(false);
        ui->ruleList->setEnabled(false);
        ui->btnTabDelete->setEnabled(false);
    }else{
        ui->btnDeleteRule->setEnabled(true);
        ui->btnEditRule->setEnabled(true);
        ui->btnNewRule->setEnabled(true);
        ui->ruleList->setEnabled(true);
        ui->btnTabDelete->setEnabled(true);
    }
    if(!view) return;
    for(int i=0;i<view->forwardingRule.count();i++){
        ForwardingRule rule = view->forwardingRule.at(i);
        ui->ruleList->addItem(getItemText(rule));
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

QString TabSettingsDialog::getItemText(const ForwardingRule &rule){
    QString str;
    if(rule.complex){
        str.append(tr("名前に%1，本文に%2を含む. ").arg(rule.name).arg(rule.content));
    }else{
        str.append(tr("名前または本文に%1を含む. ").arg(rule.nameOrContent));
    }
    if(rule.caseSensitive){
        str.append(tr("大/小文字を区別する. "));
    }
    if(rule.useRegex){
        str.append(tr("正規表現を使う. "));
    }
    if(rule.searchUri){
        str.append(tr("URIのみを検索対象にする. "));
    }
    if(rule.retweet){
        str.append(tr("公式RT. "));
    }
    return str;
}

void TabSettingsDialog::on_btnNewRule_clicked()
{
    if(!m_view) return;
    if(m_view->type() != "userdefined") return;
    ForwardRuleDialog dlg(ForwardingRule(), this);
    if(dlg.exec() == QDialog::Accepted){
        ForwardingRule rule(dlg.getRule());
        m_view->forwardingRule.append(rule);
        ui->ruleList->addItem(getItemText(rule));
    }
}

void TabSettingsDialog::on_btnEditRule_clicked()
{
    if(!m_view) return;
    if(ui->ruleList->currentRow() < 0) return;
    ForwardingRule rule = m_view->forwardingRule.at(ui->ruleList->currentRow());
    ForwardRuleDialog dlg(rule, this);
    if(dlg.exec() == QDialog::Accepted){
        ForwardingRule rule(dlg.getRule());
        m_view->forwardingRule.replace(ui->ruleList->currentRow(), rule);
        ui->ruleList->currentItem()->setText(getItemText(rule));
    }
}

void TabSettingsDialog::on_btnDeleteRule_clicked()
{
    if(!m_view) return;
    if(ui->ruleList->currentRow() < 0) return;
    m_view->forwardingRule.removeAt(ui->ruleList->currentRow());
    delete ui->ruleList->takeItem(ui->ruleList->currentRow());
}

void TabSettingsDialog::on_btnTabCreate_clicked()
{
    QString name = QString("NewTab%1").arg(m_tabCtrl->count());
    bool ok;
    QString rv = QInputDialog::getText(this, tr("新規タブ"), tr("タブ名を入力してください"), QLineEdit::Normal, name, &ok);
    if(ok){
        m_tabCtrl->addTimelineView(rv);
        ui->tabList->addItem(rv);
    }
}

void TabSettingsDialog::on_btnTabRename_clicked()
{
    if(!m_view) return;
    bool ok;
    QString rv = QInputDialog::getText(this, tr("名前変更"), tr("名前を入力してください"), QLineEdit::Normal, m_view->title(), &ok);
    if(ok){
        ui->tabList->currentItem()->setText(rv);
        m_view->setTitle(rv);
        m_tabCtrl->setTabText(m_tabCtrl->indexOf(m_view), rv);
    }
}

void TabSettingsDialog::on_btnTabDelete_clicked()
{
    int idx = ui->tabList->currentRow();
    if(m_tabCtrl->timelineView(idx)->type() != "userdefined") return;
    if(QMessageBox::question(this, tr("タブの削除"), tr("本当にタブを削除しますか？"), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes) return;
    m_tabCtrl->removeTimelineView(idx);
    delete ui->tabList->takeItem(idx);
}
