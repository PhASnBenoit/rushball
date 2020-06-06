#include "cjeu.h"

CJeu::CJeu(QObject *parent) : QObject(parent)
{
    _zdc = new CZdc();  // accès à la mémoire partagée commune
    _zdc->setEtatJeu(ETAT_JEU_ATTENTE_CONNEXION);  // en attente de connexion d'un client

    // pour le moment, qu'un seul client autorisé à se connecter.
    // EVOLUTION 2021 : Plusieurs clients se connectent
    // ne permet qu'au premier client de lancer un paramétrage.
    // les autres clients qui se connecte n'auront qu'un suivi du jeu.
    _serv = new CServeurTcp();  // mise en route du serveur TCP
    connect(_serv, &CServeurTcp::sig_newConnection, this, &CJeu::on_newConnection);
    connect(_serv, &CServeurTcp::sig_annulationPartie, this, &CJeu::on_annulationPartie);
    connect(_serv, &CServeurTcp::sig_erreur, this, &CJeu::on_erreur);
    connect(_serv, &CServeurTcp::sig_info, this, &CJeu::on_info);
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
    _zdc->setEtatJeu(ETAT_JEU_EN_COURS);
    emit sig_info("CJeu::play : Le jeu commence...");

    // A FAIRE définir à qui le tour
    suivant(255);

    emit sig_info("CJeu::play : init du bandeau d'affichage.");
    char modeFinJeu = _zdc->getModeFinJeu();
    _aff = new CCommAffichage(this, modeFinJeu);  // temps ou points
    connect(this, &CJeu::sig_majScores, _aff, &CCommAffichage::afficherScores);  // mise à jour affichage score
    connect(this, &CJeu::sig_majScores, _serv, &CServeurTcp::on_majScores);  // mise à jour affichage score
    // A FAIRE afficher RUSHBALL et version pendant 5s
    _aff->afficherBienvenue(5);  // 5s affichage
    // A FAIRE afficher le type de jeu choisi pendant 5s
    _aff->afficherTypeJeu(5);    // 5s
    // A FAIRE initialiser l'affichage des joueurs, scores.
    _aff->afficherScores();
    // mettre à jour affichage à chaque changement

    emit sig_info("CJeu::play : init de la comm avec pupitre.");
    // A FAIRE init de la comm avec le pupitre
    _pup = new CCommPupitre();

    emit sig_info("CJeu::play : Lancement thread de comm avec les cibles.");

    // A FAIRE INIT DES COULEURS DES CIBLES SUIVANT LA REGLE
    QByteArray couleurs =  genererCouleursDesCibles();

    // init thread de communication avec les cibles
    _pans = new CCommPanneaux();
    _thPans = new QThread();
    _pans->moveToThread(_thPans);
    connect(_thPans, &QThread::finished, _pans, &QObject::deleteLater);
    connect(this, &CJeu::sig_playCommCibles, _pans, &CCommPanneaux::on_playCommCibles);
    connect(_pans, &CCommPanneaux::sig_cibleTouchee, this, &CJeu::on_cibleTouchee);
    connect(_pans, &CCommPanneaux::sig_finCycleCommPanneaux, this, &CJeu::on_finCycleCommPanneau);
    _thPans->start();  // lancement du thread
    emit sig_playCommCibles();  // lance la communication I2C

    emit sig_info("CJeu::play : comm avec les cibles en cours.");
}

QByteArray CJeu::genererCouleursDesCibles()
{
    char mode = _zdc->getModeJeu();
    char fin = _zdc->getModeFinJeu();

    // A FAIRE APPLICATION SELON LA REGLE

    return _zdc->getCouleurs();
} // méthode

void CJeu::on_cibleTouchee(uint8_t noPan, uint8_t cibles)
{
    // appelé dès qu'une cible est touchée
    emit sig_info("CJeu::on_cibleTouchee : Panneau n°:"+QString::number(noPan+1)+" Cible n°:"+QString::number(cibles));

    // A FAIRE CALCULER LES NOUVELLES COULEURS
        // suivant la règle choisie
            // Calculer les nouveaux éclairages de toutes les cibles

    // A FAIRE CALCULER LES SCORES
        // Chercher combien de point vaut la cible touchée
        // mettre à jour le score correspondant dans zdc
        // sauver dans la base de données pour l'historique partie
        // avertir l'objet CCommAffichage pour mettre à jour l'affichage

    // CHANGER DE A QUI CA VIENT

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

void CJeu::on_annulationPartie()
{
    // appelée si demande d'annulation partie

    // A FAIRE effacer toutes les données ZDC
    // A FAIRE effacer les données de la partie dans la BDD
    // A FAIRE déconnecter les clients
    // A FAIRE remettre à l'état initial
}

void CJeu::on_finCycleCommPanneau()
{
    // appelée lorsque CCommPanneau a terminé son cycle de lecture écriture vers les panneaux
    // Je m'en sert que pour l'affichage
    emit sig_info("Cycle I2C terminé");
}

void CJeu::on_erreur(QString mess)
{
    emit sig_erreur(mess);  // remontée des erreurs à l'IHM
}

void CJeu::on_info(QString mess)
{
    emit sig_info(mess);  // remontée des erreurs à l'IHM
}
