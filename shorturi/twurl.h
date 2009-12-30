#ifndef TWURL_H
#define TWURL_H
#include <QObject>
#include <QUrl>
#include "simpleurishortener.h"

class TwUrl : public SimpleUriShortener
{
Q_OBJECT
public:
    explicit TwUrl(QObject *parent = 0):SimpleUriShortener(parent){}

    QString serviceName() { return "twurl"; }
    QString shortenUri() { return "http://tweetburner.com/links"; }
    QString shortenUriParam() { return ""; }
    int thresholdLength() { return QString("http://twurl.nl/xxxxxx").length(); }
    QString prefix() { return "http://twurl.nl/"; }
    QString postData(const QString &uri) {
        QString hoge = QUrl::toPercentEncoding(uri);
        return QString("link[url]=") + hoge;
    }
};

#endif // TWURL_H
