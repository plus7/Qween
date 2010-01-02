# -------------------------------------------------
# Project created by QtCreator 2009-12-20T20:03:58
# -------------------------------------------------
QT += network \ # webkit \
    xml
TARGET = Qween
TEMPLATE = app
INCLUDEPATH += ./twitlib/include \
    ./shorturi
DEPENDPATH += ./twitlib/include \
    ./shorturi
LIBS += -L./twitlib/debug \
    -lQTwitLib

# Kilroy was here
SOURCES += main.cpp \
    qweenmainwindow.cpp \
    qweensettings.cpp \
    aboutdialog.cpp \
    settingdialog.cpp \
    qweentabctrl.cpp \
    tabinfo.cpp \
    timelinelistview.cpp \
    timelinemodel.cpp \
    timelinetableview.cpp \
    timelineitemdelegate.cpp \
    timelineview.cpp \
    qweeninputbox.cpp \
    shorturi/bitly.cpp \
    iconmanager.cpp \
    qweenapplication.cpp \
    shorturi/urishortensvc.cpp \
    shorturi/simpleurishortener.cpp \
    forwardingrule.cpp \
    shorturi/shorturiresolver.cpp \
    forwardruledialog.cpp \
    tabsettingsdialog.cpp \
    usersmodel.cpp
HEADERS += qweenmainwindow.h \
    twitter.h \
    qweensettings.h \
    aboutdialog.h \
    settingdialog.h \
    qweentabctrl.h \
    tabinfo.h \
    timelinelistview.h \
    const.h \
    timelinemodel.h \
    timelinetableview.h \
    timelineitemdelegate.h \
    timelineview.h \
    qweeninputbox.h \
    shorturi/abstracturishortener.h \
    shorturi/bitly.h \
    shorturi/urishortensvc.h \
    shorturi/tinyurl.h \
    shorturi/isgd.h \
    shorturi/unu.h \
    shorturi/twurl.h \
    iconmanager.h \
    qweenapplication.h \
    shorturi/simpleurishortener.h \
    forwardingrule.h \
    shorturi/shorturiresolver.h \
    forwardruledialog.h \
    tabsettingsdialog.h \
    usersmodel.h
FORMS += qweenmainwindow.ui \
    aboutdialog.ui \
    settingdialog.ui \
    forwardruledialog.ui \
    tabsettingsdialog.ui
OTHER_FILES += memo.txt \
    LICENSE.txt
RESOURCES += res.qrc
