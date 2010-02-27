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
    QString m_token;
    QString m_token_secret;
    bool m_markAsRead1stFetch;
    bool m_xauth;
    QByteArray m_geometry;
    QByteArray m_winstate;
    QByteArray m_splitterstate;
    bool m_minOnClose;
    bool m_minToTray;
    bool m_manageUnread;
    bool m_requireCtrlOnEnter;
    bool m_uriAutoShorten;
    QString m_statusSuffix;
    int m_tlUpdateIntv;
    int m_replyUpdateIntv;
    int m_dmUpdateIntv;
    bool m_replaceZenkakuSpace;
    bool m_divideUriFromZenkaku;
    bool m_avoidApiCmd;

    QColor m_atReplyColor;
    QColor m_selUserColor;
    QColor m_replyToMeColor;
    QColor m_sel2ReplyColor;
    QColor m_reply2SelColor;
    QColor m_selfColor;

    //view
    int m_notifyBaloonName;
    bool m_showUserInTitle;
    bool m_notifyOnlyMinimized;
    int m_showWhatInTitle;
    QString m_dateTimeFormat;
    int m_iconSize;
    bool m_showUnreadIconInTab;
    bool m_setReadMyPost;

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
    bool useXAuth() { return m_xauth; }
    QString token() { return m_token; }
    QString tokenSecret() { return m_token_secret; }

    void setUserid(const QString& val) { m_userid = val; }
    void setPassword(const QString& val) { m_password = val; }
    void setXauth(bool val) { m_xauth = val; }
    void setToken(const QString& val) { m_token = val; }
    void setTokenSecret(const QString& val) { m_token_secret = val; }

    QByteArray geometry() { return m_geometry; }
    void setGeometry(const QByteArray& val) { m_geometry = val; }

    QByteArray windowState() { return m_winstate; }
    void setWindowState(const QByteArray& val) { m_winstate = val; }

    QByteArray splitterState() { return m_splitterstate; }
    void setSplitterState(const QByteArray& val) { m_splitterstate = val; }

    bool minimizeOnClose() const { return m_minOnClose; }
    void setMinimizeOnClose(bool val) { m_minOnClose = val; }

    bool minimizeToTray() const { return m_minToTray; }
    void setMinimizeToTray(bool val) { m_minToTray = val; }

    bool manageUnread() const { return m_manageUnread; }
    void setManageUnread(bool val) { m_manageUnread = val; }

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

    QString statusViewStyle() { return "margin:0px;padding:0px;font-size:12px;"; } //stub monospaceとかの設定を反映する

    QColor atReplyColor(){ return m_atReplyColor; }
    QColor selUserColor(){ return m_selUserColor; }
    QColor replyToMeColor(){ return m_replyToMeColor; }
    QColor sel2ReplyColor(){ return m_sel2ReplyColor; }
    QColor reply2SelColor(){ return m_reply2SelColor; }
    QColor selfColor(){ return m_selfColor; }
    void setAtReplyColor(const QColor& color){ m_atReplyColor = color; }
    void setSelUserColor(const QColor& color){ m_selUserColor = color; }
    void setReplyToMeColor(const QColor& color){ m_replyToMeColor = color; }
    void setSel2ReplyColor(const QColor& color){ m_sel2ReplyColor = color; }
    void setReply2SelColor(const QColor& color){ m_reply2SelColor = color; }
    void setSelfColor(const QColor& color)     { m_selfColor = color; }


    int notifyBaloonName(){ return m_notifyBaloonName; }
    void setNotifyBaloonName(int value){ m_notifyBaloonName = value; }

    bool showUserInTitle(){ return m_showUserInTitle; }
    void setShowUserInTitle(bool value){ m_showUserInTitle = value; }

    bool notifyOnlyMinimized(){ return m_notifyOnlyMinimized; }
    void setNotifyOnlyMinimized(bool value){ m_notifyOnlyMinimized = value; }

    int showWhatInTitle(){ return m_showWhatInTitle; }
    void setShowWhatInTitle(int value){ m_showWhatInTitle = value; }

    QString dateTimeFormat(){ return m_dateTimeFormat; }
    void setDateTimeFormat(const QString& value){ m_dateTimeFormat = value; }

    int iconSize(){ return m_iconSize; }
    void setIconSize(int value){ m_iconSize = value; }

    bool showUnreadIconInTab(){ return m_showUnreadIconInTab; }
    void setShowUnreadIconInTab(bool value){ m_showUnreadIconInTab = value; }

    bool setReadMyPost(){ return m_setReadMyPost; }
    void setSetReadMyPost(bool value){ m_setReadMyPost = value; }

    bool markAsRead1stFetch() const { return m_markAsRead1stFetch; }
    void setMarkAsRead1stFetch(bool value) { m_markAsRead1stFetch = value; }

    static QweenSettings* globalSettings();

signals:

public slots:

};

#endif // QWEENSETTINGS_H
