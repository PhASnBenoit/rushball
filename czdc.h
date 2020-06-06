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
    uint8_t etat_jeu;  // état du jeu 0=en attente connexion 1=1 client connecté 2=jeu en cours 3=jeu en cours avec plusieurs clients connectés
    uint8_t aQuiLeTour;
} T_DATAS_DYN;

typedef struct datasStatic {
    uint8_t nbJoueurs;  // max 4
    char nomJoueurs[MAX_JOUEURS][13];
    char modeJeu; // P(toutes cibles allumées) ou M (<=moitié des cibles allumées) ou B(extinction des cibles)
    char modeFinJeu;  // S(Score) ou T(temps)
    uint16_t cpt;  // score ou temps à atteindre
    uint8_t nbPointsFaute; // nombre de points en moins pour une faute
    uint8_t nbPanneaux;  // Nombre de panneaux connectés
    uint8_t nbCiblesOn;  // Nombre de cibles allumées au départ
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

    QByteArray getCouleurs();  //
    QByteArray getCouleursByPanneau(uint8_t noPan);
    QByteArray getCibles();
    QByteArray getCiblesByPanneau(uint8_t noPan);
    uint8_t getNbPanneaux();
    uint8_t getNbJoueurs();
    QList<QString> getNomJoueurs();
    QList<uint16_t> getScores();
    QString getNomJoueur(uint8_t num);
    void setCouleurs(T_COULEURS *tabCouleurs);  // couleurs des cibles
    void setCibles(bool *tabCibles);  // touché des cibles, 1 seule cible touchée
    int setCiblesPour1Panneau(int noPan, uint8_t cibles);
    void clear();
    uint8_t etatJeu();
    void setEtatJeu(const uint8_t &etat);
    void appliquerNewParams(T_DATAS_STATIC *ds);
    char getModeJeu();
    char getModeFinJeu();
    uint8_t joueurSuivant();
    uint8_t getAQuiLeTour();

private:
    T_ZDC *_adrZdc;

signals:
    void sig_erreur(QString mess);
};

#endif // CZDC_H
