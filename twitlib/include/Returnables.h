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

#ifndef Returnables_H
#define Returnables_H

#include <QtCore/QLinkedList>
#include <QtCore/QMap>
#include <QSharedPointer>
#include "Server.h"

namespace Returnables
{
        struct User
        {
                unsigned int id;
                QString name;
                QString screenName;
                QString description;
                QString location;
                QString profileImageUrl;
                QString url;
                bool isProtected;
                unsigned int followersCount;
        };

        struct Details
        {
                QString profileBackgroundColor;
                QString profileTextColor;
                QString profileLinkColor;
                QString profileSidebarFillColor;
                QString profileSidebarBorderColor;
                unsigned int friendsCount;
                QString createdAt;
                unsigned int favoritesCount;
                int utcOffset;
                QString timeZone;
                QString profileBackgroundImageUrl;
                bool profileBackgroundTiled;
                bool following;
                bool notifications;
                unsigned int statusesCount;
        };

//Status for "Status element"
        struct seStatus
        {
                QString createdAt;
                quint64 id;
                QString text;
                QString source;
                bool truncated;
                quint64 inReplyToStatusId;
                quint64 inReplyToUserId;
                bool favorited;
        };

//Status for "Basic user information element"
        struct biStatus
        {
                QString createdAt;
                quint64 id;
                QString text;
                QString source;
                bool truncated;
                quint64 inReplyToStatusId;
                quint64 inReplyToUserId;
                bool favorited;
                QString inReplyToScreenName;
        };

        struct DirectHeader
        {
                quint64 id;
                unsigned int senderId;
                QString text;
                unsigned int recipientId;
                QString createdAt;
                QString senderScreenName;
                QString recipientScreenName;
        };

        struct DirectMessageElement
        {
                DirectHeader headerInfo;
                User sender;
                User recipient;
        };

//"Status element"
        struct StatusElement
        {
            seStatus status;
            User user;
        };
//"Basic user information element"
        struct BasicUserInfoElement
        {
            biStatus status;
            User user;
        };
//"Extended user information element"
        struct ExtUserInfoElement
        {
            User user;
            Details details;
            biStatus status;
        };

        //Shared Pointer
        typedef QSharedPointer<Returnables::StatusElement> StatusElementPtr;
        typedef QSharedPointer<Returnables::DirectMessageElement> DirectMessageElementPtr;
        typedef QSharedPointer<Returnables::BasicUserInfoElement> BasicUserInfoElementPtr;
        typedef QSharedPointer<Returnables::ExtUserInfoElement> ExtUserInfoElementPtr;

        enum RequestId { PUBLIC_TIMELINE, FRIENDS_TIMELINE, USER_TIMELINE, SINGLE_STATUS, NEW_STATUS,\
                         RECENT_MENTIONS, REMOVE_STATUS, FRIENDS, FOLLOWERS, USER_DETAILS,\
                         RECEIVED_DIRECT_MESSAGES, SENT_DIRECT_MESSAGES, SEND_DIRECT_MESSAGE, REMOVE_DIRECT_MESSAGE,\
                         ADD_FRIENDSHIP, REMOVE_FRIENDSHIP, FRIENDSHIP_EXISTS, FRIENDS_IDS, FOLLOWERS_IDS, VERIFY_CREDENTIALS, LOGOUT,\
                         DELIVERY_DEVICE, PROFILE_COLORS, PROFILE_IMAGE, PROFILE_BACKGROUND_IMAGE,\
                         API_REQUESTS, PROFILE, FAVORITES, ADD_FAVORITE, REMOVE_FAVORITE,\
                         ENABLE_NOTIFICATIONS, DISABLE_NOTIFICATIONS, BLOCK_USER, UNBLOCK_USER,\
                         TWITTER_UP, FEATURED_USERS \
        };

