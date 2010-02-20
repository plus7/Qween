#include <QtCore>
#include <QtXml>
#include "petrel_types.h"


QUrl getUrlValue(QDomElement elm){
  return QUrl(getStrValue(elm));
}

bool getBoolValue(QDomElement elm){
  if(getStrValue(elm)=="true"){
    return true;
  }else{
    return false;
  }
}

int getIntValue(QDomElement elm){
  QString str = getStrValue(elm);
  bool ok;
  quint64 dec = str.toInt(&ok, 10);
  if(ok)
    return dec;
  else
    return 0;
}

quint64 getUInt64Value(QDomElement elm){
  QString str = getStrValue(elm);
  bool ok;
  quint64 dec = str.toULongLong(&ok, 10);
  if(ok)
    return dec;
  else
    return 0;
}

QString getStrValue(QDomElement elm){
  QDomNode child = elm.firstChild();
  if(child.isNull())
    return "";
  else
    return child.nodeValue();
}

int getMonthByEngStr(const QString& str){
    if(str == "Jan") return 1;
    else if(str == "Feb") return 2;
    else if(str == "Mar") return 3;
    else if(str == "Apr") return 4;
    else if(str == "May") return 5;
    else if(str == "Jun") return 6;
    else if(str == "Jul") return 7;
    else if(str == "Aug") return 8;
    else if(str == "Sep") return 9;
    else if(str == "Qct") return 10;
    else if(str == "Nov") return 11;
    else if(str == "Jan") return 12;
    else return -1;
}

QDateTime getDateTimeValue(QDomElement elm){
    QRegExp rx("(...) (...) (\\d\\d) (\\d\\d:\\d\\d:\\d\\d) (\\+\\d\\d\\d\\d) (\\d\\d\\d\\d)");
    int pos = 0;
    QString str = getStrValue(elm);
    if((pos = rx.indexIn(str, pos)) != -1){
      QString hoge = QString("%1 %2 %3 %4").arg(getMonthByEngStr(rx.cap(2))).arg(rx.cap(3), rx.cap(4), rx.cap(6));
      QDateTime dt = QDateTime::fromString(hoge, "M dd hh:mm:ss yyyy");
      //qDebug() << QDate::shortMonthName(2);
      //qDebug() << dt.isValid();
      //qDebug() << dt.toString();
      return dt;
    }
    return QDateTime();
}

