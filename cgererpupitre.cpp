#include "cgererpupitre.h"
#include "cihm.h"

CGererPupitre::CGererPupitre(QObject *parent) : QObject(parent)
{
// A FAIRE connaitre le nombre de joueur !

    _etatPupitre = ETAT_PUP_HORS_MENU;
    _ind = 0;
    _touches[_ind]=0;
    _score = 0;
}

CGererPupitre::~CGererPupitre()
{

}

void CGererPupitre::on_toucheRecue(int touche)
{
    // filtre touches autorisées
    switch (touche) {
    case Qt::Key_Slash:
    case Qt::Key_0 ... Qt::Key_9:
    case Qt::Key_Return ... Qt::Key_Enter:
    case Qt::Key_Period:
        _touches[_ind++]=touche; // sauve la touche
        traiterSaisie(touche);  // machine à états
        break;
    default:  // autre touche non conforme
        return;
    } // sw
}

void CGererPupitre::traiterSaisie(int touche)
{
    // machine à états
    switch (_etatPupitre) {
    case ETAT_PUP_HORS_MENU:
        if (touche == Qt::Key_Slash) {
            _etatPupitre = ETAT_PUP_CHOIX_MENU;
            emit sig_stop(); // vers CJeu pour stopper le jeu (Acquisitions)
            emit sig_affMenu();
            // Pb ! Au départ, annuler les éventuelles détection de toucher de cibles
            // produites pendant l'arrêt du jeu
            emit sig_info("CGererPupitre::traiterSaisie Affichage du menu (1-5).");
        } // if slash
        else {
            emit sig_info("CGererPupitre::traiterSaisie Touche non traitée.");
        }
        break;
    case ETAT_PUP_CHOIX_MENU:
        // seuls les touches 1-5 seront prises en compte
        if ( (touche>=Qt::Key_1) && (touche<=Qt::Key_5) ) {
            emit sig_info("CGererPupitre::traiterSaisie Menu sélectionné : "+QString::number(touche-0x30));
            switch (touche) {
            case Qt::Key_1:
                emit sig_affDureeTexte(1, "1 - Afficher scores");
                emit sig_affScores();
                emit sig_info("CGererPupitre::traiterSaisie Affichage des scores. ENTREE pour retour au menu");
                _etatPupitre = ETAT_PUP_MENU_AFFSCORES;
                break;
            case Qt::Key_2:
                emit sig_affDureeTexte(1, "2 - Pénalité !");
                emit sig_affQuelJoueur();
                emit sig_info("CGererPupitre::traiterSaisie Donner une pénalité. Entrer NO joueur 1-4 :");
                _etatPupitre = ETAT_PUP_MENU_PENALITE;
                break;
            case Qt::Key_3:
                emit sig_affDureeTexte(1, "3 - A qui le tour !");
                emit sig_affQuelJoueur();
                emit sig_info("CGererPupitre::traiterSaisie Fixer à qui le tour. Entrer NO joueur 1-4 :");
                _etatPupitre = ETAT_PUP_MENU_AQUI;
                break;
            case Qt::Key_4:
                emit sig_affDureeTexte(1, "4 - Corriger scores");
                emit sig_affQuelJoueur();
                emit sig_info("CGererPupitre::traiterSaisie Corriger un score.  Entrer NO joueur 1-4 :");
                _etatPupitre = ETAT_PUP_MENU_CORRIGER;
                break;
            case Qt::Key_5:
                emit sig_affDureeTexte(1, "5 - Sortie du menu");
                emit sig_info("CGererPupitre::traiterSaisie Sortie du menu.");
                emit sig_start(); // play le jeu
                _etatPupitre = ETAT_PUP_HORS_MENU;
                break;
            } // sw
        } // if touche 0-5
        else {
            emit sig_info("CGererPupitre::traiterSaisie Touche non traitée.");
        }
        break;

    case ETAT_PUP_MENU_AFFSCORES:
        if ( (touche==Qt::Key_Return) || (touche==Qt::Key_Enter) ) {
            _etatPupitre = ETAT_PUP_CHOIX_MENU;
            emit sig_affMenu();
            emit sig_info("CGererPupitre::traiterSaisie Retour au menu.");
        } // if entree
        break;

    case ETAT_PUP_MENU_PENALITE:
        switch (touche) {
        case Qt::Key_1 ... Qt::Key_4:
            _noJ = touche-0x30;
            emit sig_affJoueurChoisi(_noJ);
            emit sig_info("CGererPupitre::traiterSaisie Pénalité sur joueur : "+QString::number(_noJ));
            break;
        case Qt::Key_Return ... Qt::Key_Enter:
            _etatPupitre = ETAT_PUP_CHOIX_MENU;
            emit sig_appliquerPenalite(_noJ);
            emit sig_affMenu();
            emit sig_info("CGererPupitre::traiterSaisie Retour au menu.");
            break;
        } // sw
        break;

    case ETAT_PUP_MENU_AQUI:
        switch (touche) {
        case Qt::Key_1 ... Qt::Key_4:
            _noJ = touche-0x30;
            emit sig_affJoueurChoisi(_noJ);
            emit sig_info("CGererPupitre::traiterSaisie Le tour est à joueur : "+QString::number(_noJ));
            break;
        case Qt::Key_Return:
            _etatPupitre = ETAT_PUP_CHOIX_MENU;
            emit sig_appliquerChangeJoueur(_noJ);
            emit sig_affMenu();
            emit sig_info("CGererPupitre::traiterSaisie Retour au menu.");
            break;
        } // sw
        break;

    case ETAT_PUP_MENU_CORRIGER:
        switch (touche) {
        case Qt::Key_1 ... Qt::Key_4:
            _noJ = touche-0x30;
            emit sig_affJoueurChoisi(_noJ);
            emit sig_info("CGererPupitre::traiterSaisie Choix joueur : "+QString::number(_noJ));
            break;
        case Qt::Key_Return ... Qt::Key_Enter:
            _etatPupitre = ETAT_PUP_CORRIGER_QUI;
            // A FAIRE sauver le no du joueur dans donnée membre
            emit sig_info("CGererPupitre::traiterSaisie Correction joueur. Entrez le score (0-9)");
            break;
        } // sw
        break;
    case ETAT_PUP_CORRIGER_QUI:
        switch (touche) {
        case Qt::Key_0 ... Qt::Key_9:
            if (_ind <= 3) { // maxi 3 digits
                if (_ind == 1) _score = 0; // à zéro au départ
                _touches[_ind] = 0; // fin de chaine
                // convert en int
                int nb=0; int *p = _touches; int mul=1;
                while (*p!=0) {nb++;p++;} // compte le nbre de digit
                for(int i=nb ; i>=0 ; i--) {
                    _score += (_touches[i]-0x30)*mul;
                    mul *= 10;
                } // for
                emit sig_affValScore(_score);
                emit sig_info("CGererPupitre::traiterSaisie valeur du score (0-9)");
            } else { // dépassement saisie 3 digits
                _ind = 0;
                _touches[_ind] = 0;
                _score=0;
                emit sig_affValScore(_score);
                emit sig_info("CGererPupitre::traiterSaisie valeur du score (0-9)");
            } // else
            break;
        case Qt::Key_Return ... Qt::Key_Enter:
            _etatPupitre = ETAT_PUP_CHOIX_MENU;
            emit sig_appliquerChangeScore(_noJ, _score);
            emit sig_info("CGererPupitre::traiterSaisie FIN Correction joueur. Retour au menu");
            break;
        } // sw
        break;
    } // sw
    if (_etatPupitre != ETAT_PUP_CORRIGER_QUI) {
        _ind = 0;
        _touches[_ind] = 0;
    } // if etatpup
} // méthode
