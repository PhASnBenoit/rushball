#ifndef CCOMMCIBLES_H
#define CCOMMCIBLES_H

#include <QObject>

#include "czdc.h"

#include "../biblis/ci2c.h"

class CCommCibles : public QObject
{
    Q_OBJECT
public:
    explicit CCommCibles(QObject *parent = nullptr);
    ~CCommCibles();

private:
    CI2c *_i2c;
    bool _pause;

signals:
    void sig_ciblesTouchees(QByteArray cible);
    void sig_replay();

public slots:
    void on_play();
    void stop();
    void start();
};

#endif // CCOMMCIBLES_H
