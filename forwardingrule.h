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
