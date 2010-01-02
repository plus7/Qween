#include "qweenmainwindow.h"
#include "ui_qweenmainwindow.h"
#include "aboutdialog.h"
#include "qweensettings.h"
#include "qweentabctrl.h"
#include "settingdialog.h"
#include "QTwitLib.h"
#include <QtCore>
#include <QtGui>
#include "urishortensvc.h"
#include "iconmanager.h"
#include "qweenapplication.h"
#include "timelineview.h"
#include "tabsettingsdialog.h"
#include "forwardruledialog.h"
#include "usersmodel.h"

QweenMainWindow::QweenMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QweenMainWindow),m_firstShow(true),m_postAfterShorten(false),m_urisvc(NULL),m_usersModel(NULL),
    m_completer(NULL), m_newestFriendsStatus(0),m_newestRecvDM(0),m_newestSentDM(0),m_newestReply(0),m_newestFav(0)
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
    //TODO: splitterを復元
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
    if(m_twitLib)
        delete m_twitLib;
}

void QweenMainWindow::makeWidgets(){
    m_twitLib = new QTwitLib();

    m_timelineTimer = new QTimer(this);
    m_DMTimer = new QTimer(this);
    m_replyTimer = new QTimer(this);
    m_favTimer = new QTimer(this);
    m_fetchAnimTimer = new QTimer(this);

    m_trayIcon = new QSystemTrayIcon(this);

    m_postModeMenu = new QMenu(this);

    tabWidget = new QweenTabCtrl(ui->splitter);
    tabWidget->setTabPosition(QTabWidget::South);
    tabWidget->setFocusPolicy(Qt::ClickFocus);
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
}

void QweenMainWindow::applySettings(){
    ui->statusText->setStyleSheet(settings->inputStyle());
    ui->statusText->setRequireCtrlOnEnter(settings->requireCtrlOnEnter());

    setupTimers();
}

bool QweenMainWindow::isNetworkAvailable(){
    //stub
    //TODO: implement
    return true;
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
    m_trayIcon->setIcon(QIcon(":/res/normal.png"));
    setWindowIcon(QIcon(":/res/normal.png"));
    m_trayIcon->show();
}

void QweenMainWindow::setupTwitter(){
    m_twitLib->Abort();
    m_twitLib->Logout();
    m_twitLib->SetLoginInfo(settings->userid(), settings->password());
}

void QweenMainWindow::makeConnections(){
    //MainMenu
    connect(ui->actExit, SIGNAL(triggered()),
            this, SLOT(close()));
    //PostMode
    connect(m_postModeMenu, SIGNAL(aboutToShow()),
            this, SLOT(OnPostModeMenuOpen()));
    //StatusText->postButton
    connect(ui->statusText,SIGNAL(returnPressed()),
            ui->postButton,SLOT(click()));
    //TrayIcon
    connect(m_trayIcon, SIGNAL(messageClicked()), this, SLOT(OnMessageClicked()));

    //Timers
    connect(m_timelineTimer, SIGNAL(timeout()), this, SLOT(OnTimelineTimerTimeout()));
    connect(m_DMTimer, SIGNAL(timeout()), this, SLOT(OnDmTimerTimeout()));
    connect(m_replyTimer, SIGNAL(timeout()), this, SLOT(OnReplyTimerTimeout()));
    connect(m_favTimer, SIGNAL(timeout()), this, SLOT(OnFavTimerTimeout()));

    //Twitter
    connect(m_twitLib,SIGNAL(OnResponseReceived(Returnables::Response *)),this,SLOT(OnResponseReceived(Returnables::Response *)));

    //Tab
    connect(tabWidget, SIGNAL(itemSelected(Twitter::TwitterItem)),
            this, SLOT(OnItemSelected(Twitter::TwitterItem)));
    //StatusText
    connect(ui->statusText, SIGNAL(uriShorteningFinished()),
            this, SLOT(OnUriShorteningFinished()));
}

