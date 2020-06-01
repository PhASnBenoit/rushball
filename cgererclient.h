#ifndef CGERERCLIENT_H
#define CGERERCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>

#include "cprotocoleclient.h"
#include "cbdd.h"
#include "czdc.h"

class CGererClient : public QObject
{
        Q_OBJECT
public:
    explicit CGererClient(QTcpSocket *sock = nullptr);
    ~CGererClient();

private:
    QTcpSocket *_sock;
    CProtocleClient *_prot;
    CBdd *_bdd;
    CZdc *_zdc;
    void repondreAuClient(QByteArray rep);

public slots:
    void on_readyRead();
    void on_connexionAsked(QString login, QString mdp, QString origine);
    void on_paramsSaved();
    void on_trameAnnulationPartie(QByteArray tc);
    void on_erreur(QString mess); // relais de CProtocoleClient

signals:
    void sig_erreur(QString mess);
    void sig_play();
};

#endif // CGERERCLIENT_H
