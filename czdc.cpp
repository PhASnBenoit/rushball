#include "czdc.h"

CZdc::CZdc()
{
    _key = KEY;
    if (!create(sizeof(T_ZDC))) { // RW
        emit sig_error("CZdc::CZdc Erreur de création de la SHM");
        qDebug() << "CZdc::CZdc Erreur de création de la SHM";
    } // if erreur
    _zdc = static_cast<T_ZDC *>(data());
}

CZdc::~CZdc()
{
    detach();
}

T_COULEURS* CZdc::getCouleurs()
{
    T_COULEURS *couleurs;
    lock();
    couleurs = &_zdc->datasDyn.couleurs[0][0];
    unlock();
    return couleurs;
}

bool* CZdc::getCibles()
{
    bool *cibles;
    lock();
    cibles = &_zdc->datasDyn.cibles[0][0];
    unlock();
    return cibles;
}

void CZdc::setCouleurs(T_COULEURS *tabCouleurs)
{
    lock();
    memcpy(_zdc->datasDyn.couleurs, tabCouleurs, MAX_PANS*NB_CIBLES_PAN);
    unlock();
}

void CZdc::setCibles(bool *tabCibles)
{
    lock();
    memcpy(_zdc->datasDyn.cibles, tabCibles, MAX_PANS*NB_CIBLES_PAN);
    unlock();
}
