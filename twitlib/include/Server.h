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

#ifndef SERVER_H
#define SERVER_H

namespace SERVER
{
        enum DEVICES { SMS, IM, NONE };

        enum RESP
                {
                        OK = 200,
                        NOT_MODIFIED = 304,
                        BAD_REQUEST = 400,
                        NOT_AUTHORIZED = 401,
                        FORBIDDEN = 403,
                        NOT_FOUND = 404,
                        INTERNAL_SERVER_ERROR = 500,
                        BAD_GATEWAY = 502,
                        SERVICE_UNAVAILABLE = 503,
            UNKNOWN = 2723
                };

        enum PROXY_TYPE {
                NO_PROXY,
                SOCKS5_PROXY,
                HTTP_PROXY
        };

        struct Option1
        {
            Option1() {since_id=0; max_id=0; count=200; page=1; }
            /// Optional.  Returns only statuses with an ID greater than (that is, more recent than) the specified ID.
            quint64 since_id;
            /// Optional.  Returns only statuses with an ID less than (that is, older than) the specified
            quint64 max_id;
            /// Optional.  Specifies the number of statuses to retrieve. May not be greater than 200.
            unsigned int count;
            /// Optional. Specify which page to return.
            unsigned int page;
        };

        struct Option2
        {
            Option2() { id=""; user_id=0; screen_name=""; since_id=0; max_id=0; page=1;}
            /// Optional.  Specifies the ID or screen name of the user for whom to return the user_timeline.
            QString id;
            /// Optional. Specfies the ID of the user for whom to return the user_timeline. Helpful for disambiguating when a valid user ID is also a valid screen name.
            unsigned int user_id;
            /// Optional.  Specfies the screen name of the user for whom to return the user_timeline. Helpful for disambiguating when a valid screen name is also a user ID.
            QString screen_name;
            /// Optional.  Returns only statuses with an ID greater than (that is, more recent than) the specified ID.
            quint64 since_id;
            /// Optional.  Returns only statuses with an ID less than (that is, older than) the specified ID.
            quint64 max_id;
            ///  Optional. Specify which page to return.
            unsigned int page;
        };

        struct Option3
        {
            Option3() { since_id=0; max_id=0; page=1; }
            /// Optional.  Returns only statuses with an ID greater than (that is, more recent than) the specified ID.
            quint64 since_id;
            /// Optional.  Returns only statuses with an ID less than (that is, older than) the specified
            quint64 max_id;
            /// Optional. Retrieves the 20 next most recent mentions.
            unsigned int page;
        };

        struct Option4
        {
            Option4() { id=""; user_id=0; screen_name=""; page=1; }
            /// Optional.  The ID or screen name of the user for whom to request a list of friends.
            QString id;
            /// Optional.  Specfies the ID of the user for whom to return the list of friends. Helpful for disambiguating when a valid user ID is also a valid screen name.
            unsigned int user_id;
            /// Optional. Specfies the screen name of the user for whom to return the list of friends. Helpful for disambiguating when a valid screen name is also a user ID.
            QString screen_name;
            /// Optional. Retrieves the next 100 friends.
            unsigned int page;
        };

        struct Option5
        {
            Option5() {since_id=0; page=1; }
            /// Optional.  Returns only statuses with an ID greater than (that is, more recent than) the specified ID.
            quint64 since_id;
            /// Optional. Specify which page to return.
            unsigned int page;
        };
};
#endif //SERVER_H
