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
