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

void CZdc::getCouleurs(T_COULEURS *tabCouleurs)
{
    lock();
    unlock();
}

void CZdc::getCibles(bool *tabCibles)
{
    lock();
    unlock();
}

void CZdc::setCouleurs(T_COULEURS *tabCouleurs)
{
    lock();
    unlock();
}

void CZdc::setCibles(bool *tabCibles)
{
    lock();
    unlock();
}
