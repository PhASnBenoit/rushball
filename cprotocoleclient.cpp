#include "cprotocoleclient.h"

CProtocleClient::CProtocleClient(QObject *parent) : QObject(parent)
{
    _ds = new T_DATAS_STATIC;
}

CProtocleClient::~CProtocleClient()
{
    delete _ds;
}

QByteArray CProtocleClient::repondreAConnexion(char mode)
{
    // cette méthode offre la possibilité de former une trame de réponse plus complexe.
    // si le protocole change
    _rep[0] = mode;
    return _rep;  // RAS
}

QByteArray CProtocleClient::preparerTrameMajScores(uint8_t nbJoueurs, QList<QString> nomJoueurs, QList<uint16_t> scores)
{
    uint16_t crc;

    _tc.clear();
    _tc.append(':');
    _tc.append('S');
    _tc.append('|');
    for (int i=0 ; i<nbJoueurs ; i++) {
        _tc.append(13-nomJoueurs.at(i).trimmed().size(), ' '); // met les espaces en premier 13 car maxi
        _tc.append(nomJoueurs.at(i));
        _tc.append(':');
        _tc.append(QString::number(scores.at(i)));
    } // for
    _tc.append('|');
    _tc.append('x');  // à calculer
    _tc.append('x');  // à calculer
    _tc.append(':'); // fin de trame
    crc = calculerCrc16();
    _tc[_tc.size()-3] = static_cast<char>(crc>>8);  // mise en place du CRC16 PF
    _tc[_tc.size()-2] = static_cast<char>(crc);  // pf
    return _tc;
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

int CProtocleClient::decodeEtControleParams()
{
    // appelé lors de la réception du paramétrage
    // décoder la trame de paramétrage et la sauver dans la zdc
    QList<QByteArray> groupes;
    groupes = _tc.split('|');
    QList<QByteArray> params;

    bzero(_ds, sizeof (T_DATAS_STATIC));
    _ds->nbJoueurs = static_cast<uint8_t>(groupes.at(1).at(0)-0x30);  // nombre de joueurs
    if ( (_ds->nbJoueurs < 1) || (_ds->nbJoueurs > 4)) {
        _ds->nbJoueurs=1;
        return -1; // erreur !
    } // if nbjoueur

    params = groupes.at(2).split(';');
    for (int i=0 ; i<_ds->nbJoueurs ; i++)   // noms des joueurs
        strcpy(_ds->nomJoueurs[i],params.at(i).toStdString().c_str());

    params.clear();
    params = groupes.at(3).split(';');
    _ds->modeJeu = params.at(0)[0]-0x30;  // mode de jeu
    _ds->modeFinJeu = params.at(1)[0]-0x30;  // mode fin de jeu
    _ds->cpt = static_cast<uint16_t>(params.at(2).toUInt());  // score ou temps de départ

    _ds->nbPointsFaute = static_cast<uint8_t>(groupes.at(4).toUInt());  // nb points pour faute

    params.clear();
    params = groupes.at(5).split(';');
    _ds->nbPanneaux = static_cast<uint8_t>(params.at(0)[0]-0x30);  // nb de panneau
    _ds->nbCiblesOn = static_cast<uint8_t>(params.at(1).toUInt());  // nb cibles allumées

    params.clear();
    params = groupes.at(6).split(';');
    _ds->joker = static_cast<uint8_t>(params.at(0)[0]-0x30);  // présence joker
    _ds->nbPointsJoker = static_cast<uint8_t>(params.at(1).toUInt());  // nb points joker

    _ds->nbCouleurs = static_cast<uint8_t>(groupes.at(7).toUInt());  // nombre de couleurs utilisées

    params.clear();
    params = groupes.at(8).split(';');
    for (int i=0 ; i<_ds->nbCouleurs ; i+=2)   // point pour chaque couleur utilisée
        _ds->nbPointsParCouleur[params.at(i).toInt()] = static_cast<uint8_t>(params.at(i+1).toUInt());


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
        if (!decodeEtControleParams()) { // si RAS
            emit sig_newParamsReady(_ds);
        } else {
            emit sig_erreur(QString(_tc));
        } // else decode
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
        emit sig_annulationPartieAsked(_tc);
        break;
    default:
        return -1;
    } //sw
    return _tc.at(0);
} // method
