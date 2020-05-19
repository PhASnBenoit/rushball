#include "cjeu.h"

CJeu::CJeu(QObject *parent) : QObject(parent)
{
    // pour le moment, qu'un seul client autorisé à se connecter.
    // EVOLUTION 2021 : Plusieurs clients se connectent
    // ne permet qu'au premier client de lancer un paramètrage.
    // les autres clients qui se connecte n'auront qu'un suivi du jeu.
    _serv = new CCommClient();  // mise en route du serveur TCP
    _prot = new CProtocoleClient(this, _serv);

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
} // méthode

void CJeu::play()
{
    _pans = new CCommCibles();
//    _pup = new CCommPupitre();
//    _aff = new CCommAffichage();

    _thPans = new QThread();
    _pans->moveToThread(_thPans);
    connect(_thPans, &QThread::finished, _pans, &QObject::deleteLater);
    connect(this, &CJeu::sig_play, _pans, &CCommCibles::on_play);
    connect(_pans, &CCommCibles::sig_resultReady, this, &CJeu::on_resultReady);
    _thPans->start();  // lancement du thread
    emit sig_play();  // lance la communication I2C
} // méthode

void CJeu::on_resultReady(QByteArray cibles)
{
    // appelé à la fin d'un cycle de communication I2C avec les panneaux

} // méthode
