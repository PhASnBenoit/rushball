#ifndef CZDC_H
#define CZDC_H
// ZDC : Zone De Données Communes

#include <QObject>
#include <QSharedMemory>

#include "communs.h"  // Définition de la ZDC

//
// structures de la ZDC
//
#define KEY "RUSHBALL2020"

typedef struct datasDyn {
    bool toucheCibles[MAX_PANS][NB_CIBLES_PAN];
    T_COULEURS couleurCibles[MAX_PANS][NB_CIBLES_PAN];
    uint16_t scores[MAX_JOUEURS];
} T_DATAS_DYN;

typedef struct datasStatic {
    uint8_t nbreJoueurs;  // max 4
    QString nomJoueurs[MAX_JOUEURS];
    char modeJeu; // P(toutes cibles allumées) ou M (<=moitié des cibles allumées) ou B(extinction des cibles)
    char modeFinJeu;  // S(Score) ou T(temps)
    uint16_t cpt;  // score ou temps à atteindre
    uint8_t nbPointsFaute; // nombre de points en moins pour une faute
    uint8_t nbPanneaux;  // Nombre de panneaux connectés
    uint8_t nbreCiblesOn;  // Nombre de cibles allumées au départ
    bool joker;  // Joker prévu ou non
    uint16_t nbPointsJoker; // nombre de points si joker atteint
    uint8_t nbCouleurs;  // sans couleur joker
    uint16_t nbPointscouleurs[MAX_NB_COULEURS];  // indice 0 vaut toujours 0 car cible éteinte
} T_DATAS_STATIC;

typedef struct zdc {
    T_DATAS_DYN datasDyn;
    T_DATAS_STATIC datasStatic;
} T_ZDC;

/////////////////////////////////////////////////////////////////////
/// \brief The CZdc class
////
class CZdc : public QSharedMemory
{
        Q_OBJECT
public:
    CZdc();
    ~CZdc();

    T_ZDC *_adrZdc;

    T_COULEURS *getCouleurs();
    bool* getCibles();
    bool* getCiblesByPanneau(uint8_t noPan);
    void setCouleurs(T_COULEURS *tabCouleurs);  // couleurs des cibles
    void setCibles(bool *tabCibles);  // touché des cibles, 1 seule cible touchée

/*
      QString nomJoueurs[MAX_JOUEURS];
    char modeJeu; // P(toutes cibles allumées) ou M (<=moitié des cibles allumées) ou B(extinction des cibles)
    char modeFinJeu;  // S(Score) ou T(temps)
    uint16_t cpt;  // score ou temps à atteindre
    uint8_t nbPointsFaute; // nombre de points en moins pour une faute
    uint8_t nbPanneaux;  // Nombre de panneaux connectés
    uint8_t nbreCiblesOn;  // Nombre de cibles allumées au départ
    bool joker;  // Joker prévu ou non
    uint16_t nbPointsJoker; // nombre de points si joker atteint
    uint8_t nbCouleurs;  // sans couleur joker
    uint16_t nbPointscouleurs[MAX_NB_COULEURS];  // indice 0 vaut toujours 0 car cible éteinte
*/
private:

signals:
    void sig_error(QString mess);
};

#endif // CZDC_H
