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

#ifndef QTWITLIB_H
#define QTWITLIB_H

#ifdef WIN32
        #define EXPORT __declspec(dllexport)
#else
        #define EXPORT
#endif

#include <QtCore/QString>
#include "Server.h"
#include "Core.h"

class QTwitLib : public Core
{
    public:
        EXPORT QTwitLib();
        virtual ~QTwitLib();

//=====================================================================
        // Unauthenticated Methods
//=====================================================================
        /**
         * Returns the 20 most recent statuses from non-protected users who have set a custom user icon.
         * Does not require authentication.
         */
        EXPORT void GetPublicTimeline();
//=====================================================================
        /**
         * Returns a single status, specified by the id parameter below.  The status's author will be returned inline.
         * @param id Required.  The numerical ID of the status you're trying to retrieve.
         */
        EXPORT void GetSingleStatus(quint64 id);
//=====================================================================
        /**
         * WARNING!!! Not described in API
         * Returns a list of the users currently featured on the site with their current statuses inline.
         */
        EXPORT void GetFeaturedUsers();
//=====================================================================
        /**
         * Attempts to establish an authorized connection with Twitter.
         * See SetLoginInfo(QString user, QString password) and VerifyCredentials();
         */
        EXPORT void Login(QString user, QString password);
//=====================================================================
        /**
         * Ends the session of the authenticating user, returning a null cookie.
         * Use this method to sign users out of client-facing applications like widgets.
         */
        EXPORT void Logout();
//=====================================================================
        /**
         * Aborts the current request and deletes all scheduled requests.
         */
        EXPORT void Abort();
//=====================================================================
        /**
         * Sets the user name and password for establish an authorized connection with Twitter.
         */
        EXPORT void SetLoginInfo(QString user, QString password);
//=====================================================================
        /**
         * NOTE: Use only after "SetLoginInfo(QString user, QString password)" method!
         *
         * Returns an HTTP 200 OK response code and a representation of the requesting user if
         * authentication was successful; returns a 401 status code and an error message if not.
         * Use this method to test if supplied user credentials are valid.
         */
        EXPORT void VerifyCredentials();
//=====================================================================
        /**
         * Use proxy for connection with Twitter.
         */
        EXPORT void SetProxy(SERVER::PROXY_TYPE type, const QString hostName, \
                    quint16 port, const QString user, const QString password);
//=====================================================================
        /**
         * Returns the string "ok" in the requested format with a 200 OK HTTP status code.
         */
        EXPORT void IsTwitterUp();
//=====================================================================
//=====================================================================


//=====================================================================
        // Authenticated/Unauthenticated Methods
//=====================================================================
        /**
         * Returns the 20 most recent statuses posted from the authenticating user.
         * It's also possible to request another user's timeline via the id parameter below.
         * This is the equivalent of the Web /archive page for your own user, or the profile page for a third party.
         * @param opt Optional.  Options which can be user specified.
         */
        EXPORT void GetUsersTimeline(SERVER::Option2 *opt=NULL);
//=====================================================================
        /**
         * Returns the 20 most recent favorite statuses for the authenticating user or user specified by the ID parameter in the requested format.
         * @param user Optional.  The ID or screen name of the user for whom to request a list of favorite statuses.
         */
        EXPORT void GetFavorites(QString user="", unsigned int page=1);
//=====================================================================
        /**
         * Returns an array of numeric IDs for every user the specified user is following.
         * @param id Optional. The ID or screen_name of the user to retrieve
         * the friends ID list for.
         * @param user_id  Optional. Specfies the ID of the user for whom to return the friends
         * list. Helpful for disambiguating when a valid user ID is also a valid screen name.
         * @param screen_name Optional. Specfies the screen name of the user for whom to return the
         * friends list. Helpful for disambiguating when a valid screen name is also a user ID.
         */
        EXPORT void GetFriendsIDs(QString id = "", QString user_id = "", QString screen_name = "");
//=====================================================================
        /**
         * Returns an array of numeric IDs for every user the specified user is followed by.
         * @param id Optional. The ID or screen_name of the user to retrieve
         * the friends ID list for.
         * @param user_id Optional. Specfies the ID of the user for whom to return the followers list.
         * Helpful for disambiguating when a valid user ID is also a valid screen name.
         * @param screen_name Optional. Specfies the screen name of the user for whom to return
         * the followers list. Helpful for disambiguating when a valid screen name is also a user ID.
         */
        EXPORT void GetFollowersIDs(QString id = "", QString user_id = "", QString screen_name = "");
//=====================================================================
//=====================================================================


//=====================================================================
        // Authenticated Methods
//=====================================================================
        /**
         * Returns the 20 most recent statuses posted by the authenticating user and that user's friends.
         * This is the equivalent of /home on the Web.
         * @param opt Optional.  Options which can be user specified.
         */
        EXPORT void GetFriendsTimeline(SERVER::Option1 *opt=NULL);
//=====================================================================
        /**
         * Updates the authenticating user's status.  Requires the status parameter specified below.  Request must be a POST.
         * @param status Required.  The text of your status update.  Be sure to URL encode as necessary.
         * Must not be more than 160 characters and should not be more than 140 characters to ensure optimal display.
         * @param in_reply_to_status_id Optional. The ID of an existing status that the status to be
         * posted is in reply to.  This implicitly sets the in_reply_to_user_id attribute of the resulting
         * status to the user ID of the message being replied to.  Invalid/missing status IDs will be ignored.
         * WARNING!!! Not described in API.
         * @param source Optional. Twitter client name, must be registered at http://twitter.com/help/request_source
         */
        EXPORT void PostNewStatus(QString status, quint64 in_reply_to_status_id = NULL, QString source = "");
//=====================================================================
        /**
         * Returns the 20 most recent mentions (status containing @username) for the authenticating user.
         * @param opt Optional.  Options which can be user specified.
         */
        EXPORT void GetRecentMentions(SERVER::Option3 *opt=NULL);
//=====================================================================
        /**
         * Destroys the status specified by the required ID parameter.  The authenticating user must be the author of the specified status.
         * @param id Required.  The ID of the status to destroy.
         */
        EXPORT void RemoveStatus(quint64 id);
//=====================================================================
        /**
         * Returns up to 100 of the authenticating user's friends who have most recently updated, each with current status inline.
         * It's also possible to request another user's recent friends list via the id parameter below.
         * @param opt Optional.  Options which can be user specified.
         */
        EXPORT void GetFriends(SERVER::Option4 *opt=NULL);
//=====================================================================
        /**
         * Returns the authenticating user's followers, each with current status inline.
         * They are ordered by the order in which they joined Twitter (this is going to be changed).
         * @param opt Optional.  Options which can be user specified.
         */
        EXPORT void GetFollowers(SERVER::Option4 *opt=NULL);
//=====================================================================
        /**
         * Returns extended information of a given user, specified by ID or screen name as per the
         * required id parameter below.  This information includes design settings, so third party
         * developers can theme their widgets according to a given user's preferences.
         * You must be properly authenticated to request the page of a protected user.
         * One of the following is required:
         * @param id The ID or screen name of a user.
         * @param user_id Optional. Specfies the ID of the user to return.
         * Helpful for disambiguating when a valid user ID is also a valid screen name.
         * @param screen_name Optional. Specfies the screen name of the user to return.
         *  Helpful for disambiguating when a valid screen name is also a user ID.
         */
        EXPORT void GetUserDetails(QString id = "", unsigned int user_id=NULL, QString screen_name = "");
//=====================================================================
        /**
         * Returns a list of the 20 most recent direct messages sent by the authenticating user.
         * The XML and JSON versions include detailed information about the sending and recipient users.
         * @param opt Optional.  Options which can be user specified.
         */
        EXPORT void GetSentDirectMessages(SERVER::Option5 *opt=NULL);
//=====================================================================
        /**
         * Returns a list of the 20 most recent direct messages sent to the authenticating user.
         * The XML and JSON versions include detailed information about the sending and recipient users.
         * @param opt Optional.  Options which can be user specified.
         */
        EXPORT void GetReceivedDirectMessages(SERVER::Option5 *opt=NULL);;
//=====================================================================
        /**
         * Sends a new direct message to the specified user from the authenticating user.
         * Requires both the user and text parameters below.  Request must be a POST.
         * Returns the sent message in the requested format when successful.
         * @param user Required.  The ID or screen name of the recipient user.
         * @param text Required.  The text of your direct message.
         * Be sure to URL encode as necessary, and keep it under 140 characters.
         */
        EXPORT void SendDirectMessage(QString user, QString text);
//=====================================================================
        /**
         * Destroys the direct message specified in the required ID parameter.
         * The authenticating user must be the recipient of the specified direct message.
         * @param id Required.  The ID of the direct message to destroy.
         */
        EXPORT void RemoveDirectMessage(quint64 id);
//=====================================================================
        /**
         * Befriends the user specified in the ID parameter as the authenticating user.
         * Returns the befriended user in the requested format when successful.
         * Returns a string describing the failure condition when unsuccessful.
         * @param user Required.  The ID or screen name of the user to befriend.
         */
        EXPORT void AddFriendship(QString user, bool follow=true);
//=====================================================================
        /**
         * Discontinues friendship with the user specified in the ID parameter as the authenticating user.
         * Returns the un-friended user in the requested format when successful.  Returns a string describing the failure condition when unsuccessful.
         * @param user Required.  The ID or screen name of the user with whom to discontinue friendship.
         */
        EXPORT void RemoveFriendship(QString user);
//=====================================================================
        /**
         * Tests if a friendship exists between two users.
         * @param user_a Required.  The ID or screen_name of the first user to test friendship for.
         * @param user_b Required.  The ID or screen_name of the second user to test friendship for.
         */
        EXPORT void FriendshipExist(QString user_a, QString user_b);
//=====================================================================
        /**
         * Sets which device Twitter delivers updates to for the authenticating user.
         * Sending none as the device parameter will disable IM or SMS updates.
         * @param device Required.  Must be one of: sms, im, none.
         */
        EXPORT void UpdateDeliveryDevice(SERVER::DEVICES device);
//=====================================================================
        /**
         * Returns the remaining number of API requests available to the authenticating user before the API limit is reached for the current hour.
         * Calls to rate_limit_status require authentication, but will not count against the rate limit.
         */
        EXPORT void RemainingApiRequests();
//=====================================================================
        /**
         * Favorites the status specified in the ID parameter as the authenticating user.  Returns the favorite status when successful.
         * @param id  Required.  The ID of the status to favorite.
         */
        EXPORT void AddFavorite(quint64 id);
//=====================================================================
        /**
         * Un-favorites the status specified in the ID parameter as the authenticating user.
         * Returns the un-favorited status in the requested format when successful.
         * @param id Required.  The ID of the status to un-favorite.
         */
        EXPORT void RemoveFavorite(quint64 id);
//=====================================================================
        /**
         * Sets one or more hex values that control the color scheme of the authenticating
         * user's profile page on twitter.com.
         * One or more of the following parameters must be present.
         * Each parameter's value must be a valid hexidecimal value,
         * and may be either three or six characters (ex: #fff or #ffffff).
         *
         * @param profile_background_color  Optional.
         * @param profile_text_color  Optional.
         * @param profile_link_color  Optional.
         * @param profile_sidebar_fill_color  Optional.
         * @param profile_sidebar_border_color  Optional.
         */
        EXPORT void UpdateProfileColors(QString background_color = "", QString text_color = "", QString link_color = "", QString sidebar_fill_color = "", QString sidebar_border_color = "");
//=====================================================================
        /**
         * Updates the authenticating user's profile image.
         * @param image  Required.  Must be a valid GIF, JPG, or PNG image of less than 700 kilobytes
         * in size.  Images with width larger than 500 pixels will be scaled down.
         */
        EXPORT void UpdateProfileImage(QString image);
//=====================================================================
        /**
         * Updates the authenticating user's profile background image.
         * @param image.  Required.  Must be a valid GIF, JPG, or PNG image of less than 800 kilobytes in size.
         * Images with width larger than 2048 pixels will be scaled down.
         * @param tile Optional. If set to true the background image will be displayed tiled.
         * The image will not be tiled otherwise.
         */
        EXPORT void UpdateProfileBackgroundImage(QString image, bool tile = false);
//=====================================================================
        /**
         * Sets values that users are able to set under the "Account" tab of their settings page.
         * Only the parameters specified will be updated; to only update the "name" attribute,
         * for example, only include that parameter in your request.
         * One or more of the following parameters must be present.  Each parameter's value should
         * be a string.  See the individual parameter descriptions below for further constraints.
         * @param name  Optional. Maximum of 20 characters.
         * @param email  Optional. Maximum of 40 characters. Must be a valid email address.
         * @param url  Optional. Maximum of 100 characters. Will be prepended with "http://" if not present.
         * @param location  Optional. Maximum of 30 characters. The contents are not normalized or geocoded in any way.
         * @param description  Optional. Maximum of 160 characters.
         */
        EXPORT void UpdateProfile(QString name = "", QString email = "", QString url = "", \
                                QString location = "", QString description = "");
//=====================================================================
        /**
         * NOTE: The Notification Methods require the authenticated user to already be friends with
         * the specified user otherwise the error "there was a problem following the specified user"
         * will be returned.
         *
         * Enables notifications for updates from the specified user to the authenticating user.
         * Returns the specified user when successful.
         * @param id  Required.  The ID or screen name of the user to follow.
         */
        EXPORT void EnableNotifications(QString id);
//=====================================================================
        /**
         * Disables notifications for updates from the specified user to the authenticating user.
         * Returns the specified user when successful.
         * @param id Required.  The ID or screen name of the user to leave.
         */
        EXPORT void DisableNotifications(QString id);
//=====================================================================
        /**
         * Blocks the user specified in the ID parameter as the authenticating user.
         * Returns the blocked user in the requested format when successful.
         * @param id Required.  The ID or screen_name of the user to block.
         */
        EXPORT void BlockUser(QString id);
//=====================================================================
        /**
         * Un-blocks the user specified in the ID parameter as the authenticating user.
         * Returns the un-blocked user in the requested format when successful.
         * @param id Required.  The ID or screen_name of the user to un-block.
         */
        EXPORT void UnBlockUser(QString id);
//=====================================================================
};


#endif // TWITLIB_H
