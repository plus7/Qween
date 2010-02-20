#ifndef PETREL_TYPES_H
#define PETREL_TYPES_H
#include <QtCore>
#include <QtXml>
#include <QColor>
#include <QRegExp>

class Color{
public:
  Color(const QString& str){
    bool ok;
    int hex = str.toInt(&ok, 16);
    m_color.setRgb((hex >> 16)&0xFF, (hex >> 8)&0xFF, hex & 0xFF);
  }
  Color(){}
  QString toString(){return "";}
  QColor getColor(){return m_color;}
private:
  QColor m_color;
};
QUrl getUrlValue(QDomElement elm);
bool getBoolValue(QDomElement elm);
int getIntValue(QDomElement elm);
quint64 getUInt64Value(QDomElement elm);
QString getStrValue(QDomElement elm);
QDateTime getDateTimeValue(QDomElement elm);
Color getColorValue(QDomElement elm);
class friends_t;
class friends_t{
public:
    bool value;
    friends_t(QDomElement element){
        value = getBoolValue(element);
    }
};
class ok_t;
class ok_t{
public:
    bool value;
    ok_t(QDomElement element){
        value = getBoolValue(element);
    }
};
class locations_t;
class ids_t;
class id_list_t;
class trends_t;
class saved_search_t;
class statuses_t;
class direct_messages_t;
class geo_t;
class target_t;
class status_t;
class saved_searches_t;
class hash_t;
class users_t;
class retweeted_status_t;
class lists_t;
class relationship_t;
class user_t;
class recipient_t;
class direct_message_t;
class matching_trends_t;
class sender_t;
class lists_list_t;
class users_list_t;
class list_t;
class locations_t {
public:
  locations_t(QDomElement element);
  QList<QString> location;
};
class saved_search_t {
public:
  saved_search_t(QDomElement element);
  QString query;
  QDateTime created_at;
  int position;
  quint64 id;
  QString name;
};
class recipient_t {
public:
  recipient_t(QDomElement element);
  quint64 id;
  bool verified;
  Color profile_sidebar_fill_color;
  Color profile_text_color;
  quint64 followers_count;
  bool protected_;
  QString location;
  Color profile_background_color;
  int utc_offset;
  quint64 statuses_count;
  QString description;
  quint64 friends_count;
  Color profile_link_color;
  QUrl profile_image_url;
  bool notifications;
  QUrl profile_background_image_url;
  QString screen_name;
  bool profile_background_tile;
  quint64 favourites_count;
  QString name;
  QUrl url;
  QDateTime created_at;
  QString time_zone;
  Color profile_sidebar_border_color;
  bool following;
};
class hash_t {
public:
  hash_t(QDomElement element);
  QString remaining_hits;
  QString hourly_limit;
  QString reset_time_in_seconds;
  QString reset_time;
};
class geo_t {
public:
  geo_t(QDomElement element);
  QString georss_point;
};
class target_t {
public:
  target_t(QDomElement element);
  bool following;
  QString notifications_enabled;
  bool followed_by;
  quint64 id;
  QString screen_name;
};
class sender_t {
public:
  sender_t(QDomElement element);
  quint64 id;
  bool verified;
  Color profile_sidebar_fill_color;
  Color profile_text_color;
  quint64 followers_count;
  bool protected_;
  QString location;
  Color profile_background_color;
  int utc_offset;
  quint64 statuses_count;
  QString description;
  quint64 friends_count;
  Color profile_link_color;
  QUrl profile_image_url;
  bool notifications;
  QUrl profile_background_image_url;
  QString screen_name;
  bool profile_background_tile;
  quint64 favourites_count;
  QString name;
  QUrl url;
  QDateTime created_at;
  QString time_zone;
  Color profile_sidebar_border_color;
  bool following;
};
class ids_t {
public:
  ids_t(QDomElement element);
  QList<quint64> id;
};
class trends_t {
public:
  trends_t(QDomElement element);
  QString trend;
  QSharedPointer<locations_t> locations;
};
class retweeted_status_t {
public:
  retweeted_status_t(QDomElement element);
  bool favorited;
  bool truncated;
  QString text;
  QDateTime created_at;
  QString source;
  quint64 in_reply_to_status_id;
  QString in_reply_to_screen_name;
  quint64 in_reply_to_user_id;
  QSharedPointer<geo_t> geo;
  quint64 id;
  QSharedPointer<user_t> user;
};
class lists_t {
public:
  lists_t(QDomElement element);
  QSharedPointer<list_t> list;
};
class direct_message_t {
public:
  direct_message_t(QDomElement element);
  QSharedPointer<sender_t> sender;
  QString text;
  QString sender_screen_name;
  quint64 sender_id;
  quint64 id;
  quint64 recipient_id;
  QString recipient_screen_name;
  QSharedPointer<recipient_t> recipient;
  QDateTime created_at;
};
class id_list_t {
public:
  id_list_t(QDomElement element);
  QString previous_cursor;
  QString next_cursor;
  QSharedPointer<ids_t> ids;
};
class statuses_t {
public:
  statuses_t(QDomElement element);
  QList<QSharedPointer<status_t> > status;
};
class direct_messages_t {
public:
  direct_messages_t(QDomElement element);
  QList<QSharedPointer<direct_message_t> > direct_message;
};
class relationship_t {
public:
  relationship_t(QDomElement element);
  QString source;
  QSharedPointer<target_t> target;
};
class matching_trends_t {
public:
  matching_trends_t(QDomElement element);
  QSharedPointer<trends_t> trends;
};
class status_t {
public:
  status_t(QDomElement element);
  bool favorited;
  QString contributors;
  QSharedPointer<retweeted_status_t> retweeted_status;
  bool truncated;
  QString text;
  QDateTime created_at;
  QString source;
  quint64 in_reply_to_status_id;
  QString in_reply_to_screen_name;
  quint64 in_reply_to_user_id;
  QSharedPointer<geo_t> geo;
  quint64 id;
  QSharedPointer<user_t> user;
};
class saved_searches_t {
public:
  saved_searches_t(QDomElement element);
  QList<QSharedPointer<saved_search_t> > saved_search;
};
class users_t {
public:
  users_t(QDomElement element);
  QList<QSharedPointer<user_t> > user;
};
class user_t {
public:
  user_t(QDomElement element);
  quint64 id;
  bool verified;
  Color profile_sidebar_fill_color;
  Color profile_text_color;
  quint64 followers_count;
  bool protected_;
  QString location;
  Color profile_background_color;
  QSharedPointer<status_t> status;
  int utc_offset;
  quint64 statuses_count;
  QString description;
  quint64 friends_count;
  Color profile_link_color;
  QUrl profile_image_url;
  bool notifications;
  bool geo_enabled;
  QUrl profile_background_image_url;
  QString screen_name;
  QString lang;
  bool profile_background_tile;
  quint64 favourites_count;
  QString name;
  QUrl url;
  QDateTime created_at;
  bool contributors_enabled;
  QString time_zone;
  Color profile_sidebar_border_color;
  bool following;
};
class lists_list_t {
public:
  lists_list_t(QDomElement element);
  QString previous_cursor;
  QString next_cursor;
  QSharedPointer<lists_t> lists;
};
class users_list_t {
public:
  users_list_t(QDomElement element);
  QString previous_cursor;
  QString next_cursor;
  QSharedPointer<users_t> users;
};
class list_t {
public:
  list_t(QDomElement element);
  QString subscriber_count;
  QString member_count;
  QSharedPointer<user_t> user;
  QString description;
  QString uri;
  quint64 id;
  QString mode;
  QString full_name;
  QString slug;
  QString name;
};
#endif
