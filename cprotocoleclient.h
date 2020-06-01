#ifndef CPROTOCOLECLIENT_H
#define CPROTOCOLECLIENT_H

#include <QObject>
//#include <cserveurtcp.h>
#include "czdc.h"

#define LG_TRAME 109

class CProtocleClient : public QObject
{
    Q_OBJECT
public:
    explicit CProtocleClient(QObject *parent = nullptr);
    ~CProtocleClient();
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
    char on_trameClient(QByteArray tc);

};

#endif // CPROTOCOLECLIENT_H
