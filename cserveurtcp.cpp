#include "cserveurtcp.h"

CServeurTcp::CServeurTcp()
{
    connect(this, &CServeurTcp::newConnection, this, &CServeurTcp::on_newConnection);
    connect(this, &CServeurTcp::acceptError, this, &CServeurTcp::on_acceptError);
    listen(QHostAddress::Any, PORT);
}

CServeurTcp::~CServeurTcp()
{
    // suppression de toutes les connexions avec les clients
    for (int i=0 ; i<_clients.size() ; i++) {
        if (_clients.at(i)->isConnected())
            delete (_clients.at(i));
    } // for
    _clients.clear();
}

void CServeurTcp::on_newConnection()
{
    CGererClient *gererClient;
    QTcpSocket *client;

    emit sig_newConnection(); // pour debug et affichage
    client = nextPendingConnection(); // récupération de la socket client
    gererClient = new CGererClient(client);
    connect(client, &QTcpSocket::disconnected, client, &QTcpSocket::deleteLater);
    connect(gererClient, &CGererClient::sig_info, this, &CServeurTcp::on_info);
    connect(this, &CServeurTcp::sig_majScores, gererClient, &CGererClient::on_majScores);
    connect(gererClient, &CGererClient::sig_erreur, this, &CServeurTcp::on_erreur);
    connect(gererClient, &CGererClient::sig_play, this, &CServeurTcp::on_play);
    connect(client, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
        [=](QAbstractSocket::SocketError socketError) {
        this->on_error(socketError);});
    connect(client, &QTcpSocket::disconnected, this, &CServeurTcp::on_disconnected);
    _clients.append(gererClient);
    // arrêter le serveur
}

void CServeurTcp::on_disconnected()
{
    qDebug() << "CCommClient::on_disconnected";
    // purge des CGererClient déconnecté
    for (int i=0 ; i<_clients.size() ; i++) {
        if (_clients.at(i)->isConnected() == QAbstractSocket::UnconnectedState) {
//            delete _clients.at(i); // effacé par deleteLater()
            _clients.removeAt(i);
            emit sig_info("CServeurTcp::on_disconnected : Effacement d'un client");
            i=0;
            continue;  // recommence la boucle avec i=0
        } // if connected
    } // for
    emit sig_info("CServeurTcp::on_disconnected Nb Clients restant : "+QString::number(_clients.size()));
    emit sig_disconnected();
}

void CServeurTcp::on_error(QAbstractSocket::SocketError socketError)
{
    qDebug() << "CCommClient::on_error : " << socketError;
}

void CServeurTcp::on_acceptError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "CCommClient::on_acceptError : " << socketError;
}

void CServeurTcp::on_play()
{
    emit sig_play();  // relais entre client et jeu
}

void CServeurTcp::on_erreur(QString mess)
{
    emit sig_erreur(mess);
}

void CServeurTcp::on_info(QString mess)
{
    emit sig_info(mess);  // relais pour cjeu
}

void CServeurTcp::on_annulationPartie()
{
    emit sig_annulationPartie();  // relais pour cjeu
}

void CServeurTcp::on_majScores(uint8_t aQuiLeTour)
{
    // A FAIRE pour chaque client connecté, envoyer trame de mise à jour des scores
    emit sig_majScores(aQuiLeTour);
}
