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

#ifndef QWEENAPPLICATION_H
#define QWEENAPPLICATION_H

#include <QApplication>
class IconManager;
class QweenApplication : public QApplication
{
Q_OBJECT
public:
    QweenApplication( int & argc, char ** argv );
    virtual ~QweenApplication();
    void setupDB();
    static QString profileDir();
    //static QSettings *settings();
    static IconManager *iconManager();
private:
    static IconManager *s_iconManager;
};

#endif // QWEENAPPLICATION_H
