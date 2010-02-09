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
    m_geometry = ini.value("window/geometry", QByteArray()).value<QByteArray>();
    m_winstate = ini.value("window/state", QByteArray()).value<QByteArray>();
    m_splitterstate = ini.value("window/splitter_state", QByteArray()).value<QByteArray>();
    m_minOnClose = ini.value("window/minimize_on_close", QVariant(true)).toBool();
    m_tlUpdateIntv = ini.value("interval/timeline", 60).toInt();
    m_replyUpdateIntv = ini.value("interval/reply", 240).toInt();
    m_dmUpdateIntv = ini.value("interval/dm", 600).toInt();
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

}

void QweenSettings::save(){
    QSettings ini(QSettings::IniFormat, QSettings::UserScope, "Qween", "Qween");
    ini.setValue("input/background", m_inputBgColor);
    ini.setValue("input/reqire_ctrl", m_requireCtrlOnEnter);
    ini.setValue("input/suffix", m_statusSuffix);
    ini.setValue("user/id", m_userid);
    ini.setValue("user/password", m_password);
    ini.setValue("window/geometry", m_geometry);
    ini.setValue("window/state", m_winstate);
    ini.setValue("window/splitter_state", m_splitterstate);
    ini.setValue("window/minimize_on_close",m_minOnClose);
    ini.setValue("tinyurl/auto_shorten", m_uriAutoShorten);
    ini.setValue("interval/timeline", m_tlUpdateIntv);
    ini.setValue("interval/reply", m_replyUpdateIntv);
    ini.setValue("interval/dm", m_dmUpdateIntv);
    ini.setValue("input/auto_shorten", m_uriAutoShorten);
    ini.setValue("input/replace_zenkaku_space", m_replaceZenkakuSpace);
    ini.setValue("input/divide_uri_from_zenkaku", m_divideUriFromZenkaku);
    ini.setValue("input/avoid_api", m_avoidApiCmd);
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
