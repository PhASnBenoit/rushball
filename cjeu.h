#ifndef CJEU_H
#define CJEU_H

#include <QObject>
#include <QThread>
#include <QList>
#include <QString>
#include <QTimer>

#include "czdc.h"
#include "cserveurtcp.h"
#include "ccommpanneaux.h"
#include "ccommaffichage.h"
#include "cgererpupitre.h"
#include "communs.h"

class CJeu : public QObject
{
    Q_OBJECT
public:
    explicit CJeu(QObject *parent = nullptr);
    ~CJeu();
    void play();

private:
    CZdc *_zdc;
    QTimer *_tmr;
    CCommPanneaux *_pans; // comm avec les panneaux
    CServeurTcp *_serv;
    CCommAffichage *_aff;
    CGererPupitre *_pup;
    QThread *_thPans;
    QByteArray genererCouleursDesCibles();
    QList<T_SCORES> histoScores;
    bool isFinDePartie();
signals:
    void sig_erreur(QString mess);
    void sig_info(QString mess);
    void sig_playCommCibles();  // lance la comm I2C avec les panneaux
    void sig_majScores(uint8_t aQuiLeTour); // pour CCommAffichage
    void sig_finDePartie();
    void sig_toucheRecue(int touche);

private slots:

    void on_info(QString mess);
    void on_erreur(QString mess);
    void on_play();
    void on_timeout();  // timer d'affichage
    void on_finDePartie();

    // slot de CIhm
    void on_toucheRecue(int touche);

    // slots ce CServeurTcp
    void on_newConnection();
    void on_disconnected();
    void on_trameConnexionValidated();
    void on_annulationPartie();

    // slots de CCgererPupitre
    void on_stop();
    void on_start();
    void on_affScores();
    void on_affQuelJoueur();
    void on_affValScore(int val);
    void on_affJoueurChoisi(int noJ);
    void on_affMenu();
    void on_affDureeTexte(uint8_t duree, QString texte);
    void on_appliquerPenalite(int noJ);
    void on_appliquerChangeJoueur(int noJ);
    void on_appliquerChangeScore(int noJ, int val);

    // slots de CCommPanneau
    void on_cibleTouchee(uint8_t noPan, uint8_t cibles);
    void on_finCycleCommPanneau();

public slots:
};

#endif // CJEU_H
