/*
  This file is part of Qween.
  Copyright (C) 2009-2010 NOSE Takafumi <ahya365@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, NOSE Takafumi
  gives permission to link the code of its release of Qween with the
  OpenSSL project's "OpenSSL" library (or with modified versions of it
  that use the same license as the "OpenSSL" library), and distribute
  the linked executables.  You must obey the GNU General Public License
  in all respects for all of the code used other than "OpenSSL".  If you
  modify this file, you may extend this exception to your version of the
  file, but you are not obligated to do so.  If you do not wish to do
  so, delete this exception statement from your version.
*/

#include "petrel.h"
#include "xauth.h"
#include "util.h"
#include <QMessageBox>
Petrel::Petrel()
        :m_manager(new QNetworkAccessManager( this )),
        m_useXAuth(false),m_xauth(new XAuth(XAUTH_CONSUMER_KEY, XAUTH_CONSUMER_SECRET,this)),
        m_xauth_limit(0),m_xauth_remaining(0)
{
    connect(m_manager,SIGNAL(finished(QNetworkReply*)),
            this,SLOT(replyFinished(QNetworkReply*)));
    connect(m_xauth, SIGNAL(accessTokenReceived()),
            this, SLOT(accessKeyReceived()));
}

Petrel::~Petrel()
{
}

//HTTP
void Petrel::issueGetRequest(int role, QString url, QList<QPair<QString,QString> > queryItems){
    QUrl u(url);
    u.setEncodedQuery(QByteArray(util::encodeQuery(queryItems).toAscii()));
    QNetworkRequest req(u);
    req.setAttribute((QNetworkRequest::Attribute)(QNetworkRequest::User + ATTR_ROLE), role);
    if(m_useXAuth){
        QString sig = m_xauth->getSignature("GET", url, queryItems);
        req.setRawHeader( "Authorization", sig.prepend( "OAuth " ).toAscii() );
    }else{
        QByteArray auth = m_userid.toUtf8() + ":" + m_pass.toUtf8();
        req.setRawHeader( "Authorization", auth.toBase64().prepend( "Basic " ) );
    }
    QNetworkReply *r = m_manager->get(req);
    m_replies.append(r);
}

void Petrel::issuePostRequest(int role, QString url, QList<QPair<QString,QString> > queryItems){
    //QMessageBox::information(NULL, "", QString(req.url().toEncoded()));
    QUrl u(url);
    QNetworkRequest req(u);
    req.setAttribute((QNetworkRequest::Attribute)(QNetworkRequest::User + ATTR_ROLE), role);
    if(m_useXAuth){
        QString sig = m_xauth->getSignature("POST", url, queryItems);
        req.setRawHeader( "Authorization", sig.prepend( "OAuth " ).toAscii() );
    }else{
        QByteArray auth = m_userid.toUtf8() + ":" + m_pass.toUtf8();
        req.setRawHeader( "Authorization", auth.toBase64().prepend( "Basic " ) );
    }
    QNetworkReply *r = m_manager->post(req,QByteArray(util::encodeQuery(queryItems).toAscii()));
    m_replies.append(r);
}

void Petrel::issuePutRequest(int role, QString url, QList<QPair<QString,QString> > queryItems){
    //QMessageBox::information(NULL, "", QString(req.url().toEncoded()));
    QUrl u(url);
    QNetworkRequest req(u);
    req.setAttribute((QNetworkRequest::Attribute)(QNetworkRequest::User + ATTR_ROLE), role);
    if(m_useXAuth){
        QString sig = m_xauth->getSignature("PUT", url, queryItems);
        req.setRawHeader( "Authorization", sig.prepend( "OAuth " ).toAscii() );
    }else{
        QByteArray auth = m_userid.toUtf8() + ":" + m_pass.toUtf8();
        req.setRawHeader( "Authorization", auth.toBase64().prepend( "Basic " ) );
    }
    QNetworkReply *r = m_manager->put(req,QByteArray(util::encodeQuery(queryItems).toAscii()));
    m_replies.append(r);
}

void Petrel::issueDeleteRequest(int role, QString url, QList<QPair<QString,QString> > queryItems){
    QUrl u(url);
    u.setEncodedQuery(QByteArray(util::encodeQuery(queryItems).toAscii()));
    QNetworkRequest req(u);
    req.setAttribute((QNetworkRequest::Attribute)(QNetworkRequest::User + ATTR_ROLE), role);
    if(m_useXAuth){
        QString sig = m_xauth->getSignature("DELETE", url, queryItems);
        req.setRawHeader( "Authorization", sig.prepend( "OAuth " ).toAscii() );
    }else{
        QByteArray auth = m_userid.toUtf8() + ":" + m_pass.toUtf8();
        req.setRawHeader( "Authorization", auth.toBase64().prepend( "Basic " ) );
    }
    QNetworkReply *r = m_manager->deleteResource(req);
    m_replies.append(r);
}

/*void Petrel::issueImagePostRequest(QNetworkRequest& req){

}*/

void Petrel::setLoginInfo(const QString& userid, const QString& pass, bool xauth){
    m_userid = userid;
    m_pass = pass;
    m_useXAuth = xauth;
    if(m_useXAuth){
        m_xauth_limit=0;
        m_xauth_remaining=0;
        m_xauth->setToken("");
        m_xauth->setTokenSecret("");
    }

    if(m_useXAuth)
        m_xauth->getAccessToken(m_userid,m_pass);
    else
        verifyCredentials();
}

void Petrel::setToken(const QString& token, const QString& tokenSecret){
    m_useXAuth = true;
    m_xauth->setToken(token);
    m_xauth->setTokenSecret(tokenSecret);
    verifyCredentials();
}

