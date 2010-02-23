#ifndef UTIL_H
#define UTIL_H
#include <QtCore>
class util
{
public:
    util();
    static QString encodeQuery(QList<QPair<QString,QString> > list);
};

#endif // UTIL_H
