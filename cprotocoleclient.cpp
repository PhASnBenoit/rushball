#include "cprotocoleclient.h"

CProtocleClient::CProtocleClient(QObject *parent) : QObject(parent)
{
    _zdc = new CZdc();
}

CProtocleClient::~CProtocleClient()
{
    delete _zdc;
}

QByteArray CProtocleClient::repondreAConnexion(char mode)
{
    // cette méthode offre la possibilité de former une trame de réponse plus complexe.
    // si le protocole change
    _rep[0] = mode;
    return _rep;  // RAS
}

bool CProtocleClient::verifierCrc16()
{
    uint16_t crc16 = static_cast<uint16_t>((_tc[_tc.size()-3]<<8) + _tc[_tc.size()-2]);
    uint16_t crc16Calc = calculerCrc16();
    if (crc16 == crc16Calc)
        return true;
    else
        return true; // pour ne pas avoir à calculer le CRC16
}

uint16_t CProtocleClient::calculerCrc16()
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

int CProtocleClient::decodeEtSauveParams()
{
    // appelé lors de la réception du paramétrage
    // décoder la trame de paramétrage et la sauver dans la zdc
    QList<QByteArray> groupes;
    groupes = _tc.split('|');
    QList<QByteArray> params;

    _zdc->clear(); // réinitialise la zone mémoire
    _zdc->_adrZdc->datasStatic.nbreJoueurs = static_cast<uint8_t>(groupes.at(0).at(3)-0x30);  // nombre de joueurs
    if ( (_zdc->_adrZdc->datasStatic.nbreJoueurs < 1) || (_zdc->_adrZdc->datasStatic.nbreJoueurs > 4)) {
        _zdc->_adrZdc->datasStatic.nbreJoueurs=1;
        return -1; // erreur !
    } // if nbjoueur
    // contrôler nombre de joueurs

    params = groupes.at(1).split(';');
    for (int i=0 ; i<_zdc->_adrZdc->datasStatic.nbreJoueurs ; i++)   // noms des joueurs
        _zdc->_adrZdc->datasStatic.nomJoueurs[i] = params.at(i);

    params.clear();
    params = groupes.at(2).split(';');
    _zdc->_adrZdc->datasStatic.modeJeu = params.at(0)[0]-0x30;  // mode de jeu
    _zdc->_adrZdc->datasStatic.modeFinJeu = params.at(1)[0]-0x30;  // mode fin de jeu
    _zdc->_adrZdc->datasStatic.cpt = static_cast<uint16_t>(params.at(2).toUInt());  // score ou temps de départ

    _zdc->_adrZdc->datasStatic.nbPointsFaute = static_cast<uint8_t>(groupes.at(3).toUInt());  // nb points pour faute

    params.clear();
    params = groupes.at(4).split(';');
    _zdc->_adrZdc->datasStatic.nbPanneaux = static_cast<uint8_t>(params.at(0)[0]-0x30);  // nb de panneau
    _zdc->_adrZdc->datasStatic.nbCiblesOn = static_cast<uint8_t>(params.at(1).toUInt());  // nb cibles allumées

    params.clear();
    params = groupes.at(5).split(';');
    _zdc->_adrZdc->datasStatic.joker = static_cast<uint8_t>(params.at(0)[0]-0x30);  // présence joker
    _zdc->_adrZdc->datasStatic.nbPointsJoker = static_cast<uint8_t>(params.at(1).toUInt());  // nb points joker

    _zdc->_adrZdc->datasStatic.nbCouleurs = static_cast<uint8_t>(groupes.at(6).toUInt());  // nombre de couleurs utilisées

    params.clear();
    params = groupes.at(7).split(';');
    for (int i=0 ; i<_zdc->_adrZdc->datasStatic.nbCouleurs ; i+=2)   // point pour chaque couleur utilisée
        _zdc->_adrZdc->datasStatic.nbPointscouleurs[params.at(i).toInt()] = static_cast<uint8_t>(params.at(i+1).toUInt());

    return 0; // RAS
}

int CProtocleClient::decodeLoginMdp(QString &login, QString &mdp, QString &origine)
{
    // appelé lors de la demande de connexion
    // décodage de la trame
    QList<QByteArray> groupes;
    groupes = _tc.split('|');
    QList<QByteArray> params;

    params = groupes.at(1).split(';'); // 3 params
    origine = QString(params.at(0));
    if ( (origine!="M") && (origine!="P"))
        return -1;
    login = QString(params.at(1)).trimmed();
    mdp = QString(params.at(2)).trimmed();
    return 0;
}

char CProtocleClient::on_trameClient(QByteArray tc)
{
    // examen de la trame reçue

    _tc += tc;  // si la trame n'avait pas été recue complètement.

    //test et purge début trame
    int deb = _tc.indexOf(":",0); // recherche début trame
    if (deb == -1) { // si pas de car de début
        _tc.clear();
        emit sig_erreur("CProtocleClient::on_trameClient : Pas de caractère de début de trame");
        return -1;
    } // if pas de car de debut
    _tc.remove(0, deb+1);  // on enlève tout avant le : au cas ou

    // test et purge fin de trame
    int fin = _tc.indexOf(":",1);
    if (fin == -1) return 0; // on attend la suite
    _tc.remove(fin, _tc.size()-fin);  // au cas ou, on enlève tout après la fin

    bool res = verifierCrc16();
    if (!res) {
        _tc.clear();
        emit sig_erreur("CProtocleClient::on_trameClient : Erreur de CRC16");
        return -1;
    } // si crc mauvais

    // la trame existe et est bien formée
    QString login, mdp, origine;
    switch(_tc.at(0)) {
    case 'P':  // envoi du paramétrage
        if (_tc.size() != LG_TRAME_PARAMS) {
            _tc.clear();
            return -1;
        } // if size pas bon
        if (_zdc->etatJeu() == ETAT_ATTENTE_CONNEXION) {
            if (!decodeEtSauveParams()) // si RAS
                emit sig_paramsSaved(_tc);
            else
                emit sig_erreur(QString(_tc));
        } // if etat
        break;
    case 'X':   // demande de connexion
        if (_tc.size() != LG_TRAME_CONNEXION) {
            _tc.clear();
            emit sig_erreur("CProtocleClient::on_trameClient : Trame X de longueur non conforme");
            return -1;
        } // if size pas bon
        if (!decodeLoginMdp(login, mdp, origine))
            emit sig_connexionAsked(login, mdp, origine);
        else
            emit sig_erreur(QString(_tc));
        break;
    case 'C':   // annulation de la partie
        if (_tc.size() != LG_TRAME_ANNULATION_PARTIE) {
            _tc.clear();
            return -1;
        } // if size pas bon
        emit sig_demandeAnnulationPartie(_tc);
        break;
    default:
        return -1;
    } //sw
    return _tc.at(0);
} // method
