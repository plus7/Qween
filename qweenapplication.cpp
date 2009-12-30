#include "qweenapplication.h"
#include "iconmanager.h"
#include <QDir>

IconManager *s_iconMgr = 0;

QweenApplication::QweenApplication(int & argc, char ** argv)
        : QApplication(argc, argv)
{
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QDir dir(QweenApplication::profileDir() + "/icons");
    if(!dir.exists())
        dir.mkpath(dir.path());
}

QweenApplication::~QweenApplication()
{
//    if(m_settings) {
//        m_settings-
//    }
}

IconManager *QweenApplication::iconManager()
{
    if (!s_iconMgr) {
        s_iconMgr = new IconManager;
    }
    return s_iconMgr;
}

QString QweenApplication::profileDir()
{
    return QDir::homePath() + "/Qween";
}
