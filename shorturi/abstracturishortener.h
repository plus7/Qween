#ifndef ABSTRACTURISHORTENER_H
#define ABSTRACTURISHORTENER_H

#include <QObject>

class AbstractUriShortener : public QObject
{
Q_OBJECT
public:
    explicit AbstractUriShortener(QObject *parent = 0)
        :QObject(parent){ }
    //virtual ~AbstractUriShortener() = 0;

    virtual QString serviceName() = 0;
    //virtual QString shortenSync(const QString& uri);
    virtual void shortenAsync(const QString& uri) = 0;
    //virtual QString resolveSync(const QString& uri);
    virtual void resolveAsync(const QString& uri) = 0;
    /*virtual void setId(const QString& id) { m_id = id; }
    virtual void setPass(const QString& pass) { m_pass = pass; }
    virtual QString id() const { return m_id; }
    virtual QString pass() const { return m_pass; }*/
signals:
    void uriShortened(const QString& src, const QString& dest);
    void uriResolved(const QString& src, const QString& dest);
    void failed(const QString& src, int status);

public slots:
};


#endif // ABSTRACTURISHORTENER_H
