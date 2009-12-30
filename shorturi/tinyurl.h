#ifndef TINYURL_H
#define TINYURL_H

#include <QObject>
#include "simpleurishortener.h"
class TinyUrl : public SimpleUriShortener
{
Q_OBJECT
public:
    explicit TinyUrl(QObject *parent = 0):SimpleUriShortener(parent){}

    QString serviceName() { return "tinyurl"; }
    QString shortenUri() { return "http://tinyurl.com/api-create.php"; }
    QString shortenUriParam() { return "url"; }
    int thresholdLength() { return QString("http://tinyurl.com/xxxxxx").length(); }
    QString prefix() { return "http://tinyurl.com/"; }
};

#endif // TINYURL_H
