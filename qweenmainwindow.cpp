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
//TODO: 全般的に：ステータスバーに表示をしませう
//TODO: アニメーション
//TODO: タブの並べ替えは振り分け設定ダイアログと連動
#include "qweenmainwindow.h"
#include "ui_qweenmainwindow.h"
#include "aboutdialog.h"
#include "qweensettings.h"
#include "qweentabctrl.h"
#include "settingdialog.h"
#include <QtCore>
#include <QtGui>
#include "urishortensvc.h"
#include "iconmanager.h"
#include "qweenapplication.h"
#include "timelineview.h"
#include "tabsettingsdialog.h"
#include "forwardruledialog.h"
#include "usersmodel.h"
#include "const.h"
#include "xauth.h"
#include "statusbrowser.h"

QweenMainWindow::QweenMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QweenMainWindow),m_firstShow(true),m_postAfterShorten(false),m_usersModel(NULL),
    m_completer(NULL), m_urisvc(NULL), m_xauth(NULL), m_networkMan(NULL), m_idAsUInt64(0), m_in_reply_to_status_id(0), m_newestFriendsStatus(0),m_newestRecvDM(0),m_newestSentDM(0),
    m_newestReply(0),m_newestFav(0)
{
    ui->setupUi(this);
    makeWidgets();
    makeConnections();

    setupMenus();

    setAcceptDrops(true);

    settings = QweenSettings::globalSettings();
    //ユーザーIDまたはパスワードが無いので設定ダイアログで入力してもらう
    if(settings->userid().isEmpty() || settings->password().isEmpty())
    {
        SettingDialog dlg(this);
        if(dlg.exec() != QDialog::Accepted ||
           settings->userid().isEmpty() || settings->password().isEmpty())
        {
            exit(-1); //入力されなかったので終了
        }else{
            applySettings();
        }
    }else{
        applySettings();
    }

    restoreGeometry(settings->geometry());
    restoreState(settings->windowState());
    ui->splitter->restoreState(settings->splitterState());

    //TODO: if(outOfScreen()){
        //画面内に戻す
    //}

    //setupWebview

    setupTrayIcon();
    setupTabs();
    setupTimers();
    setupTwitter();
}

QweenMainWindow::~QweenMainWindow()
{
    delete ui;
    m_trayIcon->hide();
    if(m_petrelLib)
        delete m_petrelLib;
}

void QweenMainWindow::makeWidgets(){
    m_petrelLib = new Petrel();

    m_timelineTimer = new QTimer(this);
    m_DMTimer = new QTimer(this);
    m_replyTimer = new QTimer(this);
    m_favTimer = new QTimer(this);
    m_fetchAnimTimer = new QTimer(this);

    m_trayIcon = new QSystemTrayIcon(this);

    m_postModeMenu = new QMenu(this);
    m_iconMenu = new QMenu(this);

    tabWidget = new QweenTabCtrl(ui->splitter);
    tabWidget->setTabPosition(QTabWidget::South);
    tabWidget->setFocusPolicy(Qt::NoFocus);
    ui->splitter->insertWidget(0,tabWidget);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 0);

    m_usersModel = new UsersModel(QweenApplication::iconManager(), this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setDynamicSortFilter(true);
    m_proxyModel->setSourceModel(m_usersModel);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->sort(0, Qt::AscendingOrder);

    m_completer = new QCompleter(m_proxyModel, this);
    ui->statusText->setCompleter(m_completer);

    m_networkMan = new QNetworkAccessManager(this);

    ui->lblNameId->setOpenExternalLinks(true);
}

void QweenMainWindow::applySettings(){
    ui->statusText->setStyleSheet(settings->inputStyle());
    ui->statusText->setRequireCtrlOnEnter(settings->requireCtrlOnEnter());
    int size = settings->iconSize()*8 + 8;
    if(size==8) size=0;
    for(int i=0;i<tabWidget->count();i++){
        TimelineView* view = tabWidget->timelineView(i);
        view->setIconSize(QSize(size,size));
    }
    tabWidget->setManageUnread(settings->manageUnread());
    updateWindowTitle();
    updateTrayIconTitle();
    setupTimers();
}

bool QweenMainWindow::isNetworkAvailable(){
    foreach(const QNetworkInterface &intf, QNetworkInterface::allInterfaces()) {
        if (!intf.isValid()) continue;
        if (intf.flags() & QNetworkInterface::IsLoopBack) continue;
        if (intf.hardwareAddress().isEmpty()) continue;
        if (intf.flags() & QNetworkInterface::IsUp && intf.flags() & QNetworkInterface::IsRunning ) return true;
    }
    return false;
}

void QweenMainWindow::save(){
  QFile tabSettings(QweenApplication::profileDir()+"/tabs.xml");
  tabSettings.open(QFile::WriteOnly);
  tabWidget->saveState(&tabSettings);

  settings->setGeometry(saveGeometry());
  settings->setWindowState(saveState());
  settings->setSplitterState(ui->splitter->saveState());
  settings->save();
}

