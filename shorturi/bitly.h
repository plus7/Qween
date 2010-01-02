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

#ifndef BITLY_H
#define BITLY_H

#include <QObject>
#include "abstracturishortener.h"
#include <QtNetwork>

class Bitly : public AbstractUriShortener
{
Q_OBJECT
public:
    explicit Bitly(QObject *parent = 0);

    QString serviceName() { return "bitly"; }
    void shortenAsync(const QString& uri);
    void resolveAsync(const QString& uri);

signals:
private:
    QMap<QString, QNetworkReply*> m_buffer;
    QNetworkAccessManager* m_manager;
    QString m_id;
    QString m_key;
private slots:
    void replyFinished(QNetworkReply*);
};

#endif // BITLY_H
