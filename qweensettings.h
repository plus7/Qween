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

#ifndef QWEENSETTINGS_H
#define QWEENSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QColor>
#include <QByteArray>

class QweenSettings : public QObject
{
Q_OBJECT

private:
    //static QweenSettings* s_globalSettings;
    QColor m_inputBgColor;
    QColor m_inputColor;
    QString m_inputFont;
    QString m_userid;
    QString m_password;
    QByteArray m_geometry;
    QByteArray m_winstate;
    bool m_requireCtrlOnEnter;
    bool m_uriAutoShorten;
    QString m_statusSuffix;
    int m_tlUpdateIntv;
    int m_replyUpdateIntv;
    int m_dmUpdateIntv;
    bool m_replaceZenkakuSpace;
    bool m_divideUriFromZenkaku;
    bool m_avoidApiCmd;

public:
    explicit QweenSettings(QObject *parent = 0);

    void load();
    void save();

    QString inputStyle();
    QColor inputBgColor();
    QColor inputColor();
    QString inputFont();
    void setInputBgColor(QColor color);
    void setInputColor(QColor color);
    void setInputFont(QString font);

    QString userid() { return m_userid; }
    QString password() { return m_password; }

    void setUserid(const QString& val) { m_userid = val; }
    void setPassword(const QString& val) { m_password = val; }

    QByteArray geometry() { return m_geometry; }
    void setGeometry(const QByteArray& val) { m_geometry = val; }

    QByteArray windowState() { return m_winstate; }
    void setWindowState(const QByteArray& val) { m_winstate = val; }

    bool requireCtrlOnEnter() const { return m_requireCtrlOnEnter; }
    void setRequireCtrlOnEnter(bool val) { m_requireCtrlOnEnter = val; }

    bool uriAutoShorten() const { return m_uriAutoShorten; }
    void setUriAutoShorten(bool val){ m_uriAutoShorten = val; }

    QString statusSuffix() const { return m_statusSuffix; }
    void setStatusSuffix(const QString& val){ m_statusSuffix = val; }

    int tlUpdateIntv() const { return m_tlUpdateIntv; }
    void setTlUpdateIntv(int val) { m_tlUpdateIntv = val; }

    int replyUpdateIntv() const { return m_replyUpdateIntv; }
    void setReplyUpdateIntv(int val){ m_replyUpdateIntv = val; }

    int dmUpdateIntv() const { return m_dmUpdateIntv; }
    void setDmUpdateIntv(int val) { m_dmUpdateIntv = val; }

    bool replaceZenkakuSpace() const { return m_replaceZenkakuSpace; }
    void setReplaceZenkakuSpace(bool val) { m_replaceZenkakuSpace = val; }

    bool divideUriFromZenkaku() const { return m_divideUriFromZenkaku; }
    void setDivideUriFromZenkaku(bool val) { m_divideUriFromZenkaku = val; }

    bool avoidApiCmd() const { return m_avoidApiCmd; }
    void setAvoidApiCmd(bool val) { m_avoidApiCmd = val; }

    QString statusViewStyle() { return "margin:0px;padding:0px;font-size:12px;font-family:monospace;"; } //stub monospaceÇ∆Ç©ÇÃê›íËÇîΩâfÇ∑ÇÈ

    static QweenSettings* globalSettings();

signals:

public slots:

};

#endif // QWEENSETTINGS_H
