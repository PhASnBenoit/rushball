#include "cgererclient.h"

CGererClient::CGererClient(QTcpSocket *sock)
{
    _sock = sock;
//    if (!setSocketDescriptor(_desc))
//        emit sig_erreur("CGererClient::CGererClient : Erreur de socket.");
    connect(_sock, &QTcpSocket::readyRead, this, &CGererClient::on_readyRead);

    _prot = new CProtocleClient();
    connect(_prot, &CProtocleClient::sig_connexionAsked, this, &CGererClient::on_connexionAsked);
    connect(_prot, &CProtocleClient::sig_paramsSaved, this, &CGererClient::on_paramsSaved);
    connect(_prot, &CProtocleClient::sig_demandeAnnulationPartie, this, &CGererClient::on_trameAnnulationPartie);
    connect(_prot, &CProtocleClient::sig_erreur, this, &CGererClient::on_erreur);

    _bdd = new CBdd();
    _zdc = new CZdc();
    _typeClient=0;  // client inconnu
}

CGererClient::~CGererClient()
{
    _sock->close();
    //delete _sock;
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

    emit sig_info("CGererClient::on_connexionAsked : Connexion demandée");
    // vérification du login + mdp
    if (_bdd->verifierParamsConnexion(login, mdp)) { // si bon login mdp
        // vérifier si premier connecté mode P ou mode S
        emit sig_info("CGererClient::on_connexionAsked : Bon login/mdp");
    } else {
        rep = _prot->repondreAConnexion('0');
        repondreAuClient(rep);
        emit sig_erreur("Erreur d'identifiant de connexion."+login +" "+mdp);
        return;
    } // else

    // A FAIRE - mémoriser le type de client origine
    _typeClient = origine.at(0).toLatin1();

    // réponse au client
    if (_zdc->etatJeu() == ETAT_ATTENTE_CONNEXION) {
        rep = _prot->repondreAConnexion('P'); // mode paramétrage
    } else {
        rep = _prot->repondreAConnexion('S'); // mode suivi
    } // else
    repondreAuClient(rep);
} // method

void CGererClient::on_paramsSaved()
{
    QByteArray rep;

    // réponse au client
    if (_zdc->etatJeu() == ETAT_ATTENTE_CONNEXION) {
        rep = _prot->repondreAConnexion('1'); // accepté
    } else {
        rep = _prot->repondreAConnexion('0'); // Pas possible
    } // else
    repondreAuClient(rep);
    emit sig_play();  // lance le jeu relayé par CServeurTcp
} // méthode

void CGererClient::on_trameAnnulationPartie(QByteArray tc)
{
    qDebug() << tc;
}

void CGererClient::on_erreur(QString mess)
{
    emit sig_erreur(mess);
}

void CGererClient::on_info(QString mess)
{
    emit sig_info(mess);
}

void CGererClient::repondreAuClient(QByteArray rep)
{
    _sock->write(rep);
}
