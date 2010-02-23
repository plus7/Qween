#include "util.h"
#include <QtNetwork>

util::util()
{
}

QString util::encodeQuery(QList<QPair<QString,QString> > list){
    QStringList rv;
    QPair<QString,QString> pair;
    foreach(pair, list){
        rv << QUrl::toPercentEncoding(pair.first) + "=" + QUrl::toPercentEncoding(pair.second);
    }
    return rv.join("&");
}
