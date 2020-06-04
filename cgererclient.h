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
    bool isConnected();

private:
    QTcpSocket *_sock;
    CProtocleClient *_prot;
    CBdd *_bdd;
    CZdc *_zdc;
    char _typeClient;  // P=PC  M=Téléphone
    char _etatClient;  // connecté, authentifié, principal
    void repondreAuClient(QByteArray rep);

public slots:
    void on_readyRead();
    void on_connexionAsked(QString login, QString mdp, QString origine);
    void on_newParamsReady(T_DATAS_STATIC *ds);
    void on_annulationPartieAsked();
    void on_erreur(QString mess); // relais de CProtocoleClient
    void on_info(QString mess);

signals:
    void sig_erreur(QString mess);
    void sig_info(QString mess);
    void sig_play();
    void sig_annulationPartie();
};

#endif // CGERERCLIENT_H
