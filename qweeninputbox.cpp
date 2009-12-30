#include "qweeninputbox.h"
#include "urishortensvc.h"
#include "const.h"
#include <QtGui>
QweenInputBox::QweenInputBox(QWidget *parent) :
    QLineEdit(parent), m_pos(0)
{
    //TODO: サービスを設定出来るようにする
    m_uriShortenSvc = getUriShortenService("bitly", this);
    connect(m_uriShortenSvc, SIGNAL(uriShortened(QString,QString)),
            this, SLOT(OnUriShortened(QString,QString)));
    connect(m_uriShortenSvc, SIGNAL(failed(QString,int)),
            this, SLOT(OnUriShorteningFailed(QString,int)));
}

void QweenInputBox::keyPressEvent(QKeyEvent *event)
{
    if(!event->modifiers().testFlag(Qt::ShiftModifier) &&
       !event->modifiers().testFlag(Qt::ControlModifier) &&
       !event->modifiers().testFlag(Qt::AltModifier)){
        if(event->key() == Qt::Key_Space){
            if(text() == " " || text() == "　"){
                setText("");
                return;
                //TODO:JumpUnreadMenuItem_Click(Nothing, Nothing);
            }
        }else if(event->key() == Qt::Key_At){
            //TODO: Start AutoComplete
        }else if(event->key() == Qt::Key_Return){
            if(requireCtrlOnEnter()){
                event->ignore();
            }
        }
    }
    if(!event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->modifiers().testFlag(Qt::ControlModifier) &&
       !event->modifiers().testFlag(Qt::AltModifier)){
        if(event->key() == Qt::Key_Up || event->key() == Qt::Key_Down){
            //TODO:入力履歴を上下する
        }else if(event->key() == Qt::Key_PageUp){
            //TODO:右のタブを選択
        }else if(event->key() == Qt::Key_PageUp){
            //TODO:左のタブを選択
        }else if(event->key() == Qt::Key_Return){
            if(requireCtrlOnEnter()){
                emit returnPressed();
                event->ignore();
            }
        }
    }
    QLineEdit::keyPressEvent(event);
}

void QweenInputBox::doShorten(){
    QRegExp rx(URLRXDATA);
    int begin;
    if((begin=rx.indexIn(text(),m_pos))!=-1){
        m_pos = begin;
        m_uriShortenSvc->shortenAsync(rx.capturedTexts().at(0));
    }else{
        m_pos = 0;
        setEnabled(true);
        emit uriShorteningFinished();
    }
}

void QweenInputBox::shortenUri(){
    this->setEnabled(false);
    doShorten();
}

void QweenInputBox::OnUriShortened(const QString& src, const QString& dest){
    QString newstr = text().mid(0,m_pos) + dest + text().mid(m_pos+src.length(),text().length()-m_pos+src.length());
    m_pos += dest.length();
    setText(newstr);
    doShorten();
}

void QweenInputBox::OnUriShorteningFailed(const QString& src, int status){
    m_pos = 0;
    setEnabled(true);
    emit uriShorteningFinished();
}
