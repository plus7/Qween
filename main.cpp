#include <QtGui/QApplication>
#include <QTextCodec>
#include "qweenapplication.h"
#include "qweenmainwindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("Qween");
    QweenApplication a(argc, argv);
    QTextCodec* codec = QTextCodec::codecForName("Shift-JIS");
    QTextCodec::setCodecForTr(codec);
    QweenMainWindow w;
    w.show();
    return a.exec();
}
