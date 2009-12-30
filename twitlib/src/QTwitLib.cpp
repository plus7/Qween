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

#include "QTwitLib.h"
#include "Core.h"

//=====================================================================
QTwitLib::QTwitLib()
{}
//=====================================================================
QTwitLib::~QTwitLib()
{}
//=====================================================================
void QTwitLib::GetPublicTimeline()
{
    Core::GetPublicTimeline();
}
//=====================================================================
void QTwitLib::GetSingleStatus(quint64 id)
{
    Core::GetSingleStatus(QString::number(id));
}
//=====================================================================
void QTwitLib::GetFeaturedUsers()
{
    Core::GetFeaturedUsers();
}
//=====================================================================
void QTwitLib::Login(QString user, QString password)
{
    Core::Login(user, password);
}
//=====================================================================
void QTwitLib::Logout()
{
    Core::Logout();
}
//=====================================================================
void QTwitLib::Abort()
{
    Core::Abort();
}
//=====================================================================
void QTwitLib::SetLoginInfo(QString user, QString password)
{
    Core::SetLoginInfo(user, password);
}
//=====================================================================
void QTwitLib::IsTwitterUp()
{
    Core::IsTwitterUp();
}
//=====================================================================
void QTwitLib::GetUsersTimeline(SERVER::Option2 *opt  /*=NULL*/)
{
    Core::GetUsersTimeline(opt);
}
//=====================================================================
void QTwitLib::GetFavorites(QString user  /*=""*/, unsigned int page  /*=1*/)
{
    Core::GetFavorites(user, page);
}
//=====================================================================
void QTwitLib::GetFriendsTimeline(SERVER::Option1 *opt  /*=NULL*/)
{
    Core::GetFriendsTimeline(opt);
}
//=====================================================================
void QTwitLib::PostNewStatus(QString status, quint64 in_reply_to_status_id, QString source)
{
    Core::PostNewStatus(status, QString::number(in_reply_to_status_id), source);
}
//=====================================================================
void QTwitLib::GetRecentMentions(SERVER::Option3 *opt  /*=NULL*/)
{
    Core::GetRecentMentions(opt);
}
//=====================================================================
void QTwitLib::RemoveStatus(quint64 id)
{
    Core::RemoveStatus(QString::number(id));
}
//=====================================================================
void QTwitLib::GetFriends(SERVER::Option4 *opt  /*=NULL*/)
{
    Core::GetFriends(opt);
}
//=====================================================================
void QTwitLib::GetFollowers(SERVER::Option4 *opt  /*=NULL*/)
{
    Core::GetFollowers(opt);
}
//=====================================================================
void QTwitLib::GetUserDetails(QString id, unsigned int user_id, QString screen_name)
{
    Core::GetUserDetails(id, QString::number(user_id), screen_name);
}
//=====================================================================
void QTwitLib::GetSentDirectMessages(SERVER::Option5 *opt  /*=NULL*/)
{
    Core::GetSentDirectMessages(opt);
}
//=====================================================================
void QTwitLib::GetReceivedDirectMessages(SERVER::Option5 *opt  /*=NULL*/)
{
    Core::GetReceivedDirectMessages(opt);
}
//=====================================================================
void QTwitLib::SendDirectMessage(QString user, QString text)
{
    Core::SendDirectMessage(user, text);
}
//=====================================================================
void QTwitLib::RemoveDirectMessage(quint64 id)
{
    Core::RemoveDirectMessage(QString::number(id));
}
//=====================================================================
void QTwitLib::AddFriendship(QString user, bool follow /*=true*/)
{
    Core::AddFriendship(user, follow);
}
//=====================================================================
void QTwitLib::RemoveFriendship(QString user)
{
    Core::RemoveFriendship(user);
}
//=====================================================================
void QTwitLib::FriendshipExist(QString user_a, QString user_b)
{
    Core::FriendshipExist(user_a, user_b);
}
//=====================================================================
void QTwitLib::VerifyCredentials()
{
    Core::VerifyCredentials();
}
//=====================================================================
void QTwitLib::UpdateDeliveryDevice(SERVER::DEVICES device)
{
    Core::UpdateDeliveryDevice(device);
}
//=====================================================================
void QTwitLib::RemainingApiRequests()
{
    Core::RemainingApiRequests();
}
//=====================================================================
void QTwitLib::AddFavorite(quint64 id)
{
    Core::AddFavorite(QString::number(id));
}
//=====================================================================
void QTwitLib::RemoveFavorite(quint64 id)
{
    Core::RemoveFavorite(QString::number(id));
}
//=====================================================================
void QTwitLib::UpdateProfileColors(QString background_color, QString text_color, QString link_color, QString sidebar_fill_color , QString sidebar_border_color)
{
    Core::UpdateProfileColors(background_color, text_color,link_color,sidebar_fill_color, sidebar_border_color);
}
//=====================================================================
void QTwitLib::UpdateProfileImage(QString image)
{
    Core::UpdateProfileImage(image);
}
//=====================================================================
void QTwitLib::UpdateProfileBackgroundImage(QString image, bool tile)
{
    QString isTile = tile ? "true" : "false";
    Core::UpdateProfileBackgroundImage(image, isTile);
}
//=====================================================================
void QTwitLib::UpdateProfile(QString name, QString email, QString url, QString location, QString description)
{
    Core::UpdateProfile(name, email, url, location, description);
}
//=====================================================================
void QTwitLib::EnableNotifications(QString id)
{
    Core::EnableNotifications(id);
}
//=====================================================================
void QTwitLib::DisableNotifications(QString id)
{
    Core::DisableNotifications(id);
}
//=====================================================================
void QTwitLib::BlockUser(QString id)
{
    Core::BlockUser(id);
}
//=====================================================================
void QTwitLib::UnBlockUser(QString id)
{
    Core::UnBlockUser(id);
}
//=====================================================================
void QTwitLib::GetFriendsIDs(QString id, QString user_id, QString screen_name)
{
    Core::GetFriendsIDs(id, user_id, screen_name);
}
//=====================================================================

void QTwitLib::GetFollowersIDs(QString id,QString user_id, QString screen_name)
{
    Core::GetFollowersIDs(id, user_id, screen_name);
}
//=====================================================================
