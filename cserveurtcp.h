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
    void on_play();
    void on_erreur(QString mess);  // relais vers CIhm
    void on_info(QString mess);  // relais vers CIhm

signals:
    void sig_newConnection();
    void sig_disconnected();
    void sig_play();  // signal relais pour CJeu
    void sig_erreur(QString mess);
    void sig_info(QString mess);
};

#endif // CSERVEURTCP_H
