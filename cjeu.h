#ifndef CJEU_H
#define CJEU_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QTimer>

#include "cbdd.h"
#include "czdc.h"
#include "ccommpanneaux.h"
#include "ccommaffichage.h"
#include "cserveurtcp.h"
#include "cprotocoleclient.h"
#include "cgererpupitre.h"

class CJeu : public QObject
{
    Q_OBJECT
public:
    explicit CJeu(QObject *parent = nullptr);
    ~CJeu();
    void play();

private:
    CBdd *_bdd;
    CZdc *_zdc;
    QTimer *_tmr;
    CCommPanneaux *_pans; // comm avec les panneaux
    CServeurTcp *_serv;
    CCommAffichage *_aff;
    CGererPupitre *_pup;
    QThread *_thPans;
    QThread *_thPup;
    //, *_thAff;
    QByteArray genererCouleursDesCibles();
    bool isFinDePartie();

signals:
    void sig_erreur(QString mess);
    void sig_info(QString mess);
    void sig_playCommCibles();  // lance la comm I2C avec les panneaux
    void sig_majScores(uint8_t aQuiLeTour); // pour CCommAffichage
    void sig_finDePartie();
    void sig_toucheRecue(int touche);

private slots:
    void on_newConnection();
    void on_disconnected();
    void on_erreur(QString mess);
    void on_info(QString mess);
    void on_play();
    void on_annulationPartie();
    void on_finCycleCommPanneau();
    void on_stop();
    void on_start();
    void on_timeout();  // timer d'affichage
    void on_finDePartie();
    void on_trameConnexionValidated();

public slots:
    void on_toucheRecue(int touche);
    void on_reqAffScores();
    void on_cibleTouchee(uint8_t noPan, uint8_t cibles);
};

#endif // CJEU_H
