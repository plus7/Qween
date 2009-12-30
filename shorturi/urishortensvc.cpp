#include "urishortensvc.h"
#include "bitly.h"
#include "tinyurl.h"
#include "isgd.h"
#include "unu.h"
#include "twurl.h"
AbstractUriShortener *getUriShortenService(const QString& name, QObject *parent)
{
    AbstractUriShortener *p;
    if(name == "bitly"){
        p = new Bitly(parent);
        return p;
    }else if(name == "tinyurl"){
        p = new TinyUrl(parent);
        return p;
    }else if(name == "isgd"){
        p = new Isgd(parent);
        return p;
    }else if(name == "unu"){
        p = new Unu(parent);
        return p;
    }else if(name == "twurl"){
        p = new TwUrl(parent);
        return p;
    }
    else return NULL;
}