void QweenMainWindow::setupMenus()
{
    //TODO: 実際にTreeView内でCtrl+Cが機能するようにする
    ui->actCopyStot->setText(ui->actCopyStot->text()+"\tCtrl+C");
    ui->actCopyIdUri->setText(ui->actCopyIdUri->text()+"\tCtrl+Shift+C");


    m_actDivideUriFromZenkaku = new QAction(QIcon(), tr("URLからの全角文字列の切り離し"), this);
    m_actDivideUriFromZenkaku->setCheckable(true);
    connect(m_actDivideUriFromZenkaku, SIGNAL(triggered(bool)),
            this, SLOT(OnActDivideUriFromZenkakuToggled(bool)));
    m_postModeMenu->addAction(m_actDivideUriFromZenkaku);

    m_actAvoidApiCommand = new QAction(QIcon(), tr("APIコマンドを回避する"), this);
    m_actAvoidApiCommand->setCheckable(true);
    connect(m_actAvoidApiCommand, SIGNAL(triggered(bool)), this, SLOT(OnActAvoidApiCommandToggled(bool)));
    m_postModeMenu->addAction(m_actAvoidApiCommand);

    m_actAutoShortenUri = new QAction(QIcon(), tr("自動的にURLを短縮する"), this);
    m_actAutoShortenUri->setCheckable(true);
    connect(m_actAutoShortenUri, SIGNAL(triggered(bool)), this, SLOT(OnActAutoShortenUriToggled(bool)));
    m_postModeMenu->addAction(m_actAutoShortenUri);

    m_actReplaceZenkakuSpace = new QAction(QIcon(), tr("全角スペースを半角スペースにする"), this);
    m_actReplaceZenkakuSpace->setCheckable(true);
    connect(m_actReplaceZenkakuSpace, SIGNAL(triggered(bool)), this, SLOT(OnActReplaceZenkakuSpaceToggled(bool)));
    m_postModeMenu->addAction(m_actReplaceZenkakuSpace);

    ui->postButton->setMenu(m_postModeMenu);

    m_actShowIconInBrowser = new QAction(QIcon(), tr("画像をブラウザで表示"), this);
    connect(m_actShowIconInBrowser, SIGNAL(triggered()),
            this, SLOT(OnShowIconInBrowser()));
    m_iconMenu->addAction(m_actShowIconInBrowser);

    m_actSaveIcon = new QAction(QIcon(), tr("アイコンを保存..."), this);
    connect(m_actSaveIcon, SIGNAL(triggered()),
            this, SLOT(OnSaveIcon()));
    m_iconMenu->addAction(m_actSaveIcon);

    ui->userIconLabel->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->userIconLabel, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(OnIconContextMenu(const QPoint &)));

}

void QweenMainWindow::setupTabs(){
    QFile tabSettings(QweenApplication::profileDir()+"/tabs.xml");
    if(tabSettings.exists()) tabWidget->restoreState(&tabSettings);
    else tabWidget->restoreState(NULL);
}

void QweenMainWindow::setupTimers(){
    m_timelineTimer->setInterval(settings->tlUpdateIntv()*1000);
    m_timelineTimer->start();

    m_DMTimer->setInterval(settings->dmUpdateIntv()*1000);
    m_DMTimer->start();
    //m_DMTimer->stop();

    m_replyTimer->setInterval(settings->replyUpdateIntv()*1000);
    m_replyTimer->start();
    //m_replyTimer->stop();

    m_favTimer->setInterval(600*1000);
    m_favTimer->start();
    //m_favTimer->stop();

    /*m_fetchAnimTimer->setInterval(85);
    m_fetchAnimTimer->start();*/
}

void QweenMainWindow::setupTrayIcon(){
    m_trayIcon->setIcon(QIcon(":/res/qween_icon.png"));
    setWindowIcon(QIcon(":/res/qween_icon.png"));
    m_trayIcon->setContextMenu(ui->menu_File);
    m_trayIcon->show();
}

void QweenMainWindow::setupTwitter(){
    m_petrelLib->abort();
    //m_twitLib->Logout(); TODO: EndSessionで置き換える
    m_firstFetch = settings->markAsRead1stFetch();
    if(settings->useXAuth() && !settings->token().isEmpty()){
        m_petrelLib->setToken(settings->token(),settings->tokenSecret());
    }else{
        m_petrelLib->setLoginInfo(settings->userid(), settings->password(),settings->useXAuth());
    }
}

