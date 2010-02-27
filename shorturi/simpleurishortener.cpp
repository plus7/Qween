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

#include "simpleurishortener.h"

//TODO: id & key ‚ÍŠO•”‚©‚çÝ’è‚Å‚«‚é‚æ‚¤‚É‚·‚é
SimpleUriShortener::SimpleUriShortener(QObject *parent) :
    AbstractUriShortener(parent)
{
    m_manager = new QNetworkAccessManager( this );
    connect(m_manager,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

void SimpleUriShortener::shortenAsync(const QString& uri){
    if(thresholdLength() > uri.length() &&
       !uri.contains("?") &&
       !uri.contains("#")){
        emit uriShortened(uri, uri);
        return;
    }else if(!uri.startsWith("http")){
        emit uriShortened(uri, uri);
        return;
    }
    if(m_buffer.contains(uri)) return;

    QUrl req(shortenUri());
    if(!shortenUriParam().isEmpty())
        req.addQueryItem(shortenUriParam(), uri);

    QNetworkReply *rep = m_manager->post(QNetworkRequest(req), postData(uri).toAscii());
    m_buffer.insert(uri, rep);
}

void SimpleUriShortener::resolveAsync(const QString& uri){
    if(m_buffer.contains(uri)) return;
}

void SimpleUriShortener::replyFinished(QNetworkReply *reply){
    QString srcuri = m_buffer.key(reply);
    if(srcuri.isEmpty()) {
        //emit failed(srcuri, 1);
        emit uriShortened(srcuri, srcuri);
        return;
    }
    m_buffer.remove(srcuri);
    if(srcuri.startsWith(prefix())){
        /*QString result = reply->readAll();
        QRegExp rx("\"longUrl\": \"(.*)\"");
        rx.setMinimal(true);
        if(rx.indexIn(result, 0) != -1){
            emit uriResolved(srcuri, rx.cap(1));
        }else{
            emit failed(srcuri, 2);
        }
        reply->deleteLater();*/
    }else{
        QString result = reply->readAll();
        if(result.startsWith(prefix()))
            emit uriShortened(srcuri, result);
        else
            emit uriShortened(srcuri, srcuri);
    }
    reply->deleteLater();
}
