#include "cjeu.h"

CJeu::CJeu(QObject *parent) : QObject(parent)
{
    _zdc = new CZdc();  // accès à la mémoire partagée commune
    _zdc->setEtatJeu(ETAT_JEU_ATTENTE_CONNEXION);  // en attente de connexion d'un client

    _thPans = nullptr;
    _thPup = nullptr;
    _aff = nullptr;
    _pans = nullptr;
    _pup = nullptr;

    _tmr = new QTimer();
    _tmr->setInterval(1000); // un peu poins de 1s
    connect(_tmr, &QTimer::timeout, this, &CJeu::on_timeout);

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
    connect(this, &CJeu::sig_finDePartie, this, &CJeu::on_finDePartie);

    emit sig_info("CJeu:CJeu : Services ZDC, Serveur TCP activés.");
} // méthode

CJeu::~CJeu()
{
    delete _tmr;

    // arrêt serveur TCP
    delete _serv;

    // arrêt des threads
    if (_thPans != nullptr) {
        _thPans->quit();
        _thPans->wait();
        delete _thPans;
        delete _pans;
    } // if pans
    if (_thPup != nullptr) {  // PAS SUR CREATION THREAD
        _thPup->quit();
        _thPup->wait();
        delete _thPup;
        delete _pup;
    } // if aff
    if (_aff != nullptr)
        delete _aff;

    delete _zdc;
} // méthode

void CJeu::play()
{
    // méthode appelée lorsque le paramétrage est correct.

    _zdc->setEtatJeu(ETAT_JEU_EN_COURS);
    emit sig_info("CJeu::play : Le jeu commence...");

    _zdc->setDureePoints(_zdc->getCpt());  // fixe le temps ou nb de points restant
    _zdc->joueurSuivant();  // Premier joueur puisque le jeu démarre

    emit sig_info("CJeu::play : init du bandeau d'affichage.");
    char modeFinJeu = _zdc->getModeFinJeu();
    _aff = new CCommAffichage(this, modeFinJeu);  // temps ou points
    connect(this, &CJeu::sig_majScores, _aff, &CCommAffichage::on_afficherScores);  // mise à jour affichage score
    connect(this, &CJeu::sig_majScores, _serv, &CServeurTcp::on_majScores);  // mise à jour affichage score
    // A FAIRE afficher RUSHBALL et version pendant 5s
    _aff->afficherBienvenue(5);  // 5s affichage
    // A FAIRE afficher le type de jeu choisi pendant 5s
    _aff->afficherTypeJeu(5);    // 5s
    // A FAIRE initialiser l'affichage des joueurs, scores.
    _aff->on_afficherScores(_zdc->getAQuiLeTour());
    // mettre à jour affichage chaque seconde
    _tmr->start(); // lance l'affichage des scores

    emit sig_info("CJeu::play : Lancement thread de comm avec les cibles.");
    // INIT DES COULEURS DES CIBLES SUIVANT LA REGLE
    genererCouleursDesCibles();

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

    // Gestion du pupitre de correction des erreurs
    _pup = new CGererPupitre();
    connect(this, &CJeu::sig_toucheRecue, _pup, &CGererPupitre::on_toucheRecue);
    connect(_pup, &CGererPupitre::sig_stop, this, &CJeu::on_stop);
    connect(_pup, &CGererPupitre::sig_start, this, &CJeu::on_start);
    connect(_pup, &CGererPupitre::sig_afficherScores, this, &CJeu::on_sigMenuPupitre);


    emit sig_info("CJeu::play : comm avec les cibles en cours.");
}

QByteArray CJeu::genererCouleursDesCibles()
{
    // Appelé à l'initialisation du jeu seulement

    int nb; // indice
    int cpt;  // pour recherche couleur
    uint8_t nbAlCoul;
    uint8_t nbPans = _zdc->getNbPanneaux();
    uint8_t nbCouls = _zdc->getNbCouleurs();
    uint8_t nbCiblesAff = nbPans*NB_CIBLES_PAN/2;
    char mode = _zdc->getModeJeu();

    // suivant la règle choisie
    uint8_t refCibles[MAX_PANS*NB_CIBLES_PAN]; // indices des cibles éclairées
    T_COULEURS tabCibles[MAX_PANS][NB_CIBLES_PAN]; // qui peut le plus peut le moins
    QByteArray couleurs = _zdc->getCouleurs();

    switch(mode) {

    case 'M':  // jusqu'à moitié, elle se rallume
        // génération aléatoires des cibles affichées
        for (int i = 0; i < nbCiblesAff; i++) {
            refCibles[i] = qrand()%(nbPans*3); // entre 1 et NbPC*3
            for (int j = 0; j < i; j++) {      // vérification unicité du nombre
                if (refCibles[i] == refCibles[j]) {
                    i--;
                    break;
                } // if =
            } // j
        } // for i

        // génération du tableau des couleurs pour les cibles sélectionnée
        for (int i = 1; i <= 3; i++) { // forcement 3 lignes
            nb = i;
            for (int j = 1; j <= nbPans; j++) { // selon le nombre de panneau
                bool trouve = false;
                for (int k = 0; k < nbCiblesAff; k++) { // recherche si cellule à éclairer
                    if (nb == refCibles[k]) {
                        trouve = true;
                        break;
                    } // if
                } // for k
                if (!trouve) {
                    tabCibles[i-1][j-1] = ETEINT;
                } else { // si cible à éclairée
                    nbAlCoul = 1+qrand()%nbCouls;// generation aléatoire des couleurs
                    // cherche la couleur
                    cpt=0;
                    for (int k=0 ; k<MAX_NB_COULEURS ; k++) {
                        if (couleurs.at(k) > 0)
                            cpt++;
                        if (cpt == nbAlCoul) { // si trouvé couleur
                            tabCibles[i-1][j-1] = static_cast<T_COULEURS>(couleurs.at(k));
                            break;
                        } // if trouvé couleur
                    } // for k
                } // else cible à éclairer
                nb += 3;
            } // for j
        } // for i
        // maj les couleurs
        _zdc->setCouleurs(&tabCibles[0][0]);
        break;

    case 'P': // toutes les cibles allumées, il faut les éteindre
        // génération du tableau des couleurs pour les cibles sélectionnée
        for (int i = 1; i <= 3; i++) { // forcement 3 lignes
            for (int j = 1; j <= nbPans; j++) { // selon le nombre de panneau
                nbAlCoul = 1+qrand()%nbCouls;// generation aléatoire des couleurs
                // cherche la couleur
                int cpt=0;
                for (int k=0 ; k<MAX_NB_COULEURS ; k++) {
                    if (couleurs.at(k) > 0)
                        cpt++;
                    if (cpt == nbAlCoul) { // si trouvé couleur
                        tabCibles[i-1][j-1] = static_cast<T_COULEURS>(couleurs.at(k));
                        break;
                    } // if trouvé couleur
                } // for k
            } // for j
        } // for i
        // maj les couleurs
        _zdc->setCouleurs(&tabCibles[0][0]);
        break;

    default:
        emit sig_erreur("CJeu::on_cibleTouchee : Erreur de mode de jeu");
        break;
    } // sw

    return _zdc->getCouleurs();
}

