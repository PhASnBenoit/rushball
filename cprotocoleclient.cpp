#include "cprotocoleclient.h"

CProtocoleClient::CProtocoleClient(QObject *parent) : QObject(parent)
{

}

bool CProtocoleClient::verifierCrc16()
{
    uint16_t crc16 = static_cast<uint16_t>((_tc[_tc.size()-3]<<8) + _tc[_tc.size()-2]);
    uint16_t crc16Calc = calculerCrc16();
    if (crc16 == crc16Calc)
        return true;
    else
        return false;
}

uint16_t CProtocoleClient::calculerCrc16()
{
    uint8_t nbDec;
    uint8_t yaun;
    uint8_t indice;
    uint16_t crc;

    crc = 0xFFFF;
    indice = 0;

    do {
        crc ^= _tc[indice];
        nbDec = 0;

        do {
            if ( (crc&0x0001) == 1)
                yaun = 1;
            else
                yaun = 0;
            crc >>= 1;
            if (yaun == 1)
              crc ^= 0xA001;
            nbDec++;
        } while (nbDec < 8);
        indice++;
    } while (indice < (_tc.size()-3));

    return crc;
}

void CProtocoleClient::on_trameClient(QByteArray tc)
{
    // examen de la trame
    _tc += tc;  // si la trame n'avait pas été recue complètement.
    //test et purge début trame
    int deb = _tc.indexOf(":",1); // recherche début trame
    if (deb == -1) { // si pas de car de début
        _tc.clear();
        return;
    } // if pas de car de debut
    _tc.remove(0, deb+1);  // on enlève tout avant le : au cas ou

    // test et purge fin de trame
    int fin = _tc.indexOf(":",2);
    if (fin == -1) return; // on attend la suite
    _tc.remove(fin, _tc.size()-fin);  // au cas ou, on enlève tout après la fin

    if (_tc.size() != LG_TRAME) {
        _tc.clear();
        return;
    } // if size pas bon

    bool res = verifierCrc16();
    if (!res) {
        _tc.clear();
        return;
    } // si crc mauvais

    // la trame existe et est bien formée
    switch(_tc.at(1)) {
    case 'P':  // envoi du paramétrage
        emit sig_trameParametrage(_tc);
        break;
    case 'X':   // demande de connexion
        emit sig_trameConnexion(_tc);
        break;
    case 'C':   // annulation de la partie
        emit sig_trameAnnulationPartie(_tc);
        break;
    } //sw

} // method
