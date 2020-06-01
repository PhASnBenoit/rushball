#ifndef CGERERCLIENT_H
#define CGERERCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

#include "cprotocoleclient.h"

class CGererClient : public QTcpSocket
{
public:
    CGererClient(qintptr desc = 0);
    ~CGererClient();

private:
    qintptr _desc;
    CProtocleClient *_prot;

signals:
    void sig_erreur(QString mess);

private slots:
    void on_readyRead();
    void on_trameConnexion(QString login, QString mdp, QString origine);
    void on_trameParametrage(QByteArray tc);
    void on_trameAnnulationPartie(QByteArray tc);
};

#endif // CGERERCLIENT_H
