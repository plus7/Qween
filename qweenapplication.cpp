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

#include "qweenapplication.h"
#include "iconmanager.h"
#include <QDir>

IconManager *s_iconMgr = 0;

QweenApplication::QweenApplication(int & argc, char ** argv)
        : QApplication(argc, argv)
{
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QDir dir(QweenApplication::profileDir() + "/icons");
    if(!dir.exists())
        dir.mkpath(dir.path());
}

QweenApplication::~QweenApplication()
{
//    if(m_settings) {
//        m_settings-
//    }
}

IconManager *QweenApplication::iconManager()
{
    if (!s_iconMgr) {
        s_iconMgr = new IconManager;
    }
    return s_iconMgr;
}

QString QweenApplication::profileDir()
{
    return QDir::homePath() + "/Qween";
}
