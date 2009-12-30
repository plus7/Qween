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

#include "Decipher.h"
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtCore/QTextStream>


//=====================================================================
Decipher* Decipher::instance = NULL;
QString Decipher::nStatus = "status";
QString Decipher::nCreatedAt = "created_at";
QString Decipher::nId = "id";
QString Decipher::nText = "text";
QString Decipher::nSource = "source";
QString Decipher::nTruncated = "truncated";
QString Decipher::nInReplyToStatusId = "in_reply_to_status_id";
QString Decipher::nInReplyToUserId = "in_reply_to_user_id";
QString Decipher::nFavorited = "favorited";
QString Decipher::nUser = "user";
QString Decipher::nName = "name";
QString Decipher::nScreenName = "screen_name";
QString Decipher::nDescription = "description";
QString Decipher::nLocation = "location";
QString Decipher::nProfileImageUrl = "profile_image_url";
QString Decipher::nUrl = "url";
QString Decipher::nProtected = "protected";
QString Decipher::nFollowersCount = "followers_count";
QString Decipher::nOk = "ok";
QString Decipher::nProfileBackgroundColor = "profile_background_color";
QString Decipher::nProfileTextColor = "profile_text_color";
QString Decipher::nProfileLinkColor = "profile_link_color";
QString Decipher::nProfileSidebarFillColor = "profile_sidebar_fill_color";
QString Decipher::nProfileSidebarBorderColor = "profile_sidebar_border_color";
QString Decipher::nFriendsCount = "friends_count";
QString Decipher::nFavoritesCount = "favourites_count";
QString Decipher::nUtcOffset = "utc_offset";
QString Decipher::nTimeZone = "time_zone";
QString Decipher::nProfileBackgroundImageUrl = "profile_background_image_url";
QString Decipher::nProfileBackgroundTiled = "profile_background_tile";
QString Decipher::nStatusesCount = "statuses_count";
QString Decipher::nDirectMessage = "direct_message";
QString Decipher::nSenderId = "sender_id";
QString Decipher::nRecipientId = "recipient_id";
QString Decipher::nSenderScreenName = "sender_screen_name";
QString Decipher::nRecipientScreenName = "recipient_screen_name";
QString Decipher::nSender = "sender";
QString Decipher::nRecipient = "recipient";
QString Decipher::nFriends = "friends";
QString Decipher::nHash = "hash";
QString Decipher::nResetTime = "reset-time";
QString Decipher::nResetTimeSeconds = "reset-time-in-seconds";
QString Decipher::nRemainingHits = "remaining-hits";
QString Decipher::nHourlyLimit = "hourly-limit";
QString Decipher::nInReplyToScreenName = "in_reply_to_screen_name";
QString Decipher::nFollowing = "following";
QString Decipher::nNotifications = "notifications";
QString Decipher::nID = "id";
//=====================================================================
Decipher::Decipher()
{}
//=====================================================================
Decipher::~Decipher()
{}
//=====================================================================
Decipher* Decipher::Instance()
{
        if(instance == NULL)
        {
                instance = new Decipher();
        }

        return instance;
}
//=====================================================================
QLinkedList<Returnables::StatusElementPtr > Decipher::GetStatusElementList(const QString &xml)
{
        QDomDocument doc;
        QDomNodeList nodeList;
        QLinkedList<Returnables::StatusElementPtr > statusElementList;

        doc.setContent(xml);
        nodeList = doc.elementsByTagName(nStatus);

        for(int i=0; i<nodeList.count(); i++)
        {
                Returnables::StatusElementPtr item = Returnables::StatusElementPtr(new Returnables::StatusElement());
                QDomElement node;
                node = nodeList.at(i).toElement();

                Populate_seStatus(item->status,node);
                node = node.namedItem(nUser).toElement();
                PopulateUser(item->user,node);

                statusElementList.append(item);
        }
        return statusElementList;
}
//=====================================================================
QLinkedList<unsigned int> Decipher::GetIDsList(const QString &xml)
{
        QDomDocument doc;
        QDomNodeList nodeList;
        QLinkedList<unsigned int> IDsList;

        doc.setContent(xml);
        nodeList = doc.elementsByTagName(nID);

        for(int i=0; i<nodeList.count(); i++)
        {
                unsigned int item;

                QDomElement node;
                item = nodeList.at(i).toElement().text().toUInt();
                qDebug("%d", item);
                IDsList.append(item);
        }
        return IDsList;
}
//=====================================================================
QLinkedList<Returnables::BasicUserInfoElementPtr> Decipher::GetBasicUserInfoElementList(const QString &xml)
{
        QDomDocument doc;
        QDomNodeList nodeList;
        QLinkedList<Returnables::BasicUserInfoElementPtr> basicUserInfoElementList;

        doc.setContent(xml);
        nodeList = doc.elementsByTagName(nUser);

        for(int i=0; i<nodeList.count(); i++)
        {
                Returnables::BasicUserInfoElementPtr item =
                        Returnables::BasicUserInfoElementPtr(new Returnables::BasicUserInfoElement());
                QDomElement node;
                node = nodeList.at(i).toElement();

                PopulateUser(item->user,node);
                node = node.namedItem(nStatus).toElement();
                Populate_biStatus(item->status,node);

                basicUserInfoElementList.append(item);
        }
        return basicUserInfoElementList;
}
//=====================================================================
QLinkedList<Returnables::ExtUserInfoElementPtr> Decipher::GetExtUserInfoElementList(const QString &xml)
{
        QDomDocument doc;
        QDomNodeList nodeList;
        QLinkedList<Returnables::ExtUserInfoElementPtr> extUserInfoElementList;

        doc.setContent(xml);
        nodeList = doc.elementsByTagName(nUser);

        for(int i=0; i<nodeList.count(); i++)
        {
                Returnables::ExtUserInfoElementPtr item =
                        Returnables::ExtUserInfoElementPtr(new Returnables::ExtUserInfoElement);
                QDomElement node;
                node = nodeList.at(i).toElement();

                PopulateUser(item->user,node);
                PopulateDetails(item->details,node);
                node = node.namedItem(nStatus).toElement();
                Populate_biStatus(item->status, node);

                extUserInfoElementList.append(item);
        }
        return extUserInfoElementList;
}
//=====================================================================
QLinkedList<Returnables::DirectMessageElementPtr> Decipher::GetDirectMessageList(const QString &xml)
{
        QDomDocument doc;
        QDomNodeList nodeList;
        QLinkedList<Returnables::DirectMessageElementPtr> directMessageList;

        doc.setContent(xml);
        nodeList = doc.elementsByTagName(nDirectMessage);

        for(int i=0; i<nodeList.count(); i++)
        {
                Returnables::DirectMessageElementPtr directMessage =
                        Returnables::DirectMessageElementPtr(new Returnables::DirectMessageElement());
                QDomElement nodeSender, nodeRecipient;
                QDomElement node;

                node = nodeList.at(i).toElement();
                PopulateDirectHeader(directMessage->headerInfo,node);
                nodeSender = node.namedItem(nSender).toElement();
                PopulateUser(directMessage->sender,nodeSender);
                nodeRecipient = node.namedItem(nRecipient).toElement();
                PopulateUser(directMessage->recipient,nodeRecipient);

                directMessageList.append(directMessage);
        }

        return directMessageList;
}
//=====================================================================
void Decipher::Populate_seStatus(Returnables::seStatus &status, const QDomElement &node)
{
        status.createdAt = node.namedItem(nCreatedAt).toElement().text();
        status.id = node.namedItem(nId).toElement().text().toULongLong();
        status.text = node.namedItem(nText).toElement().text();
        status.source = node.namedItem(nSource).toElement().text();
        status.truncated = (node.namedItem(nTruncated).toElement().text().toLower() == "true") ? true : false;
        status.inReplyToStatusId = node.namedItem(nInReplyToStatusId).toElement().text().toULongLong();
        status.inReplyToUserId = node.namedItem(nInReplyToUserId).toElement().text().toULongLong();
        status.favorited = (node.namedItem(nFavorited).toElement().text().toLower() == "true") ? true : false;
}
//=====================================================================
void Decipher::Populate_biStatus(Returnables::biStatus &status, const QDomElement &node)
{
        status.createdAt = node.namedItem(nCreatedAt).toElement().text();
        status.id = node.namedItem(nId).toElement().text().toULongLong();
        status.text = node.namedItem(nText).toElement().text();
        status.source = node.namedItem(nSource).toElement().text();
        status.truncated = (node.namedItem(nTruncated).toElement().text().toLower() == "true") ? true : false;
        status.inReplyToStatusId = node.namedItem(nInReplyToStatusId).toElement().text().toULongLong();
        status.inReplyToUserId = node.namedItem(nInReplyToUserId).toElement().text().toULongLong();
        status.favorited = (node.namedItem(nFavorited).toElement().text().toLower() == "true") ? true : false;
        status.inReplyToScreenName = node.namedItem(nInReplyToScreenName).toElement().text();
}
//=====================================================================
void Decipher::PopulateUser(Returnables::User &user, const QDomElement &node)
{
        user.id = node.namedItem(nId).toElement().text().toUInt();
        user.name = node.namedItem(nName).toElement().text();
        user.screenName = node.namedItem(nScreenName).toElement().text();
        user.description = node.namedItem(nDescription).toElement().text();
        user.location = node.namedItem(nLocation).toElement().text();
        user.profileImageUrl = node.namedItem(nProfileImageUrl).toElement().text();
        user.url = node.namedItem(nUrl).toElement().text();
        user.isProtected = (node.namedItem(nProtected).toElement().text().toLower() == "true") ? true : false;
        user.followersCount = node.namedItem(nFollowersCount).toElement().text().toUInt();
}
//=====================================================================
void Decipher::PopulateDetails(Returnables::Details &details, const QDomElement &node)
{
        details.profileBackgroundColor = node.namedItem(nProfileBackgroundColor).toElement().text();
        details.profileTextColor = node.namedItem(nProfileTextColor).toElement().text();
        details.profileLinkColor = node.namedItem(nProfileLinkColor).toElement().text();
        details.profileSidebarFillColor = node.namedItem(nProfileSidebarFillColor).toElement().text();
        details.profileSidebarBorderColor = node.namedItem(nProfileSidebarBorderColor).toElement().text();
        details.friendsCount = node.namedItem(nFriendsCount).toElement().text().toUInt();
        details.createdAt = node.namedItem(nCreatedAt).toElement().text();
        details.favoritesCount = node.namedItem(nFavoritesCount).toElement().text().toUInt();
        details.utcOffset = node.namedItem(nUtcOffset).toElement().text().toInt();
        details.timeZone = node.namedItem(nTimeZone).toElement().text();
        details.profileBackgroundImageUrl = node.namedItem(nProfileBackgroundImageUrl).toElement().text();
        details.profileBackgroundTiled = (node.namedItem(nProfileBackgroundTiled).toElement().text().toLower() == "true") ? true : false;
        details.following = (node.namedItem(nFollowing).toElement().text().toLower() == "true") ? true : false;
        details.notifications = (node.namedItem(nNotifications).toElement().text().toLower() == "true") ? true : false;
        details.statusesCount = node.namedItem(nStatusesCount).toElement().text().toUInt();
}
//=====================================================================
void Decipher::PopulateDirectHeader(Returnables::DirectHeader &header, const QDomElement &node)
{
        header.createdAt = node.namedItem(nCreatedAt).toElement().text();
        header.id = node.namedItem(nId).toElement().text().toULongLong();
        header.recipientId = node.namedItem(nRecipientId).toElement().text().toUInt();
        header.recipientScreenName = node.namedItem(nRecipientScreenName).toElement().text();
        header.senderId = node.namedItem(nSenderId).toElement().text().toUInt();
        header.senderScreenName = node.namedItem(nSenderScreenName).toElement().text();
        header.text = node.namedItem(nText).toElement().text();
}
//=====================================================================
void Decipher::PopulateApiRequests(Returnables::ApiRequests &apiRequests, const QDomElement &node)
{
        apiRequests.hourlyLimit = node.namedItem(nHourlyLimit).toElement().text().toUInt();
        apiRequests.remainingHits = node.namedItem(nRemainingHits).toElement().text().toUInt();
        apiRequests.resetTime = node.namedItem(nResetTime).toElement().text();
        apiRequests.resetTimeSeconds = node.namedItem(nResetTimeSeconds).toElement().text().toUInt();
}
//=====================================================================
Returnables::PublicTimeline* Decipher::PublicTimeline(const QString &xml)
{
        Returnables::PublicTimeline *publicTimeline = NULL;
        QLinkedList<Returnables::StatusElementPtr> list = GetStatusElementList(xml);

        if(!list.isEmpty())
        {
                publicTimeline = new Returnables::PublicTimeline();
                publicTimeline->list = list;
                publicTimeline->reqID = Returnables::PUBLIC_TIMELINE;
        }

        return publicTimeline;
}
//=====================================================================
Returnables::FriendsTimeline* Decipher::FriendsTimeline(const QString &xml)
{
        Returnables::FriendsTimeline *friendsTimeline = NULL;
        QLinkedList<Returnables::StatusElementPtr> list = GetStatusElementList(xml);

        if(!list.isEmpty())
        {
                friendsTimeline = new Returnables::FriendsTimeline();
                friendsTimeline->list = list;
                friendsTimeline->reqID = Returnables::FRIENDS_TIMELINE;
        }

        return friendsTimeline;
}
//=====================================================================
Returnables::SingleStatus* Decipher::SingleStatus(const QString &xml)
{
        Returnables::SingleStatus *singleStatus = NULL;
        QLinkedList<Returnables::StatusElementPtr> list = GetStatusElementList(xml);

        if(!list.isEmpty())
        {
                singleStatus = new Returnables::SingleStatus();
                singleStatus->status = list.first();
                singleStatus->reqID = Returnables::SINGLE_STATUS;
        }

        return singleStatus;
}
//=====================================================================
Returnables::FeaturedUsers* Decipher::FeaturedUsers(const QString &xml)
{
        Returnables::FeaturedUsers *featuredUsers = NULL;
        QLinkedList<Returnables::BasicUserInfoElementPtr> list = GetBasicUserInfoElementList(xml);

        if(!list.isEmpty())
        {
                featuredUsers = new Returnables::FeaturedUsers();
                featuredUsers->list = list;
                featuredUsers->reqID = Returnables::FEATURED_USERS;
        }

        return featuredUsers;
}
//=====================================================================
Returnables::VerifyCredentials* Decipher::VerifyCredentials(const QString &xml)
{
        Returnables::VerifyCredentials *verifyCredentials = NULL;
        QLinkedList<Returnables::ExtUserInfoElementPtr> list = GetExtUserInfoElementList(xml);

        if(!list.isEmpty())
        {
            verifyCredentials = new Returnables::VerifyCredentials;
            verifyCredentials->userExt = list.first();
            verifyCredentials->reqID = Returnables::VERIFY_CREDENTIALS;
        }
        return verifyCredentials;
}
//=====================================================================
Returnables::TwitterUp* Decipher::TwitterUp(const QString &xml)
{
        Returnables::TwitterUp *twitterUp = NULL;
        QDomDocument doc;
        QDomElement elem;

        doc.setContent(xml);
        elem = doc.namedItem(nOk).toElement();

        if(!elem.isNull())
        {
                twitterUp = new Returnables::TwitterUp();
                twitterUp->up = (elem.text().toLower().contains("true")) ? true : false;
                twitterUp->reqID = Returnables::TWITTER_UP;
        }

        return twitterUp;
}
//=====================================================================
Returnables::UserTimeline* Decipher::UserTimeline(const QString &xml)
{
        Returnables::UserTimeline *userTimeline = NULL;
        QLinkedList<Returnables::StatusElementPtr> list = GetStatusElementList(xml);

        if(!list.isEmpty())
        {
                userTimeline = new Returnables::UserTimeline();
                userTimeline->list = list;
                userTimeline->reqID = Returnables::USER_TIMELINE;
        }

        return userTimeline;
}
//=====================================================================
Returnables::Favorites* Decipher::Favorites(const QString &xml)
{
        Returnables::Favorites *favorites = NULL;
        QLinkedList<Returnables::StatusElementPtr> list = GetStatusElementList(xml);

        if(!list.isEmpty())
        {
                favorites = new Returnables::Favorites();
                favorites->list = list;
                favorites->reqID = Returnables::FAVORITES;
        }

        return favorites;
}
//=====================================================================
Returnables::NewStatus* Decipher::NewStatus(const QString &xml)
{
        Returnables::NewStatus *newStatus = NULL;
        QLinkedList<Returnables::StatusElementPtr> list = GetStatusElementList(xml);

        if(!list.isEmpty())
        {
                newStatus = new Returnables::NewStatus();
                newStatus->status = list.first();
                newStatus->reqID = Returnables::NEW_STATUS;
        }

        return newStatus;
}
//=====================================================================
Returnables::RecentMentions* Decipher::RecentMentions(const QString &xml)
{
        Returnables::RecentMentions *mentions = NULL;
        QLinkedList<Returnables::StatusElementPtr> list = GetStatusElementList(xml);

        if(!list.isEmpty())
        {
                mentions = new Returnables::RecentMentions();
                mentions->list = list;
                mentions->reqID = Returnables::RECENT_MENTIONS;
        }

        return mentions;
}
//=====================================================================
Returnables::RemoveStatus* Decipher::RemoveStatus(const QString &xml)
{
        Returnables::RemoveStatus *removedStatus = NULL;
        QLinkedList<Returnables::StatusElementPtr> list = GetStatusElementList(xml);

        if(!list.isEmpty())
        {
                removedStatus = new Returnables::RemoveStatus();
                removedStatus->status = list.first();
                removedStatus->reqID = Returnables::REMOVE_STATUS;
        }

        return removedStatus;
}