void QweenMainWindow::makeConnections(){
    //MainMenu
    connect(ui->actExit, SIGNAL(triggered()),
            this, SLOT(OnExit()));
    connect(ui->actJumpToUnread, SIGNAL(triggered()),
            tabWidget, SLOT(jumpToUnread()));
    //PostMode
    connect(m_postModeMenu, SIGNAL(aboutToShow()),
            this, SLOT(OnPostModeMenuOpen()));
    //StatusText->postButton
    connect(ui->statusText,SIGNAL(returnPressed()),
            ui->postButton,SLOT(click()));

    //StatusBrowser
    connect(ui->textBrowser,SIGNAL(followCommand(QString)),
            this, SLOT(OnFollowCommand(QString)));
    connect(ui->textBrowser,SIGNAL(removeCommand(QString)),
            this, SLOT(OnRemoveCommand(QString)));
    connect(ui->textBrowser,SIGNAL(friendshipCommand(QString)),
            this, SLOT(OnFriendshipCommand(QString)));

    //TrayIcon
    connect(m_trayIcon, SIGNAL(messageClicked()), this, SLOT(OnMessageClicked()));
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(OnIconActivated(QSystemTrayIcon::ActivationReason)));

    //Timers
    connect(m_timelineTimer, SIGNAL(timeout()), this, SLOT(OnTimelineTimerTimeout()));
    connect(m_DMTimer, SIGNAL(timeout()), this, SLOT(OnDmTimerTimeout()));
    connect(m_replyTimer, SIGNAL(timeout()), this, SLOT(OnReplyTimerTimeout()));
    connect(m_favTimer, SIGNAL(timeout()), this, SLOT(OnFavTimerTimeout()));

    //Twitter
    connect(m_petrelLib, SIGNAL(homeTimelineReceived(statuses_t&)),
            this, SLOT(OnHomeTimelineReceived(statuses_t&)));
    connect(m_petrelLib, SIGNAL(verifyCredentialsReceived(user_t&)),
            this, SLOT(OnVerifyCredentialsReceived(user_t&)));
    connect(m_petrelLib, SIGNAL(sentDirectMessagesReceived(direct_messages_t&)),
            this, SLOT(OnSentDirectMessagesReceived(direct_messages_t&)));
    connect(m_petrelLib, SIGNAL(directMessagesReceived(direct_messages_t&)),
            this, SLOT(OnDirectMessagesReceived(direct_messages_t&)));
    connect(m_petrelLib, SIGNAL(updateReceived(status_t&)),
            this, SLOT(OnUpdateReceived(status_t&)));
    connect(m_petrelLib, SIGNAL(rateLimitStatusReceived(hash_t&)),
            this, SLOT(OnRateLimitStatusReceived(hash_t&)));
    connect(m_petrelLib, SIGNAL(mentionsReceived(statuses_t&)),
            this, SLOT(OnMentionsReceived(statuses_t&)));
    connect(m_petrelLib, SIGNAL(userTimelineReceived(statuses_t&)),
            this, SLOT(OnUserTimelineReceived(statuses_t&)));
    connect(m_petrelLib, SIGNAL(showFriendshipsReceived(relationship_t&)),
            this, SLOT(OnShowFriendshipsReceived(relationship_t&)));
    connect(m_petrelLib, SIGNAL(showUsersReceived(user_t&)),
            this, SLOT(OnShowUserDetailsReceived(user_t&)));
    connect(m_petrelLib, SIGNAL(createFriendshipReceived(user_t&)),
            this, SLOT(OnCreateFriendshipReceived(user_t&)));
    connect(m_petrelLib, SIGNAL(destroyFriendshipReceived(user_t&)),
            this, SLOT(OnDestroyFriendshipReceived(user_t&)));
    connect(m_petrelLib, SIGNAL(createFavoriteReceived(status_t&)),
            this, SLOT(OnCreateFavoriteReceived(status_t&)));
    connect(m_petrelLib, SIGNAL(destroyFavoriteReceived(status_t&)),
            this, SLOT(OnDestroyFavoriteReceived(status_t&)));
    connect(m_petrelLib, SIGNAL(favoritesReceived(statuses_t&)),
            this, SLOT(OnFavoritesReceived(statuses_t&)));
    connect(m_petrelLib, SIGNAL(error(int,QString)),
            this, SLOT(OnError(int,QString)));

    //Tab
    connect(tabWidget, SIGNAL(itemSelected(Twitter::TwitterItem)),
            this, SLOT(OnItemSelected(Twitter::TwitterItem)));
    connect(tabWidget, SIGNAL(favorite()),
            this, SLOT(on_actFavorite_triggered()));
    connect(tabWidget, SIGNAL(reply()),
            this, SLOT(on_actAtReply_triggered()));
    connect(tabWidget, SIGNAL(dm()),
            this, SLOT(on_actSendDM_triggered()));
    //StatusText
    connect(ui->statusText, SIGNAL(uriShorteningFinished()),
            this, SLOT(OnUriShorteningFinished()));

    //Icon
    connect(m_networkMan, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(OnIconOriginalImageDownloaded(QNetworkReply*)));
}

void QweenMainWindow::OnExit()
{
    save();
    QweenApplication::exit();
}

void QweenMainWindow::OnHomeTimelineReceived(statuses_t& s){
    if(s.status.count()==0) return;
    QString popupText;
    QString title(tr("新着 ") + QString::number(s.status.count()) + tr("件\n"));
    if(settings->showUserInTitle()) title.prepend(m_petrelLib->userid()+" - ");
    foreach(QSharedPointer<status_t> ptr, s.status){
        Twitter::TwitterItem item(Twitter::Status, ptr, HOME_TIMELINE, false);
        if(m_firstFetch) item.setRead(true);
        if(m_newestFriendsStatus < item.id()) m_newestFriendsStatus = item.id();
        switch(settings->notifyBaloonName()){
        case 0:
            popupText.append(QString("%1\n").arg(item.status()));
            break;
        case 1:
            popupText.append(QString("%1 : %2\n").arg(item.screenName(), item.status()));
            break;
        case 2:
            popupText.append(QString("%1 : %2\n").arg(item.userName(), item.status()));
            break;
        }
        if(!m_usersModel->userExists(item.userId()))
            m_usersModel->appendItem(item);
        tabWidget->addItem(item);
    }
    m_firstFetch = false;
    updateWindowTitle();
    //TODO: dm, reply, sound
    //バルーン・サウンドは最初は抑制するようだ
    //設定項目があるのでそこを見るべし
    if(settings->notifyOnlyMinimized() && !isMinimized() && isVisible()) return;
    m_trayIcon->showMessage(title, popupText, QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information),
                            5 * 1000);
}

void QweenMainWindow::OnVerifyCredentialsReceived(user_t& user){
    if(user.id!=0){
        settings->setToken(m_petrelLib->token());
        settings->setTokenSecret(m_petrelLib->tokenSecret());
        tabWidget->setMyId(user.id);
        m_idAsUInt64 = user.id;
        m_petrelLib->homeTimeline(m_newestFriendsStatus,0,20,0);
        OnDmTimerTimeout();
        OnReplyTimerTimeout();
        OnFavTimerTimeout();
    }
}

