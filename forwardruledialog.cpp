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

#include "forwardruledialog.h"
#include "ui_forwardruledialog.h"
#include "forwardingrule.h"
ForwardRuleDialog::ForwardRuleDialog(const ForwardingRule &rule, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForwardRuleDialog)
{
    ui->setupUi(this);

    ui->edtName->setPlaceholderText("Name");
    ui->edtContent->setPlaceholderText("Content");
    ui->edtNameOrContent->setPlaceholderText("Name or Content");

    ui->edtNameEx->setPlaceholderText("Name");
    ui->edtContentEx->setPlaceholderText("Content");
    ui->edtNameOrContentEx->setPlaceholderText("Name or Content");

    ui->edtName->setText(rule.name);
    ui->edtContent->setText(rule.content);
    ui->edtNameOrContent->setText(rule.nameOrContent);

    ui->edtNameEx->setText(rule.exName);
    ui->edtContentEx->setText(rule.exContent);
    ui->edtNameOrContentEx->setText(rule.exNameOrContent);

    if(rule.complex){
        ui->rbComplexCond->setChecked(true);
    }else{
        ui->rbSimpleCond->setChecked(true);
    }

    if(rule.exComplex){
        ui->rbComplexCondEx->setChecked(true);
    }else{
        ui->rbSimpleCondEx->setChecked(true);
    }

    if(rule.moveFromRecent){
        ui->rbMoveFromRecent->setChecked(true);
    }else{
        ui->rbCopyFromRecent->setChecked(true);
    }

    ui->chkRegExp->setChecked(rule.useRegex);
    ui->chkCaseSensitive->setChecked(rule.caseSensitive);
    ui->chkURI->setChecked(rule.searchUri);
    ui->chkReTweet->setChecked(rule.retweet);

    ui->chkRegExpEx->setChecked(rule.exUseRegex);
    ui->chkCaseSensitiveEx->setChecked(rule.exCaseSensitive);
    ui->chkURIEx->setChecked(rule.exSearchUri);
    ui->chkReTweetEx->setChecked(rule.exRetweet);
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

ForwardingRule ForwardRuleDialog::getRule(){
    ForwardingRule rule;
    rule.moveFromRecent = ui->rbMoveFromRecent->isChecked();
    rule.complex = ui->rbComplexCond->isChecked();
    rule.exComplex = ui->rbComplexCondEx->isChecked();

    rule.name = ui->edtName->text();
    rule.content = ui->edtContent->text();
    rule.nameOrContent = ui->edtNameOrContent->text();
    rule.useRegex = ui->chkRegExp->checkState() == Qt::Checked;
    rule.caseSensitive = ui->chkCaseSensitive->checkState() == Qt::Checked;
    rule.searchUri = ui->chkURI->checkState() == Qt::Checked;
    rule.retweet = ui->chkReTweet->checkState() == Qt::Checked;

    rule.exName = ui->edtNameEx->text();
    rule.exContent = ui->edtContentEx->text();
    rule.exNameOrContent = ui->edtNameOrContentEx->text();
    rule.exUseRegex = ui->chkRegExpEx->checkState() == Qt::Checked;
    rule.exCaseSensitive = ui->chkCaseSensitiveEx->checkState() == Qt::Checked;
    rule.exSearchUri = ui->chkURIEx->checkState() == Qt::Checked;
    rule.exRetweet = ui->chkReTweetEx->checkState() == Qt::Checked;

    return rule;
}
