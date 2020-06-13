#include "cgererpupitre.h"

CGererPupitre::CGererPupitre(QObject *parent, CCommAffichage *aff) : QObject(parent)
{
    _etatPupitre = ETAT_PUPITRE_HORS_MENU;
    _aff = aff;
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
    case ETAT_PUPITRE_HORS_MENU:
        if (touche == Qt::Key_Slash) {
            _etatPupitre = ETAT_PUPITRE_CHOIX_MENU;
            _aff->afficherMenu();
            emit sig_stop(); // vers CJeu
        } // if slash
        _ind = 0;
        _touches[_ind] = 0;
        break;
    case ETAT_PUPITRE_CHOIX_MENU:
        // seuls les touches 0-5 seront prises en compte
        if ( (touche>=Qt::Key_0) && (touche<=Qt::Key_5) ) {
            if (touche==Qt::Key_5) {
                _aff->afficherSortieMenu(3);  // durée 3s
                emit sig_start();
                break;
            } // if 5
            if (touche==Qt::Key_1) _aff->afficherMenuSelected(3, "Aff. scores");
            if (touche==Qt::Key_2) _aff->afficherMenuSelected(3, "Pénalité !");
            if (touche==Qt::Key_3) _aff->afficherMenuSelected(3, "A qui ?");
            if (touche==Qt::Key_4) _aff->afficherMenuSelected(3, "Corriger scores");
            _etatPupitre = ETAT_PUPITRE_CHOIX_MENU + 0x10*(touche-0x30);
        } // if touche 0-5
        _ind = 0;
        _touches[_ind] = 0;
        break;

    case ETAT_PUPITRE_VALIDE_AFFICHER_SCORES:
        if (touche == Qt::Key_Return) {
            emit sig_reqAffScores(); // requête d'affichage des scores
            _etatPupitre = ETAT_PUPITRE_SCORES_ATTENTE_RETOUR;
        } // if entree
        _ind = 0;
        _touches[_ind] = 0;
        break;
    case ETAT_PUPITRE_SCORES_ATTENTE_RETOUR:
        if (touche == Qt::Key_Return) {
            _etatPupitre = ETAT_PUPITRE_CHOIX_MENU;
            _aff->afficherMenu();
        } // if entree
        _ind = 0;
        _touches[_ind] = 0;
        break;

    case ETAT_PUPITRE_MENU_PENALITE:

        _ind = 0;
        _touches[_ind] = 0;
        break;
    case ETAT_PUPITRE_PENALITE_NO_JOUEUR:
        break;
    case ETAT_PUPITRE_PENALITE_ATTENTE_RETOUR:
        break;

    case ETAT_PUPITRE_MENU_AQUILETOUR:
        break;
    case ETAT_PUPITRE_AQUILETOUR_NO_JOUEUR:
        break;
    case ETAT_PUPITRE_AQUILETOUR_ATTENTE_RETOUR:
        break;

    case ETAT_PUPITRE_MENU_CORRIGER:
        break;
    case ETAT_PUPITRE_CORRIGER_NO_JOUEUR:
        break;
    case ETAT_PUPITRE_CORRIGER_SAISIE:
        break;
    case ETAT_PUPITRE_CORRIGER_ATTENTE_RETOUR:
        break;
    } // sw
} // méthode