Color getColorValue(QDomElement elm){
  return Color(getStrValue(elm));
}
locations_t::locations_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "location"){
        location.append(getStrValue(child));
    }
  }
}
saved_search_t::saved_search_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "query"){
        query = getStrValue(child);
    }else if(tagName == "created_at"){
      created_at = getDateTimeValue(child);
    }else if(tagName == "position"){
        position = getIntValue(child);
    }else if(tagName == "id"){
        id = getUInt64Value(child);
    }else if(tagName == "name"){
        name = getStrValue(child);
    }
  }
}
recipient_t::recipient_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "id"){
        id = getUInt64Value(child);
    }else if(tagName == "verified"){
        verified = getBoolValue(child);
    }else if(tagName == "profile_sidebar_fill_color"){
        profile_sidebar_fill_color = getColorValue(child);
    }else if(tagName == "profile_text_color"){
        profile_text_color = getColorValue(child);
    }else if(tagName == "followers_count"){
        followers_count = getUInt64Value(child);
    }else if(tagName == "protected"){
        protected_ = getBoolValue(child);
    }else if(tagName == "location"){
        location = getStrValue(child);
    }else if(tagName == "profile_background_color"){
        profile_background_color = getColorValue(child);
    }else if(tagName == "utc_offset"){
        utc_offset = getIntValue(child);
    }else if(tagName == "statuses_count"){
        statuses_count = getUInt64Value(child);
    }else if(tagName == "description"){
        description = getStrValue(child);
    }else if(tagName == "friends_count"){
        friends_count = getUInt64Value(child);
    }else if(tagName == "profile_link_color"){
        profile_link_color = getColorValue(child);
    }else if(tagName == "profile_image_url"){
        profile_image_url = getUrlValue(child);
    }else if(tagName == "notifications"){
        notifications = getBoolValue(child);
    }else if(tagName == "profile_background_image_url"){
        profile_background_image_url = getUrlValue(child);
    }else if(tagName == "screen_name"){
        screen_name = getStrValue(child);
    }else if(tagName == "profile_background_tile"){
        profile_background_tile = getBoolValue(child);
    }else if(tagName == "favourites_count"){
        favourites_count = getUInt64Value(child);
    }else if(tagName == "name"){
        name = getStrValue(child);
    }else if(tagName == "url"){
        url = getUrlValue(child);
    }else if(tagName == "created_at"){
      created_at = getDateTimeValue(child);
    }else if(tagName == "time_zone"){
        time_zone = getStrValue(child);
    }else if(tagName == "profile_sidebar_border_color"){
        profile_sidebar_border_color = getColorValue(child);
    }else if(tagName == "following"){
        following = getBoolValue(child);
    }
  }
}
hash_t::hash_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "remaining-hits"){
        remaining_hits = getStrValue(child);
    }else if(tagName == "hourly-limit"){
        hourly_limit = getStrValue(child);
    }else if(tagName == "reset-time-in-seconds"){
        reset_time_in_seconds = getStrValue(child);
    }else if(tagName == "reset-time"){
        reset_time = getStrValue(child);
    }
  }
}
geo_t::geo_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "georss:point"){
        georss_point = getStrValue(child);
    }
  }
}
target_t::target_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "following"){
        following = getBoolValue(child);
    }else if(tagName == "notifications_enabled"){
        notifications_enabled = getStrValue(child);
    }else if(tagName == "followed_by"){
        followed_by = getBoolValue(child);
    }else if(tagName == "id"){
        id = getUInt64Value(child);
    }else if(tagName == "screen_name"){
        screen_name = getStrValue(child);
    }
  }
}
sender_t::sender_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "id"){
        id = getUInt64Value(child);
    }else if(tagName == "verified"){
        verified = getBoolValue(child);
    }else if(tagName == "profile_sidebar_fill_color"){
        profile_sidebar_fill_color = getColorValue(child);
    }else if(tagName == "profile_text_color"){
        profile_text_color = getColorValue(child);
    }else if(tagName == "followers_count"){
        followers_count = getUInt64Value(child);
    }else if(tagName == "protected"){
        protected_ = getBoolValue(child);
    }else if(tagName == "location"){
        location = getStrValue(child);
    }else if(tagName == "profile_background_color"){
        profile_background_color = getColorValue(child);
    }else if(tagName == "utc_offset"){
        utc_offset = getIntValue(child);
    }else if(tagName == "statuses_count"){
        statuses_count = getUInt64Value(child);
    }else if(tagName == "description"){
        description = getStrValue(child);
    }else if(tagName == "friends_count"){
        friends_count = getUInt64Value(child);
    }else if(tagName == "profile_link_color"){
        profile_link_color = getColorValue(child);
    }else if(tagName == "profile_image_url"){
        profile_image_url = getUrlValue(child);
    }else if(tagName == "notifications"){
        notifications = getBoolValue(child);
    }else if(tagName == "profile_background_image_url"){
        profile_background_image_url = getUrlValue(child);
    }else if(tagName == "screen_name"){
        screen_name = getStrValue(child);
    }else if(tagName == "profile_background_tile"){
        profile_background_tile = getBoolValue(child);
    }else if(tagName == "favourites_count"){
        favourites_count = getUInt64Value(child);
    }else if(tagName == "name"){
        name = getStrValue(child);
    }else if(tagName == "url"){
        url = getUrlValue(child);
    }else if(tagName == "created_at"){
      created_at = getDateTimeValue(child);
    }else if(tagName == "time_zone"){
        time_zone = getStrValue(child);
    }else if(tagName == "profile_sidebar_border_color"){
        profile_sidebar_border_color = getColorValue(child);
    }else if(tagName == "following"){
        following = getBoolValue(child);
    }
  }
}
ids_t::ids_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "id"){
        id.append(getUInt64Value(child));
    }
  }
}
trends_t::trends_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "trend"){
        trend = getStrValue(child);
    }else if(tagName == "locations"){
      locations = QSharedPointer<locations_t>(new locations_t(child));
    }
  }
}
retweeted_status_t::retweeted_status_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "favorited"){
        favorited = getBoolValue(child);
    }else if(tagName == "truncated"){
        truncated = getBoolValue(child);
    }else if(tagName == "text"){
        text = getStrValue(child);
    }else if(tagName == "created_at"){
      created_at = getDateTimeValue(child);
    }else if(tagName == "source"){
        source = getStrValue(child);
    }else if(tagName == "in_reply_to_status_id"){
        in_reply_to_status_id = getUInt64Value(child);
    }else if(tagName == "in_reply_to_screen_name"){
        in_reply_to_screen_name = getStrValue(child);
    }else if(tagName == "in_reply_to_user_id"){
        in_reply_to_user_id = getUInt64Value(child);
    }else if(tagName == "geo"){
      geo = QSharedPointer<geo_t>(new geo_t(child));
    }else if(tagName == "id"){
        id = getUInt64Value(child);
    }else if(tagName == "user"){
      user = QSharedPointer<user_t>(new user_t(child));
    }
  }
}
lists_t::lists_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "list"){
      list = QSharedPointer<list_t>(new list_t(child));
    }
  }
}
direct_message_t::direct_message_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "sender"){
      sender = QSharedPointer<sender_t>(new sender_t(child));
    }else if(tagName == "text"){
        text = getStrValue(child);
    }else if(tagName == "sender_screen_name"){
        sender_screen_name = getStrValue(child);
    }else if(tagName == "sender_id"){
        sender_id = getUInt64Value(child);
    }else if(tagName == "id"){
        id = getUInt64Value(child);
    }else if(tagName == "recipient_id"){
        recipient_id = getUInt64Value(child);
    }else if(tagName == "recipient_screen_name"){
        recipient_screen_name = getStrValue(child);
    }else if(tagName == "recipient"){
      recipient = QSharedPointer<recipient_t>(new recipient_t(child));
    }else if(tagName == "created_at"){
      created_at = getDateTimeValue(child);
    }
  }
}
id_list_t::id_list_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "previous_cursor"){
        previous_cursor = getStrValue(child);
    }else if(tagName == "next_cursor"){
        next_cursor = getStrValue(child);
    }else if(tagName == "ids"){
      ids = QSharedPointer<ids_t>(new ids_t(child));
    }
  }
}
statuses_t::statuses_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "status"){
      status.append(QSharedPointer<status_t>(new status_t(child)));
    }
  }
}
direct_messages_t::direct_messages_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "direct_message"){
      direct_message.append(QSharedPointer<direct_message_t>(new direct_message_t(child)));
    }
  }
}
relationship_t::relationship_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "source"){
        source = getStrValue(child);
    }else if(tagName == "target"){
      target = QSharedPointer<target_t>(new target_t(child));
    }
  }
}
matching_trends_t::matching_trends_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "trends"){
      trends = QSharedPointer<trends_t>(new trends_t(child));
    }
  }
}
status_t::status_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "favorited"){
        favorited = getBoolValue(child);
    }else if(tagName == "contributors"){
        contributors = getStrValue(child);
    }else if(tagName == "retweeted_status"){
      retweeted_status = QSharedPointer<retweeted_status_t>(new retweeted_status_t(child));
    }else if(tagName == "truncated"){
        truncated = getBoolValue(child);
    }else if(tagName == "text"){
        text = getStrValue(child);
    }else if(tagName == "created_at"){
      created_at = getDateTimeValue(child);
    }else if(tagName == "source"){
        source = getStrValue(child);
    }else if(tagName == "in_reply_to_status_id"){
        in_reply_to_status_id = getUInt64Value(child);
    }else if(tagName == "in_reply_to_screen_name"){
        in_reply_to_screen_name = getStrValue(child);
    }else if(tagName == "in_reply_to_user_id"){
        in_reply_to_user_id = getUInt64Value(child);
    }else if(tagName == "geo"){
      geo = QSharedPointer<geo_t>(new geo_t(child));
    }else if(tagName == "id"){
        id = getUInt64Value(child);
    }else if(tagName == "user"){
      user = QSharedPointer<user_t>(new user_t(child));
    }
  }
}
saved_searches_t::saved_searches_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "saved_search"){
      saved_search.append(QSharedPointer<saved_search_t>(new saved_search_t(child)));
    }
  }
}
users_t::users_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "user"){
      user.append(QSharedPointer<user_t>(new user_t(child)));
    }
  }
}
user_t::user_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "id"){
        id = getUInt64Value(child);
    }else if(tagName == "verified"){
        verified = getBoolValue(child);
    }else if(tagName == "profile_sidebar_fill_color"){
        profile_sidebar_fill_color = getColorValue(child);
    }else if(tagName == "profile_text_color"){
        profile_text_color = getColorValue(child);
    }else if(tagName == "followers_count"){
        followers_count = getUInt64Value(child);
    }else if(tagName == "protected"){
        protected_ = getBoolValue(child);
    }else if(tagName == "location"){
        location = getStrValue(child);
    }else if(tagName == "profile_background_color"){
        profile_background_color = getColorValue(child);
    }else if(tagName == "status"){
      status = QSharedPointer<status_t>(new status_t(child));
    }else if(tagName == "utc_offset"){
        utc_offset = getIntValue(child);
    }else if(tagName == "statuses_count"){
        statuses_count = getUInt64Value(child);
    }else if(tagName == "description"){
        description = getStrValue(child);
    }else if(tagName == "friends_count"){
        friends_count = getUInt64Value(child);
    }else if(tagName == "profile_link_color"){
        profile_link_color = getColorValue(child);
    }else if(tagName == "profile_image_url"){
        profile_image_url = getUrlValue(child);
    }else if(tagName == "notifications"){
        notifications = getBoolValue(child);
    }else if(tagName == "geo_enabled"){
        geo_enabled = getBoolValue(child);
    }else if(tagName == "profile_background_image_url"){
        profile_background_image_url = getUrlValue(child);
    }else if(tagName == "screen_name"){
        screen_name = getStrValue(child);
    }else if(tagName == "lang"){
        lang = getStrValue(child);
    }else if(tagName == "profile_background_tile"){
        profile_background_tile = getBoolValue(child);
    }else if(tagName == "favourites_count"){
        favourites_count = getUInt64Value(child);
    }else if(tagName == "name"){
        name = getStrValue(child);
    }else if(tagName == "url"){
        url = getUrlValue(child);
    }else if(tagName == "created_at"){
      created_at = getDateTimeValue(child);
    }else if(tagName == "contributors_enabled"){
        contributors_enabled = getBoolValue(child);
    }else if(tagName == "time_zone"){
        time_zone = getStrValue(child);
    }else if(tagName == "profile_sidebar_border_color"){
        profile_sidebar_border_color = getColorValue(child);
    }else if(tagName == "following"){
        following = getBoolValue(child);
    }
  }
}
lists_list_t::lists_list_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "previous_cursor"){
        previous_cursor = getStrValue(child);
    }else if(tagName == "next_cursor"){
        next_cursor = getStrValue(child);
    }else if(tagName == "lists"){
      lists = QSharedPointer<lists_t>(new lists_t(child));
    }
  }
}
users_list_t::users_list_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "previous_cursor"){
        previous_cursor = getStrValue(child);
    }else if(tagName == "next_cursor"){
        next_cursor = getStrValue(child);
    }else if(tagName == "users"){
      users = QSharedPointer<users_t>(new users_t(child));
    }
  }
}
list_t::list_t(QDomElement element){
  QDomElement child = element.firstChildElement();
  QString tagName;
  for (; !child.isNull(); child = child.nextSiblingElement()) {
    tagName = child.tagName();
    if(tagName == "subscriber_count"){
        subscriber_count = getStrValue(child);
    }else if(tagName == "member_count"){
        member_count = getStrValue(child);
    }else if(tagName == "user"){
      user = QSharedPointer<user_t>(new user_t(child));
    }else if(tagName == "description"){
        description = getStrValue(child);
    }else if(tagName == "uri"){
        uri = getStrValue(child);
    }else if(tagName == "id"){
        id = getUInt64Value(child);
    }else if(tagName == "mode"){
        mode = getStrValue(child);
    }else if(tagName == "full_name"){
        full_name = getStrValue(child);
    }else if(tagName == "slug"){
        slug = getStrValue(child);
    }else if(tagName == "name"){
        name = getStrValue(child);
    }
  }
}
