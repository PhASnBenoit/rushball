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

    _zdc->joueurSuivant();  // Premier joueur

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
    _aff->afficherScores(_zdc->getAQuiLeTour());
    // mettre à jour affichage à chaque changement

    emit sig_info("CJeu::play : init de la comm avec pupitre.");
    // A FAIRE init de la comm avec le pupitre
    _pup = new CCommPupitre();

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

    emit sig_info("CJeu::play : comm avec les cibles en cours.");
}

QByteArray CJeu::genererCouleursDesCibles()
{
    // Appelé à l'initialisation du jeu seulement

    uint8_t nbPans = _zdc->getNbPanneaux();
    uint8_t nbCiblesAff = nbPans*NB_CIBLES_PAN/2;
    char mode = _zdc->getModeJeu();

    // suivant la règle choisie
    uint8_t refCibles[MAX_PANS*NB_CIBLES_PAN]; // indices des cibles éclairées
    T_COULEURS tabCibles[MAX_PANS][NB_CIBLES_PAN]; // qui peut le plus peut le moins
    QByteArray couleurs = _zdc->getCouleurs();

    switch(mode) {

    case 'P': // toutes les cibles allumées, il faut les éteindre

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
        int nb;
        for (int i = 1; i <= 3; i++) { // forcement 3 lignes
            nb = i;
            for (int j = 1; j <= nbPans; j++) { // selon le nombre de panneau
                int k;
                bool trouve = false;
                for (k = 0; k < nbCiblesAff; k++) { // recherche si cellule à éclairer
                    if (nb == refCibles[k]) {
                        trouve = true;
                        break;
                    } // if
                } // for k
                if (!trouve) {
                    tabCibles[i-1][j-1] = ETEINT;
                } else { // si cible à éclairée
                    uint8_t nbAlCoul = 1+qrand()%(_zdc->getNbCouleurs());// generation aléatoire des couleurs
                    // cherche la couleur
                    int cpt=0;
                    for (k=0 ; k<MAX_NB_COULEURS ; k++) {
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

    case 'M':  // jusqu'à moitié, elle se rallume

        // A FAIRE CALCULER LES NOUVELLES COULEURS

        break;

    default:
        emit sig_erreur("CJeu::on_cibleTouchee : Erreur de mode de jeu");
        break;
    } // sw

    return _zdc->getCouleurs();
} // méthode

void CJeu::on_cibleTouchee(uint8_t noPan, uint8_t cibles)
{
    // appelé dès qu'une cible est touchée
    emit sig_info("CJeu::on_cibleTouchee : Panneau n°:"+QString::number(noPan+1)+" Cible n°:"+QString::number(cibles));

        // suivant la règle choisie
    switch(_zdc->getModeJeu()) {
    case 'P': // toutes les cibles allumées, il faut les éteindre

        // A FAIRE CALCULER LES NOUVELLES COULEURS

        break;
    case 'M':  // jusqu'à moitié, elle se rallume

        // A FAIRE CALCULER LES NOUVELLES COULEURS

        break;
    default:
        emit sig_erreur("CJeu::on_cibleTouchee : Erreur de mode de jeu");
        break;
    } // sw
            // Calculer les nouveaux éclairages de toutes les cibles

    // A FAIRE CALCULER LES SCORES
        // Chercher combien de point vaut la cible touchée
        // mettre à jour le score correspondant dans zdc
        // sauver dans la base de données pour l'historique partie
        // avertir l'objet CCommAffichage pour mettre à jour l'affichage

    // CHANGER DE A QUI CA VIENT
    _zdc->joueurSuivant();
    emit sig_majScores(_zdc->getAQuiLeTour());
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
