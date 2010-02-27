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

#ifndef QWEENINPUTBOX_H
#define QWEENINPUTBOX_H

#include <QLineEdit>
class AbstractUriShortener;
class QCompleter;
class QweenInputBox : public QLineEdit
{
Q_OBJECT
public:
    explicit QweenInputBox(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event );

    bool requireCtrlOnEnter() const { return m_requireCtrlOnEnter; }
    void setRequireCtrlOnEnter(bool val) { m_requireCtrlOnEnter = val; }

    quint64 replyToId(){ return m_reply_to_id; }
    void setReplyToId(quint64 val){ m_reply_to_id = val; }

    void setCompleter(QCompleter *completer);
    QCompleter* completer() const;
    void setUriShortenSvc(const QString& name);
    QString getUriShortenSvc(){ return m_shortenSvcName; }

    void shortenUri(const QString& svcName = "");
    void doShorten();
signals:
    void uriShorteningFinished();

public slots:
    void OnUriShortened(const QString& src, const QString& dest);
    void OnUriShorteningFailed(const QString& src, int status);
    void insertCompletion(const QString &completion);

private:
    quint64 m_reply_to_id;
    QString m_reply_to_name;
    bool m_requireCtrlOnEnter;
    int m_pos;
    QCompleter *m_completer;
    QString m_shortenSvcName;
    AbstractUriShortener* m_uriShortenSvc;
};

#endif // QWEENINPUTBOX_H
