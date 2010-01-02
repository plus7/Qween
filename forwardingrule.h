#ifndef FORWARDINGRULE_H
#define FORWARDINGRULE_H
#include <QtCore>

class ForwardingRule
{
public:
    ForwardingRule();
    QString name;
    QString exname;
    QString body;
    QString exbody;
    bool useBoth;
    bool exUseBoth;
    bool moveFromRecent;
    bool setMark;
    bool searchUrl;
    bool exSearchUrl;
    bool caseSensitive;
    bool exCaseSensitive;
    bool useRegex;
    bool exUseRegex;
};

#endif // FORWARDINGRULE_H
