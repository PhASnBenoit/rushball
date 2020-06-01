#include "cjeu.h"

CJeu::CJeu(QObject *parent) : QObject(parent)
{
    _zdc = new CZdc();  // accès à la mémoire partagée commune
    _zdc->setEtatJeu(ETAT_ATTENTE_CONNEXION);  // en attente de connexion d'un client

    // pour le moment, qu'un seul client autorisé à se connecter.
    // EVOLUTION 2021 : Plusieurs clients se connectent
    // ne permet qu'au premier client de lancer un paramétrage.
    // les autres clients qui se connecte n'auront qu'un suivi du jeu.
    _serv = new CServeurTcp();  // mise en route du serveur TCP
    connect(_serv, &CServeurTcp::sig_newConnection, this, &CJeu::on_newConnection);
    connect(_serv, &CServeurTcp::sig_erreur, this, &CJeu::on_erreur);
    connect(_serv, &CServeurTcp::sig_disconnected, this, &CJeu::on_disconnected);
    connect(_serv, &CServeurTcp::sig_play, this, &CJeu::on_play);
    emit sig_info("CJeu:CJeu : Services ZDC, Serveur TCP activés.");
} // méthode

CJeu::~CJeu()
{
    // arrêt serveur TCP
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
    
    emit sig_info("CJeu::play : init du bandeau d'affichage.");
    // initialiser l'affichage des joueurs à faire
    //    _aff = new CCommAffichage();

    emit sig_info("CJeu::play : init de la comm avec pupitre.");
    // init de la comm avec le pupitre
    //    _pup = new CCommPupitre();

    emit sig_info("CJeu::play : Lancement thread de comm avec les cibles.");
    /*
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
    */
    emit sig_info("CJeu::play : comm avec les cibles en cours.");
} // méthode

void CJeu::on_ciblesTouchees(QByteArray cibles)
{
    // appelé à la fin d'un cycle de communication I2C avec les panneaux
    qDebug() << cibles;
}

void CJeu::on_newConnection()
{
    emit sig_info("CJeu::on_newConnection : Un client vient de se connecter.");
}

void CJeu::on_disconnected()
{
    emit sig_info("CJeu::on_disconnected : Un client vien de se déconnecter");
}

void CJeu::on_play()
{
    emit sig_info("CJeu::on_play : Lancement jeu, params correctes");
    play(); // lancement du jeu
}

void CJeu::on_erreur(QString mess)
{
    emit sig_erreur(mess);  // remontée des erreurs à l'IHM
}

void CJeu::on_info(QString mess)
{
    emit sig_info(mess);  // remontée des erreurs à l'IHM
}
