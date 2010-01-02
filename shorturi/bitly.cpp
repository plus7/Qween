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

#include "bitly.h"
#include <QtNetwork>

#define BITLY_API_VER "2.0.1"

//TODO: id & key ‚ÍŠO•”‚©‚çÝ’è‚Å‚«‚é‚æ‚¤‚É‚·‚é
Bitly::Bitly(QObject *parent) :
    AbstractUriShortener(parent), m_id("qweenapi"), m_key("R_c0cb4d3c35a83960b1a473b6ae051ab9")
{
    m_manager = new QNetworkAccessManager( this );
    connect(m_manager,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

void Bitly::shortenAsync(const QString& uri){
    if(QString("http://bit.ly/xxxx").length() > uri.length() &&
       !uri.contains("?") &&
       !uri.contains("#")){
        emit uriShortened(uri, uri);
        return;
    }else if(!uri.startsWith("http")){
        emit failed(uri, 0);
        return;
    }
    if(m_buffer.contains(uri)) return;

    QUrl req("http://api.bit.ly/shorten");
    req.addQueryItem("version", BITLY_API_VER);
    req.addQueryItem("login", m_id);
    req.addQueryItem("apiKey",m_key);
    req.addQueryItem("longUrl",uri);

    //???
    if(m_id=="qweenapi"){
        req.addQueryItem("history","1");
    }

    QNetworkReply *rep = m_manager->post(QNetworkRequest(req),"");
    m_buffer.insert(uri, rep);
}

void Bitly::resolveAsync(const QString& uri){
    if(m_buffer.contains(uri)) return;
}

void Bitly::replyFinished(QNetworkReply *reply){
    QString srcuri = m_buffer.key(reply);
    if(srcuri.isEmpty()) {
        emit failed(srcuri, 1);
        return;
    }
    if(srcuri.startsWith("http://bit.ly/")){
        m_buffer.remove(srcuri);
        QString result = reply->readAll();
        QRegExp rx("\"longUrl\": \"(.*)\"");
        rx.setMinimal(true);
        if(rx.indexIn(result, 0) != -1){
            emit uriResolved(srcuri, rx.cap(1));
        }else{
            emit failed(srcuri, 2);
        }
        reply->deleteLater();
    }else{
        m_buffer.remove(srcuri);
        QString result = reply->readAll();
        QRegExp rx("\"shortUrl\": \"(.*)\"");
        rx.setMinimal(true);
        if(rx.indexIn(result, 0) != -1){
            emit uriShortened(srcuri, rx.cap(1));
        }else{
            emit failed(srcuri, 2);
        }
        reply->deleteLater();
    }
}
