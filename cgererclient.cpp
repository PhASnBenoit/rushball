#include "cgererclient.h"

CGererClient::CGererClient(qintptr desc)
{
    _desc = desc;
    if (!setSocketDescriptor(_desc))
        emit sig_erreur("CGererClient::CGererClient : Erreur de socket.");
    connect(this, &CGererClient::readyRead, this, &CGererClient::on_readyRead);

    _prot = new CProtocleClient();
}

CGererClient::~CGererClient()
{
    delete _prot;
}

void CGererClient::on_readyRead()
{
    // réception de données d'un client
    // pour 2020 un seul client possible.
    // 2021 plusieurs client.
    QByteArray tc;  // trame reçu du client
    tc = readAll();
    //emit sig_trameClient(tc);
    char commande = _prot->on_trameClient(tc);
    switch(commande) {

    } //
}
