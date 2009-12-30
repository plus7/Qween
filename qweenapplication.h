#ifndef QWEENAPPLICATION_H
#define QWEENAPPLICATION_H

#include <QApplication>
class IconManager;
class QweenApplication : public QApplication
{
Q_OBJECT
public:
    QweenApplication( int & argc, char ** argv );
    virtual ~QweenApplication();
    void setupDB();
    static QString profileDir();
    //static QSettings *settings();
    static IconManager *iconManager();
private:
    static IconManager *s_iconManager;
};

#endif // QWEENAPPLICATION_H
