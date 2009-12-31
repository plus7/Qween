#ifndef TWITTER_H
#define TWITTER_H

#include <QObject>
#include "QTwitLib.h"
class TimelineModel;
namespace Twitter{
    enum ItemType
    {
        Undefined, Status, DirectMessage
    };

    class TwitterItem{
        //Q_OBJECT
    public:
        TwitterItem(TimelineModel *parent = 0)
            :m_type(Undefined),m_read(false),m_parent(parent)
        {}

        TwitterItem(ItemType type, Returnables::StatusElementPtr ptr, Returnables::RequestId origin,
                    bool read, TimelineModel *parent = 0)
            :m_type(type), m_sePtr(ptr), m_origin(origin), m_read(read),m_parent(parent)
        {}

        TwitterItem(ItemType type, Returnables::DirectMessageElementPtr ptr, Returnables::RequestId origin,
                    bool read, TimelineModel *parent = 0)
            :m_type(type), m_dmPtr(ptr), m_origin(origin), m_read(read),m_parent(parent)
        {}

        QString userName() const {
            switch(m_type){
            case Twitter::Status:
                return m_sePtr->user.name;
                break;
            case Twitter::DirectMessage:
                return m_dmPtr->sender.name;
                break;
            default:
                return QString();
            }
        }

        QString screenName() const {
            switch(m_type){
            case Twitter::Status:
                return m_sePtr->user.screenName;
                break;
            case Twitter::DirectMessage:
                return m_dmPtr->sender.screenName;
                break;
            default:
                return QString();
            }
        }

        QString createdAt() const {
            switch(m_type){
            case Twitter::Status:
                return m_sePtr->status.createdAt;
                break;
            case Twitter::DirectMessage:
                return m_dmPtr->headerInfo.createdAt;
                break;
            default:
                return QString();
            }
        }

        QString status() const {
            switch(m_type){
            case Twitter::Status:
                return m_sePtr->status.text;
                break;
            case Twitter::DirectMessage:
                return m_dmPtr->headerInfo.text;
                break;
            default:
                return QString();
            }
        }

        quint64 id()const {
            switch(m_type){
            case Twitter::Status:
                return m_sePtr->status.id;
                break;
            case Twitter::DirectMessage:
                return m_dmPtr->headerInfo.id;
                break;
            default:
                return 0;
            }
        }

        quint64 userId() const {
            switch(m_type){
            case Twitter::Status:
                return m_sePtr->user.id;
                break;
            case Twitter::DirectMessage:
                return m_dmPtr->sender.id;
                break;
            default:
                return 0;
            }
        }

        QString iconUri() const {
            switch(m_type){
            case Twitter::Status:
                return m_sePtr->user.profileImageUrl;
                break;
            case Twitter::DirectMessage:
                return m_dmPtr->sender.profileImageUrl;
                break;
            default:
                return 0;
            }
        }
        TimelineModel *parent() const { return m_parent; }
        void setParent(TimelineModel *parent){ m_parent = parent; }
        enum ItemType type() const { return m_type; }
        Returnables::RequestId origin() const { return m_origin; }
        bool read() const { return m_read; }
        void setRead(bool val = true) { m_read = val; }
    private:
        enum ItemType m_type;
        Returnables::StatusElementPtr m_sePtr;
        Returnables::DirectMessageElementPtr m_dmPtr;
        Returnables::RequestId m_origin;
        bool m_read;
        TimelineModel *m_parent;
    };
};


#endif // TWITTER_H