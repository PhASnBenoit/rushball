#ifndef CCOMMPANNEAUX_H
#define CCOMMPANNEAUX_H

#include <QObject>

#include "ci2c.h"
#include "cprotocolepanneaux.h"
#include "czdc.h"


class CCommPanneaux : public QObject
{
    Q_OBJECT
public:
    explicit CCommPanneaux(QObject *parent = nullptr);
    ~CCommPanneaux();

private:
    CI2c *_i2c;
    CProtocolePanneaux *_prot;
    bool _pause;
    CZdc *_zdc;
signals:
    void sig_finCycleCommPanneaux();
    void sig_cibleTouchee(uint8_t noPan,uint8_t cibles);
    void sig_replay();
    void sig_info(QString mess);

public slots:
    void on_playCommCibles();
    void stop();
    void start();
};

#endif // CCOMMPANNEAUX_H
