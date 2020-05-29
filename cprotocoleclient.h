#ifndef CPROTOCOLECLIENT_H
#define CPROTOCOLECLIENT_H

#include <QObject>
#include <ccommclient.h>
#include "czdc.h"

#define LG_TRAME 109

class CProtocoleClient : public QObject
{
    Q_OBJECT
public:
    explicit CProtocoleClient(QObject *parent = nullptr);
    ~CProtocoleClient();
    QByteArray repondreAConnexion(char mode);

private:
    QByteArray _tc;  // trame du client
    QByteArray _rep;  // trame de réponse
    CZdc *_zdc;     // accès aux données communes    
    bool verifierCrc16();
    uint16_t calculerCrc16();
    int decodeEtSauveParams();
    int decodeLoginMdp(QString &login, QString &mdp, QString &origine);

signals:
    void sig_emettreVersClient(QByteArray tc);
    void sig_trameParametrage(QByteArray tc);
    void sig_trameConnexion(QString login, QString mdp, QString origine);
    void sig_trameAnnulationPartie(QByteArray tc);
    void sig_erreurParams(QByteArray tc);

public slots:
    void on_trameClient(QByteArray tc);

};

#endif // CPROTOCOLECLIENT_H
