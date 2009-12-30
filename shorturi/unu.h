#ifndef UNU_H
#define UNU_H

#include <QObject>
#include "simpleurishortener.h"
class Unu : public SimpleUriShortener
{
Q_OBJECT
public:
    explicit Unu(QObject *parent = 0):SimpleUriShortener(parent){}

    QString serviceName() { return "unu"; }
    QString shortenUri() { return "http://u.nu/unu-api-simple"; }
    QString shortenUriParam() { return "url"; }
    int thresholdLength() { return QString("http://u.nu/xxxx").length(); }
    QString prefix() { return "http://u.nu/"; }
};

#endif // UNU_H
