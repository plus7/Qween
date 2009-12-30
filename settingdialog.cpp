#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "qweensettings.h"
#include <QtGui>
SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    settings = QweenSettings::globalSettings();
    m_inputBgColor = settings->inputBgColor();
    updateUi();
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::updateUi(){

    ui->edtPassword->setText(settings->password());
    ui->edtUserid->setText(settings->userid());
    setLineEditBgColor(ui->edtInputBgColorSample, m_inputBgColor);
    ui->chkRequireCtrlOnEnter->setChecked(settings->requireCtrlOnEnter());
    ui->chkAutoShortenUri->setChecked(settings->uriAutoShorten());
    ui->edtStatusSuffix->setText(settings->statusSuffix());
}

void SettingDialog::accept(){
    QDialog::accept();
    QweenSettings *settings = QweenSettings::globalSettings();
    settings->setPassword(ui->edtPassword->text());
    settings->setUserid(ui->edtUserid->text());
    settings->setInputBgColor(m_inputBgColor);
    settings->setStatusSuffix(ui->edtStatusSuffix->text());
    settings->setRequireCtrlOnEnter(ui->chkRequireCtrlOnEnter->checkState() == Qt::Checked);
    settings->setUriAutoShorten(ui->chkAutoShortenUri->checkState() == Qt::Checked);
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
