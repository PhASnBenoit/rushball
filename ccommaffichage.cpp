#include "ccommaffichage.h"

CCommAffichage::CCommAffichage(QObject *parent, char modeFinJeu) : QObject(parent)
{
    _modeFinJeu = modeFinJeu;
}

void CCommAffichage::afficherBienvenue(int duree)
{

}

void CCommAffichage::afficherTypeJeu(int duree)
{

}

void CCommAffichage::afficherMenu()
{

}

void CCommAffichage::afficherMenuSelected(int duree, QString optionMenu)
{

}

void CCommAffichage::afficherTextePermanent(QString texte)
{

}

void CCommAffichage::on_afficherScores(uint8_t aQuiLeTour)
{

}
