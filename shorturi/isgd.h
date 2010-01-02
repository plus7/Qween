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
