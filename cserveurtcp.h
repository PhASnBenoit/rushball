#ifndef CSERVEURTCP_H
#define CSERVEURTCP_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "communs.h"
#include "cgererclient.h"

class CServeurTcp : public QTcpServer
{
        Q_OBJECT

public:
    CServeurTcp();
    ~CServeurTcp();

private:
    QList<CGererClient *> _clients;  // liste des clients connectés

private slots:
    void on_newConnection();
    void on_disconnected();
    void on_error(QAbstractSocket::SocketError socketError);
    void on_acceptError(QAbstractSocket::SocketError socketError);
    void on_readyRead();

signals:
    void sig_trameClient(QByteArray tc);
    void sig_newConnection();
    void sig_disconnected();
};

#endif // CSERVEURTCP_H
