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

  In addition, as a special exception, NOSE Takafumi
  gives permission to link the code of its release of Qween with the
  OpenSSL project's "OpenSSL" library (or with modified versions of it
  that use the same license as the "OpenSSL" library), and distribute
  the linked executables.  You must obey the GNU General Public License
  in all respects for all of the code used other than "OpenSSL".  If you
  modify this file, you may extend this exception to your version of the
  file, but you are not obligated to do so.  If you do not wish to do
  so, delete this exception statement from your version.
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
