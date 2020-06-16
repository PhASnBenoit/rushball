#include "cgererclient.h"

CGererClient::CGererClient(QTcpSocket *sock)
{
    _sock = sock;
    connect(_sock, &QTcpSocket::readyRead, this, &CGererClient::on_readyRead);

    _prot = new CProtocleClient();
    connect(_prot, &CProtocleClient::sig_connexionAsked, this, &CGererClient::on_connexionAsked);
    connect(_prot, &CProtocleClient::sig_newParamsReady, this, &CGererClient::on_newParamsReady);
    connect(_prot, &CProtocleClient::sig_annulationPartieAsked, this, &CGererClient::on_annulationPartieAsked);
    connect(_prot, &CProtocleClient::sig_erreur, this, &CGererClient::on_erreur);

    _bdd = new CBdd();
    _zdc = new CZdc();
    _typeClient=0;  // client inconnu
    _etatClient = ETAT_CLIENT_CONNECTED; // Non authentifié
}

CGererClient::~CGererClient()
{
    if (_sock->isOpen()) {
        _sock->close();
        delete _sock;
    } // if open
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
    if (_etatClient == ETAT_CLIENT_CONNECTED) {  // si client pas encore authentifié
        emit sig_info("CGererClient::on_connexionAsked : Connexion demandée");
        if (_bdd->verifierParamsConnexion(login, mdp)) { // si bon login mdp
            emit sig_info("CGererClient::on_connexionAsked : Bon login/mdp");
        } else {
            rep = _prot->repondreAConnexion('0');
            envoyerAuClient(rep);
            emit sig_erreur("CGererClient::on_connexionAsked Erreur d'identifiant de connexion."+login +" "+mdp);
            return;
        } // else

        // A FAIRE - mémoriser le type de client origine
        _typeClient = origine.at(0).toLatin1();

        // réponse au client si premier ou non
        if (_zdc->etatJeu() == ETAT_JEU_ATTENTE_CONNEXION) {
            _etatClient = ETAT_CLIENT_AUTHENTIFIED|ETAT_CLIENT_PREMIER;
            emit sig_trameConnexionValidated();  // vers CJeu via CServeurTcp
            //_zdc->setEtatJeu(ETAT_JEU_ATTENTE_PARAMS); // entorse, ce devrait être dans CJeu.
            rep = _prot->repondreAConnexion('P'); // mode paramétrage
            emit sig_info("CGererClient::on_connexionAsked Vous êtes premier !");
        } else {
            _etatClient = ETAT_CLIENT_AUTHENTIFIED;
            rep = _prot->repondreAConnexion('S'); // mode suivi
        } // else
        envoyerAuClient(rep);
    } // if si client pas connecté
} // method

void CGererClient::on_newParamsReady(T_DATAS_STATIC *ds)
{
    // Paramètres bien reçus
    QByteArray rep;

    if ( (_zdc->etatJeu()==ETAT_JEU_ATTENTE_PARAMS) && (_etatClient&ETAT_CLIENT_PREMIER)) {
        _zdc->appliquerNewParams(ds);
        _etatClient &= ~ETAT_CLIENT_AUTHENTIFIED;
        _etatClient |= ETAT_CLIENT_PARAMETRED;
        emit sig_play();  // lance le jeu relayé par CServeurTcp
    }
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

void CGererClient::on_majScores(uint8_t aQuiLeTour)
{
    QByteArray req;
    QList<QString> nomJoueurs;
    QList<uint16_t> scores;

    if (_zdc->etatJeu() == ETAT_JEU_EN_COURS) {
        nomJoueurs = _zdc->getNomJoueurs();
// A FAIRE idée : Mettre le bit de poids fort à 1 pour signifier a qui vient le tour
        scores = _zdc->getScores();
        req = _prot->preparerTrameMajScores(_zdc->getNbJoueurs(), nomJoueurs, scores, aQuiLeTour);
        envoyerAuClient(req);
    } // if etat
}

void CGererClient::envoyerAuClient(QByteArray rep)
{
    _sock->write(rep);
}
