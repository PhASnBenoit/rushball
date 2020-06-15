#ifndef COMMUNS_H
#define COMMUNS_H

#include <QObject>
#include <QDebug>

//
// A REMPLACER DANS LE FUTUR PAR UN FICHIER .INI (QStettings)
//
// Parametres généraux du jeu
#define MAX_PANS 8
#define NB_CIBLES_PAN 3
#define MAX_JOUEURS 4
#define MAX_NB_COULEURS 8  // 7 couleurs max ETEINT n'est pas une couleur mais compte comme une valeur.
// params panneaux
#define ADR_BASE_PAN 0x31   // adresse de base du premier panneau
// PORT SERVEUR
#define PORT 4444  // A METTRE DANS CONFIG.INI
// JEU
#define ETAT_JEU_ATTENTE_CONNEXION 1
#define ETAT_JEU_ATTENTE_PARAMS 2
#define ETAT_JEU_EN_COURS 4
#define ETAT_JEU_EN_PAUSE 8
// CLIENT CONNECTé
#define ETAT_CLIENT_CONNECTED 1  // connexion TCP seule
#define ETAT_CLIENT_AUTHENTIFIED 2 // Client authentifié correctement
#define ETAT_CLIENT_PARAMETRED 4 // client ayant paramétré
#define ETAT_CLIENT_PREMIER 8  // premier client connecté

// PUPITRE
typedef enum etats_pupitre {
    ETAT_PUP_CHOIX_MENU=0x00,
    ETAT_PUP_VALIDE_AFFICHER_SCORES=0x10,
    ETAT_PUP_SCORES_ATTENTE_RETOUR,
    ETAT_PUP_VALIDE_PENALITE=0x20,
    ETAT_PUP_PENALITE_NO_JOUEUR,
    ETAT_PUP_PENALITE_ATTENTE_RETOUR,
    ETAT_PUP_VALIDE_AQUILETOUR=0x30,
    ETAT_PUP_AQUILETOUR_NO_JOUEUR,
    ETAT_PUP_AQUILETOUR_ATTENTE_RETOUR,
    ETAT_PUP_VALIDE_CORRIGER=0x40,
    ETAT_PUP_CORRIGER_NO_JOUEUR,
    ETAT_PUP_CORRIGER_SAISIE,
    ETAT_PUP_CORRIGER_ATTENTE_RETOUR,
    ETAT_PUP_VALIDE_SORTIR = 0x50,
    ETAT_PUP_HORS_MENU=0x100
} T_ETATS_PUPITRE;


#define TOUCHE_STOP "/"

typedef enum couleurs {
    ETEINT,
    ROUGE,
    VERT,
    BLEU,
    VIOLET,
    CYAN,
    BLANC,
    COULEUR_JOKER
} T_COULEURS;

#endif // COMMUNS_H
