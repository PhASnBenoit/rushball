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
// params panneaux
#define ADR_BASE_PAN 0x31   // adresse de base du premier panneau
// PORT SERVEUR
#define PORT 4444

//
// définitions ZDC
//
#define KEY "RUSHBALL2020"
typedef struct datas {
    bool cibles[MAX_PANS][NB_CIBLES_PAN];
    uint8_t couleurs[MAX_PANS][NB_CIBLES_PAN];
    uint8_t nbPanneaux;
    QString pseudos[MAX_JOUEURS];
    uint16_t scores[MAX_JOUEURS];
} T_DATAS;


#endif // COMMUNS_H
