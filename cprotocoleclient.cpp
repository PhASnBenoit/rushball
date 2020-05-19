#include "cprotocoleclient.h"

CProtocoleClient::CProtocoleClient(QObject *parent, CCommClient *client) : QObject(parent)
{
    _client = client;
    if (_client==nullptr)
        _erreur = true;
}
