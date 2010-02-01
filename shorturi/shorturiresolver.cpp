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

#include "shorturiresolver.h"
#include <QtNetwork>
ShortUriResolver::ShortUriResolver(QObject *parent) :
    QObject(parent)
{
    QNetworkAccessManager *m_manager = new QNetworkAccessManager(this);
    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

void ShortUriResolver::replyFinished(QNetworkReply* reply){
    QString srcuri = m_buffer.key(reply);
    if(srcuri.isEmpty()) {
        emit uriResolved(srcuri, srcuri);
        reply->deleteLater();
        return;
    }
    m_buffer.remove(srcuri);
    QVariant redirUriVar = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    QUrl redirUri = redirUriVar.toUrl();
    if(redirUri.isEmpty()){
        emit uriResolved(srcuri, srcuri);
    }else{
        if(srcuri == redirUri.toString()){
            emit uriResolved(srcuri, srcuri);
        }else{
            emit uriResolved(srcuri, redirUri.toString());
        }
    }
    reply->deleteLater();
}


void ShortUriResolver::resolveAsync(const QString& uri){
    if(m_buffer.contains(uri)) return;
    QNetworkRequest request;
    request.setUrl(QUrl(uri));
    QNetworkReply *rep = m_manager->head(request);
    m_buffer.insert(uri, rep);
}
