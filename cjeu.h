#ifndef CJEU_H
#define CJEU_H

#include <QObject>
#include <QThread>
#include <QString>

#include "cbdd.h"
#include "czdc.h"
#include "ccommpanneaux.h"
#include "ccommpupitre.h"
#include "ccommaffichage.h"
#include "cserveurtcp.h"
#include "cprotocoleclient.h"

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
    CCommPanneaux *_pans; // comm avec les panneaux
    CCommPupitre *_pup;
    CServeurTcp *_serv;
    CCommAffichage *_aff;
    QThread *_thPans, *_thPup, *_thAff;
    QByteArray genererCouleursDesCibles();

signals:
    void sig_erreur(QString mess);
    void sig_info(QString mess);
    void sig_playCommCibles();  // lance la comm I2C avec les panneaux
    void sig_majScores(uint8_t aQuiLeTour); // pour CCommAffichage

private slots:
    void on_cibleTouchee(uint8_t noPan, uint8_t cibles);
    void on_newConnection();
    void on_disconnected();
    void on_erreur(QString mess);
    void on_info(QString mess);
    void on_play();
    void on_annulationPartie();
    void on_finCycleCommPanneau();

};

#endif // CJEU_H