        class Response
        {
                public:
                        Response() {}
                        virtual ~Response() {}
                        RequestId reqID;
        };
//Status Methods
//==================================================================================================
        class PublicTimeline : public Response
        {
                public:
                        PublicTimeline() {}
                        ~PublicTimeline() {}
                        QLinkedList<StatusElementPtr > list;
        };
        class FriendsTimeline : public Response
        {
                public:
                        FriendsTimeline() {}
                        ~FriendsTimeline() {}
                        QLinkedList<StatusElementPtr > list;
        };
        class UserTimeline : public Response
        {
                public:
                        UserTimeline() {}
                        ~UserTimeline() {}
                        QLinkedList<StatusElementPtr > list;
        };
//show
        class SingleStatus : public Response
        {
                public:
                        SingleStatus() { status = StatusElementPtr(new StatusElement); }
                        ~SingleStatus() { /*if(status) delete status;*/ }
                        StatusElementPtr status;
        };
//update
        class NewStatus : public Response
        {
                public:
                        NewStatus() { status = StatusElementPtr(new StatusElement); }
                        ~NewStatus() { /*if(status) delete status;*/ }
                        StatusElementPtr status;
        };
//mentions
        class RecentMentions : public Response
        {
                public:
                        RecentMentions() {}
                        ~RecentMentions() {}
                        QLinkedList<StatusElementPtr > list;
        };
//destroy
        class RemoveStatus : public Response
        {
                public:
                        RemoveStatus() { status = StatusElementPtr(new StatusElement); }
                        ~RemoveStatus() { /*if(status) delete status;*/ }
                        StatusElementPtr status;
        };
//==================================================================================================

//User Methods
//==================================================================================================
        class Friends : public Response
        {
                public:
                        Friends() {}
                        ~Friends() {}
                        QLinkedList<BasicUserInfoElementPtr > list;
        };
        class Followers : public Response
        {
                public:
                        Followers() {}
                        ~Followers() {}
                        QLinkedList<BasicUserInfoElementPtr > list;
        };
//show
        class UserDetails : public Response
        {
                public:
                        UserDetails() { userExt = ExtUserInfoElementPtr(new ExtUserInfoElement); }
                        ~UserDetails() { /*if(userExt) delete userExt;*/ }
                        ExtUserInfoElementPtr userExt;
        };
//==================================================================================================

//Direct Message Methods
//==================================================================================================
//direct_messages
        class ReceivedDirectMessages : public Response
        {
                public:
                        ReceivedDirectMessages() {}
                        ~ReceivedDirectMessages() {}
                        QLinkedList<DirectMessageElementPtr > list;
        };
//sent
        class SentDirectMessages : public Response
        {
                public:
                        SentDirectMessages() {}
                        ~SentDirectMessages() {}
                        QLinkedList<DirectMessageElementPtr > list;
        };
//new
        class SendDirectMessage : public Response
        {
                public:
                        SendDirectMessage() { message = DirectMessageElementPtr(new DirectMessageElement); }
                        ~SendDirectMessage() { /*if(message) delete message;*/ }
                        DirectMessageElementPtr message;
        };
//destroy
        class RemoveDirectMessage : public Response
        {
                public:
                        RemoveDirectMessage() { message = DirectMessageElementPtr(new DirectMessageElement); }
                        ~RemoveDirectMessage() { /*if(message) delete message;*/ }
                        DirectMessageElementPtr message;
        };
//==================================================================================================

//Friendship Methods
//==================================================================================================
//create
        class AddFriendship : public Response
        {
                public:
                        AddFriendship() { user = BasicUserInfoElementPtr(new BasicUserInfoElement); }
                        ~AddFriendship() { /*if(user) delete user;*/ }
                        BasicUserInfoElementPtr user;
        };
//destroy
        class RemoveFriendship : public Response
        {
                public:
                        RemoveFriendship() { user = BasicUserInfoElementPtr(new BasicUserInfoElement); }
                        ~RemoveFriendship() { /*if(user) delete user;*/ }
                        BasicUserInfoElementPtr user;
        };
//exists
        class FriendshipExist : public Response
        {
                public:
                        FriendshipExist() {}
                        ~FriendshipExist() {}
                        bool friends;
        };
//==================================================================================================

//Social Graph Methods
//==================================================================================================
//friends/ids
        class FriendsIDs : public Response
        {
            public:
                FriendsIDs() {}
                ~FriendsIDs() {}
                QLinkedList<unsigned int> list;
        };
//followers/ids
        class FollowersIDs : public Response
        {
            public:
                FollowersIDs() {}
                ~FollowersIDs() {}
                QLinkedList<unsigned int> list;
        };
//==================================================================================================

//Account Methods
//==================================================================================================
//verify_credentials
        class VerifyCredentials : public Response
        {
                public:
                        VerifyCredentials() { userExt = ExtUserInfoElementPtr(new ExtUserInfoElement); }
                        ~VerifyCredentials() { /*if(userExt) delete userExt;*/ }
                        ExtUserInfoElementPtr userExt;
        };
//update_delivery_device
        class DeliveryDevice : public Response
        {
                public:
                        DeliveryDevice() { user = BasicUserInfoElementPtr(new BasicUserInfoElement); }
                        ~DeliveryDevice() { /*if(user) delete user;*/ }
                        BasicUserInfoElementPtr user;
        };
//update_profile_colors
        class ProfileColors : public Response
        {
                public:
                        ProfileColors() { userExt = ExtUserInfoElementPtr(new ExtUserInfoElement); }
                        ~ProfileColors() { /*if(userExt) delete userExt;*/ }
                        ExtUserInfoElementPtr userExt;
        };
//update_profile_image
        class ProfileImage : public Response
        {
                public:
                        ProfileImage() { userExt = ExtUserInfoElementPtr(new ExtUserInfoElement); }
                        ~ProfileImage() { /*if(userExt) delete userExt;*/ }
                        ExtUserInfoElementPtr userExt;
        };
//update_profile_background_image
        class ProfileBackgroundImage : public Response
        {
                public:
                        ProfileBackgroundImage() { userExt = ExtUserInfoElementPtr(new ExtUserInfoElement); }
                        ~ProfileBackgroundImage() { /*if(userExt) delete userExt;*/ }
                        ExtUserInfoElementPtr userExt;
        };
//rate_limit_status
        class ApiRequests : public Response
        {
                public:
                        ApiRequests() {}
                        ~ApiRequests() {}
                        QString resetTime;
                        unsigned int resetTimeSeconds;
                        unsigned int remainingHits;
                        unsigned int hourlyLimit;
        };
        //update_profile
        class Profile : public Response
        {
                public:
                        Profile() { userExt = ExtUserInfoElementPtr(new ExtUserInfoElement); }
                        ~Profile() { /*if(userExt) delete userExt;*/ }
                        ExtUserInfoElementPtr userExt;
        };
//==================================================================================================

//Favorite Methods
//==================================================================================================
        class Favorites : public Response
        {
                public:
                        Favorites() {}
                        ~Favorites() {}
                        QLinkedList<StatusElementPtr > list;
        };
//create
        class AddFavorite : public Response
        {
                public:
                        AddFavorite() { status = StatusElementPtr(new StatusElement); }
                        ~AddFavorite() { /*if(status) delete status;*/ }
                        StatusElementPtr status;
        };
//destroy
        class RemoveFavorite : public Response
        {
                public:
                        RemoveFavorite() { status = StatusElementPtr(new StatusElement); }
                        ~RemoveFavorite() { /*if(status) delete status;*/ }
                        StatusElementPtr status;
        };
//==================================================================================================

//Notification Methods
//==================================================================================================
//follow
        class EnableNotifications : public Response
        {
                public:
                        EnableNotifications() { user = BasicUserInfoElementPtr(new BasicUserInfoElement); }
                        ~EnableNotifications() { /*if(user) delete user;*/ }
                        BasicUserInfoElementPtr user;
        };
//leave
        class DisableNotifications : public Response
        {
                public:
                        DisableNotifications() { user = BasicUserInfoElementPtr(new BasicUserInfoElement); }
                        ~DisableNotifications() { /*if(user) delete user;*/ }
                        BasicUserInfoElementPtr user;
        };
//==================================================================================================

//Block Methods
//==================================================================================================
//create
//==================================================================================================
        class BlockUser : public Response
        {
                public:
                        BlockUser() { user = BasicUserInfoElementPtr(new BasicUserInfoElement); }
                        ~BlockUser() { /*if(user) delete user;*/ }
                        BasicUserInfoElementPtr user;
        };
//destroy
        class UnBlockUser : public Response
        {
                public:
                        UnBlockUser() { user = BasicUserInfoElementPtr(new BasicUserInfoElement); }
                        ~UnBlockUser() { /*if(user) delete user;*/ }
                        BasicUserInfoElementPtr user;
        };
//==================================================================================================

//Help Methods
//==================================================================================================
//test
        class TwitterUp : public Response
        {
                public:
                        TwitterUp() {}
                        ~TwitterUp() {}
                        bool up;
        };
//==================================================================================================

//Not in API
//==================================================================================================
        class FeaturedUsers : public Response
        {
                public:
                        FeaturedUsers() {}
                        ~FeaturedUsers() {}
                        QLinkedList<BasicUserInfoElementPtr > list;
        };
//==================================================================================================
};


#endif //Returnables_H
