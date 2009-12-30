#include "qweensettings.h"

QweenSettings::QweenSettings(QObject *parent) :
    QObject(parent)
{
    load();
}

void QweenSettings::load(){
    QSettings ini(QSettings::IniFormat, QSettings::UserScope, "Qween", "Qween");
    m_inputBgColor = ini.value("input/background",
                               QColor(255,255,0)).value<QColor>();
    m_requireCtrlOnEnter = ini.value("input/require_ctrl", QVariant(false)).toBool();
    m_statusSuffix = ini.value("input/suffix", "").toString();
    m_userid = ini.value("user/id", "").toString();
    m_password = ini.value("user/password", "").toString();
    m_geometry = ini.value("window/geometry", QByteArray()).value<QByteArray>();
    m_winstate = ini.value("window/state", QByteArray()).value<QByteArray>();
    //interval/timeline
    //interval/reply
    //interval/dm
    //startup/recentpages
    //startup/replypages
    //startup/dmpages
    //startup/markread
    //startup/versioncheck
    //startup/wedata? ‚¢‚ç‚È‚¢‚æ‚Ë
    //startup/checkoneway
    //tinyurl/
    m_uriAutoShorten = ini.value("tinyurl/auto_shorten", QVariant(false)).toBool();

}

void QweenSettings::save(){
    QSettings ini(QSettings::IniFormat, QSettings::UserScope, "Qween", "Qween");
    ini.setValue("input/background", m_inputBgColor);
    ini.setValue("input/reqire_ctrl", m_requireCtrlOnEnter);
    ini.setValue("input/suffix", m_statusSuffix);
    ini.setValue("user/id", m_userid);
    ini.setValue("user/password", m_password);
    ini.setValue("window/geometry", m_geometry);
    ini.setValue("window/state", m_winstate);
    ini.setValue("tinyurl/auto_shorten", m_uriAutoShorten);
}

QString QweenSettings::inputStyle(){
    return QString("*:focus{background-color:rgb(%1,%2,%3);}")
            .arg(m_inputBgColor.red())
            .arg(m_inputBgColor.green())
            .arg(m_inputBgColor.blue());
}

QColor QweenSettings::inputBgColor(){
    return m_inputBgColor;
}

QColor QweenSettings::inputColor(){
    return m_inputColor;
}

QString QweenSettings::inputFont(){
    return m_inputFont;
}

void QweenSettings::setInputBgColor(QColor color){
    m_inputBgColor = color;
}

void QweenSettings::setInputColor(QColor color){
    m_inputColor = color;
}

void QweenSettings::setInputFont(QString font){
    m_inputFont = font;
}

QweenSettings* QweenSettings::globalSettings(){
    static QweenSettings *global = 0;
    if(!global){
        global = new QweenSettings();
    }
    return global;
}
