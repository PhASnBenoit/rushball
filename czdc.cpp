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

QByteArray CZdc::getCouleurs()
{
    QByteArray couleurs;
    lock();
        for (int i=0 ; i<_adrZdc->datasStatic.nbPanneaux ; i++)
            for(int j=0 ; i<NB_CIBLES_PAN ; i++)
                couleurs.append(_adrZdc->datasDyn.couleurCibles[i][j]);
    unlock();
    return couleurs;
}

QByteArray CZdc::getCibles()
{
    QByteArray cibles;
    lock();
        for (int i=0 ; i<_adrZdc->datasStatic.nbPanneaux ; i++)
            for(int j=0 ; i<NB_CIBLES_PAN ; i++)
                cibles.append(_adrZdc->datasDyn.toucheCibles[i][j]);
    unlock();
    return cibles;
}

QByteArray CZdc::getCiblesByPanneau(uint8_t noPan)
{
    QByteArray cibles1Panneau;
    cibles1Panneau.clear();

    if (noPan>getNbPanneaux())
        return cibles1Panneau;

    lock();
        for (int i=0 ; i<NB_CIBLES_PAN ; i++)
            cibles1Panneau.append(_adrZdc->datasDyn.toucheCibles[noPan][i]);
    unlock();
    return cibles1Panneau;
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

uint8_t CZdc::getNbPanneaux()
{
    uint8_t nbPan;
    lock();
        nbPan = _adrZdc->datasStatic.nbPanneaux;
    unlock();
    return nbPan;
}

QByteArray CZdc::getCouleursByPanneau(uint8_t noPan)
{
    QByteArray couls1Panneau;
    couls1Panneau.clear();

    if (noPan>getNbPanneaux())
        return couls1Panneau;

    lock();
        for (int i=0 ; i<NB_CIBLES_PAN ; i++)
            couls1Panneau.append(_adrZdc->datasDyn.couleurCibles[noPan][i]);
    unlock();
    return couls1Panneau;
}

void CZdc::setEtatJeu(const uint8_t &etat)
{
    lock();
        _adrZdc->datasDyn.etat_jeu = etat;
    unlock();
}

void CZdc::appliquerNewParams(T_DATAS_STATIC *ds)
{
    clear(); // réinitialise la zone mémoire
    lock();
        memcpy(&_adrZdc->datasStatic, ds, sizeof(T_DATAS_STATIC));
    unlock();
}
