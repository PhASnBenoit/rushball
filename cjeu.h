#ifndef CJEU_H
#define CJEU_H

#include <QObject>
#include <QThread>
#include <QString>

#include "cbdd.h"
#include "czdc.h"
#include "ccommcibles.h"
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
    CCommCibles *_pans; // comm avec les panneaux
    CCommPupitre *_pup;
    CServeurTcp *_serv;
    CCommAffichage *_aff;
    QThread *_thPans, *_thPup, *_thAff;

signals:
    void sig_erreur(QString mess);
    void sig_info(QString mess);

public slots:
    void on_ciblesTouchees(QByteArray cibles);
    void on_newConnection();
    void on_disconnected();
    void on_erreur(QString mess);
    void on_info(QString mess);
    void on_play();
};

#endif // CJEU_H
