#ifndef ISGD_H
#define ISGD_H
#include <QObject>
#include "simpleurishortener.h"
class Isgd : public SimpleUriShortener
{
Q_OBJECT
public:
    explicit Isgd(QObject *parent = 0):SimpleUriShortener(parent){}

    QString serviceName() { return "isgd"; }
    QString shortenUri() { return "http://is.gd/api.php"; }
    QString shortenUriParam() { return "longurl"; }
    int thresholdLength() { return QString("http://is.gd/xxxx").length(); }
    QString prefix() { return "http://is.gd/"; }
};
#endif // ISGD_H
