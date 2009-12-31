#ifndef TWEETFORWARDER_H
#define TWEETFORWARDER_H

#include <QObject>
#include "twitter.h"

class TweetForwarder : public QObject
{
Q_OBJECT
public:
    explicit TweetForwarder(QObject *parent = 0);

signals:

public slots:

};

#endif // TWEETFORWARDER_H
