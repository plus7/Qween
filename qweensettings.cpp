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

#include "qweensettings.h"

QweenSettings::QweenSettings(QObject *parent) :
    QObject(parent)
{
    load();
}

void QweenSettings::load(){
    QSettings ini(QSettings::IniFormat, QSettings::UserScope, "Qween", "Qween");
    m_inputBgColor = ini.value("input/background",
                               QColor(255,255,0)).value<QColor>();
    m_requireCtrlOnEnter = ini.value("input/require_ctrl", QVariant(false)).toBool();
    m_statusSuffix = ini.value("input/suffix", "").toString();
    m_userid = ini.value("user/id", "").toString();
    m_password = ini.value("user/password", "").toString();
    m_xauth = ini.value("user/xauth",QVariant(true)).toBool();
    m_token = ini.value("user/token","").toString();
    m_token_secret = ini.value("user/token_secret","").toString();
    m_geometry = ini.value("window/geometry", QByteArray()).value<QByteArray>();
    m_winstate = ini.value("window/state", QByteArray()).value<QByteArray>();
    m_splitterstate = ini.value("window/splitter_state", QByteArray()).value<QByteArray>();
    m_minOnClose = ini.value("window/minimize_on_close", QVariant(true)).toBool();
    m_minToTray = ini.value("window/minimize_to_tray", QVariant(false)).toBool();
    m_manageUnread = ini.value("view/manage_unread", QVariant(true)).toBool();
    m_tlUpdateIntv = ini.value("interval/timeline", 60).toInt();
    m_replyUpdateIntv = ini.value("interval/reply", 240).toInt();
    m_dmUpdateIntv = ini.value("interval/dm", 600).toInt();

    m_atReplyColor = ini.value("color/atreply",QColor(255,192,192)).value<QColor>();
    m_selUserColor = ini.value("color/selecteduser",QColor(255,255,0)).value<QColor>();
    m_replyToMeColor = ini.value("color/reply_to_me",QColor(255,192,255)).value<QColor>();
    m_sel2ReplyColor = ini.value("color/selected2reply",QColor(192,192,255)).value<QColor>();
    m_reply2SelColor = ini.value("color/reply2selected",QColor(192,255,192)).value<QColor>();
    m_selfColor = ini.value("color/myself",QColor(255,0,0)).value<QColor>();

    //startup/recentpages
    //startup/replypages
    //startup/dmpages
    //startup/markread
    //startup/versioncheck
    //startup/wedata? いらないよね
    //startup/checkoneway
    //tinyurl/
    m_uriAutoShorten = ini.value("input/auto_shorten", QVariant(false)).toBool();
    m_replaceZenkakuSpace = ini.value("input/replace_zenkaku_space", QVariant(true)).toBool();
    m_divideUriFromZenkaku = ini.value("input/divide_uri_from_zenkaku", QVariant(false)).toBool();
    m_avoidApiCmd = ini.value("input/avoid_api", QVariant(true)).toBool();

    m_notifyBaloonName = ini.value("view/name_in_baloon", QVariant(2)).toInt();
    m_showUserInTitle = ini.value("view/show_user_in_title", QVariant(false)).toBool();
    m_notifyOnlyMinimized = ini.value("view/notify_only_min", QVariant(true)).toBool();
    m_showWhatInTitle = ini.value("view/show_what_in_title", QVariant(2)).toInt();
    m_dateTimeFormat = ini.value("view/datetime_fmt", QVariant("yyyy/MM/dd H:mm:ss")).toString();
    m_iconSize = ini.value("view/iconsize", QVariant(1)).toInt();
    m_showUnreadIconInTab = ini.value("view/unread_icon_tab", QVariant(true)).toBool();
    m_setReadMyPost = ini.value("view/set_read_my_post", QVariant(true)).toBool();

}

void QweenSettings::save(){
    QSettings ini(QSettings::IniFormat, QSettings::UserScope, "Qween", "Qween");
    ini.setValue("input/background", m_inputBgColor);
    ini.setValue("input/reqire_ctrl", m_requireCtrlOnEnter);
    ini.setValue("input/suffix", m_statusSuffix);
    ini.setValue("user/id", m_userid);
    ini.setValue("user/password", m_password);
    ini.setValue("user/xauth",m_xauth);
    ini.setValue("user/token",m_token);
    ini.setValue("user/token_secret",m_token_secret);
    ini.setValue("window/geometry", m_geometry);
    ini.setValue("window/state", m_winstate);
    ini.setValue("window/splitter_state", m_splitterstate);
    ini.setValue("window/minimize_on_close",m_minOnClose);
    ini.setValue("window/minimize_to_tray",m_minToTray);
    ini.setValue("view/manage_unread",m_manageUnread);
    ini.setValue("tinyurl/auto_shorten", m_uriAutoShorten);
    ini.setValue("interval/timeline", m_tlUpdateIntv);
    ini.setValue("interval/reply", m_replyUpdateIntv);
    ini.setValue("interval/dm", m_dmUpdateIntv);
    ini.setValue("input/auto_shorten", m_uriAutoShorten);
    ini.setValue("input/replace_zenkaku_space", m_replaceZenkakuSpace);
    ini.setValue("input/divide_uri_from_zenkaku", m_divideUriFromZenkaku);
    ini.setValue("input/avoid_api", m_avoidApiCmd);

    ini.setValue("color/atreply", m_atReplyColor);
    ini.setValue("color/selecteduser", m_selUserColor);
    ini.setValue("color/reply_to_me", m_replyToMeColor);
    ini.setValue("color/selected2reply", m_sel2ReplyColor);
    ini.setValue("color/reply2selected", m_reply2SelColor);
    ini.setValue("color/myself", m_selfColor);

    ini.setValue("view/name_in_baloon", m_notifyBaloonName);
    ini.setValue("view/show_user_in_title", m_showUserInTitle);
    ini.setValue("view/notify_only_min", m_notifyOnlyMinimized);
    ini.setValue("view/show_what_in_title", m_showWhatInTitle);
    ini.setValue("view/datetime_fmt", m_dateTimeFormat);
    ini.setValue("view/iconsize", m_iconSize);
    ini.setValue("view/unread_icon_tab", m_showUnreadIconInTab);
    ini.setValue("view/set_read_my_post", m_setReadMyPost);
}

QString QweenSettings::inputStyle(){
    return QString("*:focus{background-color:rgb(%1,%2,%3);}")
            .arg(m_inputBgColor.red())
            .arg(m_inputBgColor.green())
            .arg(m_inputBgColor.blue());
}

QColor QweenSettings::inputBgColor(){
    return m_inputBgColor;
}

QColor QweenSettings::inputColor(){
    return m_inputColor;
}

QString QweenSettings::inputFont(){
    return m_inputFont;
}

void QweenSettings::setInputBgColor(QColor color){
    m_inputBgColor = color;
}

void QweenSettings::setInputColor(QColor color){
    m_inputColor = color;
}

void QweenSettings::setInputFont(QString font){
    m_inputFont = font;
}

QweenSettings* QweenSettings::globalSettings(){
    static QweenSettings *global = 0;
    if(!global){
        global = new QweenSettings();
    }
    return global;
}
