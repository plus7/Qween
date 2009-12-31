/*
    This file is part of QTwitlib.

    QTwitlib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    QTwitlib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QTwitlib.  If not, see <http://www.gnu.org/licenses/>.

	Original Author: Bradley Lackey
*/


#include "Core.h"
#include "Decipher.h"
#include <QtXml/QDomDocument>
#include <QtGui/QMessageBox>
#include <QtCore/QUrl>
#include <QtCore/QFile>
#include <QtNetwork/QNetworkProxy>

//=====================================================================
QString Core::TWITTER_HOST = "twitter.com";
QString Core::VERIFY_CREDENTIALS_URL = "/account/verify_credentials.xml";
QString Core::PUBLIC_TIMELINE_URL = "/statuses/public_timeline.xml";
QString Core::GET_SINGLE_STATUS_URL = "/statuses/show/[req-id].xml";
QString Core::FEATURED_USERS_URL = "/statuses/featured.xml";
QString Core::LOGOUT_URL = "/account/end_session";
QString Core::IS_TWITTER_UP_URL = "/help/test.xml";
QString Core::USERS_TIMELINE_URL = "/statuses/user_timeline[/opt-user].xml";
QString Core::GET_FAVORITES_URL = "/favorites[/opt-user].xml";
QString Core::FRIENDS_TIMELINE_URL = "/statuses/friends_timeline.xml";
QString Core::POST_NEW_STATUS_URL = "/statuses/update.xml";
QString Core::GET_MENTIONS_URL = "/statuses/mentions.xml";
QString Core::REMOVE_STATUS_URL = "/statuses/destroy/[req-id].xml";
QString Core::GET_FRIENDS_URL = "/statuses/friends[/opt-user].xml";
QString Core::GET_FOLLOWERS_URL = "/statuses/followers[/opt-user].xml";
QString Core::GET_USER_DETAILS_URL = "/users/show[/req-user].xml";
QString Core::GET_SENT_DIRECT_MESSAGES_URL = "/direct_messages/sent.xml";
QString Core::GET_RECEIVED_DIRECT_MESSAGES_URL = "/direct_messages.xml";
QString Core::SEND_NEW_DIRECT_MESSAGE_URL = "/direct_messages/new.xml";
QString Core::REMOVE_DIRECT_MESSAGE_URL = "/direct_messages/destroy/[req-id].xml";
QString Core::CREATE_FRIENDSHIP_URL = "/friendships/create/[req-user].xml";
QString Core::REMOVE_FRIENDSHIP_URL = "/friendships/destroy/[req-user].xml";
QString Core::FRIENDSHIP_EXIST_URL = "http://twitter.com/friendships/exists.xml";
QString Core::UPDATE_DELIVERY_DEVICE_URL = "/account/update_delivery_device.xml";
QString Core::REMAINING_API_REQUESTS_URL = "/account/rate_limit_status.xml";
QString Core::ADD_FAVORITE_URL = "/favorites/create/[req-id].xml";
QString Core::REMOVE_FAVORITE_URL = "/favorites/destroy/[req-id].xml";
QString Core::PROFILE_COLORS_URL = "/account/update_profile_colors.xml";
QString Core::PROFILE_IMAGE_URL = "/account/update_profile_image.xml";
QString Core::PROFILE_BACKGROUND_IMAGE_URL = "/account/update_profile_background_image.xml";
QString Core::PROFILE_URL = "/account/update_profile.xml";
QString Core::ENABLE_NOTIFICATIONS_URL = "/notifications/follow/[req-id].xml";
QString Core::DISABLE_NOTIFICATIONS_URL = "/notifications/leave/[req-id].xml";
QString Core::BLOCK_USER_URL = "/blocks/create/[req-id].xml";
QString Core::UNBLOCK_USER_URL = "/blocks/destroy/[req-id].xml";
QString Core::FRIENDS_IDS_URL = "/friends/ids[/req-id].xml";
QString Core::FOLLOWERS_IDS_URL = "/followers/ids[/req-id].xml";
//=====================================================================
Core::Core()
{
    m_eventLoop = new QEventLoop(this);
	m_http = new QHttp(TWITTER_HOST);

	MakeConnections();
}
//=====================================================================
Core::~Core()
{
    if(m_eventLoop)
        delete m_eventLoop;
    if(m_http)
        delete m_http;
}
//=====================================================================
void Core::SetProxy(SERVER::PROXY_TYPE type, const QString hostName, \
                    quint16 port, const QString user, const QString password)
{
    QNetworkProxy::ProxyType proxy_type = QNetworkProxy::NoProxy;
    switch(type)
    {
        case SERVER::NO_PROXY:
            proxy_type = QNetworkProxy::NoProxy;
        break;
        case SERVER::SOCKS5_PROXY:
            proxy_type = QNetworkProxy::Socks5Proxy;
        break;
        case SERVER::HTTP_PROXY:
            proxy_type = QNetworkProxy::HttpProxy;
        break;
    }

    QNetworkProxy m_proxy = QNetworkProxy(proxy_type, hostName, port, user, password);
    m_http->setProxy(m_proxy);
}
//=====================================================================
void Core::Abort()
{
    m_http->abort();
}
//=====================================================================
void Core::MakeConnections()   
{
    connect(m_http, SIGNAL(dataReadProgress(int, int)), this, SLOT(DataReadProgress(int, int)));   
    connect(m_http, SIGNAL(requestStarted(int)), this, SLOT(RequestStarted(int)));
    connect(m_http, SIGNAL(requestFinished(int,bool)), this, SLOT(ReqFinished(int, bool)));
    connect(m_http, SIGNAL(done(bool)), this, SLOT(Done(bool)));
    connect(this, SIGNAL(QueryDone()), m_eventLoop, SLOT(quit()));
}
//=====================================================================
void Core::Done ( bool /*error*/ )
{
    emit QueryDone();
}
//=====================================================================
void Core::DataReadProgress ( int /*done*/, int /*total*/ )
{
    //emit OnMessageReceived(QString::number(done).toStdString()+" / "+QString::number(total).toStdString());
}
//=====================================================================
int Core::MakeGetRequest(QString req,Returnables::RequestId reqId)
{
    int id;
    QBuffer *tempBuffer = new QBuffer;
    tempBuffer->open(QIODevice::ReadWrite);
    id = m_http->get(req,tempBuffer);
	m_buffer[id].buffer = tempBuffer;
	m_buffer[id].requestid = reqId;
	return id;
}
//=====================================================================
int Core::MakePostRequest(QString path,QByteArray req,Returnables::RequestId reqId, bool isMultipart)
{
    int id;
    QBuffer *tempBuffer = new QBuffer;
    tempBuffer->open(QIODevice::ReadWrite);

    if(isMultipart)
    {
        id = m_http->request(header, req, tempBuffer);
    } else {
        id = m_http->post(path,req,tempBuffer);
    }
        m_buffer[id].buffer = tempBuffer;
        m_buffer[id].requestid = reqId;
	return id;
}
//=====================================================================
void Core::RequestStarted(int /*id*/)
{}
//=====================================================================
void Core::ReqFinished(int id, bool error)
{
    QString response;
    QHttpResponseHeader head;

        if(error)
        {
                emit OnErrorString(m_http->errorString());
                emit OnError(m_http->error());
        }

	if(m_buffer[id].buffer)
		response = QString(m_buffer[id].buffer->data());
	else
		response = QString::null;
     
	if(!response.isNull())
        {
                head = m_http->lastResponse();
                if(head.isValid())
                    responseHeaderReceived(head);

		switch(m_buffer[id].requestid)
		{
		case Returnables::PUBLIC_TIMELINE:
			Returnables::PublicTimeline *pTimeline;
			pTimeline = Decipher::Instance()->PublicTimeline(response);
			emit OnResponseReceived(pTimeline);
			break;
		case Returnables::FRIENDS_TIMELINE:
			Returnables::FriendsTimeline *fTimeline;
			fTimeline = Decipher::Instance()->FriendsTimeline(response);
			emit OnResponseReceived(fTimeline);
			break;
		case Returnables::SINGLE_STATUS:
			Returnables::SingleStatus *singleStatus;
			singleStatus = Decipher::Instance()->SingleStatus(response);
			emit OnResponseReceived(singleStatus);
			break;
		case Returnables::FEATURED_USERS:
			Returnables::FeaturedUsers *featuredUsers;
			featuredUsers = Decipher::Instance()->FeaturedUsers(response);
			emit OnResponseReceived(featuredUsers);
			break;
		case Returnables::VERIFY_CREDENTIALS:
                        Returnables::VerifyCredentials *verifyCredentials;
                        verifyCredentials = Decipher::Instance()->VerifyCredentials(response);
                        emit OnResponseReceived(verifyCredentials);
			break;
		case Returnables::TWITTER_UP:
			Returnables::TwitterUp *twitterUp;
			twitterUp = Decipher::Instance()->TwitterUp(response);
			emit OnResponseReceived(twitterUp);
			break;
		case Returnables::USER_TIMELINE:
			Returnables::UserTimeline *userTimeline;
			userTimeline = Decipher::Instance()->UserTimeline(response);
			emit OnResponseReceived(userTimeline);
			break;
		case Returnables::FAVORITES:
			Returnables::Favorites *favorites;
			favorites = Decipher::Instance()->Favorites(response);
			emit OnResponseReceived(favorites);
			break;
		case Returnables::NEW_STATUS:
			Returnables::NewStatus *newStatus;
			newStatus = Decipher::Instance()->NewStatus(response);
			emit OnResponseReceived(newStatus);
			break;
                case Returnables::RECENT_MENTIONS:
                        Returnables::RecentMentions *mentions;
                        mentions = Decipher::Instance()->RecentMentions(response);
                        emit OnResponseReceived(mentions);
			break;
		case Returnables::REMOVE_STATUS:
			Returnables::RemoveStatus *removedStatus;
			removedStatus = Decipher::Instance()->RemoveStatus(response);
			emit OnResponseReceived(removedStatus);
			break;
		case Returnables::FRIENDS:
			Returnables::Friends *friends;
			friends = Decipher::Instance()->Friends(response);
			emit OnResponseReceived(friends);
			break;
		case Returnables::FOLLOWERS:
			Returnables::Followers *followers;
			followers = Decipher::Instance()->Followers(response);
			emit OnResponseReceived(followers);
			break;
		case Returnables::USER_DETAILS:
			Returnables::UserDetails *userDetails;
			userDetails = Decipher::Instance()->UserDetails(response);
			emit OnResponseReceived(userDetails);
			break;
		case Returnables::SENT_DIRECT_MESSAGES:
			Returnables::SentDirectMessages *sentDirectMessages;
			sentDirectMessages = Decipher::Instance()->SentDirectMessages(response);
			emit OnResponseReceived(sentDirectMessages);
			break;
		case Returnables::RECEIVED_DIRECT_MESSAGES:
			Returnables::ReceivedDirectMessages *receivedDirectMessages;
			receivedDirectMessages = Decipher::Instance()->ReceivedDirectMessages(response);
			emit OnResponseReceived(receivedDirectMessages);
			break;
		case Returnables::SEND_DIRECT_MESSAGE:
			Returnables::SendDirectMessage *sendDirectMessage;
			sendDirectMessage = Decipher::Instance()->SendDirectMessage(response);
			emit OnResponseReceived(sendDirectMessage);
			break;
		case Returnables::REMOVE_DIRECT_MESSAGE:
			Returnables::RemoveDirectMessage *removeDirectMessage;
			removeDirectMessage = Decipher::Instance()->RemoveDirectMessage(response);
			emit OnResponseReceived(removeDirectMessage);
			break;
		case Returnables::ADD_FRIENDSHIP:
			Returnables::AddFriendship *addFriendship;
			addFriendship = Decipher::Instance()->AddFriendShip(response);
			emit OnResponseReceived(addFriendship);
			break;
		case Returnables::REMOVE_FRIENDSHIP:
			Returnables::RemoveFriendship *removeFriendship;
			removeFriendship = Decipher::Instance()->RemoveFriendship(response);
			emit OnResponseReceived(removeFriendship);
			break;
		case Returnables::FRIENDSHIP_EXISTS:
			Returnables::FriendshipExist *friendshipExists;
			friendshipExists = Decipher::Instance()->FriendshipExist(response);
			emit OnResponseReceived(friendshipExists);
			break;
		case Returnables::DELIVERY_DEVICE:
			Returnables::DeliveryDevice *deliveryDevice;
			deliveryDevice = Decipher::Instance()->DeliveryDevice(response);
			emit OnResponseReceived(deliveryDevice);
			break;
		case Returnables::API_REQUESTS:
			Returnables::ApiRequests *apiRequests;
			apiRequests = Decipher::Instance()->ApiRequests(response);
			emit OnResponseReceived(apiRequests);
			break;
		case Returnables::ADD_FAVORITE:
			Returnables::AddFavorite *addFavorite;
			addFavorite = Decipher::Instance()->AddFavorite(response);
			emit OnResponseReceived(addFavorite);
			break;
		case Returnables::REMOVE_FAVORITE:
			Returnables::RemoveFavorite *removeFavorite;
			removeFavorite = Decipher::Instance()->RemoveFavorite(response);
			emit OnResponseReceived(removeFavorite);
			break;
                case Returnables::PROFILE_COLORS:
                        Returnables::ProfileColors *profileColors;
                        profileColors = Decipher::Instance()->ProfileColors(response);
                        emit OnResponseReceived(profileColors);
                        break;
                case Returnables::PROFILE_IMAGE:
                        Returnables::ProfileImage *profileImage;
                        profileImage = Decipher::Instance()->ProfileImage(response);
                        emit OnResponseReceived(profileImage);
                        break;
                case Returnables::PROFILE_BACKGROUND_IMAGE:
                        Returnables::ProfileBackgroundImage *profileBackgroundImage;
                        profileBackgroundImage = Decipher::Instance()->ProfileBackgroundImage(response);
                        emit OnResponseReceived(profileBackgroundImage);
                        break;
                case Returnables::PROFILE:
                        Returnables::Profile *profile;
                        profile = Decipher::Instance()->Profile(response);
                        emit OnResponseReceived(profile);
                        break;
                case Returnables::ENABLE_NOTIFICATIONS:
                        Returnables::EnableNotifications *enableNotifications;
                        enableNotifications = Decipher::Instance()->EnableNotifications(response);
                        emit OnResponseReceived(enableNotifications);
                        break;
                case Returnables::DISABLE_NOTIFICATIONS:
                        Returnables::DisableNotifications *disableNotifications;
                        disableNotifications = Decipher::Instance()->DisableNotifications(response);
                        emit OnResponseReceived(disableNotifications);
                        break;
                case Returnables::BLOCK_USER:
                        Returnables::BlockUser *blockUser;
                        blockUser = Decipher::Instance()->BlockUser(response);
                        emit OnResponseReceived(blockUser);
                        break;
                case Returnables::UNBLOCK_USER:
                        Returnables::UnBlockUser *unBlockUser;
                        unBlockUser = Decipher::Instance()->UnBlockUser(response);
                        emit OnResponseReceived(unBlockUser);
                        break;
                case Returnables::FRIENDS_IDS:
                        Returnables::FriendsIDs *friendsIDs;
                        friendsIDs = Decipher::Instance()->FriendsIDs(response);
                        emit OnResponseReceived(friendsIDs);
                        break;
                case Returnables::FOLLOWERS_IDS:
                        Returnables::FollowersIDs *followersIDs;
                        followersIDs = Decipher::Instance()->FollowersIDs(response);
                        emit OnResponseReceived(followersIDs);
                        break;
		default:
			emit OnMessageReceived(response);
		}
	}

	if(m_buffer[id].buffer)
    {
		m_buffer[id].buffer->close();
		delete m_buffer[id].buffer;
		m_buffer[id].buffer = NULL;
    }
}
//=====================================================================
void Core::responseHeaderReceived(const QHttpResponseHeader &resp)
{
	switch(resp.statusCode())
	{
		case SERVER::OK:
            emit OnStatusReceived(SERVER::OK);
			break;
		case SERVER::NOT_MODIFIED:
            emit OnStatusReceived(SERVER::NOT_MODIFIED);
			break;
		case SERVER::BAD_REQUEST:
            emit OnStatusReceived(SERVER::BAD_REQUEST);
			break;
		case SERVER::NOT_AUTHORIZED:
            emit OnStatusReceived(SERVER::NOT_AUTHORIZED);
			break;
		case SERVER::FORBIDDEN:
            emit OnStatusReceived(SERVER::FORBIDDEN);
			break;
		case SERVER::NOT_FOUND:
            emit OnStatusReceived(SERVER::NOT_FOUND);
			break;
		case SERVER::INTERNAL_SERVER_ERROR:
            emit OnStatusReceived(SERVER::INTERNAL_SERVER_ERROR);
			break;
		case SERVER::BAD_GATEWAY:
            emit OnStatusReceived(SERVER::BAD_GATEWAY);
			break;
		case SERVER::SERVICE_UNAVAILABLE:
            emit OnStatusReceived(SERVER::SERVICE_UNAVAILABLE);
			break;
		default:
            emit OnStatusReceived(SERVER::UNKNOWN);
			break;
	}
}
//=====================================================================
void Core::GetPublicTimeline()
{
    MakeGetRequest(PUBLIC_TIMELINE_URL,Returnables::PUBLIC_TIMELINE);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::GetSingleStatus(QString id)
{
    QString req;
    req = GET_SINGLE_STATUS_URL;
    req.replace("[req-id]",id);
    MakeGetRequest(req,Returnables::SINGLE_STATUS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::GetFeaturedUsers()
{
    MakeGetRequest(FEATURED_USERS_URL,Returnables::FEATURED_USERS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::Login(QString user, QString passw)
{
    m_http->setUser(user, passw);
    VerifyCredentials();
}
//=====================================================================
void Core::Logout()
{
    m_http->setUser("","");
    MakePostRequest(LOGOUT_URL,"",Returnables::LOGOUT);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::SetLoginInfo(QString user, QString passw)
{
    m_http->setUser(user, passw);
}
//=====================================================================
void Core::IsTwitterUp()
{
    MakeGetRequest(IS_TWITTER_UP_URL,Returnables::TWITTER_UP);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::GetUsersTimeline(SERVER::Option2 *opt  /*=NULL*/)
{
    QString buildUrl = USERS_TIMELINE_URL;
    
    if(opt)
    {
        QString id        = opt->id;
        QString user_id = QString::number(opt->user_id);
        QString screen_name = opt->screen_name;
        QString since_id    = QString::number(opt->since_id);
        QString max_id = QString::number(opt->max_id);
        QString page       = QString::number(opt->page);
        
        if(!id.isEmpty())
            buildUrl.replace("[/opt-user]","/"+id);
        else
            buildUrl.replace("[/opt-user]","");
        
        QString param;
        if(opt->user_id) param += "&user_id="+user_id;
        param += "&screen_name="+screen_name;
        if(opt->since_id) param += "&since_id="+since_id;
        if(opt->max_id)   param += "&max_id="+max_id;
        if(opt->page)  param += "&page="+page;
        param[0] = '?';
        buildUrl += param;
    }
    else
    {
        buildUrl.replace("[/opt-user]","");
    }

    MakeGetRequest(buildUrl,Returnables::USER_TIMELINE);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::GetFavorites(QString user  /*=""*/, unsigned int page  /*=1*/)
{
    QString buildUrl  = GET_FAVORITES_URL;
    
    if(!user.isEmpty())
        buildUrl.replace("[/opt-user]","/"+user);
    else
        buildUrl.replace("[/opt-user]","");
    
	buildUrl += "?page="+QString::number(page);

    MakeGetRequest(buildUrl,Returnables::FAVORITES);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::GetFriendsTimeline(SERVER::Option1 *opt  /*=NULL*/)
{
    QString buildUrl = FRIENDS_TIMELINE_URL;
    
    if(opt)
    {       
        QString since_id    = QString::number(opt->since_id);
        QString max_id = QString::number(opt->max_id);
        QString count      = QString::number(opt->count);
        QString page       = QString::number(opt->page);
        
        QString param;
        if(opt->since_id) param += "&since_id="+since_id;
        if(opt->max_id) param += "&max_id="+max_id;
        if(opt->count) param += "&count="+count;
        if(opt->page) param += "&page="+page;
        param[0] = '?';
        buildUrl += param;
    }

    MakeGetRequest(buildUrl,Returnables::FRIENDS_TIMELINE);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::PostNewStatus(QString status, QString in_reply_to_status_id, QString source)
{
    QByteArray encodedUrl, req;
    encodedUrl = QUrl::toPercentEncoding(status);
    
    req = "status=";
    req += encodedUrl;

    if(!in_reply_to_status_id.isEmpty())
    {
        req += "&in_reply_to_status_id=";
        req += in_reply_to_status_id;
    }

    if(!source.isEmpty())
    {
    req += "&source=";
    req += source;
    }
    
    MakePostRequest(POST_NEW_STATUS_URL,req,Returnables::NEW_STATUS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::GetRecentMentions(SERVER::Option3 *opt  /*=NULL*/)
{
    QString buildUrl = GET_MENTIONS_URL;
    
    if(opt)
    {
        QString since_id    = QString::number(opt->since_id);
        QString max_id = QString::number(opt->max_id);
        QString page       = QString::number(opt->page);

        QString param;
        if(opt->since_id) param += "&since_id="+since_id;
        if(opt->max_id) param += "&max_id="+max_id;
        if(opt->page) param += "&page="+page;
        param[0] = '?';
        buildUrl += param;
    }

    MakeGetRequest(buildUrl,Returnables::RECENT_MENTIONS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::RemoveStatus(QString id)
{
    QString buildUrl = REMOVE_STATUS_URL;
    buildUrl = buildUrl.replace("[req-id]",id);
    
    MakePostRequest(buildUrl,"",Returnables::REMOVE_STATUS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::GetFriends(SERVER::Option4 *opt  /*=NULL*/)
{
    QString buildUrl = GET_FRIENDS_URL;
    
    if(opt)
    {
        QString id        = opt->id;
        QString user_id = QString::number(opt->user_id);
        QString screen_name = opt->screen_name;
        QString page       = QString::number(opt->page);
        
        if(!id.isEmpty())
            buildUrl.replace("[/opt-user]","/"+id);
        else
            buildUrl.replace("[/opt-user]","");
        
        QString param;
        if(opt->user_id) param += "&user_id="+user_id;
        param += "&screen_name="+screen_name;
        if(opt->page) param += "&page="+page;
        param[0] = '?';
        buildUrl += param;
    }
    else
    {
        buildUrl.replace("[/opt-user]","");
    }

    MakeGetRequest(buildUrl,Returnables::FRIENDS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::GetFollowers(SERVER::Option4 *opt  /*=NULL*/)
{
    QString buildUrl = GET_FOLLOWERS_URL;
    
    if(opt)
    {
        QString id        = opt->id;
        QString user_id = QString::number(opt->user_id);
        QString screen_name = opt->screen_name;
        QString page       = QString::number(opt->page);
        
        if(!id.isEmpty())
            buildUrl.replace("[/opt-user]","/"+id);
        else
            buildUrl.replace("[/opt-user]","");
        
        QString param;
        if(opt->user_id) param += "&user_id="+user_id;
        param += "&screen_name="+screen_name;
        if(opt->page) param += "&page="+page;
        param[0] = '?';
        buildUrl += param;
    }
    else
    {
        buildUrl.replace("[/opt-user]","");
    }

    MakeGetRequest(buildUrl,Returnables::FOLLOWERS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::GetUserDetails(QString id, QString user_id, QString screen_name)
{
    if( !id.isEmpty() || !user_id.isEmpty() || !screen_name.isEmpty() )
    {
        QString buildUrl = GET_USER_DETAILS_URL;

        if(!id.isEmpty())
        {
            buildUrl.replace("[/req-user]", "/" + id);
        }
        else if(!user_id.isEmpty())
        {
            buildUrl.replace("[/req-user]", "");
            buildUrl += "?user_id=" + user_id;
        }
        else if(!screen_name.isEmpty())
        {
            buildUrl.replace("[/req-user]", "");
            buildUrl += "?screen_name=" + screen_name;
        }

        MakeGetRequest(buildUrl,Returnables::USER_DETAILS);
        //m_eventLoop->exec(QEventLoop::AllEvents);
    }
}
//=====================================================================
void Core::GetSentDirectMessages(SERVER::Option5 *opt  /*=NULL*/)
{
    QString buildUrl = GET_SENT_DIRECT_MESSAGES_URL;
    
    if(opt)
    {       
        QString since_id    = QString::number(opt->since_id);
        QString page       = QString::number(opt->page);
        
        QString param;
        if(opt->since_id) param += "&since_id="+since_id;
        if(opt->page) param += "&page="+page;
        param[0] = '?';
        buildUrl += param;
    }

    MakeGetRequest(buildUrl,Returnables::SENT_DIRECT_MESSAGES);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::GetReceivedDirectMessages(SERVER::Option5 *opt  /*=NULL*/)
{
    QString buildUrl = GET_RECEIVED_DIRECT_MESSAGES_URL;
    
    if(opt)
    {       
        QString since_id    = QString::number(opt->since_id);
        QString page       = QString::number(opt->page);
        
        QString param;
        if(opt->since_id) param += "&since_id="+since_id;
        if(opt->page) param += "&page="+page;
        param[0] = '?';
        buildUrl += param;
    }

    MakeGetRequest(buildUrl,Returnables::RECEIVED_DIRECT_MESSAGES);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::SendDirectMessage(QString user, QString text)
{
    QByteArray encodedText, req;
    encodedText = QUrl::toPercentEncoding(text);
    
    req = "user=";
    req += user.toAscii();
    req += "&";
    req += "text=";
    req += encodedText;
    
    MakePostRequest(SEND_NEW_DIRECT_MESSAGE_URL,req,Returnables::SEND_DIRECT_MESSAGE);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=================================================================================================================
void Core::RemoveDirectMessage(QString id)
{
    QString buildUrl = REMOVE_DIRECT_MESSAGE_URL;
    
    buildUrl = buildUrl.replace("[req-id]",id);

    MakePostRequest(buildUrl,"",Returnables::REMOVE_DIRECT_MESSAGE);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::AddFriendship(QString user, bool follow)
{
    QByteArray req;
	QString buildUrl;

	buildUrl = CREATE_FRIENDSHIP_URL;
	buildUrl = buildUrl.replace("[req-user]",user);
  
        req = "follow=";
	req += follow ? "true" : "false";
      
    MakePostRequest(buildUrl,req,Returnables::ADD_FRIENDSHIP);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::RemoveFriendship(QString user)
{
    QString buildUrl = REMOVE_FRIENDSHIP_URL;
    
    buildUrl = buildUrl.replace("[req-user]",user);
    
    MakePostRequest(buildUrl,"",Returnables::REMOVE_FRIENDSHIP);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::FriendshipExist(QString user_a, QString user_b)
{
    QString buildUrl = FRIENDSHIP_EXIST_URL;
    
    buildUrl += "?user_a="+user_a;
    buildUrl += "&user_b="+user_b;
    
    MakeGetRequest(buildUrl,Returnables::FRIENDSHIP_EXISTS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::VerifyCredentials()
{
    MakeGetRequest(VERIFY_CREDENTIALS_URL,Returnables::VERIFY_CREDENTIALS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::UpdateDeliveryDevice(SERVER::DEVICES device)
{
	QByteArray req;

	switch(device)
    {
        case SERVER::SMS:
            req = "device=sms";
            break;
        case SERVER::IM:
            req = "device=im";
           break;
        case SERVER::NONE:
            req = "device=none";
            break;
    }
    
    MakePostRequest(UPDATE_DELIVERY_DEVICE_URL,req,Returnables::DELIVERY_DEVICE);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::RemainingApiRequests()
{
    MakeGetRequest(REMAINING_API_REQUESTS_URL,Returnables::API_REQUESTS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::AddFavorite(QString id)
{
    QString buildUrl = ADD_FAVORITE_URL;
    
    buildUrl = buildUrl.replace("[req-id]",id);
    
    MakePostRequest(buildUrl,"",Returnables::ADD_FAVORITE);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::RemoveFavorite(QString id)
{
    QString buildUrl = REMOVE_FAVORITE_URL;
    
    buildUrl = buildUrl.replace("[req-id]",id);
    
    MakePostRequest(buildUrl,"",Returnables::REMOVE_FAVORITE);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::UpdateProfileColors(QString background_color, QString text_color, QString link_color, QString sidebar_fill_color, QString sidebar_border_color)
{
    QByteArray req = "";

    if(!background_color.isEmpty())
    {
        req = "profile_background_color=";
        req += background_color.remove("#").toAscii();
        req += "&";
    }
    if(!text_color.isEmpty())
    {
        req += "profile_text_color=";
        req += text_color.remove("#").toAscii();
        req += "&";
    }
    if(!link_color.isEmpty())
    {
        req += "profile_link_color=";
        req += link_color.remove("#").toAscii();
        req += "&";
    }
    if(!sidebar_fill_color.isEmpty())
    {
        req += "profile_sidebar_fill_color=";
        req += sidebar_fill_color.remove("#").toAscii();
        req += "&";
    }
    if(!sidebar_border_color.isEmpty())
    {
        req += "profile_sidebar_border_color=";
        req += sidebar_border_color.remove("#").toAscii();
    }

    if(req.endsWith("&")){ req.chop(1); }

    MakePostRequest(PROFILE_COLORS_URL,req,Returnables::PROFILE_COLORS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::UpdateProfile(QString name, QString email, QString url, QString location, QString description)
{
    QByteArray req = "";

    if(!name.isEmpty())
    {
        req = "name=";
        req += QUrl::toPercentEncoding(name);
        req += "&";
    }
    if(!email.isEmpty())
    {
        req += "email=";
        req += QUrl::toPercentEncoding(email);
        req += "&";
    }
    if(!url.isEmpty())
    {
        req += "url=";
        req += QUrl::toPercentEncoding(url);
        req += "&";
    }
    if(!location.isEmpty())
    {
        req += "location=";
        req += QUrl::toPercentEncoding(location);
        req += "&";
    }
    if(!description.isEmpty())
    {
        req += "description=";
        req += QUrl::toPercentEncoding(description);
    }

    if(req.endsWith("&")){ req.chop(1); }

    MakePostRequest(PROFILE_URL,req,Returnables::PROFILE);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::UpdateProfileImage(QString image)
{
    QByteArray req;

    QFile file(image);
        file.open(QIODevice::ReadOnly);

    QString filename = file.fileName();
    int pos = filename.lastIndexOf("/");
    filename.remove(0, pos + 1);

    QByteArray boundary = filename.toUtf8().toBase64();

    header.setRequest("POST", PROFILE_IMAGE_URL, 1, 1);
        header.setValue("Host", TWITTER_HOST);
        header.setValue("Connection", "keep-alive");
        header.setValue("Content-Type", "multipart/form-data; boundary=" + boundary);

        req.append("--" + boundary + "\r\n");
        req += "Content-Disposition: ";
        req += "form-data; name=\"image\"; filename=\"" + filename.toUtf8() + "\"\r\n";
        req += "Content-Type: image/png\r\n";
        req += "\r\n";
        req += file.readAll();
        req += "\r\n";
        req.append("--" + boundary + "--");
        req += "\r\n";

        header.setContentLength(req.length());

        file.close();
        MakePostRequest(PROFILE_IMAGE_URL, req, Returnables::PROFILE_IMAGE, true);
        //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::UpdateProfileBackgroundImage(QString image, QString isTile)
{
    QByteArray req;
    QFile file(image);
        file.open(QIODevice::ReadOnly);

    QString filename = file.fileName();
    int pos = filename.lastIndexOf("/");
    filename.remove(0, pos + 1);

    QByteArray boundary = filename.toUtf8().toBase64();

    header.setRequest("POST", PROFILE_BACKGROUND_IMAGE_URL, 1, 1);
        header.setValue("Host", TWITTER_HOST);
        header.setValue("Connection", "keep-alive");
        header.setValue("Content-Type", "multipart/form-data; boundary=" + boundary);

        req.append("--" + boundary + "\r\n");
        req += "Content-Disposition: ";
        req += "form-data; name=\"image\"; filename=\"" + filename.toUtf8() + "\"\r\n";
        req += "Content-Type: image/png\r\n";
        req += "\r\n";
        req += file.readAll();
        req += "\r\n";
        req.append("--" + boundary + "\r\n");
        req += "Content-Disposition: ";
        req += "form-data; name=\"tile\"\r\n";
        req += "\r\n";
        req += isTile;
        req += "\r\n";
        req.append("--" + boundary + "--");
        req += "\r\n";

        header.setContentLength(req.length());

    file.close();
    MakePostRequest(PROFILE_BACKGROUND_IMAGE_URL, req, Returnables::PROFILE_BACKGROUND_IMAGE, true);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::EnableNotifications(QString id)
{
    QString buildUrl = ENABLE_NOTIFICATIONS_URL;

    buildUrl = buildUrl.replace("[req-id]",id);

    MakePostRequest(buildUrl,"",Returnables::ENABLE_NOTIFICATIONS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::DisableNotifications(QString id)
{
    QString buildUrl = DISABLE_NOTIFICATIONS_URL;

    buildUrl = buildUrl.replace("[req-id]",id);

    MakePostRequest(buildUrl,"",Returnables::DISABLE_NOTIFICATIONS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::BlockUser(QString id)
{
    QString buildUrl = BLOCK_USER_URL;

    buildUrl = buildUrl.replace("[req-id]",id);

    MakePostRequest(buildUrl,"",Returnables::BLOCK_USER);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::UnBlockUser(QString id)
{
    QString buildUrl = UNBLOCK_USER_URL;

    buildUrl = buildUrl.replace("[req-id]",id);

    MakePostRequest(buildUrl,"",Returnables::UNBLOCK_USER);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::GetFriendsIDs(QString id, QString user_id, QString screen_name)
{
    QString buildUrl = FRIENDS_IDS_URL;

    if(!id.isEmpty())
    {
        buildUrl.replace("[/req-id]", "/" + id);
    }
    else if(!user_id.isEmpty())
    {
        buildUrl.replace("[/req-id]","");
        buildUrl += "?user_id=" + user_id;
    }
    else if(!screen_name.isEmpty())
    {
        buildUrl.replace("[/req-id]","");
        buildUrl += "?screen_name="+screen_name;
    }
    else
    {
        buildUrl.replace("[/req-id]","");
    }

    MakeGetRequest(buildUrl, Returnables::FRIENDS_IDS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================
void Core::GetFollowersIDs(QString id, QString user_id, QString screen_name)
{
    QString buildUrl = FOLLOWERS_IDS_URL;

    if(!id.isEmpty())
    {
        buildUrl.replace("[/req-id]", "/" + id);
    }
    else if(!user_id.isEmpty())
    {
        buildUrl.replace("[/req-id]","");
        buildUrl += "?user_id=" + user_id;
    }
    else if(!screen_name.isEmpty())
    {
        buildUrl.replace("[/req-id]","");
        buildUrl += "?screen_name="+screen_name;
    }
    else
    {
        buildUrl.replace("[/req-id]","");
    }

    MakeGetRequest(buildUrl, Returnables::FOLLOWERS_IDS);
    //m_eventLoop->exec(QEventLoop::AllEvents);
}
//=====================================================================

