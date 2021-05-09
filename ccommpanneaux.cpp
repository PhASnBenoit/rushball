#include "ccommpanneaux.h"

CCommPanneaux::CCommPanneaux(QObject *parent) : QObject(parent)
{
 _zdc = new CZdc();
 _i2c = CI2c::getInstance();
 _prot = new CProtocolePanneaux();
 connect(this, &CCommPanneaux::sig_replay, &CCommPanneaux::on_playCommCibles);
}

CCommPanneaux::~CCommPanneaux()
{
    delete _prot;
    CI2c::freeInstance();
    delete _zdc;
}

void CCommPanneaux::on_playCommCibles()
{
    uint8_t panneau;
    uint8_t nbPanneaux = _zdc->getNbPanneaux();

    // RECHERCHE CIBLE TOUCHEE
    for (uint8_t i=0 ; i<nbPanneaux ; i++) {
        _i2c->lire(static_cast<uint8_t>(ADR_BASE_PAN+i), &panneau, 1);
        // A FAIRE SAUVER DANS ZDC ETAT CIBLES
        _zdc->setCiblesPour1Panneau(i, panneau);
        if (panneau > 0) { // si au moins une cible touchée sur le panneau
            emit sig_cibleTouchee(i, panneau);
            return;  // fin méthod pour traitement rapide
        } // if panneau
    } //for

    // LIRE DANS ZDC COULEURS A AFFICHER
    QByteArray couleurs = _zdc->getCouleurs();
    uint8_t couls[MAX_PANS*NB_CIBLES_PAN];
    memcpy(couls, couleurs.constData(), static_cast<size_t>(couleurs.size()));

    // MISE A JOUR DES COULEURS
    for (uint8_t i=0 ; i<nbPanneaux ; i++) {
        _i2c->ecrire(static_cast<uint8_t>(ADR_BASE_PAN+i), couls+3*i, 3);
    } //for

    if (!_pause) { // on relance sauf si pause demandée
        emit sig_info("Relance de l'interrogation des panneaux.");
        emit sig_replay();
    } // if pause
}

void CCommPanneaux::stop()
{
    _pause = true;
}

void CCommPanneaux::start()
{
    _pause = false;
    on_playCommCibles();
}
