#ifndef CJEU_H
#define CJEU_H

#include <QObject>
#include <QThread>

#include "cbdd.h"
#include "czdc.h"
#include "ccommcibles.h"
#include "ccommpupitre.h"
#include "ccommaffichage.h"
#include "ccommclient.h"
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
    CCommClient *_serv;
    CCommAffichage *_aff;
    CProtocoleClient *_prot;
    QThread *_thPans, *_thPup, *_thAff;

signals:
    void sig_play();

public slots:
    void on_resultReady(QByteArray cibles);
};

#endif // CJEU_H
