#include "ccommcibles.h"

CCommCibles::CCommCibles(QObject *parent) : QObject(parent)
{
    _zdc = new CZdc();
    _i2c = CI2c::getInstance(this, '1');
    _pause = false;
    connect(this, &CCommCibles::sig_replay, this, &CCommCibles::on_playCommCibles);
}

CCommCibles::~CCommCibles()
{
    delete _i2c;
    delete _zdc;
}

void CCommCibles::on_playCommCibles()
{
    QByteArray cibles;
    uint8_t couleurs[3];
    uint8_t panneau;
    // LIRE DANS ZDC NOMBRE DE PANNEAUX
    uint8_t nbPanneaux = _zdc->getNbPanneaux();  // Simulation

    // LIRE DANS ZDC COULEURS A AFFICHER

    // MISE A JOUR DES COULEURS
    for (uint8_t i=0 ; i<nbPanneaux ; i++) {
        couleurs[0] = ROUGE;
        couleurs[1] = VERT;
        couleurs[2] = ETEINT; // Simulation
        _i2c->ecrire(static_cast<uint8_t>(ADR_BASE_PAN+i), couleurs, 3);
    } //for

    // TEST CIBLE TOUCHEE
    for (int i=0 ; i<nbPanneaux ; i++) {
        _i2c->lire(static_cast<uint8_t>(ADR_BASE_PAN+i), &panneau, 1);
        cibles.append(static_cast<char>(panneau));
        // SAUVER DANS ZDC ETAT CIBLES

    } //for

    emit sig_ciblesTouchees(cibles);
    if (!_pause)  // on relance sauf si pause demandée
        emit sig_replay();
}

void CCommCibles::stop()
{
    _pause = true;
}

void CCommCibles::start()
{
    _pause = false;
    on_playCommCibles();
}
