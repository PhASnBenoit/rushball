#ifndef CPROTOCOLECLIENT_H
#define CPROTOCOLECLIENT_H

#include <QObject>
#include <ccommclient.h>

class CProtocoleClient : public QObject
{
    Q_OBJECT
public:
    explicit CProtocoleClient(QObject *parent = nullptr, CCommClient *client = nullptr);

private:
    CCommClient *_client;
    bool _erreur;  // true=client non défini

signals:

public slots:
};

#endif // CPROTOCOLECLIENT_H
