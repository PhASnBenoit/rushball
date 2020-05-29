#include "ccommclient.h"

CCommClient::CCommClient()
{
    connect(this, &CCommClient::newConnection, this, &CCommClient::on_newConnection);
    connect(this, &CCommClient::acceptError, this, &CCommClient::on_acceptError);
    listen(QHostAddress::Any, PORT);
}

CCommClient::~CCommClient()
{
    delete _client;
}

int CCommClient::repondreAuClient(QByteArray rep)
{
    _client->write(rep);
    return 0; // RAS
}

void CCommClient::on_newConnection()
{
    _client = nextPendingConnection(); // récupération de la socket client
    connect(_client, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
        [=](QAbstractSocket::SocketError socketError) {
        this->on_error(socketError);});
    connect(_client, &QTcpSocket::disconnected, this, &CCommClient::on_disconnected);
    connect(_client, &QTcpSocket::readyRead, this, &CCommClient::on_readyRead);
    close(); // le serveur n'écoute plus
    // arrêter le serveur
}

void CCommClient::on_disconnected()
{
    qDebug() << "CCommClient::on_disconnected";
    delete _client;
    _client = nullptr;
    listen(QHostAddress::Any, PORT); // mise en écoute de nouveau du serveur
}

void CCommClient::on_error(QAbstractSocket::SocketError socketError)
{
    qDebug() << "CCommClient::on_error : " << socketError;

}

void CCommClient::on_acceptError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "CCommClient::on_acceptError : " << socketError;
}

void CCommClient::on_readyRead()
{
    // réception de données d'un client
    // pour 2020 un seul client possible.
    // 2021 plusieurs client.
    QByteArray tc;  // trame reçu du client
    tc = _client->readAll();
    emit sig_trameClient(tc);
}
