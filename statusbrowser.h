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

#ifndef STATUSBROWSER_H
#define STATUSBROWSER_H

#include <QTextBrowser>

class StatusBrowser : public QTextBrowser
{
Q_OBJECT
public:
    explicit StatusBrowser(QWidget *parent = 0);
    void contextMenuEvent(QContextMenuEvent *event);
signals:

    void followCommand(const QString& name);
    void removeCommand(const QString& name);
    void friendshipCommand(const QString& name);

public slots:
    void followClicked();
    void removeClicked();
    void friendshipClicked();

private:
    QString m_selectedName;
};

#endif // STATUSBROWSER_H
