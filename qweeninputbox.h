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

    void shortenUri();
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
    AbstractUriShortener* m_uriShortenSvc;
    int m_pos;
    QCompleter *m_completer;
};

#endif // QWEENINPUTBOX_H