void QweenMainWindow::OnSentDirectMessagesReceived(direct_messages_t& direct_messages){
    foreach(QSharedPointer<direct_message_t> ptr, direct_messages.direct_message){
        Twitter::TwitterItem item(Twitter::DirectMessage, ptr, SENT_DIRECT_MESSAGES, false);
        if(m_newestSentDM < item.id()) m_newestSentDM = item.id();
        tabWidget->addItem(item);
    }
}

void QweenMainWindow::OnDirectMessagesReceived(direct_messages_t& direct_messages){
    foreach(QSharedPointer<direct_message_t> ptr, direct_messages.direct_message){
        Twitter::TwitterItem item(Twitter::DirectMessage, ptr, DIRECT_MESSAGES, false);
        if(m_newestRecvDM < item.id()) m_newestRecvDM = item.id();
        if(!m_usersModel->userExists(item.userId()))
            m_usersModel->appendItem(item);
        tabWidget->addItem(item);
    }
}

void QweenMainWindow::OnUpdateReceived(status_t& status){
    ui->statusText->setText("");
    ui->statusText->setEnabled(true);
    ui->postButton->setEnabled(true);
    m_in_reply_to_status_id = 0;
    m_latestMyPost = status.text;
    QSharedPointer<status_t> s(new status_t(status));
    tabWidget->addItem(Twitter::TwitterItem(Twitter::Status, s, UPDATE, false));
    updateWindowTitle();
}

void QweenMainWindow::OnRateLimitStatusReceived(hash_t& hash){
    QMessageBox::information(this,tr("API情報"),
                             tr("上限: %1\n残数: %2\nリセット日時: %3\n")
                             .arg(hash.hourly_limit,
                                  hash.remaining_hits,
                                  hash.reset_time));
}


void QweenMainWindow::OnMentionsReceived(statuses_t& s){
    foreach(QSharedPointer<status_t> ptr, s.status){
        Twitter::TwitterItem item(Twitter::Status, ptr, MENTIONS, false);
        if(m_newestReply < item.id()) m_newestReply = item.id();
        if(!m_usersModel->userExists(item.userId()))
            m_usersModel->appendItem(item);
        tabWidget->addItem(item);
    }
}

void QweenMainWindow::OnFavoritesReceived(statuses_t& s){
    foreach(QSharedPointer<status_t> ptr, s.status){
        Twitter::TwitterItem item(Twitter::Status, ptr, FAVORITES, false);
        if(m_newestFav < item.id()) m_newestFav = item.id();
        if(!m_usersModel->userExists(item.userId()))
            m_usersModel->appendItem(item);
        tabWidget->addItem(item);
    }
}

void QweenMainWindow::OnUserTimelineReceived(statuses_t& s){
    QSharedPointer<status_t> st = s.status.takeFirst();
    if(!st.isNull())
        QMessageBox::information(this,tr("@twj の最新のTweet"),st->text);
}

void QweenMainWindow::OnShowFriendshipsReceived(relationship_t& r){
    QString arrow;
    QString msg;
    if(r.source->followed_by && r.source->following){
        arrow = r.source->screen_name + " <-> " + r.target->screen_name;
    }else if(r.source->followed_by){
        arrow = r.source->screen_name + " <- " + r.target->screen_name;
    }else if(r.source->following){
        arrow = r.source->screen_name + " -> " + r.target->screen_name;
    }else{
        QMessageBox::information(this, tr("友達関係"),
                                 tr("フォロー関係はありません。"));
        return;
    }
    QMessageBox::information(this, tr("友達関係"), arrow);
}

void QweenMainWindow::OnShowUserDetailsReceived(user_t& user){
    QMessageBox::information(this, tr("プロファイル情報"),
                             tr("Following : %1\n"
                                "Followers : %2\n"
                                "Statuses count : %3\n"
                                "Location : %4\n"
                                "Bio : %5")
                             .arg(QString::number(user.friends_count),
                                  QString::number(user.followers_count),
                                  QString::number(user.statuses_count),
                                  user.location,
                                  user.description));
}

void QweenMainWindow::OnCreateFriendshipReceived(user_t& user){
    if(!user.screen_name.isEmpty())
        QMessageBox::information(this, "Follow", tr("@%1 をFollow開始しました。").arg(user.screen_name));
    //TODO: エラーの場合は失敗とちゃんと言う！
}

void QweenMainWindow::OnDestroyFriendshipReceived(user_t& user){
    if(!user.screen_name.isEmpty())
        QMessageBox::information(this, "Remove", tr("@%1 をRemoveしました。").arg(user.screen_name));
}

void QweenMainWindow::OnCreateFavoriteReceived(status_t& status){
   quint64 id = status.id;
   tabWidget->favorited(id,true);
}

void QweenMainWindow::OnDestroyFavoriteReceived(status_t& status){
   quint64 id = status.id;
   tabWidget->favorited(id,false);
}

void QweenMainWindow::OnError(int role, const QString& msg){
    switch(role){
    case UPDATE:
      {
        ui->statusText->setEnabled(true);
        ui->postButton->setEnabled(true);
        break;
      }
    default:
        QMessageBox::information(this,"error",msg);
        break;
    }
}

void QweenMainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    case QEvent::WindowStateChange:
        if(settings->minimizeToTray() && isMinimized() && isVisible()){
            hide();
        }
        break;
    default:
        break;
    }
}

void QweenMainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    if (settings->minimizeOnClose()) {
         hide();
         event->ignore();
    }else{
        save();
  }
}

void QweenMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
        event->acceptProposedAction();
}

void QweenMainWindow::dropEvent(QDropEvent *event)
{
    ui->statusText->setText(ui->statusText->text() + event->mimeData()->text());
    event->acceptProposedAction();
}

void QweenMainWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    if(isNetworkAvailable() && m_firstShow){
        //m_petrelLib->verifyCredentials();
        m_firstShow = false;
    //TODO: version check
    /*
    if(settings->checkVersionOnStartup()){
        checkNewVersion();
    }
    */
    }
}

void QweenMainWindow::on_actOptions_triggered()
{
    SettingDialog dlg(this);
    //bool chgUseApi = false;
    if(dlg.exec() == QDialog::Accepted){
        applySettings();

        if(dlg.loginInfoChanged()){
            setupTwitter();
            //m_petrelLib->verifyCredentials();
        }
    }
}

void QweenMainWindow::on_actAboutQt_triggered()
{
    QMessageBox::aboutQt(this);
}

void QweenMainWindow::on_actAboutQween_triggered()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void QweenMainWindow::doPost(){
    QString postText = ui->statusText->text().trimmed();
    if(postText.isEmpty()){
        //TODO: refresh();
        return;
    }
    ui->statusText->setEnabled(false);
    ui->postButton->setEnabled(false);
    m_petrelLib->update(postText + tr(" ") + settings->statusSuffix(),m_in_reply_to_status_id,"",""); // TODO:クライアント名"Qween"を付加 OAuth対応後
    //TODO: Outputz対応?
}

void QweenMainWindow::postOutputz(const QString& str){
    QString entryPoint("http://outputz.com/api/post");
    /*
     * key : 設定ページに表示されている復活の呪文（他人に知られていはいけない）
     * uri : アウトプットの URI
     * size : アウトプットの文字数
     */
    QString key;
    QString uri("http://twitter.com/");
    int size = str.length();
}

void QweenMainWindow::makeReplyOrDirectStatus(bool isAuto, bool isReply, bool isAll){
    Q_UNUSED(isAuto)
    Q_UNUSED(isReply)
    Q_UNUSED(isAll)
}

void QweenMainWindow::OnUriShorteningFinished(){
    if(m_postAfterShorten){
        m_postAfterShorten = false;
        doPost();
    }
}

void QweenMainWindow::on_postButton_clicked()
{
    if(settings->uriAutoShorten()){
        m_postAfterShorten = true;
        ui->statusText->shortenUri();
    }else{
        doPost();
    }

}

void QweenMainWindow::OnTimelineTimerTimeout()
{
    //TODO: if not IsNetworkAvailable() exit
    m_petrelLib->homeTimeline(m_newestFriendsStatus,0,200,0);
}

void QweenMainWindow::OnDmTimerTimeout(){
    //TODO: if not IsNetworkAvailable() exit
    m_petrelLib->sentDirectMessages(m_newestSentDM,0,0,1);
    m_petrelLib->directMessages(m_newestRecvDM,0,0,1);
}

void QweenMainWindow::OnReplyTimerTimeout(){
    //TODO: if not IsNetworkAvailable() exit
    m_petrelLib->mentions(m_newestReply,0,0,0);
}

void QweenMainWindow::OnFavTimerTimeout(){
    //TODO: if not IsNetworkAvailable() exit
    m_petrelLib->favorites(0,0);
}

void QweenMainWindow::OnItemSelected(const Twitter::TwitterItem &item)
{
    m_detailItem = item;
    QString status(item.status());
    QRegExp linkrx(LINK_RX_DATA);
    int pos=0;
    while ((pos = linkrx.indexIn(status, pos)) != -1) {
        QStringList list = linkrx.capturedTexts();
        QString str;
        QString anchor;
        int length;
        if (list[1] != ""){ //hashtag
            str = list[1];
            if (str.at(0) != '#'){
                str.remove(0,1);
                pos++;
            }
            QString str2 = str;
            str2.remove(0,1);
            QUrl url("http://twitter.com/");
            url.setFragment("search?q=%23"+str2);
            anchor = QString("<a href=\"%2\">%1</a>")
                     .arg(str,url.toString());
            length = str.length();
        }
        else if(list[2] != ""){ //reply
            str = list[2];
            if (str.at(0) == '@'){
                str.remove(0,1);
                pos++;
            }else{
                str.remove(0,2);
                pos+=2;
            }
            anchor = QString("<a href=\"http://twitter.com/%1\">%1</a>").arg(str);
            length = str.length();
        }
        else if(list[3] != ""){ //URI
            str = list[3];
            anchor = QString("<a href=\"%1\">%1</a>").arg(str);
            length = str.length();
        }
        status.replace(pos, length, anchor);
        pos += anchor.length();
    }
    switch(item.type()){
    case Twitter::Status:
    {
        if(item.isProtected()){
            status.prepend("<img src=\":/res/lock.png\">");
        }
        ui->lblNameId->setText(QString("<a href=\"http://twitter.com/%0\">%0</a>/%1").arg(item.screenName()).arg(item.userName()));
        ui->lblUpdateDatetime->setText(item.createdAt().toString());
        break;
    }
    case Twitter::DirectMessage:
    {
        ui->lblNameId->setText("DM: " + item.screenName() + " -> " + item.replyTo());
        ui->lblUpdateDatetime->setText(item.createdAt().toString());
        break;
    }
    default:
        break;
    }
    ui->textBrowser->setHtml(tr("<html><body style=\"%1\">")
                             .arg(settings->statusViewStyle()) +
                             status + tr("</body></html>"));
    if(QweenApplication::iconManager()->isIconAvailable(item.userId())){
        QIcon icon(QweenApplication::iconManager()->getIcon(item.userId()));
        ui->userIconLabel->setPixmap(icon.pixmap(50,50,QIcon::Normal,QIcon::On));
    }else{
        ui->userIconLabel->setPixmap(QPixmap(50,50));
        QweenApplication::iconManager()->fetchIcon(item.userId(), item.iconUri());
    }
    ui->userIconLabel->repaint();
    updateWindowTitle();
}

