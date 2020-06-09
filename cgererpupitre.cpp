#include "cgererpupitre.h"

CGererPupitre::CGererPupitre(QObject *parent) : QObject(parent)
{
//    _stop=false;
}

CGererPupitre::~CGererPupitre()
{
}

void CGererPupitre::on_lirePupitre()
{
    QByteArray chaine;
    QTextStream in(stdin);
    in >> chaine;
    emit sig_saisiePupitre(chaine);
}

/*
void CGererPupitre::on_stopLirePupitre()
{
    _stop=true;
}
*/
