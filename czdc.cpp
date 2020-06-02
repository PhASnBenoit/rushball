#include "czdc.h"

CZdc::CZdc()
{
    setKey(KEY);
    if (!attach())
        if (!create(sizeof(T_ZDC))) { // RW
            emit sig_erreur("CZdc::CZdc Erreur de création de la SHM");
            qDebug() << "CZdc::CZdc Erreur de création de la SHM";
        } // if erreur
    _adrZdc = static_cast<T_ZDC *>(data());
    clear();  // toutes les infos à 0
}

CZdc::~CZdc()
{
    detach();
}

T_COULEURS* CZdc::getCouleurs()
{
    T_COULEURS *couleurs;
    lock();
        couleurs = &_adrZdc->datasDyn.couleurCibles[0][0];
    unlock();
    return couleurs;
}

bool* CZdc::getCibles()
{
    bool *cibles;
    lock();
        cibles = &_adrZdc->datasDyn.toucheCibles[0][0];
    unlock();
    return cibles;
}

bool *CZdc::getCiblesByPanneau(uint8_t noPan)
{
    bool *cibles;
    lock();
        cibles = &_adrZdc->datasDyn.toucheCibles[noPan][0];
    unlock();
    return cibles;
}

void CZdc::setCouleurs(T_COULEURS *tabCouleurs)
{
    lock();
        memcpy(_adrZdc->datasDyn.couleurCibles, tabCouleurs, MAX_PANS*NB_CIBLES_PAN);
    unlock();
}

void CZdc::setCibles(bool *tabCibles)
{
    lock();
        memcpy(_adrZdc->datasDyn.toucheCibles, tabCibles, MAX_PANS*NB_CIBLES_PAN);
    unlock();
}

void CZdc::clear()
{
    // RAZ de toutes les informations
    lock();
        bzero(_adrZdc, sizeof(T_ZDC));
    unlock();
}

uint8_t CZdc::etatJeu()
{
    uint8_t etat;
    lock();
        etat = _adrZdc->datasDyn.etat_jeu;
    unlock();
    return etat;
}

void CZdc::setEtatJeu(const uint8_t &etat)
{
    lock();
        _adrZdc->datasDyn.etat_jeu = etat;
        unlock();
}

void CZdc::appliquerNewParams(T_DATAS_STATIC *ds)
{
    lock();
    clear(); // réinitialise la zone mémoire
    memcpy(&_adrZdc->datasStatic, ds, sizeof(T_DATAS_STATIC));
    unlock();
}