void QweenMainWindow::OnPostModeMenuOpen(){
    m_actAutoShortenUri->setChecked(settings->uriAutoShorten());
    m_actAvoidApiCommand->setChecked(settings->avoidApiCmd());
    m_actDivideUriFromZenkaku->setChecked(settings->divideUriFromZenkaku());
    m_actReplaceZenkakuSpace->setChecked(settings->replaceZenkakuSpace());
}

void QweenMainWindow::OnUriShortened(const QString& src, const QString& dest){
    Q_UNUSED(src)
    QMessageBox::information(this, "", dest);
}

void QweenMainWindow::OnIconDownloaded(quint64 userid, const QIcon &icon){
    Q_UNUSED(userid)
    ui->userIconLabel->setPixmap(icon.pixmap(50,50,QIcon::Normal,QIcon::On));
}

void QweenMainWindow::OnIconContextMenu(const QPoint &pt){
    m_iconMenu->exec(ui->userIconLabel->mapToGlobal(pt));
}

void QweenMainWindow::OnShowIconInBrowser(){
    if(m_detailItem.type()!=Twitter::Undefined){
        QString uri(m_detailItem.iconUri());
        QRegExp rx("_normal(\\..+)$");
        if(rx.indexIn(uri,0)>=0){
            uri.replace(rx,rx.cap(1));
            QDesktopServices::openUrl(QUrl(uri));
        }
    }
}

void QweenMainWindow::OnSaveIcon(){
    if(m_detailItem.type()!=Twitter::Undefined){
        QString uri(m_detailItem.iconUri());
        QRegExp rx("_normal(\\..+)$");
        if(rx.indexIn(uri,0)>=0){
            uri.replace(rx,rx.cap(1));
            int pos = uri.lastIndexOf('/');
            QString fileName = QFileDialog::getSaveFileName(this, tr("ファイルを保存"),
                                                              uri.right(pos+1),
                                                              tr("すべてのファイル (*.*)"));
            QNetworkRequest req(uri);
            req.setAttribute(QNetworkRequest::User, fileName);
            m_networkMan->get(req);
        }
    }
}

void QweenMainWindow::OnIconOriginalImageDownloaded(QNetworkReply* r){
    QFile file(r->request().attribute(QNetworkRequest::User).toString());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
             return;
    file.write(r->readAll());
    file.close();
}

void QweenMainWindow::OnMessageClicked(){
    //FIXME: X11環境だと動かないことがある？
    this->raise();
    this->activateWindow();
}

void QweenMainWindow::OnIconActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch (reason) {
     case QSystemTrayIcon::Trigger:
     case QSystemTrayIcon::DoubleClick:
        if(!this->isVisible())
            this->setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
            this->show();
         break;
     case QSystemTrayIcon::MiddleClick:
         break;
     default:
         ;
  }
}

void QweenMainWindow::on_statusText_textChanged(QString string)
{
    Q_UNUSED(string)
    int rest = getRestStatusCount(ui->statusText->text().trimmed());
    ui->lblStatusLength->setText(QString("%1").arg(rest));
    if(rest < 0){ 
        ui->statusText->setStyleSheet(settings->inputStyle()+" *{color:rgb(255,0,0);}");
    }else{
        ui->statusText->setStyleSheet(settings->inputStyle());
    }
}

int QweenMainWindow::getRestStatusCount(const QString &str, bool footer)
{
    int rv = 140 - str.length();
    if(footer)
        rv -= settings->statusSuffix().length()+1;
    if(settings->avoidApiCmd()){

    }

    if(settings->replaceZenkakuSpace()){

    }

    if(settings->divideUriFromZenkaku()){

    }

    //TODO: フッタ機能と連動
    //TODO: Shiftキー
    //詳しくはTweenのソースを検索 GetRestStatusCount
    return rv;
}

void QweenMainWindow::on_actShowUserStatus_triggered()
{
    m_petrelLib->showUsers(m_idAsUInt64,0,"");
}

void QweenMainWindow::on_actApiInfo_triggered()
{
    m_petrelLib->rateLimitStatus();
}

void QweenMainWindow::on_actQweenHomepage_triggered()
{
    //TODO: ブラウザを設定できるようにする
    QDesktopServices::openUrl(QUrl("http://qween.tnose.net/"));
}

void QweenMainWindow::on_actionTest_bitly_triggered()
{
    if(m_urisvc) delete m_urisvc;
    QInputDialog dlg(this);
    dlg.setOption(QInputDialog::UseListViewForComboBoxItems, true);
    dlg.setComboBoxEditable(false);
    QStringList items;
    items << tr("bitly") << tr("tinyurl") << tr("twurl") << tr("isgd") << tr("unu");
    dlg.setComboBoxItems(items);
    dlg.setWindowTitle("test uri shortener");
    dlg.setLabelText("Services:");
    if (dlg.exec()==QDialog::Accepted && !dlg.textValue().isEmpty()){
        m_urisvc = getUriShortenService(dlg.textValue(), this);
        if(m_urisvc){
            connect(m_urisvc, SIGNAL(uriShortened(QString,QString)),
                    this, SLOT(OnUriShortened(QString,QString)));
            m_urisvc->shortenAsync("http://is2008.is-a-geek.org/");
        }
    }
}

void QweenMainWindow::on_statusText_returnPressed()
{
    //TODO: 複数行対応？やりたくねー
}

