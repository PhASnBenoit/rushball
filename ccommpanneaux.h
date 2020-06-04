#ifndef CCOMMCIBLES_H
#define CCOMMCIBLES_H

#include <QObject>

#include "czdc.h"
#include "../biblis/ci2c.h"

class CCommPanneaux : public QObject
{
    Q_OBJECT
public:
    explicit CCommPanneaux(QObject *parent = nullptr);
    ~CCommPanneaux();

private:
    CI2c *_i2c;
    bool _pause;
    CZdc *_zdc;

signals:
    void sig_finCycleCommPanneaux(QByteArray cible);
    void sig_cibleTouchee();
    void sig_replay();

public slots:
    void on_playCommCibles();
    void stop();
    void start();
};

#endif // CCOMMCIBLES_H
