#ifndef CJEU_H
#define CJEU_H

#include <QObject>
#include <QThread>

#include "cbdd.h"
#include "czdc.h"
#include "ccommcibles.h"
#include "ccommpupitre.h"
#include "ccommaffichage.h"
#include "cserveurtcp.h"
#include "cprotocoleclient.h"

#define ETAT_ATTENTE_CONNEXION 0
#define ETAT_JEU_EN_COURS 1

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
    CCommCibles *_pans; // comm avec les panneaux
    CCommPupitre *_pup;
    CServeurTcp *_serv;
    CCommAffichage *_aff;
    CProtocleClient *_prot;
    QThread *_thPans, *_thPup, *_thAff;
    uint8_t _etat;  // état du jeu 0=en attente connexion 1=1 client connecté 2=jeu en cours 3=jeu en cours avec plusieurs clients connectés

signals:
    void sig_playCommCibles();
    void sig_erreur(QString mess);
    void sig_info(QString mess);

public slots:
    void on_ciblesTouchees(QByteArray cibles);
    void on_emettreVersClient(QByteArray tc);
    void on_newConnection();
    void on_trameConnexion(QString login, QString mdp, QString origine);
    void on_trameParametrage(QByteArray tc);
    void on_trameAnnulationPartie(QByteArray tc);
    void on_erreurParams(QByteArray tc);
};

#endif // CJEU_H
