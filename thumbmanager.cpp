#include "thumbmanager.h"
#include "qweenapplication.h"

ThumbManager::ThumbManager(QObject *parent) :
    QObject(parent)
{
    m_manager = new QNetworkAccessManager( this );
    connect(m_manager,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

void ThumbManager::fetchThumb(const QString& uri){
    if(m_connections.contains(uri)) return;
    QString fetchUri(getFetchUri(uri));
    if(fetchUri.isEmpty()) return;
    QNetworkReply *rep = m_manager->get(QNetworkRequest(fetchUri));
    m_connections.insert(uri, rep);
}

bool ThumbManager::isThumbAvailable(const QString& uri){
    return QFile(getThumbFilePath(uri)).exists();
}

QString ThumbManager::getThumbFilePath(const QString& uri){
    QDir thumbdir(QweenApplication::profileDir() + "/thumb");
    if(!thumbdir.exists())
        thumbdir.mkpath(thumbdir.path());

    QString fileName;
    QRegExp tprx("http://twitpic.com/([0-9a-z]+)$");
    QRegExp mprx("http://movapic.com/pic/([0-9a-z]+)$");
    QRegExp ytrx("http://www.youtube.com/watch\\?v=([0-9a-zA-Z_]+)");
    QRegExp htrx("http://f.hatena.ne.jp/([0-9a-zA-Z]+)/([0-9]+)$");
    if(tprx.indexIn(uri) != -1){
        fileName = QString("twitpic_%1").arg(tprx.cap(1));
    }else if(mprx.indexIn(uri) != -1){
        fileName = QString("movapic_%1").arg(mprx.cap(1));
    }else if(ytrx.indexIn(uri) != -1){
        fileName = QString("youtube_%1").arg(ytrx.cap(1));
    }else if(htrx.indexIn(uri) != -1){
        fileName = QString("fotolife_%1_%2").arg(htrx.cap(1), htrx.cap(2));
    }else{
        return "";
    }
    return thumbdir.path() + "/" + fileName;
}

QString ThumbManager::getFetchUri(const QString& uri){
    QString fetchUri;
    QRegExp tprx("http://twitpic.com/([0-9a-z]+)$");
    QRegExp mprx("http://movapic.com/pic/([0-9a-z]+)$");
    QRegExp ytrx("http://www.youtube.com/watch\\?v=([0-9a-zA-Z_]+)");
    QRegExp htrx("http://f.hatena.ne.jp/([0-9a-zA-Z]+)/([0-9]+)$");
    if(tprx.indexIn(uri) != -1){
        fetchUri = QString("http://twitpic.com/show/mini/%1").arg(tprx.cap(1));
    }else if(mprx.indexIn(uri) != -1){
        fetchUri = QString("http://image.movapic.com/pic/t_%1.jpeg").arg(mprx.cap(1));
    }else if(ytrx.indexIn(uri) != -1){
        fetchUri = QString("http://i.ytimg.com/vi/%1/default.jpg").arg(ytrx.cap(1));
    }else if(htrx.indexIn(uri) != -1){
        /*
        Page: http://f.hatena.ne.jp/<user-id>/YYYYMMDD<image-id>
        Thumb:http://img.f.hatena.ne.jp/images/fotolife/<user-id>[0]/<user-id>/YYYYMMDD/YYYYMMDD<image-id>_m.jpg
        */
        fetchUri = QString("http://img.f.hatena.ne.jp/images/fotolife/%1/%2/%3/%4_m.jpg")
                   .arg(QString(htrx.cap(1)[0]),
                        htrx.cap(1),
                        htrx.cap(2).left(8), //Y10K :-P
                        htrx.cap(2));
    }else{
        return "";
    }
    return fetchUri;
}

void ThumbManager::replyFinished(QNetworkReply* reply){
    QString uri = m_connections.key(reply);
    m_connections.remove(uri);
    if(uri.isEmpty()) {
        return;
    }
    if(reply->error() != QNetworkReply::NoError){
        reply->deleteLater();
        return;
    }else if(!reply->attribute(QNetworkRequest::RedirectionTargetAttribute).isNull()){
        QUrl newuri(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl());
        QNetworkReply *rep = m_manager->get(QNetworkRequest(newuri));
        m_connections.insert(uri, rep);
        reply->deleteLater();
        return;
    }
    QFile thumbFile(getThumbFilePath(uri));
    thumbFile.open(QFile::WriteOnly);
    thumbFile.write(reply->readAll());
    thumbFile.close();
    emit thumbDownloaded(uri);
    reply->deleteLater();
}
