#include "cprotocolepanneaux.h"

CProtocolePanneaux::CProtocolePanneaux(QObject *parent) : QObject(parent)
{

}

bool CProtocolePanneaux::verifyReadFrame(uint8_t *tab, uint8_t datalen) //permet de vérifier la lecture de la trame
{
    uint8_t crc, crcp;
    crc=generateCRC(tab, datalen);
    crcp=tab[1];
    if(crc!=crcp)
        return false;
    else
        return true;

}

void CProtocolePanneaux::doWriteFrame(uint8_t *tab, T_COULEURS c1, T_COULEURS c2,T_COULEURS c3 ) //écris la  trame
{
    tab[0]=c1;
    tab[1]=c2;
    tab[2]=c3;
    tab[3]=generateCRC(tab, 3);
}

uint8_t CProtocolePanneaux::generateCRC(uint8_t *data, uint8_t datalen)
{
  // calculates 8-Bit checksum with given polynomial
  uint8_t crc = CRC_INIT;

  for (uint8_t i = 0; i < datalen; i++) {
    crc ^= data[i];
    for (uint8_t b = 0; b < 8; b++) {
      if (crc & 0x80)
        crc = static_cast<uint8_t>((crc << 1) ^ CRC_POLY);
      else
        crc <<= 1;
    }
  }
  return crc;
}
