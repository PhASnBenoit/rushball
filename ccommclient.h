#ifndef CCOMMCLIENT_H
#define CCOMMCLIENT_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "communs.h"

class CCommClient : public QTcpServer
{
        Q_OBJECT

public:
    CCommClient();
    ~CCommClient();
    int repondreAuClient(QByteArray rep);

private:
    QTcpSocket *_client;

private slots:
    void on_newConnection();
    void on_disconnected();
    void on_error(QAbstractSocket::SocketError socketError);
    void on_acceptError(QAbstractSocket::SocketError socketError);
    void on_readyRead();

signals:
    void sig_trameClient(QByteArray tc);
};

#endif // CCOMMCLIENT_H
