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

#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "qweensettings.h"
#include <QtGui>
SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog),m_loginInfoChanged(false)
{
    ui->setupUi(this);
    settings = QweenSettings::globalSettings();
    updateUi();
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::updateUi(){
    m_password = settings->password();
    ui->edtPassword->setText(m_password);
    m_id = settings->userid();
    ui->edtUserid->setText(m_id);
    //Color
    m_inputBgColor = settings->inputBgColor();
    setLineEditBgColor(ui->edtInputBgColorSample, m_inputBgColor);
    m_atReplyColor = settings->atReplyColor();
    setLineEditBgColor(ui->edtAtReplyColor, m_atReplyColor);
    m_selUserColor = settings->selUserColor();
    setLineEditBgColor(ui->edtSelUserColor, m_selUserColor);
    m_selfColor = settings->selfColor();
    ui->edtMyselfColor->setStyleSheet(QString(" *{color:rgb(%0,%1,%2);}")
                                      .arg(m_selfColor.red())
                                      .arg(m_selfColor.green())
                                      .arg(m_selfColor.blue()));
    m_replyToMeColor = settings->replyToMeColor();
    setLineEditBgColor(ui->edtReplyToMeColor, m_replyToMeColor);
    m_sel2ReplyColor = settings->sel2ReplyColor();
    setLineEditBgColor(ui->edtSel2ReplyColor, m_sel2ReplyColor);
    m_reply2SelColor = settings->reply2SelColor();
    setLineEditBgColor(ui->edtReply2SelColor, m_reply2SelColor);

    //view
    ui->cboNotifyBaloonName->setCurrentIndex(settings->notifyBaloonName());
    ui->chkShowUserInTitle->setChecked(settings->showUserInTitle());
    ui->chkNotifyOnlyMin->setChecked(settings->notifyOnlyMinimized());
    ui->cboShowWhatInTitle->setCurrentIndex(settings->showWhatInTitle());
    ui->cboDateTimeFmt->setEditText(settings->dateTimeFormat());
    ui->cboIconSize->setCurrentIndex(settings->iconSize());
    ui->chkShowUnreadIcon->setChecked(settings->showUnreadIconInTab());
    ui->chkSetReadMyPost->setChecked(settings->setReadMyPost());

    ui->chkRequireCtrlOnEnter->setChecked(settings->requireCtrlOnEnter());
    ui->chkAutoShortenUri->setChecked(settings->uriAutoShorten());
    ui->edtStatusSuffix->setText(settings->statusSuffix());
    ui->spinTLUpdateIntv->setValue(settings->tlUpdateIntv());
    ui->spinReplyUpdateIntv->setValue(settings->replyUpdateIntv());
    ui->spinDMUpdateIntv->setValue(settings->dmUpdateIntv());
    ui->chkMinimizeOnClose->setChecked(settings->minimizeOnClose());
}

void SettingDialog::accept(){
    QDialog::accept();
    QweenSettings *settings = QweenSettings::globalSettings();
    if(ui->edtPassword->text() != m_password){
        settings->setPassword(ui->edtPassword->text());
        m_loginInfoChanged = true;
    }
    if(ui->edtUserid->text() != m_id){
        settings->setUserid(ui->edtUserid->text());
        m_loginInfoChanged = true;
    }
    //Color
    settings->setInputBgColor(m_inputBgColor);
    settings->setAtReplyColor(m_atReplyColor);
    settings->setSelUserColor(m_selUserColor);
    settings->setReplyToMeColor(m_replyToMeColor);
    settings->setSel2ReplyColor(m_sel2ReplyColor);
    settings->setReply2SelColor(m_reply2SelColor);
    settings->setSelfColor(m_selfColor);

    //view
    settings->setNotifyBaloonName(ui->cboNotifyBaloonName->currentIndex());
    settings->setShowUserInTitle(ui->chkShowUserInTitle->checkState() == Qt::Checked);
    settings->setNotifyOnlyMinimized(ui->chkNotifyOnlyMin->checkState() == Qt::Checked);
    settings->setShowWhatInTitle(ui->cboShowWhatInTitle->currentIndex());
    settings->setDateTimeFormat(ui->cboDateTimeFmt->lineEdit()->text());
    settings->setIconSize(ui->cboIconSize->currentIndex());
    settings->setShowUnreadIconInTab(ui->chkShowUnreadIcon->checkState() == Qt::Checked);
    settings->setSetReadMyPost(ui->chkSetReadMyPost->checkState() == Qt::Checked);

    settings->setStatusSuffix(ui->edtStatusSuffix->text());
    settings->setRequireCtrlOnEnter(ui->chkRequireCtrlOnEnter->checkState() == Qt::Checked);
    settings->setUriAutoShorten(ui->chkAutoShortenUri->checkState() == Qt::Checked);
    settings->setTlUpdateIntv(ui->spinTLUpdateIntv->value());
    settings->setReplyUpdateIntv(ui->spinReplyUpdateIntv->value());
    settings->setDmUpdateIntv(ui->spinDMUpdateIntv->value());
    settings->setMinimizeOnClose(ui->chkMinimizeOnClose->checkState() == Qt::Checked);
    settings->save();
}

void SettingDialog::changeEvent(QEvent *e)
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

void SettingDialog::setLineEditBgColor(QLineEdit *edit, const QColor& color){
    //TODO: あとで背景色を簡単に設定できるLineEditのクラスを作る
    edit->setStyleSheet(
            QString("*{background-color:rgb(%1,%2,%3);}")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue()));
}

