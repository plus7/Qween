#ifndef URISHORTENSVC_H
#define URISHORTENSVC_H

#include "abstracturishortener.h"

AbstractUriShortener *getUriShortenService(const QString& name, QObject *parent=0);

#endif // URISHORTENSVC_H
