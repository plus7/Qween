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

#ifndef QWEENMAINWINDOW_H
#define QWEENMAINWINDOW_H
#include <QtGui>
#include <QtNetwork>
#include <QMainWindow>
#include "twitter.h"
#include "petrel/petrel.h"
namespace Ui {
    class QweenMainWindow;
}

class QweenSettings;
class QweenTabCtrl;
class QTimer;
class QMenu;
class QIcon;
class QSystemTrayIcon;
class AbstractUriShortener;
class UsersModel;
class QCompleter;
class QSortFilterProxyModel;
class XAuth;
class QweenMainWindow : public QMainWindow {
    Q_OBJECT
public:
    QweenMainWindow(QWidget *parent = 0);
    ~QweenMainWindow();

private:
    void applySettings();
    /*QString convertUri(ShortUriSvc svc, const QString& source, bool shorten = true);
    void doUriConvert(ShortUriSvc svc);*/
    bool isNetworkAvailable();
    void save();
    void setupMenus();
    void setupTabs();
    void setupTimers();
    void setupTrayIcon();
    void setupTwitter();
    void makeConnections();
    void makeWidgets();
    void doPost();
    void postOutputz(const QString& str);
    void makeReplyOrDirectStatus(bool isAuto, bool isReply, bool isAll);
    int getRestStatusCount(const QString& str, bool footer = true);
    void doFollowCommand(const QString& name);
    void doRemoveCommand(const QString& name);
    void doFriendshipCommand(const QString& name);
    void updateWindowTitle();
    void updateTrayIconTitle();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void showEvent(QShowEvent *event);

private:
    Ui::QweenMainWindow *ui;
    QweenSettings *settings;
    QweenTabCtrl *tabWidget;
    Petrel *m_petrelLib;
    QTimer *m_timelineTimer;
    QTimer *m_DMTimer;
    QTimer *m_replyTimer;
    QTimer *m_favTimer;
    QTimer *m_fetchAnimTimer;
    QMenu *m_postModeMenu;
    QAction *m_actAutoShortenUri;
    QAction *m_actReplaceZenkakuSpace;
    QAction *m_actDivideUriFromZenkaku;
    QAction *m_actAvoidApiCommand;
    QMenu *m_iconMenu;
    QAction *m_actShowIconInBrowser;
    QAction *m_actSaveIcon;

    QSystemTrayIcon *m_trayIcon;
    QIcon *m_normalIcon;
    QIcon *m_errorIcon;
    QIcon *m_offlineIcon;
    QIcon *m_replyIcon;
    QIcon *m_unreadIcon;
    QList<QIcon *> m_busyIconList;

    QList<QString> m_inputHistory;

    UsersModel *m_usersModel;
    QSortFilterProxyModel *m_proxyModel;
    QCompleter *m_completer;

    //flags:
    //最初のshowEventかどうかを判断するフラグ
    bool m_firstShow;
    //短縮URI機能はEnterで発動する場合と、メニューから発動する場合がある。
    //Enterの時はPostしなければいけないので、その判断をするフラグ
    bool m_postAfterShorten;

    //DEBUG
    AbstractUriShortener *m_urisvc;
    XAuth *m_xauth;

    //アイコンのダウンロードに使う
    QNetworkAccessManager *m_networkMan;

    //最新のIDたち
    quint64 m_idAsUInt64;
    quint64 m_in_reply_to_status_id;
    quint64 m_newestFriendsStatus;
    quint64 m_newestRecvDM;
    quint64 m_newestSentDM;
    quint64 m_newestReply;
    quint64 m_newestFav;

    //詳細表示しているitem
    Twitter::TwitterItem m_detailItem;

    //最新の自発言
    QString m_latestMyPost;
    //ログインして最初の取得か？
    bool m_firstFetch;

public slots:
    void OnError(int role, const QString& msg);
    void OnExit();
    void OnHomeTimelineReceived(statuses_t& s);
    void OnVerifyCredentialsReceived(user_t& user);
    void OnSentDirectMessagesReceived(direct_messages_t& direct_messages);
    void OnDirectMessagesReceived(direct_messages_t& direct_messages);
    void OnUpdateReceived(status_t& status);
    void OnRateLimitStatusReceived(hash_t& hash);
    void OnMentionsReceived(statuses_t& statuses);
    void OnFavoritesReceived(statuses_t& statuses);
    void OnUserTimelineReceived(statuses_t& statuses);
    void OnShowFriendshipsReceived(relationship_t& relationship);
    void OnShowUserDetailsReceived(user_t& user);
    void OnCreateFriendshipReceived(user_t& user);
    void OnDestroyFriendshipReceived(user_t& user);
    void OnCreateFavoriteReceived(status_t& status);
    void OnDestroyFavoriteReceived(status_t& status);

    void OnItemSelected(const Twitter::TwitterItem &item);
    void OnPostModeMenuOpen();
    void OnUriShortened(const QString& src, const QString& dest);
    void OnIconDownloaded(quint64 userid, const QIcon& icon);
    void OnIconContextMenu(const QPoint &);
    void OnShowIconInBrowser();
    void OnSaveIcon();
    void OnIconOriginalImageDownloaded(QNetworkReply*);
    void OnMessageClicked();
    void OnIconActivated(QSystemTrayIcon::ActivationReason reason);
    void OnUriShorteningFinished();
    void OnActDivideUriFromZenkakuToggled(bool);
    void OnActAvoidApiCommandToggled(bool);
    void OnActAutoShortenUriToggled(bool);
    void OnActReplaceZenkakuSpaceToggled(bool);
    void OnFollowCommand(const QString& name);
    void OnRemoveCommand(const QString& name);
    void OnFriendshipCommand(const QString& name);

private slots:
    void on_actSetSelRead_triggered();
    void on_actSetSelUnread_triggered();
    void on_actReTweetUnofficial_triggered();
    void on_actionTest_xauth_triggered();
    void on_actUnu_triggered();
    void on_actTwurl_triggered();
    void on_actBitly_triggered();
    void on_actIsgd_triggered();
    void on_actTinyURL_triggered();
    void on_actionTest_rx_triggered();
    void on_actionTest_url_triggered();
    void on_actionTest_network_triggered();
    void on_actUnFavorite_triggered();
    void on_actFavorite_triggered();
    void on_actReTweet_triggered();
    void on_actSendDM_triggered();
    void on_actAtReply_triggered();
    void on_actTabSettings_triggered();
    void on_actTwitterNews_triggered();
    void on_actRenameTab_triggered();
    void on_actCreateTab_triggered();
    void on_actRemove_triggered();
    void on_actFollow_triggered();
    void on_actShowFriendships_triggered();
    void on_actExplosion_triggered();
    void on_actCopyIdUri_triggered();
    void on_actCopyStot_triggered();
    void on_actUpdate_triggered();
    void on_actionTest_icon_triggered();
    void on_actionTest_iconmanager_triggered();
    void on_statusText_returnPressed();
    void on_actionTest_bitly_triggered();
    void on_actQweenHomepage_triggered();
    void on_actApiInfo_triggered();
    void on_actShowUserStatus_triggered();
    void on_statusText_textChanged(QString );
    void on_postButton_clicked();
    void on_actAboutQween_triggered();
    void on_actAboutQt_triggered();
    void on_actOptions_triggered();
    void OnTimelineTimerTimeout();
    void OnDmTimerTimeout();
    void OnReplyTimerTimeout();
    void OnFavTimerTimeout();
};

#endif // QWEENMAINWINDOW_H
