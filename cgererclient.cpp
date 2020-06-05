#include "cgererclient.h"

CGererClient::CGererClient(QTcpSocket *sock)
{
    _sock = sock;
//    if (!setSocketDescriptor(_desc))
//        emit sig_erreur("CGererClient::CGererClient : Erreur de socket.");
    connect(_sock, &QTcpSocket::readyRead, this, &CGererClient::on_readyRead);

    _prot = new CProtocleClient();
    connect(_prot, &CProtocleClient::sig_connexionAsked, this, &CGererClient::on_connexionAsked);
    connect(_prot, &CProtocleClient::sig_newParamsReady, this, &CGererClient::on_newParamsReady);
    connect(_prot, &CProtocleClient::sig_annulationPartieAsked, this, &CGererClient::on_annulationPartieAsked);
    connect(_prot, &CProtocleClient::sig_erreur, this, &CGererClient::on_erreur);

    _bdd = new CBdd();
    _zdc = new CZdc();
    _typeClient=0;  // client inconnu
    _etatClient = ETAT_CLIENT_CONNECTED;
}

CGererClient::~CGererClient()
{
    _sock->close();
    delete _sock;
    delete _zdc;
    delete _bdd;
    delete _prot;
}

bool CGererClient::isConnected()
{
    return _sock->state();
}

void CGererClient::on_readyRead()
{
    // réception de données d'un client
    // pour 2020 un seul client possible.
    // 2021 plusieurs client.
    QByteArray tc;  // trame reçu du client
    tc = _sock->readAll();
    //emit sig_trameClient(tc);
    char commande = _prot->on_trameClient(tc);  // vérification et décodage de la trame
    // commande = X,P,C, -1 si mal passé, 0 si trame non complète
    // _prot->on_trameClient envoie un signal correspondant à la trame reçue
    if (commande == -1) // si erreur dans trame
        emit sig_erreur("CGererClient::on_readyRead : Erreur dans le format de la trame.");
    emit sig_info("Commande "+QString(commande)+" bien reçue");
}

void CGererClient::on_connexionAsked(QString login, QString mdp, QString origine)
{
    QByteArray rep;

    rep = _prot->repondreAConnexion('0');  // par défaut pas possible
    if ( !(_etatClient|ETAT_CLIENT_CONNECTED)) {  // si client pas encore connecté
        emit sig_info("CGererClient::on_connexionAsked : Connexion demandée");
        // vérification du login + mdp
        if (_bdd->verifierParamsConnexion(login, mdp)) { // si bon login mdp
            // vérifier si premier connecté mode P ou mode S
            emit sig_info("CGererClient::on_connexionAsked : Bon login/mdp");
        } else {
            rep = _prot->repondreAConnexion('0');
            envoyerAuClient(rep);
            emit sig_erreur("Erreur d'identifiant de connexion."+login +" "+mdp);
            return;
        } // else

        // A FAIRE - mémoriser le type de client origine
        _typeClient = origine.at(0).toLatin1();

        // réponse au client
        if (_zdc->etatJeu() == ETAT_JEU_ATTENTE_CONNEXION) {
            _etatClient = ETAT_CLIENT_PREMIER | ETAT_CLIENT_AUTHENTIFIED;
            rep = _prot->repondreAConnexion('P'); // mode paramétrage
        } else {
            _etatClient = ETAT_CLIENT_AUTHENTIFIED;
            rep = _prot->repondreAConnexion('S'); // mode suivi
        } // else
    } // if si client pas connecté
    envoyerAuClient(rep);
} // method

void CGererClient::on_newParamsReady(T_DATAS_STATIC *ds)
{
    QByteArray rep;

    // réponse au client
    if ( (_zdc->etatJeu()==ETAT_JEU_ATTENTE_PARAMS)) {
        _zdc->appliquerNewParams(ds);
        _etatClient |= ETAT_CLIENT_PARAMETRED;
        _zdc->setEtatJeu(ETAT_JEU_EN_COURS);
        rep = _prot->repondreAConnexion('1'); // accepté
        emit sig_play();  // lance le jeu relayé par CServeurTcp
    } else {
        rep = _prot->repondreAConnexion('0'); // Pas possible
    } // else
    envoyerAuClient(rep);
} // méthode

void CGererClient::on_annulationPartieAsked()
{
    QByteArray rep;
    rep = _prot->repondreAConnexion('1'); // accepté
    envoyerAuClient(rep);
    emit sig_annulationPartie();
}

void CGererClient::on_erreur(QString mess)
{
    emit sig_erreur(mess);
}

void CGererClient::on_info(QString mess)
{
    emit sig_info(mess);
}

void CGererClient::on_majScores()
{
    QByteArray req;
    QList<QString> nomJoueurs;
    QList<uint16_t> scores;

    if (_zdc->etatJeu() == ETAT_JEU_EN_COURS) {
        nomJoueurs = _zdc->getNomJoueurs();
        scores = _zdc->getScores();
        req = _prot->preparerTrameMajScores(_zdc->getNbJoueurs(), nomJoueurs, scores);
        // A FAIRE ENVOYER AU CLIENT
        envoyerAuClient(req);
    } // if etat
}

void CGererClient::envoyerAuClient(QByteArray rep)
{
    _sock->write(rep);
}