//=====================================================================
Returnables::Friends* Decipher::Friends(const QString &xml)
{
        Returnables::Friends *friends = NULL;
        QLinkedList<Returnables::BasicUserInfoElementPtr> list = GetBasicUserInfoElementList(xml);

        if(!list.isEmpty())
        {
                friends = new Returnables::Friends();
                friends->list = list;
                friends->reqID = Returnables::FRIENDS;
        }

        return friends;
}
//=====================================================================
Returnables::Followers* Decipher::Followers(const QString &xml)
{
        Returnables::Followers *followers = NULL;
        QLinkedList<Returnables::BasicUserInfoElementPtr> list = GetBasicUserInfoElementList(xml);

        if(!list.isEmpty())
        {
                followers = new Returnables::Followers();
                followers->list = list;
                followers->reqID = Returnables::FOLLOWERS;
        }

        return followers;
}
//=====================================================================
Returnables::UserDetails* Decipher::UserDetails(const QString &xml)
{
        Returnables::UserDetails *userDetails = NULL;
        QLinkedList<Returnables::ExtUserInfoElementPtr> list = GetExtUserInfoElementList(xml);

        if(!list.isEmpty())
        {
            userDetails = new Returnables::UserDetails();
            userDetails->userExt = list.first();
            userDetails->reqID = Returnables::USER_DETAILS;
        }
//Not in API
//	elem = elem.firstChildElement(nStatus);
//	success = (success && !elem.isNull()) ? true : false;
//	PopulateStatus(*userDetails->status,elem);

        return userDetails;
}
//=====================================================================
Returnables::SentDirectMessages* Decipher::SentDirectMessages(const QString &xml)
{
        Returnables::SentDirectMessages *sentDirectMessage = NULL;
        QLinkedList<Returnables::DirectMessageElementPtr> list = GetDirectMessageList(xml);

        if(!list.isEmpty())
        {
                sentDirectMessage = new Returnables::SentDirectMessages();
                sentDirectMessage->list = list;
                sentDirectMessage->reqID = Returnables::SENT_DIRECT_MESSAGES;
        }

        return sentDirectMessage;
}
//=====================================================================
Returnables::ReceivedDirectMessages* Decipher::ReceivedDirectMessages(const QString &xml)
{
        Returnables::ReceivedDirectMessages *receivedDirectMessages = NULL;
        QLinkedList<Returnables::DirectMessageElementPtr> list = GetDirectMessageList(xml);

        if(!list.isEmpty())
        {
                receivedDirectMessages = new Returnables::ReceivedDirectMessages();
                receivedDirectMessages->list = list;
                receivedDirectMessages->reqID = Returnables::RECEIVED_DIRECT_MESSAGES;
        }

        return receivedDirectMessages;
}
//=====================================================================
Returnables::SendDirectMessage* Decipher::SendDirectMessage(const QString &xml)
{
        Returnables::SendDirectMessage *sendDirectMessage = NULL;
        QLinkedList<Returnables::DirectMessageElementPtr> list = GetDirectMessageList(xml);

        if(!list.isEmpty())
        {
                sendDirectMessage = new Returnables::SendDirectMessage();
                sendDirectMessage->message = list.first();
                sendDirectMessage->reqID = Returnables::SEND_DIRECT_MESSAGE;
        }

        return sendDirectMessage;
}
//=====================================================================
Returnables::RemoveDirectMessage* Decipher::RemoveDirectMessage(const QString &xml)
{
        Returnables::RemoveDirectMessage *removeDirectMessage = NULL;
        QLinkedList<Returnables::DirectMessageElementPtr> list = GetDirectMessageList(xml);

        if(!list.isEmpty())
        {
                removeDirectMessage = new Returnables::RemoveDirectMessage();
                removeDirectMessage->message = list.first();
                removeDirectMessage->reqID = Returnables::REMOVE_DIRECT_MESSAGE;
        }

        return removeDirectMessage;
}
//=====================================================================
Returnables::AddFriendship* Decipher::AddFriendShip(const QString &xml)
{
        Returnables::AddFriendship *addFriendship = NULL;
        QLinkedList<Returnables::BasicUserInfoElementPtr> list = GetBasicUserInfoElementList(xml);

        if(!list.isEmpty())
        {
                addFriendship = new Returnables::AddFriendship();
                addFriendship->user = list.first();
                addFriendship->reqID = Returnables::ADD_FRIENDSHIP;
        }

        return addFriendship;
}
//=====================================================================
Returnables::RemoveFriendship* Decipher::RemoveFriendship(const QString &xml)
{
        Returnables::RemoveFriendship *removeFriendship = NULL;
        QLinkedList<Returnables::BasicUserInfoElementPtr> list = GetBasicUserInfoElementList(xml);

        if(!list.isEmpty())
        {
                removeFriendship = new Returnables::RemoveFriendship();
                removeFriendship->user = list.first();
                removeFriendship->reqID = Returnables::REMOVE_FRIENDSHIP;
        }

        return removeFriendship;
}
//=====================================================================
Returnables::FriendshipExist* Decipher::FriendshipExist(const QString &xml)
{
        Returnables::FriendshipExist* friendshipExists = NULL;
        QDomDocument doc;
        QDomElement elem;

        doc.setContent(xml);
        elem = doc.namedItem(nFriends).toElement();

        if(!elem.isNull())
        {
                friendshipExists = new Returnables::FriendshipExist();
                friendshipExists->friends = (elem.text().toLower().contains("true")) ? true : false;
                friendshipExists->reqID = Returnables::FRIENDSHIP_EXISTS;
        }

        return friendshipExists;
}
//=====================================================================
Returnables::DeliveryDevice* Decipher::DeliveryDevice(const QString &xml)
{
        Returnables::DeliveryDevice *deliveryDevice = NULL;
        QLinkedList<Returnables::BasicUserInfoElementPtr> list = GetBasicUserInfoElementList(xml);

        if(!list.isEmpty())
        {
                deliveryDevice = new Returnables::DeliveryDevice();
                deliveryDevice->user = list.first();
                deliveryDevice->reqID = Returnables::DELIVERY_DEVICE;
        }

        return deliveryDevice;
}
//=====================================================================
Returnables::ApiRequests* Decipher::ApiRequests(const QString &xml)
{
        Returnables::ApiRequests* apiRequests = NULL;
        QDomDocument doc;
        QDomElement elem;

        doc.setContent(xml);
        elem = doc.namedItem(nHash).toElement();

        if(!elem.isNull())
        {
                apiRequests = new Returnables::ApiRequests();
                PopulateApiRequests(*apiRequests,elem);
                apiRequests->reqID = Returnables::API_REQUESTS;
        }

        return apiRequests;
}
//=====================================================================
Returnables::AddFavorite* Decipher::AddFavorite(const QString &xml)
{
        Returnables::AddFavorite *addFavorite = NULL;
        QLinkedList<Returnables::StatusElementPtr> list = GetStatusElementList(xml);

        if(!list.isEmpty())
        {
                addFavorite = new Returnables::AddFavorite();
                addFavorite->status = list.first();
                addFavorite->reqID = Returnables::ADD_FAVORITE;
        }

        return addFavorite;
}
//=====================================================================
Returnables::RemoveFavorite* Decipher::RemoveFavorite(const QString &xml)
{
        Returnables::RemoveFavorite *removeFavorite = NULL;
        QLinkedList<Returnables::StatusElementPtr> list = GetStatusElementList(xml);

        if(!list.isEmpty())
        {
                removeFavorite = new Returnables::RemoveFavorite();
                removeFavorite->status = list.first();
                removeFavorite->reqID = Returnables::REMOVE_FAVORITE;
        }

        return removeFavorite;
}
//=====================================================================
Returnables::ProfileColors* Decipher::ProfileColors(const QString &xml)
{
        Returnables::ProfileColors *profileColors = NULL;
        QLinkedList<Returnables::ExtUserInfoElementPtr> list = GetExtUserInfoElementList(xml);

        if(!list.isEmpty())
        {
            profileColors = new Returnables::ProfileColors();
            profileColors->userExt = list.first();
            profileColors->reqID = Returnables::PROFILE_COLORS;
        }

        return profileColors;
}
//=====================================================================
Returnables::ProfileImage* Decipher::ProfileImage(const QString &xml)
{
        Returnables::ProfileImage *profileImage = NULL;
        QLinkedList<Returnables::ExtUserInfoElementPtr> list = GetExtUserInfoElementList(xml);

        if(!list.isEmpty())
        {
            profileImage = new Returnables::ProfileImage;
            profileImage->userExt = list.first();
            profileImage->reqID = Returnables::PROFILE_IMAGE;
        }

        return profileImage;
}
//=====================================================================
Returnables::ProfileBackgroundImage* Decipher::ProfileBackgroundImage(const QString &xml)
{
        Returnables::ProfileBackgroundImage *profileBackgroundImage = NULL;
        QLinkedList<Returnables::ExtUserInfoElementPtr> list = GetExtUserInfoElementList(xml);

        if(!list.isEmpty())
        {
            profileBackgroundImage = new Returnables::ProfileBackgroundImage;
            profileBackgroundImage->userExt = list.first();
            profileBackgroundImage->reqID = Returnables::PROFILE_BACKGROUND_IMAGE;
        }

        return profileBackgroundImage;
}
//=====================================================================
Returnables::Profile* Decipher::Profile(const QString &xml)
{
        Returnables::Profile *profile = NULL;
        QLinkedList<Returnables::ExtUserInfoElementPtr> list = GetExtUserInfoElementList(xml);

        if(!list.isEmpty())
        {
            profile = new Returnables::Profile;
            profile->userExt = list.first();
            profile->reqID = Returnables::PROFILE;
        }

        return profile;
}
//=====================================================================
Returnables::EnableNotifications* Decipher::EnableNotifications(const QString &xml)
{
        Returnables::EnableNotifications *enableNotifications = NULL;
        QLinkedList<Returnables::BasicUserInfoElementPtr > list = GetBasicUserInfoElementList(xml);

        if(!list.isEmpty())
        {
                enableNotifications = new Returnables::EnableNotifications();
                enableNotifications->user = list.first();
                enableNotifications->reqID = Returnables::ENABLE_NOTIFICATIONS;
        }

        return enableNotifications;
}
//=====================================================================
Returnables::DisableNotifications* Decipher::DisableNotifications(const QString &xml)
{
        Returnables::DisableNotifications *disableNotifications = NULL;
        QLinkedList<Returnables::BasicUserInfoElementPtr> list = GetBasicUserInfoElementList(xml);

        if(!list.isEmpty())
        {
                disableNotifications = new Returnables::DisableNotifications();
                disableNotifications->user = list.first();
                disableNotifications->reqID = Returnables::DISABLE_NOTIFICATIONS;
        }

        return disableNotifications;
}
//=====================================================================
Returnables::BlockUser* Decipher::BlockUser(const QString &xml)
{
        Returnables::BlockUser *blockUser = NULL;
        QLinkedList<Returnables::BasicUserInfoElementPtr > list = GetBasicUserInfoElementList(xml);

        if(!list.isEmpty())
        {
                blockUser = new Returnables::BlockUser();
                blockUser->user = list.first();
                blockUser->reqID = Returnables::BLOCK_USER;
        }

        return blockUser;
}
//=====================================================================
Returnables::UnBlockUser* Decipher::UnBlockUser(const QString &xml)
{
        Returnables::UnBlockUser *unBlockUser = NULL;
        QLinkedList<Returnables::BasicUserInfoElementPtr> list = GetBasicUserInfoElementList(xml);

        if(!list.isEmpty())
        {
                unBlockUser = new Returnables::UnBlockUser();
                unBlockUser->user = list.first();
                unBlockUser->reqID = Returnables::UNBLOCK_USER;
        }

        return unBlockUser;
}
//=====================================================================
Returnables::FriendsIDs* Decipher::FriendsIDs(const QString &xml)
{
        Returnables::FriendsIDs *friendsIDs = NULL;
        QLinkedList<unsigned int> list = GetIDsList(xml);

        if(!list.isEmpty())
        {
                friendsIDs = new Returnables::FriendsIDs();
                friendsIDs->list = list;
                friendsIDs->reqID = Returnables::FRIENDS_IDS;
        }
        return friendsIDs;
}
//=====================================================================
Returnables::FollowersIDs* Decipher::FollowersIDs(const QString &xml)
{
        Returnables::FollowersIDs *followersIDs = NULL;
        QLinkedList<unsigned int> list = GetIDsList(xml);

        if(!list.isEmpty())
        {
                followersIDs = new Returnables::FollowersIDs;
                followersIDs->list = list;
                followersIDs->reqID = Returnables::FOLLOWERS_IDS;
        }

        return followersIDs;
}
//=====================================================================
