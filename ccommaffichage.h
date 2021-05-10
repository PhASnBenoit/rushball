#ifndef CCOMMAFFICHAGE_H
#define CCOMMAFFICHAGE_H

#include <QObject>
#include <QtSerialPort>
#include "communs.h"

#define ID "00"  // tous les afficheurs

class CCommAffichage : public QObject
{
    Q_OBJECT
public:
    explicit CCommAffichage(char modeFinJeu = 'S', QObject *parent = nullptr);
    ~CCommAffichage();
    void afficherBienvenue(uint duree);
    void afficherTypeJeu(uint duree);
    void affScores(uint8_t aQuiLeTour, QList<uint16_t> scores);
    void affQuelJoueur();
    void affValScore(int val);
    void affJoueurChoisi(int noJ);
    void affMenu();
    void affDureeTexte(uint8_t duree, QString texte);

private:
    char _modeFinJeu; // pour savoir si score ou temps
    QSerialPort _sp;  // communication avec l'afficheur
    QByteArray trame;
    int initPortSerie(); // init du port série afficheur
    uint8_t calculerCS(); // Cheksum
   // CRs232 *_
   unsigned char calculerChecksum(char *trame);
   void afficher(char *prot, char *mess);

public slots:
};

#endif // CCOMMAFFICHAGE_H
