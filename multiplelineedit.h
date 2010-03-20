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

#ifndef MULTIPLELINEEDIT_H
#define MULTIPLELINEEDIT_H

#include <QPlainTextEdit>

class MultipleLineEdit : public QPlainTextEdit
{
Q_OBJECT
public:
    explicit MultipleLineEdit(QWidget *parent = 0);

    void setMultipleLine(bool val);
    bool multipleLine();
protected:
    virtual void keyPressEvent(QKeyEvent *event);
    void insertFromMimeData ( const QMimeData * source );
signals:
    void returnPressed();
public slots:
private:
    bool m_multipleLine;
};

#endif // MULTIPLELINEEDIT_H