QString Petrel::token(){ return m_xauth->token(); }

QString Petrel::tokenSecret(){ return m_xauth->tokenSecret(); }

void Petrel::accessKeyReceived(){
    verifyCredentials();
}

//BEGIN autogenerated methods
void Petrel::availableTrends(const QString& lat, const QString& long_){

    QString requestStr("http://api.twitter.com/1/trends/available.xml");
    PropertyBag propBag;
    //addQueryItem
    if(lat!="") propBag.addQueryItem("lat",lat);
    if(long_!="") propBag.addQueryItem("long",long_);


    issueGetRequest(AVAILABLE_TRENDS, requestStr, propBag.items);

}
void Petrel::blocking_IdsBlocks(){

    QString requestStr("http://api.twitter.com/1/blocks/blocking/ids.xml");
    PropertyBag propBag;
    //addQueryItem



    issueGetRequest(BLOCKING_IDS_BLOCKS, requestStr, propBag.items);

}
void Petrel::createBlock(quint64 id, quint64 user_id, const QString& screen_name){

    QString requestStr("http://api.twitter.com/1/blocks/create/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem
    if(user_id!=0) propBag.addQueryItem("user_id",QString::number(user_id,10));
    if(screen_name!="") propBag.addQueryItem("screen_name",screen_name);



    issuePostRequest(CREATE_BLOCK, requestStr, propBag.items);

}
void Petrel::createFavorite(quint64 id){

    QString requestStr("http://api.twitter.com/1/favorites/create/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem




    issuePostRequest(CREATE_FAVORITE, requestStr, propBag.items);

}
void Petrel::createFriendship(quint64 id, quint64 user_id, const QString& screen_name, const QString& follow){
    QString requestStr;
    if(id==0)
        requestStr = "http://api.twitter.com/1/friendships/create.xml";
    else
        requestStr = "http://api.twitter.com/1/friendships/create/"+QString::number(id,10)+".xml";

    PropertyBag propBag;
    //addQueryItem
    if(user_id!=0) propBag.addQueryItem("user_id",QString::number(user_id,10));
    if(screen_name!="") propBag.addQueryItem("screen_name",screen_name);
    if(follow!="") propBag.addQueryItem("follow",follow);



    issuePostRequest(CREATE_FRIENDSHIP, requestStr, propBag.items);

}
void Petrel::createSavedSearch(const QString& query){

    QString requestStr("http://api.twitter.com/1/saved_searches/create.xml");
    PropertyBag propBag;
    //addQueryItem
    if(query!="") propBag.addQueryItem("query",query);



    issuePostRequest(CREATE_SAVED_SEARCH, requestStr, propBag.items);

}
void Petrel::deleteListId(quint64 id, quint64 list_id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/lists/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem
    if(list_id!=0) propBag.addQueryItem("list_id",QString::number(list_id,10));


    issueDeleteRequest(DELETE_LIST_ID, requestStr, propBag.items);

}
void Petrel::deleteListMember(quint64 list_id, quint64 id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/"+QString::number(list_id,10)+"/members.xml");
    PropertyBag propBag;
    //addQueryItem
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));


    issueDeleteRequest(DELETE_LIST_MEMBER, requestStr, propBag.items);

}
void Petrel::deleteListSubscriber(const QString& user, quint64 list_id, quint64 id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/"+QString::number(list_id,10)+"/subscribers.xml");
    PropertyBag propBag;
    //addQueryItem
    if(user!="") propBag.addQueryItem("user",user);
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));


    issueDeleteRequest(DELETE_LIST_SUBSCRIBER, requestStr, propBag.items);

}
void Petrel::destroy(quint64 id){

    QString requestStr("http://api.twitter.com/1/statuses/destroy/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem



    issueDeleteRequest(DESTROY, requestStr, propBag.items);

}
void Petrel::destroyBlock(quint64 id, quint64 user_id, const QString& screen_name){

    QString requestStr("http://api.twitter.com/1/blocks/destroy/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem
    if(user_id!=0) propBag.addQueryItem("user_id",QString::number(user_id,10));
    if(screen_name!="") propBag.addQueryItem("screen_name",screen_name);


    issueDeleteRequest(DESTROY_BLOCK, requestStr, propBag.items);

}
void Petrel::destroyDirectMessage(quint64 id){

    QString requestStr("http://api.twitter.com/1/direct_messages/destroy/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem



    issueDeleteRequest(DESTROY_DIRECT_MESSAGE, requestStr, propBag.items);

}
void Petrel::destroyFavorite(quint64 id){

    QString requestStr("http://api.twitter.com/1/favorites/destroy/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem



    issueDeleteRequest(DESTROY_FAVORITE, requestStr, propBag.items);

}
void Petrel::destroyFriendship(quint64 id, quint64 user_id, const QString& screen_name){

    QString requestStr;
    if(id==0)
        requestStr = "http://api.twitter.com/1/friendships/destroy.xml";
    else
        requestStr = "http://api.twitter.com/1/friendships/destroy/"+QString::number(id,10)+".xml";

    PropertyBag propBag;
    //addQueryItem
    if(user_id!=0) propBag.addQueryItem("user_id",QString::number(user_id,10));
    if(screen_name!="") propBag.addQueryItem("screen_name",screen_name);


    issueDeleteRequest(DESTROY_FRIENDSHIP, requestStr, propBag.items);

}
void Petrel::destroySavedSearch(quint64 id){

    QString requestStr("http://api.twitter.com/1/saved_searches/destroy/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem



    issueDeleteRequest(DESTROY_SAVED_SEARCH, requestStr, propBag.items);

}
void Petrel::directMessages(quint64 since_id, quint64 max_id, int count, int page){

    QString requestStr("http://api.twitter.com/1/direct_messages.xml");
    PropertyBag propBag;
    //addQueryItem
    if(since_id!=0) propBag.addQueryItem("since_id",QString::number(since_id,10));
    if(max_id!=0) propBag.addQueryItem("max_id",QString::number(max_id,10));
    if(count!=0) propBag.addQueryItem("count",QString::number(count,10));
    if(page!=0) propBag.addQueryItem("page",QString::number(page,10));


    issueGetRequest(DIRECT_MESSAGES, requestStr, propBag.items);

}
void Petrel::existsFriendships(const QString& user_a, const QString& user_b){

    QString requestStr("http://api.twitter.com/1/friendships/exists.xml");
    PropertyBag propBag;
    //addQueryItem
    if(user_a!="") propBag.addQueryItem("user_a",user_a);
    if(user_b!="") propBag.addQueryItem("user_b",user_b);


    issueGetRequest(EXISTS_FRIENDSHIPS, requestStr, propBag.items);

}
void Petrel::favorites(quint64 id, int page){

    QString requestStr("http://api.twitter.com/1/favorites.xml");
    PropertyBag propBag;
    //addQueryItem
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));
    if(page!=0) propBag.addQueryItem("page",QString::number(page,10));


    issueGetRequest(FAVORITES, requestStr, propBag.items);

}
void Petrel::followNotification(quint64 id, quint64 user_id, const QString& screen_name){

    QString requestStr("http://api.twitter.com/1/notifications/follow/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem
    if(user_id!=0) propBag.addQueryItem("user_id",QString::number(user_id,10));
    if(screen_name!="") propBag.addQueryItem("screen_name",screen_name);



    issuePostRequest(FOLLOW_NOTIFICATION, requestStr, propBag.items);

}
void Petrel::followers(quint64 id, quint64 user_id, const QString& screen_name, qint64 cursor){

    QString requestStr("http://api.twitter.com/1/statuses/followers.xml");
    PropertyBag propBag;
    //addQueryItem
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));
    if(user_id!=0) propBag.addQueryItem("user_id",QString::number(user_id,10));
    if(screen_name!="") propBag.addQueryItem("screen_name",screen_name);
    if(cursor!=0) propBag.addQueryItem("cursor",QString::number(cursor,10));


    issueGetRequest(FOLLOWERS, requestStr, propBag.items);

}
void Petrel::friends(quint64 id, quint64 user_id, const QString& screen_name, qint64 cursor){

    QString requestStr("http://api.twitter.com/1/statuses/friends.xml");
    PropertyBag propBag;
    //addQueryItem
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));
    if(user_id!=0) propBag.addQueryItem("user_id",QString::number(user_id,10));
    if(screen_name!="") propBag.addQueryItem("screen_name",screen_name);
    if(cursor!=0) propBag.addQueryItem("cursor",QString::number(cursor,10));


    issueGetRequest(FRIENDS, requestStr, propBag.items);

}
void Petrel::friendsTimeline(quint64 since_id, quint64 max_id, int count, int page){

    QString requestStr("http://api.twitter.com/1/statuses/friends_timeline.xml");
    PropertyBag propBag;
    //addQueryItem
    if(since_id!=0) propBag.addQueryItem("since_id",QString::number(since_id,10));
    if(max_id!=0) propBag.addQueryItem("max_id",QString::number(max_id,10));
    if(count!=0) propBag.addQueryItem("count",QString::number(count,10));
    if(page!=0) propBag.addQueryItem("page",QString::number(page,10));


    issueGetRequest(FRIENDS_TIMELINE, requestStr, propBag.items);

}
void Petrel::getListId(quint64 id, quint64 list_id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/lists/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem
    if(list_id!=0) propBag.addQueryItem("list_id",QString::number(list_id,10));


    issueGetRequest(GET_LIST_ID, requestStr, propBag.items);

}
void Petrel::getListMembers(quint64 list_id, qint64 cursor, quint64 id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/"+QString::number(list_id,10)+"/members.xml");
    PropertyBag propBag;
    //addQueryItem
    if(cursor!=0) propBag.addQueryItem("cursor",QString::number(cursor,10));
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));


    issueGetRequest(GET_LIST_MEMBERS, requestStr, propBag.items);

}
void Petrel::getListMembersId(quint64 list_id, quint64 id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/"+QString::number(list_id,10)+"/members/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem



    issueGetRequest(GET_LIST_MEMBERS_ID, requestStr, propBag.items);

}
void Petrel::getListMemberships(qint64 cursor, quint64 list_id, quint64 id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/lists/memberships.xml");
    PropertyBag propBag;
    //addQueryItem
    if(cursor!=0) propBag.addQueryItem("cursor",QString::number(cursor,10));
    if(list_id!=0) propBag.addQueryItem("list_id",QString::number(list_id,10));
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));


    issueGetRequest(GET_LIST_MEMBERSHIPS, requestStr, propBag.items);

}
void Petrel::getListStatuses(quint64 since_id, quint64 max_id, const QString& per_page, int page, quint64 list_id, quint64 id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/lists/"+QString::number(list_id,10)+"/statuses.xml");
    PropertyBag propBag;
    //addQueryItem
    if(since_id!=0) propBag.addQueryItem("since_id",QString::number(since_id,10));
    if(max_id!=0) propBag.addQueryItem("max_id",QString::number(max_id,10));
    if(per_page!="") propBag.addQueryItem("per_page",per_page);
    if(page!=0) propBag.addQueryItem("page",QString::number(page,10));
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));


    issueGetRequest(GET_LIST_STATUSES, requestStr, propBag.items);

}
void Petrel::getListSubscribers(quint64 list_id, qint64 cursor, quint64 id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/"+QString::number(list_id,10)+"/subscribers.xml");
    PropertyBag propBag;
    //addQueryItem
    if(cursor!=0) propBag.addQueryItem("cursor",QString::number(cursor,10));
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));


    issueGetRequest(GET_LIST_SUBSCRIBERS, requestStr, propBag.items);

}
void Petrel::getListSubscribersId(const QString& user, quint64 list_id, quint64 id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/"+QString::number(list_id,10)+"/subscribers/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem
    if(user!="") propBag.addQueryItem("user",user);


    issueGetRequest(GET_LIST_SUBSCRIBERS_ID, requestStr, propBag.items);

}
void Petrel::getListSubscriptions(qint64 cursor, quint64 list_id, quint64 id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/lists/subscriptions.xml");
    PropertyBag propBag;
    //addQueryItem
    if(cursor!=0) propBag.addQueryItem("cursor",QString::number(cursor,10));
    if(list_id!=0) propBag.addQueryItem("list_id",QString::number(list_id,10));
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));


    issueGetRequest(GET_LIST_SUBSCRIPTIONS, requestStr, propBag.items);

}
void Petrel::getLists(qint64 cursor, quint64 list_id, quint64 id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/lists.xml");
    PropertyBag propBag;
    //addQueryItem
    if(cursor!=0) propBag.addQueryItem("cursor",QString::number(cursor,10));
    if(list_id!=0) propBag.addQueryItem("list_id",QString::number(list_id,10));
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));


    issueGetRequest(GET_LISTS, requestStr, propBag.items);

}
void Petrel::homeTimeline(quint64 since_id, quint64 max_id, int count, int page){

    QString requestStr("http://api.twitter.com/1/statuses/home_timeline.xml");
    PropertyBag propBag;
    //addQueryItem
    if(since_id!=0) propBag.addQueryItem("since_id",QString::number(since_id,10));
    if(max_id!=0) propBag.addQueryItem("max_id",QString::number(max_id,10));
    if(count!=0) propBag.addQueryItem("count",QString::number(count,10));
    if(page!=0) propBag.addQueryItem("page",QString::number(page,10));


    issueGetRequest(HOME_TIMELINE, requestStr, propBag.items);

}
void Petrel::idsFollowers(qint64 cursor){

    QString requestStr("http://api.twitter.com/1/followers/ids.xml");
    PropertyBag propBag;
    //addQueryItem
    if(cursor!=0) propBag.addQueryItem("cursor",QString::number(cursor,10));


    issueGetRequest(IDS_FOLLOWERS, requestStr, propBag.items);

}
void Petrel::idsFriends(qint64 cursor){

    QString requestStr("http://api.twitter.com/1/friends/ids.xml");
    PropertyBag propBag;
    //addQueryItem
    if(cursor!=0) propBag.addQueryItem("cursor",QString::number(cursor,10));


    issueGetRequest(IDS_FRIENDS, requestStr, propBag.items);

}
void Petrel::leaveNotification(quint64 id, quint64 user_id, const QString& screen_name){

    QString requestStr("http://api.twitter.com/1/notifications/leave/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem
    if(user_id!=0) propBag.addQueryItem("user_id",QString::number(user_id,10));
    if(screen_name!="") propBag.addQueryItem("screen_name",screen_name);



    issuePostRequest(LEAVE_NOTIFICATION, requestStr, propBag.items);

}
void Petrel::locationTrends(const QString& woeid){

    QString requestStr("http://api.twitter.com/1/trends/"+woeid+".xml");
    PropertyBag propBag;
    //addQueryItem



    issueGetRequest(LOCATION_TRENDS, requestStr, propBag.items);

}
void Petrel::mentions(quint64 since_id, quint64 max_id, int count, int page){

    QString requestStr("http://api.twitter.com/1/statuses/mentions.xml");
    PropertyBag propBag;
    //addQueryItem
    if(since_id!=0) propBag.addQueryItem("since_id",QString::number(since_id,10));
    if(max_id!=0) propBag.addQueryItem("max_id",QString::number(max_id,10));
    if(count!=0) propBag.addQueryItem("count",QString::number(count,10));
    if(page!=0) propBag.addQueryItem("page",QString::number(page,10));


    issueGetRequest(MENTIONS, requestStr, propBag.items);

}
void Petrel::newDirectMessage(const QString& user, const QString& text){

    QString requestStr("http://api.twitter.com/1/direct_messages/new.xml");
    PropertyBag propBag;
    //addQueryItem
    if(user!="") propBag.addQueryItem("user",user);
    if(text!="") propBag.addQueryItem("text",text);



    issuePostRequest(NEW_DIRECT_MESSAGE, requestStr, propBag.items);

}
void Petrel::postList(const QString& name, const QString& mode, const QString& description, quint64 list_id, quint64 id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/lists.xml");
    PropertyBag propBag;
    //addQueryItem
    if(name!="") propBag.addQueryItem("name",name);
    if(mode!="") propBag.addQueryItem("mode",mode);
    if(description!="") propBag.addQueryItem("description",description);
    if(list_id!=0) propBag.addQueryItem("list_id",QString::number(list_id,10));
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));



    issuePostRequest(POST_LIST, requestStr, propBag.items);

}
void Petrel::postListMember(quint64 list_id, quint64 id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/"+QString::number(list_id,10)+"/members.xml");
    PropertyBag propBag;
    //addQueryItem
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));



    issuePostRequest(POST_LIST_MEMBER, requestStr, propBag.items);

}
void Petrel::postListSubscriber(quint64 list_id, quint64 id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/"+QString::number(list_id,10)+"/subscribers.xml");
    PropertyBag propBag;
    //addQueryItem
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));



    issuePostRequest(POST_LIST_SUBSCRIBER, requestStr, propBag.items);

}
void Petrel::postListsId(const QString& name, const QString& mode, const QString& description, quint64 list_id, quint64 id){

    QString requestStr("http://api.twitter.com/1/"+m_userid+"/lists/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem
    if(name!="") propBag.addQueryItem("name",name);
    if(mode!="") propBag.addQueryItem("mode",mode);
    if(description!="") propBag.addQueryItem("description",description);
    if(list_id!=0) propBag.addQueryItem("list_id",QString::number(list_id,10));



    issuePutRequest(POST_LISTS_ID, requestStr, propBag.items);

}
void Petrel::publicTimeline(){

    QString requestStr("http://api.twitter.com/1/statuses/public_timeline.xml");
    PropertyBag propBag;
    //addQueryItem



    issueGetRequest(PUBLIC_TIMELINE, requestStr, propBag.items);

}
void Petrel::rateLimitStatus(){

    QString requestStr("http://api.twitter.com/1/account/rate_limit_status.xml");
    PropertyBag propBag;
    //addQueryItem



    issueGetRequest(RATE_LIMIT_STATUS, requestStr, propBag.items);

}
void Petrel::reportSpam(quint64 id, quint64 user_id, const QString& screen_name){

    QString requestStr("http://api.twitter.com/1/report_spam.xml");
    PropertyBag propBag;
    //addQueryItem
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));
    if(user_id!=0) propBag.addQueryItem("user_id",QString::number(user_id,10));
    if(screen_name!="") propBag.addQueryItem("screen_name",screen_name);



    issuePostRequest(REPORT_SPAM, requestStr, propBag.items);

}
void Petrel::retweet(quint64 id){

    QString requestStr("http://api.twitter.com/1/statuses/retweet/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem




    issuePostRequest(RETWEET, requestStr, propBag.items);

}
void Petrel::retweetedByMe(quint64 since_id, quint64 max_id, int count, int page){

    QString requestStr("http://api.twitter.com/1/statuses/retweeted_by_me.xml");
    PropertyBag propBag;
    //addQueryItem
    if(since_id!=0) propBag.addQueryItem("since_id",QString::number(since_id,10));
    if(max_id!=0) propBag.addQueryItem("max_id",QString::number(max_id,10));
    if(count!=0) propBag.addQueryItem("count",QString::number(count,10));
    if(page!=0) propBag.addQueryItem("page",QString::number(page,10));


    issueGetRequest(RETWEETED_BY_ME, requestStr, propBag.items);

}
void Petrel::retweetedToMe(quint64 since_id, quint64 max_id, int count, int page){

    QString requestStr("http://api.twitter.com/1/statuses/retweeted_to_me.xml");
    PropertyBag propBag;
    //addQueryItem
    if(since_id!=0) propBag.addQueryItem("since_id",QString::number(since_id,10));
    if(max_id!=0) propBag.addQueryItem("max_id",QString::number(max_id,10));
    if(count!=0) propBag.addQueryItem("count",QString::number(count,10));
    if(page!=0) propBag.addQueryItem("page",QString::number(page,10));


    issueGetRequest(RETWEETED_TO_ME, requestStr, propBag.items);

}
void Petrel::retweets(quint64 id, int count){

    QString requestStr("http://api.twitter.com/1/statuses/retweets/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem
    if(count!=0) propBag.addQueryItem("count",QString::number(count,10));


    issueGetRequest(RETWEETS, requestStr, propBag.items);

}
void Petrel::retweetsOfMe(quint64 since_id, quint64 max_id, int count, int page){

    QString requestStr("http://api.twitter.com/1/statuses/retweets_of_me.xml");
    PropertyBag propBag;
    //addQueryItem
    if(since_id!=0) propBag.addQueryItem("since_id",QString::number(since_id,10));
    if(max_id!=0) propBag.addQueryItem("max_id",QString::number(max_id,10));
    if(count!=0) propBag.addQueryItem("count",QString::number(count,10));
    if(page!=0) propBag.addQueryItem("page",QString::number(page,10));


    issueGetRequest(RETWEETS_OF_ME, requestStr, propBag.items);

}
void Petrel::savedSearches(){

    QString requestStr("http://api.twitter.com/1/saved_searches.xml");
    PropertyBag propBag;
    //addQueryItem



    issueGetRequest(SAVED_SEARCHES, requestStr, propBag.items);

}
void Petrel::searchUsers(const QString& q, const QString& per_page, int page){

    QString requestStr("http://api.twitter.com/1/users/search.xml");
    PropertyBag propBag;
    //addQueryItem
    if(q!="") propBag.addQueryItem("q",q);
    if(per_page!="") propBag.addQueryItem("per_page",per_page);
    if(page!=0) propBag.addQueryItem("page",QString::number(page,10));


    issueGetRequest(SEARCH_USERS, requestStr, propBag.items);

}
void Petrel::sentDirectMessages(quint64 since_id, quint64 max_id, int count, int page){

    QString requestStr("http://api.twitter.com/1/direct_messages/sent.xml");
    PropertyBag propBag;
    //addQueryItem
    if(since_id!=0) propBag.addQueryItem("since_id",QString::number(since_id,10));
    if(max_id!=0) propBag.addQueryItem("max_id",QString::number(max_id,10));
    if(count!=0) propBag.addQueryItem("count",QString::number(count,10));
    if(page!=0) propBag.addQueryItem("page",QString::number(page,10));


    issueGetRequest(SENT_DIRECT_MESSAGES, requestStr, propBag.items);

}
void Petrel::show(quint64 id){

    QString requestStr("http://api.twitter.com/1/statuses/show/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem



    issueGetRequest(SHOW, requestStr, propBag.items);

}
void Petrel::showFriendships(quint64 source_id, const QString& source_screen_name, quint64 target_id, const QString& target_screen_name){

    QString requestStr("http://api.twitter.com/1/friendships/show.xml");
    PropertyBag propBag;
    //addQueryItem
    if(source_id!=0) propBag.addQueryItem("source_id",QString::number(source_id,10));
    if(source_screen_name!="") propBag.addQueryItem("source_screen_name",source_screen_name);
    if(target_id!=0) propBag.addQueryItem("target_id",QString::number(target_id,10));
    if(target_screen_name!="") propBag.addQueryItem("target_screen_name",target_screen_name);


    issueGetRequest(SHOW_FRIENDSHIPS, requestStr, propBag.items);

}
void Petrel::showSavedSearch(quint64 id){

    QString requestStr("http://api.twitter.com/1/saved_searches/show/"+QString::number(id,10)+".xml");
    PropertyBag propBag;
    //addQueryItem



    issueGetRequest(SHOW_SAVED_SEARCH, requestStr, propBag.items);

}
void Petrel::showUsers(quint64 id, quint64 user_id, const QString& screen_name){

    QString requestStr("http://api.twitter.com/1/users/show.xml");
    PropertyBag propBag;
    //addQueryItem
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));
    if(user_id!=0) propBag.addQueryItem("user_id",QString::number(user_id,10));
    if(screen_name!="") propBag.addQueryItem("screen_name",screen_name);


    issueGetRequest(SHOW_USERS, requestStr, propBag.items);

}
void Petrel::testHelp(){

    QString requestStr("http://api.twitter.com/1/help/test.xml");
    PropertyBag propBag;
    //addQueryItem



    issueGetRequest(TEST_HELP, requestStr, propBag.items);

}
void Petrel::update(const QString& status, quint64 in_reply_to_status_id, const QString& lat, const QString& long_){

    QString requestStr("http://api.twitter.com/1/statuses/update.xml");
    PropertyBag propBag;
    //addQueryItem
    if(status!="") propBag.addQueryItem("status",status);
    if(in_reply_to_status_id!=0) propBag.addQueryItem("in_reply_to_status_id",QString::number(in_reply_to_status_id,10));
    if(lat!="") propBag.addQueryItem("lat",lat);
    if(long_!="") propBag.addQueryItem("long",long_);



    issuePostRequest(UPDATE, requestStr, propBag.items);

}
void Petrel::updateDeliveryDevice(const QString& device){

    QString requestStr("http://api.twitter.com/1/account/update_delivery_device.xml");
    PropertyBag propBag;
    //addQueryItem
    if(device!="") propBag.addQueryItem("device",device);



    issuePostRequest(UPDATE_DELIVERY_DEVICE, requestStr, propBag.items);

}
void Petrel::updateProfile(const QString& name, const QString& url, const QString& location, const QString& description){

    QString requestStr("http://api.twitter.com/1/account/update_profile.xml");
    PropertyBag propBag;
    //addQueryItem
    if(name!="") propBag.addQueryItem("name",name);
    if(url!="") propBag.addQueryItem("url",url);
    if(location!="") propBag.addQueryItem("location",location);
    if(description!="") propBag.addQueryItem("description",description);



    issuePostRequest(UPDATE_PROFILE, requestStr, propBag.items);

}
void Petrel::updateProfileBackgroundImage(const QString& image, const QString& tile){

}
void Petrel::updateProfileColor(const QString& profile_background_color, const QString& profile_text_color, const QString& profile_link_color, const QString& profile_sidebar_fill_color, const QString& profile_sidebar_border_color){

    QString requestStr("http://api.twitter.com/1/account/update_profile_colors.xml");
    PropertyBag propBag;
    //addQueryItem
    if(profile_background_color!="") propBag.addQueryItem("profile_background_color",profile_background_color);
    if(profile_text_color!="") propBag.addQueryItem("profile_text_color",profile_text_color);
    if(profile_link_color!="") propBag.addQueryItem("profile_link_color",profile_link_color);
    if(profile_sidebar_fill_color!="") propBag.addQueryItem("profile_sidebar_fill_color",profile_sidebar_fill_color);
    if(profile_sidebar_border_color!="") propBag.addQueryItem("profile_sidebar_border_color",profile_sidebar_border_color);



    issuePostRequest(UPDATE_PROFILE_COLOR, requestStr, propBag.items);

}
void Petrel::updateProfileImage(const QString& image){

}
void Petrel::userTimeline(quint64 id, quint64 user_id, const QString& screen_name, quint64 since_id, quint64 max_id, int count, int page){

    QString requestStr("http://api.twitter.com/1/statuses/user_timeline.xml");
    PropertyBag propBag;
    //addQueryItem
    if(id!=0) propBag.addQueryItem("id",QString::number(id,10));
    if(user_id!=0) propBag.addQueryItem("user_id",QString::number(user_id,10));
    if(screen_name!="") propBag.addQueryItem("screen_name",screen_name);
    if(since_id!=0) propBag.addQueryItem("since_id",QString::number(since_id,10));
    if(max_id!=0) propBag.addQueryItem("max_id",QString::number(max_id,10));
    if(count!=0) propBag.addQueryItem("count",QString::number(count,10));
    if(page!=0) propBag.addQueryItem("page",QString::number(page,10));


    issueGetRequest(USER_TIMELINE, requestStr, propBag.items);

}
void Petrel::verifyCredentials(){

    QString requestStr("http://api.twitter.com/1/account/verify_credentials.xml");
    PropertyBag propBag;
    //addQueryItem



    issueGetRequest(VERIFY_CREDENTIALS, requestStr, propBag.items);

}

//END autogenerated methods

void Petrel::replyFinished(QNetworkReply *reply)
{
    m_replies.removeOne(reply);
    QNetworkRequest request = reply->request();
    QString replyStr( reply->readAll() );
    int role = request.attribute( (QNetworkRequest::Attribute)(QNetworkRequest::User + ATTR_ROLE) ).toInt();
    QDomDocument doc;
    doc.setContent(replyStr);
    if(doc.documentElement().tagName()=="error"){
        emit error(role, doc.documentElement());
        return;
    }
    if(doc.documentElement().tagName()=="hash"){
        QDomElement child = doc.documentElement().firstChildElement();
        for (; !child.isNull(); child = child.nextSiblingElement()) {
          if(child.tagName()=="error"){
              emit error(role, doc.documentElement());
              return;
          }
        }
    }
    if(m_useXAuth){
        qDebug() << reply->rawHeaderList();
        if(reply->hasRawHeader("X-RateLimit-Limit")){
            m_xauth_limit = QString(reply->rawHeader("X-RateLimit-Limit")).toInt(NULL,10);
        }
        if(reply->hasRawHeader("X-RateLimit-Remaining")){
            m_xauth_remaining = QString(reply->rawHeader("X-RateLimit-Remaining")).toInt(NULL,10);
        }
    }
    switch(role){
        //autogenerated choice
        case AVAILABLE_TRENDS:
        {
          locations_t l(doc.documentElement());
          emit availableTrendsReceived(l);
          break;
        }
        case BLOCKING_IDS_BLOCKS:
        {
          ids_t i(doc.documentElement());
          emit blocking_IdsBlocksReceived(i);
          break;
        }
        case CREATE_BLOCK:
        {
          user_t u(doc.documentElement());
          emit createBlockReceived(u);
          break;
        }
        case CREATE_FAVORITE:
        {
          status_t s(doc.documentElement());
          emit createFavoriteReceived(s);
          break;
        }
        case CREATE_FRIENDSHIP:
        {
          user_t u(doc.documentElement());
          emit createFriendshipReceived(u);
          break;
        }
        case CREATE_SAVED_SEARCH:
        {
          saved_search_t s(doc.documentElement());
          emit createSavedSearchReceived(s);
          break;
        }
        case DELETE_LIST_ID:
        {
          list_t l(doc.documentElement());
          emit deleteListIdReceived(l);
          break;
        }
        case DELETE_LIST_MEMBER:
        {
          list_t l(doc.documentElement());
          emit deleteListMemberReceived(l);
          break;
        }
        case DELETE_LIST_SUBSCRIBER:
        {
          list_t l(doc.documentElement());
          emit deleteListSubscriberReceived(l);
          break;
        }
        case DESTROY:
        {
          status_t s(doc.documentElement());
          emit destroyReceived(s);
          break;
        }
        case DESTROY_BLOCK:
        {
          user_t u(doc.documentElement());
          emit destroyBlockReceived(u);
          break;
        }
        case DESTROY_DIRECT_MESSAGE:
        {
          direct_message_t d(doc.documentElement());
          emit destroyDirectMessageReceived(d);
          break;
        }
        case DESTROY_FAVORITE:
        {
          status_t s(doc.documentElement());
          emit destroyFavoriteReceived(s);
          break;
        }
        case DESTROY_FRIENDSHIP:
        {
          user_t u(doc.documentElement());
          emit destroyFriendshipReceived(u);
          break;
        }
        case DESTROY_SAVED_SEARCH:
        {
          saved_search_t s(doc.documentElement());
          emit destroySavedSearchReceived(s);
          break;
        }
        case DIRECT_MESSAGES:
        {
          direct_messages_t d(doc.documentElement());
          emit directMessagesReceived(d);
          break;
        }
        case EXISTS_FRIENDSHIPS:
        {
          friends_t f(doc.documentElement());
          emit existsFriendshipsReceived(f);
          break;
        }
        case FAVORITES:
        {
          statuses_t s(doc.documentElement());
          emit favoritesReceived(s);
          break;
        }
        case FOLLOW_NOTIFICATION:
        {
          user_t u(doc.documentElement());
          emit followNotificationReceived(u);
          break;
        }
        case FOLLOWERS:
        {
          users_t u(doc.documentElement());
          emit followersReceived(u,0,0);
          break;
        }
        case FRIENDS:
        {
          users_t u(doc.documentElement());
          emit friendsReceived(u,0,0);
          break;
        }
        case FRIENDS_TIMELINE:
        {
          statuses_t s(doc.documentElement());
          emit friendsTimelineReceived(s);
          break;
        }
        case GET_LIST_ID:
        {
          list_t l(doc.documentElement());
          emit getListIdReceived(l);
          break;
        }
        case GET_LIST_MEMBERS:
        {
          users_list_t u(doc.documentElement());
          emit getListMembersReceived(u);
          break;
        }
        case GET_LIST_MEMBERS_ID:
        {
          user_t u(doc.documentElement());
          emit getListMembersIdReceived(u);
          break;
        }
        case GET_LIST_MEMBERSHIPS:
        {
          lists_list_t l(doc.documentElement());
          emit getListMembershipsReceived(l);
          break;
        }
        case GET_LIST_STATUSES:
        {
          statuses_t s(doc.documentElement());
          emit getListStatusesReceived(s);
          break;
        }
        case GET_LIST_SUBSCRIBERS:
        {
          users_list_t u(doc.documentElement());
          emit getListSubscribersReceived(u);
          break;
        }
        case GET_LIST_SUBSCRIBERS_ID:
        {
          user_t u(doc.documentElement());
          emit getListSubscribersIdReceived(u);
          break;
        }
        case GET_LIST_SUBSCRIPTIONS:
        {
          lists_list_t l(doc.documentElement());
          emit getListSubscriptionsReceived(l);
          break;
        }
        case GET_LISTS:
        {
          lists_list_t l(doc.documentElement());
          emit getListsReceived(l);
          break;
        }
        case HOME_TIMELINE:
        {
          statuses_t s(doc.documentElement());
          if(m_useXAuth){
              qDebug() << reply->rawHeaderList();
          }
          emit homeTimelineReceived(s);
          break;
        }
        case IDS_FOLLOWERS:
        {
          id_list_t i(doc.documentElement());
          emit idsFollowersReceived(i);
          break;
        }
        case IDS_FRIENDS:
        {
          id_list_t i(doc.documentElement());
          emit idsFriendsReceived(i);
          break;
        }
        case LEAVE_NOTIFICATION:
        {
          user_t u(doc.documentElement());
          emit leaveNotificationReceived(u);
          break;
        }
        case LOCATION_TRENDS:
        {
          matching_trends_t m(doc.documentElement());
          emit locationTrendsReceived(m);
          break;
        }
        case MENTIONS:
        {
          statuses_t s(doc.documentElement());
          emit mentionsReceived(s);
          break;
        }
        case NEW_DIRECT_MESSAGE:
        {
          direct_message_t d(doc.documentElement());
          emit newDirectMessageReceived(d);
          break;
        }
        case POST_LIST:
        {
          list_t l(doc.documentElement());
          emit postListReceived(l);
          break;
        }
        case POST_LIST_MEMBER:
        {
          list_t l(doc.documentElement());
          emit postListMemberReceived(l);
          break;
        }
        case POST_LIST_SUBSCRIBER:
        {
          list_t l(doc.documentElement());
          emit postListSubscriberReceived(l);
          break;
        }
        case POST_LISTS_ID:
        {
          list_t l(doc.documentElement());
          emit postListsIdReceived(l);
          break;
        }
        case PUBLIC_TIMELINE:
        {
          statuses_t s(doc.documentElement());
          emit publicTimelineReceived(s);
          break;
        }
        case RATE_LIMIT_STATUS:
        {
          hash_t h(doc.documentElement());
          if(m_xauth&&m_xauth_limit>0){
              h.hourly_limit = QString::number(m_xauth_limit,10);
              h.remaining_hits = QString::number(m_xauth_remaining,10);
          }
          emit rateLimitStatusReceived(h);
          break;
        }
        case REPORT_SPAM:
        {
          user_t u(doc.documentElement());
          emit reportSpamReceived(u);
          break;
        }
        case RETWEET:
        {
          status_t s(doc.documentElement());
          emit retweetReceived(s);
          break;
        }
        case RETWEETED_BY_ME:
        {
          statuses_t s(doc.documentElement());
          emit retweetedByMeReceived(s);
          break;
        }
        case RETWEETED_TO_ME:
        {
          statuses_t s(doc.documentElement());
          emit retweetedToMeReceived(s);
          break;
        }
        case RETWEETS:
        {
          statuses_t s(doc.documentElement());
          emit retweetsReceived(s);
          break;
        }
        case RETWEETS_OF_ME:
        {
          statuses_t s(doc.documentElement());
          emit retweetsOfMeReceived(s);
          break;
        }
        case SAVED_SEARCHES:
        {
          saved_searches_t s(doc.documentElement());
          emit savedSearchesReceived(s);
          break;
        }
        case SEARCH_USERS:
        {
          users_t u(doc.documentElement());
          emit searchUsersReceived(u);
          break;
        }
        case SENT_DIRECT_MESSAGES:
        {
          direct_messages_t d(doc.documentElement());
          emit sentDirectMessagesReceived(d);
          break;
        }
        case SHOW:
        {
          status_t s(doc.documentElement());
          emit showReceived(s);
          break;
        }
        case SHOW_FRIENDSHIPS:
        {
          relationship_t r(doc.documentElement());
          emit showFriendshipsReceived(r);
          break;
        }
        case SHOW_SAVED_SEARCH:
        {
          saved_search_t s(doc.documentElement());
          emit showSavedSearchReceived(s);
          break;
        }
        case SHOW_USERS:
        {
          user_t u(doc.documentElement());
          emit showUsersReceived(u);
          break;
        }
        case TEST_HELP:
        {
          ok_t o(doc.documentElement());
          emit testHelpReceived(o);
          break;
        }
        case UPDATE:
        {
          status_t s(doc.documentElement());
          emit updateReceived(s);
          break;
        }
        case UPDATE_DELIVERY_DEVICE:
        {
          user_t u(doc.documentElement());
          emit updateDeliveryDeviceReceived(u);
          break;
        }
        case UPDATE_PROFILE:
        {
          user_t u(doc.documentElement());
          emit updateProfileReceived(u);
          break;
        }
        case UPDATE_PROFILE_BACKGROUND_IMAGE:
        {
          user_t u(doc.documentElement());
          emit updateProfileBackgroundImageReceived(u);
          break;
        }
        case UPDATE_PROFILE_COLOR:
        {
          user_t u(doc.documentElement());
          emit updateProfileColorReceived(u);
          break;
        }
        case UPDATE_PROFILE_IMAGE:
        {
          user_t u(doc.documentElement());
          emit updateProfileImageReceived(u);
          break;
        }
        case USER_TIMELINE:
        {
          statuses_t s(doc.documentElement());
          emit userTimelineReceived(s);
          break;
        }
        case VERIFY_CREDENTIALS:
        {
          user_t u(doc.documentElement());
          emit verifyCredentialsReceived(u);
          break;
        }

    }
    reply->deleteLater();
}
