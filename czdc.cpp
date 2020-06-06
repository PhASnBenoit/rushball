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
    clear();  // init de toute la ZDC
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

uint8_t CZdc::getNbCouleurs()
{
    uint8_t nbCouls;
    lock();
        nbCouls = _adrZdc->datasStatic.nbCouleurs;
    unlock();
    return nbCouls;
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

int CZdc::setCiblesPour1Panneau(int noPan, uint8_t cibles)
{
    if (noPan>getNbPanneaux())
        return -1;

    lock();
        for (int i=0 ; i<NB_CIBLES_PAN ; i++)
            _adrZdc->datasDyn.toucheCibles[noPan][i] = cibles&((i+1)*2);
    unlock();
    return 0;
}

void CZdc::clear()
{
    // RAZ de toutes les informations
    lock();
        bzero(_adrZdc, sizeof(T_ZDC));
        _adrZdc->datasDyn.aQuiLeTour=255;
        _adrZdc->datasDyn.etat_jeu=ETAT_JEU_ATTENTE_CONNEXION;
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

uint8_t CZdc::getNbJoueurs()
{
    uint8_t nbJoueurs;
    lock();
        nbJoueurs = _adrZdc->datasStatic.nbJoueurs;
    unlock();
    return nbJoueurs;
}

QList<QString> CZdc::getNomJoueurs()
{
    QList<QString> nomJoueurs;
    lock();
        for (int i=0 ; i<_adrZdc->datasStatic.nbJoueurs ; i++) {
                nomJoueurs.append(QString(_adrZdc->datasStatic.nomJoueurs[i]));
        } // for
    unlock();
    return nomJoueurs;
}

QList<uint16_t> CZdc::getScores()
{
    QList<uint16_t> scores;
    lock();
        for (int i=0 ; i<_adrZdc->datasStatic.nbJoueurs ; i++) {
                scores.append(_adrZdc->datasDyn.scores[i]);
        } // for
    unlock();
    return scores;
}

QString CZdc::getNomJoueur(uint8_t num)
{
    QString nomJoueur="";
    if (num > getNbJoueurs())
        return nomJoueur;
    lock();
        nomJoueur = QString(_adrZdc->datasStatic.nomJoueurs[num]);
    unlock();
    return nomJoueur;
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

char CZdc::getModeJeu()
{
    char mode;
    lock();
        mode = _adrZdc->datasStatic.modeJeu;
    unlock();
    return mode;
}

char CZdc::getModeFinJeu()
{
    char fin;
    lock();
        fin = _adrZdc->datasStatic.modeFinJeu;
    unlock();
    return fin;
}

uint8_t CZdc::joueurSuivant()
{
    uint8_t val;
    lock();
        if (_adrZdc->datasDyn.aQuiLeTour==255) {
            val = _adrZdc->datasDyn.aQuiLeTour = 0;
        } else {
            _adrZdc->datasDyn.aQuiLeTour++;
            val = _adrZdc->datasDyn.aQuiLeTour;
            if (_adrZdc->datasDyn.aQuiLeTour >= _adrZdc->datasStatic.nbJoueurs)
                val = _adrZdc->datasDyn.aQuiLeTour = 0;
        } // else
    unlock();
    return val;
}

uint8_t CZdc::getAQuiLeTour()
{
    uint8_t val;
    lock();
        val = _adrZdc->datasDyn.aQuiLeTour;
    unlock();
    return val;
}
