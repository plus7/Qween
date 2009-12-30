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

#ifndef Decipher_H
#define Decipher_H

#include <QtCore/QLinkedList>
#include <QtCore/QString>
#include "Returnables.h"

class QDomElement;

class Decipher
{
	private:
		Decipher();
		static Decipher *instance; 

	public:
		virtual ~Decipher();
		static Decipher *Instance();

	public:
		Returnables::FriendsTimeline* FriendsTimeline(const QString &xml);
		Returnables::PublicTimeline* PublicTimeline(const QString &xml);
		Returnables::SingleStatus* SingleStatus(const QString &xml);
		Returnables::FeaturedUsers* FeaturedUsers(const QString &xml);
                Returnables::VerifyCredentials* VerifyCredentials(const QString &xml);
		Returnables::TwitterUp* TwitterUp(const QString &xml);
		Returnables::UserTimeline* UserTimeline(const QString &xml);
		Returnables::Favorites* Favorites(const QString &xml);
		Returnables::NewStatus* NewStatus(const QString &xml);
                Returnables::RecentMentions* RecentMentions(const QString &xml);
		Returnables::RemoveStatus* RemoveStatus(const QString &xml);
		Returnables::Friends* Friends(const QString &xml);
		Returnables::Followers* Followers(const QString &xml);
		Returnables::UserDetails* UserDetails(const QString &xml);
		Returnables::SentDirectMessages* SentDirectMessages(const QString &xml);
		Returnables::ReceivedDirectMessages* ReceivedDirectMessages(const QString &xml);
		Returnables::SendDirectMessage* SendDirectMessage(const QString &xml);
		Returnables::RemoveDirectMessage* RemoveDirectMessage(const QString &xml);
		Returnables::AddFriendship* AddFriendShip(const QString &xml);
		Returnables::RemoveFriendship* RemoveFriendship(const QString &xml);
		Returnables::FriendshipExist* FriendshipExist(const QString &xml);
		Returnables::DeliveryDevice* DeliveryDevice(const QString &xml);
		Returnables::ApiRequests* ApiRequests(const QString &xml);
		Returnables::AddFavorite* AddFavorite(const QString &xml);
		Returnables::RemoveFavorite* RemoveFavorite(const QString &xml);
                Returnables::ProfileColors* ProfileColors(const QString &xml);
                Returnables::ProfileImage* ProfileImage(const QString &xml);
                Returnables::ProfileBackgroundImage* ProfileBackgroundImage(const QString &xml);
                Returnables::Profile* Profile(const QString &xml);
                Returnables::EnableNotifications* EnableNotifications(const QString &xml);
                Returnables::DisableNotifications* DisableNotifications(const QString &xml);
                Returnables::BlockUser* BlockUser(const QString &xml);
                Returnables::UnBlockUser* UnBlockUser(const QString &xml);
                Returnables::FriendsIDs* FriendsIDs(const QString &xml);
                Returnables::FollowersIDs* FollowersIDs(const QString &xml);

	private:
		enum Parent { USER, STATUS };
                QLinkedList<Returnables::StatusElementPtr > GetStatusElementList(const QString &xml);
                QLinkedList<Returnables::BasicUserInfoElementPtr > GetBasicUserInfoElementList(const QString &xml);
                QLinkedList<Returnables::ExtUserInfoElementPtr > GetExtUserInfoElementList(const QString &xml);
                QLinkedList<unsigned int> GetIDsList(const QString &xml);
                QLinkedList<Returnables::DirectMessageElementPtr > GetDirectMessageList(const QString &xml);

                void Populate_seStatus(Returnables::seStatus &status, const QDomElement &node);
                void Populate_biStatus(Returnables::biStatus &status, const QDomElement &node);
		void PopulateUser(Returnables::User &user, const QDomElement &node);
		void PopulateDetails(Returnables::Details &details, const QDomElement &node);
		void PopulateDirectHeader(Returnables::DirectHeader &header, const QDomElement &node);
		void PopulateApiRequests(Returnables::ApiRequests &apiRequests, const QDomElement &node);

	private:
		static QString nStatus;
		static QString nCreatedAt;
		static QString nId;
		static QString nText;
		static QString nSource;
		static QString nTruncated;
		static QString nInReplyToStatusId;
		static QString nInReplyToUserId;
		static QString nFavorited;
		static QString nUser;
		static QString nName;
		static QString nScreenName;
		static QString nDescription;
		static QString nLocation;
		static QString nProfileImageUrl;
		static QString nUrl;
		static QString nProtected;
		static QString nFollowersCount;
		static QString nOk;
		static QString nProfileBackgroundColor;
		static QString nProfileTextColor;
		static QString nProfileLinkColor;
		static QString nProfileSidebarFillColor;
		static QString nProfileSidebarBorderColor;
		static QString nFriendsCount;
		static QString nFavoritesCount;
		static QString nUtcOffset;
		static QString nTimeZone;
		static QString nProfileBackgroundImageUrl;
		static QString nProfileBackgroundTiled;
		static QString nStatusesCount;
		static QString nDirectMessage;
		static QString nSenderId;
		static QString nRecipientId;
		static QString nSenderScreenName;
		static QString nRecipientScreenName;
		static QString nSender;
		static QString nRecipient;
		static QString nFriends;
		static QString nHash;
		static QString nResetTime;
		static QString nResetTimeSeconds;
		static QString nRemainingHits;
		static QString nHourlyLimit;
                static QString nInReplyToScreenName;
                static QString nFollowing;
                static QString nNotifications;
                static QString nID;
};


#endif //Decipher_H
