#include "cbdd.h"

CBdd::CBdd(QObject *parent) : QObject(parent)
{

}

bool CBdd::verifierParamsConnexion(QString login, QString mdp)
{
    // A FAIRE CODE DE CONTROLE DES PARAMETRES LOGIN
    qDebug() << login << mdp;
    return true; // bonne connexion par défaut.
}
