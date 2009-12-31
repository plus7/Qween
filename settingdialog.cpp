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
    m_inputBgColor = settings->inputBgColor();
    setLineEditBgColor(ui->edtInputBgColorSample, m_inputBgColor);
    ui->chkRequireCtrlOnEnter->setChecked(settings->requireCtrlOnEnter());
    ui->chkAutoShortenUri->setChecked(settings->uriAutoShorten());
    ui->edtStatusSuffix->setText(settings->statusSuffix());
    ui->spinTLUpdateIntv->setValue(settings->tlUpdateIntv());
    ui->spinReplyUpdateIntv->setValue(settings->replyUpdateIntv());
    ui->spinDMUpdateIntv->setValue(settings->dmUpdateIntv());
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
    settings->setInputBgColor(m_inputBgColor);
    settings->setStatusSuffix(ui->edtStatusSuffix->text());
    settings->setRequireCtrlOnEnter(ui->chkRequireCtrlOnEnter->checkState() == Qt::Checked);
    settings->setUriAutoShorten(ui->chkAutoShortenUri->checkState() == Qt::Checked);
    settings->setTlUpdateIntv(ui->spinTLUpdateIntv->value());
    settings->setReplyUpdateIntv(ui->spinReplyUpdateIntv->value());
    settings->setDmUpdateIntv(ui->spinDMUpdateIntv->value());
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
    //XXX: ‚ ‚Æ‚Å”wŒiF‚ðŠÈ’P‚ÉÝ’è‚Å‚«‚éLineEdit‚ÌƒNƒ‰ƒX‚ðì‚é
    edit->setStyleSheet(
            QString("*{background-color:rgb(%1,%2,%3);}")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue()));
}

void SettingDialog::on_btnInputBgColor_clicked()
{
    QColorDialog dlg(this);
    dlg.setCurrentColor(m_inputBgColor);
    if(dlg.exec() == QDialog::Accepted){
        setLineEditBgColor(ui->edtInputBgColorSample, dlg.selectedColor());
        m_inputBgColor = dlg.selectedColor();
    }
}

void SettingDialog::on_btnInputFont_clicked()
{
    QFontDialog dlg;
    //dlg.setCurrentFont();
    dlg.exec();
}
