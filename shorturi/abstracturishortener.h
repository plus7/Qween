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

#ifndef ABSTRACTURISHORTENER_H
#define ABSTRACTURISHORTENER_H

#include <QObject>

class AbstractUriShortener : public QObject
{
Q_OBJECT
public:
    explicit AbstractUriShortener(QObject *parent = 0)
        :QObject(parent){ }
    //virtual ~AbstractUriShortener() = 0;

    virtual QString serviceName() = 0;
    //virtual QString shortenSync(const QString& uri);
    virtual void shortenAsync(const QString& uri) = 0;
    //virtual QString resolveSync(const QString& uri);
    virtual void resolveAsync(const QString& uri) = 0;
    /*virtual void setId(const QString& id) { m_id = id; }
    virtual void setPass(const QString& pass) { m_pass = pass; }
    virtual QString id() const { return m_id; }
    virtual QString pass() const { return m_pass; }*/
signals:
    void uriShortened(const QString& src, const QString& dest);
    void uriResolved(const QString& src, const QString& dest);
    void failed(const QString& src, int status);

public slots:
};


#endif // ABSTRACTURISHORTENER_H
