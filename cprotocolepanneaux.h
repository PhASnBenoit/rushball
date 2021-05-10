#ifndef CPROTOCOLEPANNEAUX_H
#define CPROTOCOLEPANNEAUX_H

#include <QObject>
#include "communs.h"

#define CRC_INIT 0xFF
#define CRC_POLY 0x31


class CProtocolePanneaux : public QObject
{
    Q_OBJECT
public:
    explicit CProtocolePanneaux(QObject *parent = nullptr);
    bool verifyReadFrame(uint8_t *tab, uint8_t datalen);
    void doWriteFrame(uint8_t *tab, T_COULEURS c1, T_COULEURS c2,T_COULEURS c3 );
private:
    uint8_t generateCRC(uint8_t *data, uint8_t datalen);

signals:

public slots:
};

#endif // CPROTOCOLEPANNEAUX_H
