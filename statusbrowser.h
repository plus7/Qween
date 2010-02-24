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
