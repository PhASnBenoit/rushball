#ifndef CPROTOCOLECLIENT_H
#define CPROTOCOLECLIENT_H

#include <QObject>
#include "czdc.h"

#define LG_TRAME_PARAMS 115 // moins les :
#define LG_TRAME_CONNEXION 34
#define LG_TRAME_ANNULATION_PARTIE 6

class CProtocleClient : public QObject
{
    Q_OBJECT
public:
    explicit CProtocleClient(QObject *parent = nullptr);
    ~CProtocleClient();
    QByteArray repondreAConnexion(char mode);
    QByteArray preparerTrameMajScores(uint8_t nbJoueurs, QList<QString> nomJoueurs, QList<uint16_t> scores, uint8_t aQuiLeTour);

private:
    QByteArray _tc;  // trame du client
    QByteArray _rep;  // trame de réponse
    T_DATAS_STATIC *_ds;  // buffer data statique
    bool verifierCrc16();
    uint16_t calculerCrc16();
    int decodeEtControleParams();
    int decodeLoginMdp(QString &login, QString &mdp, QString &origine);

signals:
    void sig_emettreVersClient(QByteArray tc);
    void sig_newParamsReady(T_DATAS_STATIC *ds);
    void sig_connexionAsked(QString login, QString mdp, QString origine);
    void sig_annulationPartieAsked(QByteArray tc);
    void sig_erreur(QString mess);
    void sig_info(QString mess);

public slots:
    char on_trameClient(QByteArray tc);
};

#endif // CPROTOCOLECLIENT_H
