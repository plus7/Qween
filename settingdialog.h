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

#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
    class SettingDialog;
}
class QweenSettings;
class QLineEdit;
class SettingDialog : public QDialog {
    Q_OBJECT
public:
    SettingDialog(QWidget *parent = 0);
    ~SettingDialog();

    void updateUi();
    bool loginInfoChanged() { return m_loginInfoChanged; }

public slots:
    virtual void accept();

protected:
    void changeEvent(QEvent *e);

private:
    static void setLineEditBgColor(QLineEdit *edit, const QColor& color);
    Ui::SettingDialog *ui;
    QweenSettings *settings;
    QColor m_inputBgColor;
    QColor m_atReplyColor;
    QColor m_selUserColor;
    QColor m_replyToMeColor;
    QColor m_sel2ReplyColor;
    QColor m_reply2SelColor;
    QColor m_selfColor;
    QString m_password;
    QString m_id;
    bool m_xauth;
    bool m_loginInfoChanged;

private slots:
    void on_btnReply2SelColor_clicked();
    void on_btnSel2ReplyColor_clicked();
    void on_btnReplyToMeColor_clicked();
    void on_btnMyselfColor_clicked();
    void on_btnSelUserColor_clicked();
    void on_btnAtReplyColor_clicked();
    void on_btnInputFont_clicked();
    void on_btnInputBgColor_clicked();
};

#endif // SETTINGDIALOG_H
