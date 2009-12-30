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

#ifndef Core_H
#define Core_H

#include <QtCore/QObject>
#include <QtCore/QBuffer>
#include <QtCore/QEventLoop>
#include <QtCore/QMap>
#include <QtNetwork/QHttp>
#include "Server.h"
#include "Returnables.h"

class Core : public QObject
{
	Q_OBJECT;
          
	public:
		Core();
		virtual ~Core();
        
    public:
        void GetPublicTimeline();
        void GetSingleStatus(QString id);
        void GetFeaturedUsers();
        void Login(QString user, QString passw);
        void Logout();
        void SetLoginInfo(QString user, QString passw);
        void SetProxy(SERVER::PROXY_TYPE type = SERVER::NO_PROXY, const QString hostName = QString(), \
                    quint16 port = 0, const QString user = QString(), const QString password = QString());
        void IsTwitterUp();
        void GetUsersTimeline(SERVER::Option2 *opt=NULL);
        void GetFavorites(QString user="", unsigned int page=1);
        void GetFriendsTimeline(SERVER::Option1 *opt=NULL);
        void PostNewStatus(QString status, QString in_reply_to_status_id = "", QString source = "");
        void GetRecentMentions(SERVER::Option3 *opt=NULL);
        void RemoveStatus(QString id);
        void GetFriends(SERVER::Option4 *opt=NULL);
        void GetFollowers(SERVER::Option4 *opt=NULL);
        void GetUserDetails(QString id = "", QString user_id= "", QString screen_name = "");
        void GetSentDirectMessages(SERVER::Option5 *opt=NULL);
        void GetReceivedDirectMessages(SERVER::Option5 *opt=NULL);
        void SendDirectMessage(QString user, QString text);
        void RemoveDirectMessage(QString id);
        void AddFriendship(QString user, bool follow=true);
        void RemoveFriendship(QString user);
        void FriendshipExist(QString user_a, QString user_b);
        void VerifyCredentials();
        void UpdateDeliveryDevice(SERVER::DEVICES device);
        void RemainingApiRequests();
        void AddFavorite(QString id);
        void RemoveFavorite(QString id);
        void UpdateProfileColors(QString background_color = "", QString text_color = "", QString link_color = "", QString sidebar_fill_color = "", QString sidebar_border_color = "");
        void UpdateProfileImage(QString image);
        void UpdateProfileBackgroundImage(QString image, QString isTile);
        void UpdateProfile(QString name = "", QString email = "", QString url = "", \
                            QString location = "", QString description = "");
        void EnableNotifications(QString id);
        void DisableNotifications(QString id);
        void BlockUser(QString id);
        void UnBlockUser(QString id);
        void GetFriendsIDs(QString id = "", QString user_id = "", QString screen_name = "");
        void GetFollowersIDs(QString id = "", QString user_id = "", QString screen_name = "");
		
	public slots:
        void RequestStarted(int id);
		void ReqFinished(int id, bool error);
        void DataReadProgress(int done, int total);
        void Done(bool error);
        void Abort();
        
    signals:
        void QueryDone();
        void OnErrorString(QString error);
        void OnError(QHttp::Error error);
        void OnMessageReceived(QString message);
        void OnStatusReceived(SERVER::RESP resp);
		void OnResponseReceived(Returnables::Response *resp);

	private:
		struct Info
		{
			Info() { buffer = NULL; }
			QBuffer *buffer;
			Returnables::RequestId requestid;
		};

    private:
        void MakeConnections();
		void responseHeaderReceived(const QHttpResponseHeader &resp);
        int MakeGetRequest(QString req,Returnables::RequestId reqId);
        int MakePostRequest(QString path,QByteArray req,Returnables::RequestId reqId, bool isMultipart = false);
               
	private:
        QMap<int,Info> m_buffer;
        QEventLoop  *m_eventLoop;
		QHttp       *m_http;
                QHttpRequestHeader header;
		
	private:
		static QString TWITTER_HOST;
                static QString VERIFY_CREDENTIALS_URL;
		static QString PUBLIC_TIMELINE_URL;
                static QString GET_SINGLE_STATUS_URL;
                static QString FEATURED_USERS_URL;
                static QString LOGOUT_URL;
                static QString IS_TWITTER_UP_URL;
                static QString USERS_TIMELINE_URL;
                static QString GET_FAVORITES_URL;
                static QString FRIENDS_TIMELINE_URL;
                static QString POST_NEW_STATUS_URL;
                static QString GET_MENTIONS_URL;
                static QString REMOVE_STATUS_URL;
                static QString GET_FRIENDS_URL;
                static QString GET_FOLLOWERS_URL;
                static QString GET_USER_DETAILS_URL;
                static QString GET_SENT_DIRECT_MESSAGES_URL;
                static QString GET_RECEIVED_DIRECT_MESSAGES_URL;
                static QString SEND_NEW_DIRECT_MESSAGE_URL;
                static QString REMOVE_DIRECT_MESSAGE_URL;
                static QString CREATE_FRIENDSHIP_URL;
                static QString REMOVE_FRIENDSHIP_URL;
                static QString FRIENDSHIP_EXIST_URL;
                static QString UPDATE_DELIVERY_DEVICE_URL;
                static QString REMAINING_API_REQUESTS_URL;
                static QString ADD_FAVORITE_URL;
                static QString REMOVE_FAVORITE_URL;
                static QString PROFILE_COLORS_URL;
                static QString PROFILE_IMAGE_URL;
                static QString PROFILE_BACKGROUND_IMAGE_URL;
                static QString PROFILE_URL;
                static QString ENABLE_NOTIFICATIONS_URL;
                static QString DISABLE_NOTIFICATIONS_URL;
                static QString BLOCK_USER_URL;
                static QString UNBLOCK_USER_URL;
                static QString FRIENDS_IDS_URL;
                static QString FOLLOWERS_IDS_URL;
};

#endif // Core_H