void QweenMainWindow::OnResponseReceived(Returnables::Response *resp){
    if(resp)
    {
        switch(resp->reqID)
        {
        case Returnables::FRIENDS_TIMELINE:
            {
                Returnables::FriendsTimeline *pTimeline = static_cast<Returnables::FriendsTimeline *>(resp);
                QString popupText;
                QString title(tr("新着 ") + QString::number(pTimeline->list.count()) + tr("件\n"));
                while(!pTimeline->list.isEmpty()){
                    Returnables::StatusElementPtr element = pTimeline->list.takeLast();
                    Twitter::TwitterItem item(Twitter::Status, element, resp->reqID, false);
                    if(m_newestFriendsStatus < item.id()) m_newestFriendsStatus = item.id();
                    popupText.append(QString("%1 : %2\n").arg(item.userName(), item.status()));
                    if(!m_usersModel->userExists(item.userId()))
                        m_usersModel->appendItem(item);
                    tabWidget->addItem(item);
                }
                delete pTimeline;
                //TODO: dm, reply, sound
                //バルーン・サウンドは最初は抑制するようだ
                //設定項目があるのでそこを見るべし
                m_trayIcon->showMessage(title, popupText, QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information),
                                        5 * 1000);
                break;
            }
        case Returnables::NEW_STATUS:
        {
            ui->statusText->setText("");
            ui->statusText->setEnabled(true);
            ui->postButton->setEnabled(true);
            Returnables::NewStatus *pNewstatus = static_cast<Returnables::NewStatus*>(resp);
            Returnables::StatusElementPtr element = pNewstatus->status;
            tabWidget->addItem(Twitter::TwitterItem(Twitter::Status, element, resp->reqID, false));
            delete pNewstatus;
            break;
        }
        case Returnables::RECENT_MENTIONS:
        {
            Returnables::RecentMentions *p = static_cast<Returnables::RecentMentions *>(resp);
            while(!p->list.isEmpty()){
                Returnables::StatusElementPtr element = p->list.takeLast();
                Twitter::TwitterItem item(Twitter::Status, element, resp->reqID, false);
                if(m_newestReply < item.id()) m_newestReply = item.id();
                if(!m_usersModel->userExists(item.userId()))
                    m_usersModel->appendItem(item);
                tabWidget->addItem(item);
            }
            delete p;
            break;
        }
        case Returnables::SENT_DIRECT_MESSAGES:
        {
            Returnables::SentDirectMessages *p = static_cast<Returnables::SentDirectMessages *>(resp);
            while(!p->list.isEmpty()){
                Returnables::DirectMessageElementPtr element = p->list.takeLast();
                Twitter::TwitterItem item(Twitter::DirectMessage, element, resp->reqID, false);
                if(m_newestSentDM < item.id()) m_newestSentDM = item.id();
                tabWidget->addItem(item);
            }
            delete p;
            break;
        }
        case Returnables::RECEIVED_DIRECT_MESSAGES:
        {
            Returnables::ReceivedDirectMessages *p = static_cast<Returnables::ReceivedDirectMessages *>(resp);
            while(!p->list.isEmpty()){
                Returnables::DirectMessageElementPtr element = p->list.takeLast();
                Twitter::TwitterItem item(Twitter::DirectMessage, element, resp->reqID, false);
                if(m_newestRecvDM < item.id()) m_newestRecvDM = item.id();
                if(!m_usersModel->userExists(item.userId()))
                    m_usersModel->appendItem(item);
                tabWidget->addItem(item);
            }
            delete p;
            break;
        }
        case Returnables::FAVORITES:
        {
            Returnables::Favorites *p = static_cast<Returnables::Favorites *>(resp);
            while(!p->list.isEmpty()){
                Returnables::StatusElementPtr element = p->list.takeLast();
                Twitter::TwitterItem item(Twitter::Status, element, resp->reqID, false);
                if(m_newestFav < item.id()) m_newestFav = item.id();
                if(!m_usersModel->userExists(item.userId()))
                    m_usersModel->appendItem(item);
                tabWidget->addItem(item);
            }
            delete p;
            break;
        }
        case Returnables::VERIFY_CREDENTIALS:
        {
            Returnables::VerifyCredentials *p = static_cast<Returnables::VerifyCredentials*>(resp);
            Returnables::ExtUserInfoElementPtr element = p->userExt;
            tabWidget->setMyId(element->user.id);
            if(element->user.id!=0){
                SERVER::Option1 opt;
                opt.since_id = m_newestFriendsStatus;
                opt.count = 20;
                m_twitLib->GetFriendsTimeline(&opt);
                OnDmTimerTimeout();
                OnReplyTimerTimeout();
                OnFavTimerTimeout();
            }
            delete p;
            break;
        }
        case Returnables::API_REQUESTS:
        {
            Returnables::ApiRequests *p = static_cast<Returnables::ApiRequests*>(resp);
            QMessageBox::information(this,tr("API情報"),
                                     tr("上限: %1\n残数: %2\nリセット日時: %3\n")
                                     .arg(QString::number(p->hourlyLimit),
                                          QString::number(p->remainingHits),
                                          p->resetTime));
            delete p;
            break;
        }
        case Returnables::USER_TIMELINE:
        {
            Returnables::UserTimeline *p = static_cast<Returnables::UserTimeline*>(resp);
            Returnables::StatusElementPtr element = p->list.takeFirst();
            QMessageBox::information(this,tr("@twj の最新のTweet"),element->status.text);
            delete p;
            break;
        }
        case Returnables::FRIENDSHIP_EXISTS:
        {
            Returnables::FriendshipExist *p = static_cast<Returnables::FriendshipExist*>(resp);
            if(p->friends)
                QMessageBox::information(this, tr("友達関係"),
                                         tr("相互にフォローしています。")); //TODO: existsじゃなくてshowを使う
            delete p;
            break;
        }
        case Returnables::USER_DETAILS:
        {
            Returnables::UserDetails *p = static_cast<Returnables::UserDetails*>(resp);
            Returnables::ExtUserInfoElementPtr element = p->userExt;
            QMessageBox::information(this, tr("プロファイル情報"),
                                     tr("Following : %1\n"
                                        "Followers : %2\n"
                                        "Statuses count : %3\n"
                                        "Location : %4\n"
                                        "Bio : %5")
                                     .arg(QString::number(element->details.friendsCount),
                                          QString::number(element->user.followersCount),
                                          QString::number(element->details.statusesCount),
                                          element->user.location,
                                          element->user.description));
            break;
        }
        case Returnables::ADD_FRIENDSHIP:
        {
            Returnables::AddFriendship *p = static_cast<Returnables::AddFriendship*>(resp);
            Returnables::BasicUserInfoElementPtr user = p->user;
            if(!user->user.screenName.isEmpty())
                QMessageBox::information(this, "Follow", tr("@%1 をFollow開始しました。").arg(user->user.screenName));
            delete p;
            break;
        }
        case Returnables::REMOVE_FRIENDSHIP:
        {
            Returnables::RemoveFriendship *p = static_cast<Returnables::RemoveFriendship*>(resp);
            Returnables::BasicUserInfoElementPtr user = p->user;
            if(!user->user.screenName.isEmpty())
                QMessageBox::information(this, "Remove", tr("@%1 をRemoveしました。").arg(user->user.screenName));
            delete p;
            break;
        }
        default:
            break;
        }
    }
}

void QweenMainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QweenMainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    /*if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }*/

    QFile tabSettings(QweenApplication::profileDir()+"/tabs.xml");
    tabSettings.open(QFile::WriteOnly);
    tabWidget->saveState(&tabSettings);

    settings->setGeometry(saveGeometry());
    settings->setWindowState(saveState());
    settings->save();
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
        m_twitLib->VerifyCredentials();
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
            m_twitLib->VerifyCredentials();
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
    m_twitLib->PostNewStatus(postText + tr(" ") + settings->statusSuffix(), 0, "Qween");
}

void QweenMainWindow::makeReplyOrDirectStatus(bool isAuto, bool isReply, bool isAll){

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
    SERVER::Option1 opt;
    opt.since_id = m_newestFriendsStatus;
    opt.count = 200;
    m_twitLib->GetFriendsTimeline(&opt);
}


void QweenMainWindow::OnDmTimerTimeout(){
    SERVER::Option5 s_opt;
    s_opt.since_id = m_newestSentDM;
    s_opt.page = 1;
    m_twitLib->GetSentDirectMessages(&s_opt);

    SERVER::Option5 r_opt;
    r_opt.since_id = m_newestRecvDM;
    r_opt.page = 1;
    m_twitLib->GetReceivedDirectMessages(&r_opt);
}

