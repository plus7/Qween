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
