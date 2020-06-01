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
#define ETAT_ATTENTE_CONNEXION 0
#define ETAT_JEU_EN_COURS 1

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
