#include "cjeu.h"

CJeu::CJeu(QObject *parent) : QObject(parent)
{
    _zdc = new CZdc();  // accès à la mémoire partagée commune

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
    // initialiser l'écran à faire


    _pans = new CCommCibles();
//    _pup = new CCommPupitre();
//    _aff = new CCommAffichage();

    _thPans = new QThread();
    _pans->moveToThread(_thPans);
    connect(_thPans, &QThread::finished, _pans, &QObject::deleteLater);
    connect(this, &CJeu::sig_play, _pans, &CCommCibles::on_play);
    connect(_pans, &CCommCibles::sig_ciblesTouchees, this, &CJeu::on_ciblesTouchees);
    _thPans->start();  // lancement du thread
    emit sig_play();  // lance la communication I2C
} // méthode

void CJeu::on_ciblesTouchees(QByteArray cibles)
{
    // appelé à la fin d'un cycle de communication I2C avec les panneaux


}

void CJeu::on_emettreVersClient(QByteArray tc)
{
    // préparer la réponse et l'envoyer au client
}

void CJeu::on_trameConnexion(QByteArray tc)
{
    // vérifie si première connexion pour autorisation paramétrage
    // vérification du login + mdp
    // réponse au client
    // gérer mode suivi ou Paramétrage
}

void CJeu::on_trameParametrage(QByteArray tc)
{
    // décoder la trame de paramétrage et la sauver dans la zdc
    _zdc->_adrZdc->datasStatic.nbreJoueurs = static_cast<uint8_t>(tc.at(3)-0x30);

    QList<QByteArray> params;
    params = tc.split('|');
    QList<QByteArray> joueurs;
    joueurs = params.at(1).split(';');
    for (int i=0 ; i<_zdc->_adrZdc->datasStatic.nbreJoueurs ; i++)
        _zdc->_adrZdc->datasStatic.nomJoueurs[i] = joueurs.at(i);



    play(); // lancement du jeu
}

void CJeu::on_trameAnnulationPartie(QByteArray tc)
{

} // méthode