void SettingDialog::on_btnInputFont_clicked()
{
    QFontDialog dlg;
    //dlg.setCurrentFont();
    dlg.exec();
}

//TODO: なんだかここの羅列があまりにも芸がないので、専用のコントロール作ってバインドしたいところ
void SettingDialog::on_btnInputBgColor_clicked()
{
    QColorDialog dlg(this);
    dlg.setCurrentColor(m_inputBgColor);
    if(dlg.exec() == QDialog::Accepted){
        setLineEditBgColor(ui->edtInputBgColorSample, dlg.selectedColor());
        m_inputBgColor = dlg.selectedColor();
    }
}

void SettingDialog::on_btnAtReplyColor_clicked()
{
    QColorDialog dlg(this);
    dlg.setCurrentColor(m_atReplyColor);
    if(dlg.exec() == QDialog::Accepted){
        setLineEditBgColor(ui->edtAtReplyColor, dlg.selectedColor());
        m_atReplyColor = dlg.selectedColor();
    }
}

void SettingDialog::on_btnSelUserColor_clicked()
{
    QColorDialog dlg(this);
    dlg.setCurrentColor(m_selUserColor);
    if(dlg.exec() == QDialog::Accepted){
        setLineEditBgColor(ui->edtSelUserColor, dlg.selectedColor());
        m_selUserColor = dlg.selectedColor();
    }
}

void SettingDialog::on_btnMyselfColor_clicked()
{
    QColorDialog dlg(this);
    dlg.setCurrentColor(m_selfColor);
    if(dlg.exec() == QDialog::Accepted){
        m_selfColor = dlg.selectedColor();
        ui->edtMyselfColor->setStyleSheet(QString(" *{color:rgb(%0,%1,%2);}")
                                          .arg(m_selfColor.red())
                                          .arg(m_selfColor.green())
                                          .arg(m_selfColor.blue()));
    }
}

void SettingDialog::on_btnReplyToMeColor_clicked()
{
    QColorDialog dlg(this);
    dlg.setCurrentColor(m_replyToMeColor);
    if(dlg.exec() == QDialog::Accepted){
        setLineEditBgColor(ui->edtReplyToMeColor, dlg.selectedColor());
        m_replyToMeColor = dlg.selectedColor();
    }
}

void SettingDialog::on_btnSel2ReplyColor_clicked()
{
    QColorDialog dlg(this);
    dlg.setCurrentColor(m_sel2ReplyColor);
    if(dlg.exec() == QDialog::Accepted){
        setLineEditBgColor(ui->edtSel2ReplyColor, dlg.selectedColor());
        m_sel2ReplyColor = dlg.selectedColor();
    }
}

void SettingDialog::on_btnReply2SelColor_clicked()
{
    QColorDialog dlg(this);
    dlg.setCurrentColor(m_reply2SelColor);
    if(dlg.exec() == QDialog::Accepted){
        setLineEditBgColor(ui->edtReply2SelColor, dlg.selectedColor());
        m_reply2SelColor = dlg.selectedColor();
    }
}