void QweenMainWindow::OnReplyTimerTimeout(){
    SERVER::Option3 opt;
    opt.since_id = m_newestReply;
    m_twitLib->GetRecentMentions(&opt);
}

void QweenMainWindow::OnFavTimerTimeout(){
    m_twitLib->GetFavorites();
}

void QweenMainWindow::OnItemSelected(const Twitter::TwitterItem &item)
{
    switch(item.type()){
    case Twitter::Status:
        ui->textBrowser->setHtml(tr("<html><body style=\"%1\">")
                                 .arg(settings->statusViewStyle()) +
                                 item.status() + tr("</body></html>"));
        ui->lblNameId->setText(item.screenName() + "/" + item.userName());
        ui->lblUpdateDatetime->setText(item.createdAt());
        if(QweenApplication::iconManager()->isIconAvailable(item.userId())){
            QIcon icon(QweenApplication::iconManager()->getIcon(item.userId()));
            ui->userIconLabel->setPixmap(icon.pixmap(50,50,QIcon::Normal,QIcon::On));
        }else{
            ui->userIconLabel->setPixmap(QPixmap(50,50));
            QweenApplication::iconManager()->fetchIcon(item.userId(), item.iconUri());
        }
        ui->userIconLabel->repaint();
        break;
    default:
        break;
    }
}

void QweenMainWindow::OnPostModeMenuOpen(){
    m_actAutoShortenUri->setChecked(settings->uriAutoShorten());
    m_actAvoidApiCommand->setChecked(settings->avoidApiCmd());
    m_actDivideUriFromZenkaku->setChecked(settings->divideUriFromZenkaku());
    m_actReplaceZenkakuSpace->setChecked(settings->replaceZenkakuSpace());
}

void QweenMainWindow::OnUriShortened(const QString& src, const QString& dest){
    QMessageBox::information(this, "", dest);
}

void QweenMainWindow::OnIconDownloaded(quint64 userid, const QIcon &icon){
    ui->userIconLabel->setPixmap(icon.pixmap(50,50,QIcon::Normal,QIcon::On));
}

void QweenMainWindow::OnMessageClicked(){
    this->activateWindow();
}

void QweenMainWindow::on_statusText_textChanged(QString string)
{
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
    m_twitLib->GetUserDetails(settings->userid());
}

void QweenMainWindow::on_actApiInfo_triggered()
{
    m_twitLib->RemainingApiRequests();
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
    SERVER::Option1 opt;
    opt.since_id = m_newestFriendsStatus;
    opt.count = 200;
    m_twitLib->GetFriendsTimeline(&opt);
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
    QString data = "http://twitter.com/%0/status/%1]";
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(data.arg(item.screenName(), QString::number(item.id())), QClipboard::Clipboard);
}

void QweenMainWindow::on_actExplosion_triggered()
{
    QMessageBox::information(this, tr("ｺﾞﾙｧ"), tr("だからやめれっての"));
}

void QweenMainWindow::on_actShortenUri_triggered()
{
    ui->statusText->shortenUri();
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
    bool ok;
    QString rv = QInputDialog::getText(this, tr("フォロー関係を調べる"), tr("IDを入力してください"), QLineEdit::Normal, name, &ok);
    if(ok){
        m_twitLib->FriendshipExist(settings->userid(), rv);
    }
}

void QweenMainWindow::on_actFollow_triggered()
{
    QString name = tabWidget->currentItem().screenName();
    bool ok;
    QString rv = QInputDialog::getText(this, tr("Follow"), tr("IDを入力してください"), QLineEdit::Normal, name, &ok);
    if(ok){
        m_twitLib->AddFriendship(rv, true);
    }
}

void QweenMainWindow::on_actRemove_triggered()
{
    QString name = tabWidget->currentItem().screenName();
    bool ok;
    QString rv = QInputDialog::getText(this, tr("Follow"), tr("IDを入力してください"), QLineEdit::Normal, name, &ok);
    if(ok){
        m_twitLib->RemoveFriendship(rv);
    }
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
    SERVER::Option2 opt;
    opt.screen_name = "twj";
    m_twitLib->GetUsersTimeline(&opt);
}

void QweenMainWindow::on_actTabSettings_triggered()
{
    TabSettingsDialog dlg(tabWidget);
    dlg.exec();
}