void QweenMainWindow::on_actionTest_iconmanager_triggered()
{
    QMessageBox::information(this, "", QString("%1").arg(255,0,16));
}

void QweenMainWindow::on_actionTest_icon_triggered()
{
    connect(QweenApplication::iconManager(),SIGNAL(iconDownloaded(quint64,QIcon)),
            this,SLOT(OnIconDownloaded(quint64,QIcon)));
    QweenApplication::iconManager()->fetchIcon(1261519751, "http://a1.twimg.com/profile_images/525002820/CIMG0272_bigger.JPG");
}

void QweenMainWindow::on_actUpdate_triggered()
{
    m_petrelLib->homeTimeline(m_newestFriendsStatus,0,20,0);
    //TODO: DM だったら SentDM/ReceivedDMを更新, Favも同様
}

void QweenMainWindow::on_actCopyStot_triggered()
{
    //TODO: 複数選択可能にする
    Twitter::TwitterItem item = tabWidget->currentItem();
    if(item.type()==Twitter::Undefined) return;
    //TODO: Protectedならコピーしない設定を追加
    //TODO: ReTweet対応
    QString data = "%0:%1 [http://twitter.com/%0/status/%2]";
    QString dataRT = "%0:%1 [http://twitter.com/%2/status/%3]";
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(data.arg(item.screenName(), item.status(), QString::number(item.id())), QClipboard::Clipboard);
}

void QweenMainWindow::on_actCopyIdUri_triggered()
{
    //TODO: 複数選択可能にする
    Twitter::TwitterItem item = tabWidget->currentItem();
    if(item.type()==Twitter::Undefined) return;
    //TODO: Protectedならコピーしない設定を追加
    //TODO: ReTweet対応
    QString data = "http://twitter.com/%0/status/%1";
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(data.arg(item.screenName(), QString::number(item.id())), QClipboard::Clipboard);
}

void QweenMainWindow::on_actExplosion_triggered()
{
    QMessageBox::information(this, tr("ゴルァ"), tr("だからやめれっての"));
}

void QweenMainWindow::OnActDivideUriFromZenkakuToggled(bool val){
    settings->setDivideUriFromZenkaku(val);
}

void QweenMainWindow::OnActAvoidApiCommandToggled(bool val){
    settings->setAvoidApiCmd(val);
}

void QweenMainWindow::OnActAutoShortenUriToggled(bool val){
    settings->setUriAutoShorten(val);
}

void QweenMainWindow::OnActReplaceZenkakuSpaceToggled(bool val){
    settings->setReplaceZenkakuSpace(val);
}

void QweenMainWindow::on_actShowFriendships_triggered()
{
    QString name = tabWidget->currentItem().screenName();
    doFriendshipCommand(name);
}

void QweenMainWindow::on_actFollow_triggered()
{
    QString name = tabWidget->currentItem().screenName();
    doFollowCommand(name);
}

void QweenMainWindow::on_actRemove_triggered()
{
    QString name = tabWidget->currentItem().screenName();
    doRemoveCommand(name);
}

void QweenMainWindow::on_actCreateTab_triggered()
{
    QString name = QString("NewTab%1").arg(tabWidget->count());
    bool ok;
    QString rv = QInputDialog::getText(this, tr("新規タブ"), tr("タブ名を入力してください"), QLineEdit::Normal, name, &ok);
    if(ok){
        tabWidget->addTimelineView(rv);
    }
}

void QweenMainWindow::on_actRenameTab_triggered()
{
    TimelineView *view = tabWidget->currentTimelineView();
    bool ok;
    QString rv = QInputDialog::getText(this, tr("名前変更"), tr("名前を入力してください"), QLineEdit::Normal, view->title(), &ok);
    if(ok){
        view->setTitle(rv);
        tabWidget->setTabText(tabWidget->indexOf(view), rv);
    }
}

void QweenMainWindow::on_actTwitterNews_triggered()
{
    m_petrelLib->userTimeline(0,0,"twj",0,0,0,0);
}

void QweenMainWindow::on_actTabSettings_triggered()
{
    TabSettingsDialog dlg(tabWidget);
    dlg.exec();
}

void QweenMainWindow::on_actAtReply_triggered()
{
    //stub.
    //TODO: なにやら複雑な処理
    ui->statusText->setCursorPosition(0);
    ui->statusText->insert("@"+tabWidget->currentItem().screenName()+" ");
    ui->statusText->setFocus();
    m_in_reply_to_status_id = tabWidget->currentItem().id();
}

void QweenMainWindow::on_actSendDM_triggered()
{
    //stub.
    ui->statusText->setCursorPosition(0);
    ui->statusText->insert("D "+tabWidget->currentItem().screenName()+" ");
    ui->statusText->setFocus();
}

void QweenMainWindow::on_actReTweet_triggered()
{
    //TODO: Protectedの場合は拒否
    Twitter::TwitterItem item =tabWidget->currentItem();
    if(item.type()==Twitter::Status)
        m_petrelLib->retweet(item.id());
}

void QweenMainWindow::on_actFavorite_triggered()
{
    Twitter::TwitterItem item =tabWidget->currentItem();
    //TODO: RTの場合はretweeted_statusのidを使う
    if(item.type()==Twitter::Status)
        m_petrelLib->createFavorite(item.id());
}

void QweenMainWindow::on_actUnFavorite_triggered()
{
    Twitter::TwitterItem item =tabWidget->currentItem();
    if(item.type()==Twitter::Status)
        m_petrelLib->destroyFavorite(item.id());
}

