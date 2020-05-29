#include "cjeu.h"

CJeu::CJeu(QObject *parent) : QObject(parent)
{
    _zdc = new CZdc();  // accès à la mémoire partagée commune
    _etat = ETAT_ATTENTE_CONNEXION;  // en attente de connexion d'un client

    // pour le moment, qu'un seul client autorisé à se connecter.
    // EVOLUTION 2021 : Plusieurs clients se connectent
    // ne permet qu'au premier client de lancer un paramètrage.
    // les autres clients qui se connecte n'auront qu'un suivi du jeu.
    _serv = new CCommClient();  // mise en route du serveur TCP
    _prot = new CProtocoleClient(this);
    connect(_serv, &CCommClient::sig_trameClient, _prot, &CProtocoleClient::on_trameClient);
    connect(_prot, &CProtocoleClient::sig_emettreVersClient, this, &CJeu::on_emettreVersClient);
    connect(_prot, &CProtocoleClient::sig_trameConnexion, this, &CJeu::on_trameConnexion);
    connect(_prot, &CProtocoleClient::sig_trameParametrage, this, &CJeu::on_trameParametrage);
    connect(_prot, &CProtocoleClient::sig_trameAnnulationPartie, this, &CJeu::on_trameAnnulationPartie);
    connect(_prot, &CProtocoleClient::sig_erreurParams, this, &CJeu::on_erreurParams);

} // méthode

CJeu::~CJeu()
{
    // arrêt serveur TCP
    delete _prot;
    delete _serv;

    // arrêt des threads
    if (_thPans != nullptr) {
        _thPans->quit();
        _thPans->wait();
        delete _thPans;
        delete _pans;
    } // if pans
    if (_thAff != nullptr) {
        _thAff->quit();
        _thAff->wait();
        delete _thAff;
        delete _aff;
    } // if aff
    if (_thPup != nullptr) {
        _thPup->quit();
        _thPup->wait();
        delete _thPup;
        delete _pup;
    } // if pup
    delete _zdc;
} // méthode

void CJeu::play()
{
    // appelé lorsque le paramétrage est correct.
    
    // initialiser l'affichage des joueurs à faire
    //    _aff = new CCommAffichage();

    // init de la comm avec le pupitre
    //    _pup = new CCommPupitre();

    // init thread de communication avec les cibles
    _etat = ETAT_JEU_EN_COURS;
    _pans = new CCommCibles();
    _thPans = new QThread();
    _pans->moveToThread(_thPans);
    connect(_thPans, &QThread::finished, _pans, &QObject::deleteLater);
    connect(this, &CJeu::sig_playCommCibles, _pans, &CCommCibles::on_play);
    connect(_pans, &CCommCibles::sig_ciblesTouchees, this, &CJeu::on_ciblesTouchees);
    _thPans->start();  // lancement du thread
    emit sig_playCommCibles();  // lance la communication I2C
} // méthode

void CJeu::on_ciblesTouchees(QByteArray cibles)
{
    // appelé à la fin d'un cycle de communication I2C avec les panneaux


}

void CJeu::on_emettreVersClient(QByteArray tc)
{
    // préparer la réponse et l'envoyer au client
}

void CJeu::on_trameConnexion(QString login, QString mdp, QString origine)
{
    QByteArray rep;

    // vérification du login + mdp
    if (!_bdd->verifierParamsConnexion(login, mdp)) { // si bon login mdp
        // vérifier si premier connecté mode P ou mode S
    } else {
        rep = _prot->repondreAConnexion('0');
        _serv->repondreAuClient(rep);
        emit sig_erreur("Erreur d'identifiant de connexion."+login +" "+mdp);
        return;
    } // else

    // A FAIRE - mémoriser le type de client origine

    // réponse au client
    if (_etat == ETAT_ATTENTE_CONNEXION)
        rep = _prot->repondreAConnexion('P'); // mode paramétrage
    else
        rep = _prot->repondreAConnexion('S'); // mode suivi

    // gérer mode suivi ou Paramétrage
}

void CJeu::on_trameParametrage(QByteArray tc)
{
    // Paramétrage opérationnel
    play(); // lancement du jeu
}

void CJeu::on_trameAnnulationPartie(QByteArray tc)
{

}

void CJeu::on_erreurParams(QByteArray tc)
{
    qDebug() << "Erreur de décodage de la trame de paramétrage : " << tc;

} // méthode
