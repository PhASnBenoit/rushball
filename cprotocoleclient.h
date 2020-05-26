#ifndef CPROTOCOLECLIENT_H
#define CPROTOCOLECLIENT_H

#include <QObject>
#include <ccommclient.h>

#define LG_TRAME 109

class CProtocoleClient : public QObject
{
    Q_OBJECT
public:
    explicit CProtocoleClient(QObject *parent = nullptr);

private:
    QByteArray _tc;  // trame du client
    QByteArray _rep;  // trame de réponse

    bool verifierCrc16();
    uint16_t calculerCrc16();

signals:
    void sig_emettreVersClient(QByteArray tc);
    void sig_trameParametrage(QByteArray tc);
    void sig_trameConnexion(QByteArray tc);
    void sig_trameAnnulationPartie(QByteArray tc);

public slots:
    void on_trameClient(QByteArray tc);

};

#endif // CPROTOCOLECLIENT_H
