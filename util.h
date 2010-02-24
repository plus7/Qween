#ifndef UTIL_H
#define UTIL_H
#include <QtCore>
class util
{
public:
    util();
    static QString encodeQuery(QList<QPair<QString,QString> > list);
};

class PropertyBag
{
public:
    PropertyBag(){}
    void addQueryItem(const QString& key, const QString& value){
        items.append(QPair<QString,QString>(key,value));
    }
    QList<QPair<QString,QString> > items;
};


#endif // UTIL_H
