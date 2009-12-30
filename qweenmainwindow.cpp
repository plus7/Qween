#include "qweenmainwindow.h"
#include "ui_qweenmainwindow.h"
#include "aboutdialog.h"
#include "qweensettings.h"
#include "qweentabctrl.h"
#include "settingdialog.h"
#include "QTwitLib.h"
#include <QDialog>
#include <QMessageBox>
#include <QtGui>
//#include <QtWebKit>
#include "urishortensvc.h"
#include "iconmanager.h"
#include "qweenapplication.h"

QweenMainWindow::QweenMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QweenMainWindow),m_firstShow(true),m_postAfterShorten(false),m_urisvc(NULL)
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
    m_fetchAnimTimer = new QTimer(this);

    m_trayIcon = new QSystemTrayIcon(this);

    m_postModeMenu = new QMenu(this);

    tabWidget = new QweenTabCtrl(ui->splitter);
    tabWidget->setTabPosition(QTabWidget::South);
    tabWidget->setFocusPolicy(Qt::ClickFocus);
    ui->splitter->insertWidget(0,tabWidget);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 0);
}

void QweenMainWindow::applySettings(){
    ui->statusText->setStyleSheet(settings->inputStyle());
    ui->statusText->setRequireCtrlOnEnter(settings->requireCtrlOnEnter());

    //setupTimers();
    //setupTwitter();
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
    QAction *act = new QAction(QIcon(), tr("URLからの全角文字列の切り離し"), this);
    //connect(act, SIGNAL(triggered()), this, SLOT(open()));
    m_postModeMenu->addAction(act);

    act = new QAction(QIcon(), tr("APIコマンドを回避する"), this);
    //connect(act, SIGNAL(triggered()), this, SLOT(open()));
    m_postModeMenu->addAction(act);

    m_actAutoShortenUri = new QAction(QIcon(), tr("自動的にURLを短縮する"), this);
    m_actAutoShortenUri->setCheckable(true);
    //connect(act, SIGNAL(triggered()), this, SLOT(open()));
    m_postModeMenu->addAction(m_actAutoShortenUri);

    act = new QAction(QIcon(), tr("全角スペースを半角スペースにする"), this);
    //connect(act, SIGNAL(triggered()), this, SLOT(open()));
    m_postModeMenu->addAction(act);

    ui->postButton->setMenu(m_postModeMenu);
}

void QweenMainWindow::setupTabs(){
    tabWidget->initWithTabInfo(NULL);
}

void QweenMainWindow::setupTimers(){
    m_timelineTimer->setInterval(60*1000);
    m_timelineTimer->start();

    m_DMTimer->setInterval(60*1000);
    m_DMTimer->start();

    m_replyTimer->setInterval(60*1000);
    m_replyTimer->start();

    m_fetchAnimTimer->setInterval(85);
    m_fetchAnimTimer->start();
}

void QweenMainWindow::setupTrayIcon(){
    m_trayIcon->setIcon(QIcon(":/res/normal.png"));
    setWindowIcon(QIcon(":/res/normal.png"));
    m_trayIcon->show();
}

void QweenMainWindow::setupTwitter(){
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
    connect(m_timelineTimer, SIGNAL(timeout()), this, SLOT(on_timelineTimer_timeout()));
    //TODO: connect(m_DMTimer, SIGNAL(timeout()), this, SLOT(on_timelineTimer_timeout()));
    //TODO: connect(m_replyTimer, SIGNAL(timeout()), this, SLOT(on_timelineTimer_timeout()));
    //TODO: connect(m_replyTimer, SIGNAL(timeout()), this, SLOT(on_timelineTimer_timeout()));

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
                    popupText.append(QString("%1 : %2\n").arg(item.userName(), item.status()));
                    tabWidget->addItem(item);
                }
                delete pTimeline;
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
        case Returnables::VERIFY_CREDENTIALS:
        {
            Returnables::VerifyCredentials *p = static_cast<Returnables::VerifyCredentials*>(resp);
            Returnables::ExtUserInfoElementPtr element = p->userExt;
            tabWidget->setMyId(element->user.id);
            if(element->user.id!=0){
                SERVER::Option1 opt;
                opt.since_id = tabWidget->getNewestTimelineId();
                opt.count = 20;
                m_twitLib->GetFriendsTimeline(&opt);
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

void QweenMainWindow::on_timelineTimer_timeout()
{
    SERVER::Option1 opt;
    opt.since_id = tabWidget->getNewestTimelineId();
    opt.count = 200;
    m_twitLib->GetFriendsTimeline(&opt);
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
    //TODO:その他三つのMenu
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
    opt.since_id = tabWidget->getNewestTimelineId();
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
