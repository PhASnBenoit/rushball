#ifndef CPROTOCOLECLIENT_H
#define CPROTOCOLECLIENT_H

#include <QObject>
//#include <cserveurtcp.h>
#include "czdc.h"

#define LG_TRAME_PARAMS 117 // moins les :
#define LG_TRAME_CONNEXION 36
#define LG_TRAME_ANNULATION_PARTIE 6

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
    void sig_paramsSaved(QByteArray tc);
    void sig_connexionAsked(QString login, QString mdp, QString origine);
    void sig_demandeAnnulationPartie(QByteArray tc);
    void sig_erreur(QString mess);
    void sig_info(QString mess);

public slots:
    char on_trameClient(QByteArray tc);
};

#endif // CPROTOCOLECLIENT_H