void QweenMainWindow::on_actionTest_network_triggered()
{
    if(isNetworkAvailable())
        QMessageBox::information(this,"",tr("Network is available"));
    else
        QMessageBox::information(this,"",tr("Network is not available"));
}

void QweenMainWindow::on_actionTest_url_triggered()
{
    QUrl url("http://mozilla.org/query.cgi");
    url.addQueryItem("numa",tr("++++と&と のテスト"));
    QMessageBox::information(this,"",url.encodedQuery());
    QString tmp(url.encodedQuery());
    tmp.replace('+',"%2B");
    QMessageBox::information(this,"",tmp.toAscii());
}

void QweenMainWindow::on_actionTest_rx_triggered()
{
    QRegExp rx1(ATREPLY_RX_DATA_2);

    QString hoge("@mozilla @mozilla/enemy mozilla");
    hoge.indexOf(rx1);
    QStringList list;
    for(int i=0;i<rx1.captureCount()+1;i++){
        list << rx1.cap(i);
    }
    QMessageBox::information(this,"",QString::number(rx1.captureCount(),10) + ":" + list.join(" & "));
}

void QweenMainWindow::on_actTinyURL_triggered()
{
    ui->statusText->shortenUri("tinyurl");
}

void QweenMainWindow::on_actIsgd_triggered()
{
    ui->statusText->shortenUri("isgd");
}

void QweenMainWindow::on_actBitly_triggered()
{
    ui->statusText->shortenUri("bitly");
}

void QweenMainWindow::on_actTwurl_triggered()
{
    ui->statusText->shortenUri("twurl");
}

void QweenMainWindow::on_actUnu_triggered()
{
    ui->statusText->shortenUri("unu");
}

void QweenMainWindow::on_actionTest_xauth_triggered()
{
    //残骸 TODO:削除?
}

void QweenMainWindow::OnFollowCommand(const QString& name){
    doFollowCommand(name);
}

void QweenMainWindow::OnRemoveCommand(const QString& name){
    doRemoveCommand(name);
}

void QweenMainWindow::OnFriendshipCommand(const QString& name){
    doFriendshipCommand(name);
}

void QweenMainWindow::doFollowCommand(const QString& name){
    bool ok;
    QString rv = QInputDialog::getText(this, tr("Follow"), tr("IDを入力してください"), QLineEdit::Normal, name, &ok);
    if(ok){
        m_petrelLib->createFriendship(0,0,rv,false);
    }
}

void QweenMainWindow::doRemoveCommand(const QString& name){
    bool ok;
    QString rv = QInputDialog::getText(this, tr("Follow"), tr("IDを入力してください"), QLineEdit::Normal, name, &ok);
    if(ok){
        m_petrelLib->destroyFriendship(0,0,rv);
    }
}

void QweenMainWindow::doFriendshipCommand(const QString& name){
    bool ok;
    QString rv = QInputDialog::getText(this, tr("フォロー関係を調べる"), tr("IDを入力してください"), QLineEdit::Normal, name, &ok);
    if(ok){
        m_petrelLib->showFriendships(0,settings->userid(),0,rv);
    }
}

void QweenMainWindow::updateWindowTitle(){
    QString t;
    if(settings->showUserInTitle() && !settings->userid().isEmpty()){
        t.append(settings->userid()+" - ");
    }
    t.append("Qween");
    if(settings->showWhatInTitle()!=0){
        t.append(" ");
        switch(settings->showWhatInTitle()){
        case 1:
            t.append(QString("%0.%1.%2").arg(QWEEN_VERSION_MAJOR).arg(QWEEN_VERSION_MINOR).arg(QWEEN_VERSION_REV));
            break;
        case 2:
            t.append(m_latestMyPost);
            break;
        case 3: // at unread
            if(!tabWidget->replyView() ||
               !tabWidget->replyView()->model()) break;
            t.append(QString::number(tabWidget->replyView()->model()->unreadCount()));
            break;
        case 4: // unread
            if(!tabWidget->homeView() ||
               !tabWidget->homeView()->model()) break;
            t.append(QString::number(tabWidget->homeView()->model()->unreadCount()));
            break;
        case 5: // unread(at unread)
            if(!tabWidget->homeView() ||
               !tabWidget->replyView() ||
               !tabWidget->homeView()->model() ||
               !tabWidget->replyView()->model()) break;
            t.append(QString("%0(%1)")
                     .arg(tabWidget->homeView()->model()->unreadCount())
                     .arg(tabWidget->replyView()->model()->unreadCount()));
            break;
        case 6: // unread / all
            if(!tabWidget->homeView() ||
               !tabWidget->homeView()->model()) break;
            t.append(QString("%0 / %1")
                     .arg(tabWidget->homeView()->model()->unreadCount())
                     .arg(tabWidget->homeView()->model()->count()));
            break;
        }
    }
    this->setWindowTitle(t);
}

void QweenMainWindow::updateTrayIconTitle(){
    QString t;
    if(settings->showUserInTitle() && !settings->userid().isEmpty()){
        t.append(m_petrelLib->userid()+" - ");
    }
    t.append("Qween");
    m_trayIcon->setToolTip(t);
}

void QweenMainWindow::on_actReTweetUnofficial_triggered()
{
    ui->statusText->setText("RT @"+tabWidget->currentItem().screenName()+": "+tabWidget->currentItem().status());
    ui->statusText->setCursorPosition(0);
    ui->statusText->setFocus();
}

void QweenMainWindow::on_actSetSelUnread_triggered()
{
    tabWidget->setRead(false);
}

void QweenMainWindow::on_actSetSelRead_triggered()
{
    tabWidget->setRead(true);
}