bool CJeu::isFinDePartie()
{
    QList<uint16_t> scores;
    int nbj;  // nombre de joueur
    char mfj = _zdc->getModeFinJeu();
    uint16_t cpt = _zdc->getCpt();

    switch(mfj) {
    case 'S': // atteindre un nombre de points
        scores = _zdc->getScores();
        nbj = _zdc->getNbJoueurs();
        for (int i=0 ; i<nbj ; i++) {
            if (scores.at(i) >= cpt)
                return true;
        } // for i
        break;
    case 'T': // temps décroissant
        if (cpt == 0)
            return true;
        break;
    default:
        emit sig_erreur("CJeu::isFinDePartie : Mode de fin de jeu non cohérent !");
        break;
    } // sw
    return false;
}

void CJeu::on_cibleTouchee(uint8_t noPan, uint8_t cibles)
{
    // appelé dès qu'une cible est touchée
    emit sig_info("CJeu::on_cibleTouchee : Panneau n°:"+QString::number(noPan+1)+" Cible n°:"+QString::number(cibles));

    if (_zdc->etatJeu() == ETAT_JEU_EN_COURS) {
        // Chercher combien de point vaut la cible touchée
        uint16_t nbPoints = _zdc->getNbPoint1Cible(noPan, cibles);
        // mettre à jour le score correspondant dans zdc
        uint8_t qui = _zdc->getAQuiLeTour();
        _zdc->mettreAjourScore1Joueur(qui, nbPoints);
    // A FAIRE sauver dans la base de données pour l'historique partie

        // maj des couleurs des cibles suite au touché
        switch(_zdc->getModeJeu()) {
        case 'M':  // jusqu'à moitié, elle se rallume
            _zdc->eteindre1Cible(noPan, cibles);
            break;
        case 'P': // toutes les cibles allumées, il faut les éteindre
            _zdc->allumer1AutreCible(noPan, cibles);  // coordonnées de la cible pour conservation de la couleur
            break;
        default:
            emit sig_erreur("CJeu::on_cibleTouchee : Erreur de mode de jeu");
            break;
        } // sw

        // A FAIRE tester fin de partie
        if (isFinDePartie())
            emit sig_finDePartie();
        _zdc->joueurSuivant();
        emit sig_majScores(_zdc->getAQuiLeTour());
    } // if etat
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

void CJeu::on_stop()
{
    // provoqué par le bouton STOP du pupitre
    _tmr->stop();
    _zdc->setEtatJeu(ETAT_JEU_EN_PAUSE);
    _aff->afficherMenu(); // afficher le menu
}

void CJeu::on_start()
{
    // méthode le jeu reprend après une correction
    _tmr->start();
    _zdc->setEtatJeu(ETAT_JEU_EN_COURS);
}

void CJeu::on_timeout()
{
    // méthode timer qui provoque l'affichage de la durée et des scores tous les intervalles
    char mfj = _zdc->getModeFinJeu();
    if (mfj == 'T') {
        uint16_t val = _zdc->getDureePoints();
        _zdc->setDureePoints(val-1);
        emit sig_majScores(_zdc->getAQuiLeTour());
        if (isFinDePartie())
            emit sig_finDePartie();
    } // if mfj
}

void CJeu::on_finDePartie()
{
    // A FAIRE fin de partie
    // Arrêt timer
    _tmr->stop();
    _zdc->setEtatJeu(ETAT_JEU_ATTENTE_PARAMS);
    // réinitialisation de tout
}

void CJeu::on_toucheRecue(int touche)
{
    emit sig_toucheRecue(touche); // vers CGererPupitre
}

void CJeu::on_sigMenuPupitre(int menu)
{
    switch(menu){
    case ETAT_PUPITRE_AFFICHER_SCORES:
        break;
    } // sw
}

void CJeu::on_erreur(QString mess)
{
    emit sig_erreur(mess);  // remontée des erreurs à l'IHM
}

void CJeu::on_info(QString mess)
{
    emit sig_info(mess);  // remontée des erreurs à l'IHM
}
