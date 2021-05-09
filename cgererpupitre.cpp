#include "cgererpupitre.h"
#include "cihm.h"

CGererPupitre::CGererPupitre(QObject *parent) : QObject(parent)
{
    _etatPupitre = ETAT_PUP_HORS_MENU;
    _ind = 0;
    _touches[_ind]=0;
}

CGererPupitre::~CGererPupitre()
{

}

void CGererPupitre::on_toucheRecue(int touche)
{
    // filtre touches autorisées
    switch (touche) {
    case Qt::Key_Slash:
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
    case Qt::Key_Return:
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
            emit sig_afficherMenu();
            emit sig_stop(); // vers CJeu pour stopper le jeu (Acquisitions)
            // Pb ! Au départ, annuler les éventuelles détection de toucher de cibles
            // produits pendant l'arrêt du jeu
            emit sig_info("CGererPupitre::traiterSaisie Affichage du menu (1-5).");
        } // if slash
        else {
            emit sig_info("CGererPupitre::traiterSaisie Touche non traitée.");
        }
        _ind = 0;
        _touches[_ind] = 0;
        break;
    case ETAT_PUP_CHOIX_MENU:
        // seuls les touches 1-5 seront prises en compte
        if ( (touche>=Qt::Key_1) && (touche<=Qt::Key_5) ) {
            switch (touche) {
            case Qt::Key_1:
                emit sig_afficherMenuSelected(2, "Aff. scores");
                emit sig_reqAffScores();
                break;
            case Qt::Key_2:
                emit sig_afficherMenuSelected(2, "Pénalité !");
                emit sig_reqAffQuelJoueur();
                break;
            case Qt::Key_3:
                emit sig_afficherMenuSelected(2, "A qui ?");
                emit sig_reqAffQuelJoueur();
                break;
            case Qt::Key_4:
                emit sig_afficherMenuSelected(2, "Corriger scores");
                emit sig_reqAffQuelJoueur();
                break;
            case Qt::Key_5:
                _etatPupitre = ETAT_PUP_HORS_MENU;
                emit sig_afficherMenuSelected(1, "Retour");
                emit sig_info("CGererPupitre::traiterSaisie Sortie du menu.");
                emit sig_afficherSortieMenu(2);  // durée
                emit sig_start();
                break;
            } // sw
            _etatPupitre = ETAT_PUP_CHOIX_MENU + (touche-0x30); //si la touche 0 est préssé c'est égale à 0 car 0x10=16 et la code ascii pour la touche 0 est de 0 donc 16 * 0 = 0
            emit sig_info("CGererPupitre::traiterSaisie Menu sélectionné : "+QString::number(touche-0x30));
        } // if touche 0-5
        else {
            emit sig_info("CGererPupitre::traiterSaisie Touche non traitée.");
        }
        _ind = 0;
        _touches[_ind] = 0;
        break;

    case ETAT_PUP_MENU_AFFSCORES:
        if (touche == Qt::Key_Return) { //Si la touche Return est appuyé ça valide la touche 0.1.2.3.4.5 préssé juste avant
            _etatPupitre = ETAT_PUP_CHOIX_MENU;
            emit sig_afficherMenu();
            emit sig_info("CGererPupitre::traiterSaisie Retour au menu.");
        } // if entree
        _ind = 0;
        _touches[_ind] = 0;
        break;

    case ETAT_PUP_MENU_PENALITE:
        switch (touche) {
        case Qt::Key_1 ... Qt::Key_4:
            emit sig_reqCalcEtAffNomScores(touche-0x30);
            break;
        case Qt::Key_Return:
            _etatPupitre = ETAT_PUP_CHOIX_MENU;
            emit sig_afficherMenu();
            emit sig_info("CGererPupitre::traiterSaisie Retour au menu.");
            break;
        } // sw
        _ind = 0;
        _touches[_ind] = 0;
        break;

    case ETAT_PUP_MENU_AQUI:
        switch (touche) {
        case Qt::Key_1 ... Qt::Key_4:
            emit sig_reqChangeJoueur(touche-0x30);
            break;
        case Qt::Key_Return:
            _etatPupitre = ETAT_PUP_CHOIX_MENU;
            emit sig_afficherMenu();
            emit sig_info("CGererPupitre::traiterSaisie Retour au menu.");
            break;
        } // sw
        _ind = 0;
        _touches[_ind] = 0;
        break;

    case ETAT_PUP_MENU_CORRIGER:
        // A FAIRE
        break;
    } // sw
} // méthode
