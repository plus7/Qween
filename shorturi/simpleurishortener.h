/*
  This file is part of Qween.
  Copyright (C) 2009-2010 NOSE Takafumi <ahya365@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SIMPLEURISHORTENER_H
#define SIMPLEURISHORTENER_H

#include <QObject>
#include "abstracturishortener.h"
#include <QtNetwork>

class SimpleUriShortener : public AbstractUriShortener
{
Q_OBJECT
public:
    explicit SimpleUriShortener(QObject *parent = 0);

    virtual QString serviceName() = 0;
    virtual QString shortenUri() = 0;
    virtual QString shortenUriParam() = 0;
    virtual int thresholdLength() = 0;
    virtual QString prefix() = 0;
    virtual QString postData(const QString& uri) { return ""; }
    virtual void shortenAsync(const QString& uri);
    virtual void resolveAsync(const QString& uri);

signals:
private:
    QMap<QString, QNetworkReply*> m_buffer;
    QNetworkAccessManager* m_manager;
private slots:
    void replyFinished(QNetworkReply*);
};

#endif // SIMPLEURISHORTENER_H
