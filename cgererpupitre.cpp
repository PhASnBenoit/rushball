#include "cgererpupitre.h"

CGererPupitre::CGererPupitre(QObject *parent, CCommAffichage *aff) : QObject(parent)
{
    _etatPupitre = ETAT_PUPITRE_HORS_MENU;
    _aff = aff;
}

CGererPupitre::~CGererPupitre()
{
}

void CGererPupitre::on_toucheRecue(int touche)
{
    if ( (_etatPupitre==ETAT_PUPITRE_HORS_MENU)
        &(touche!=Qt::Key_Slash))
            return;

    // filtre touches autorisées
    switch (touche)
    {
    case Qt::Key_Slash:
        _etatPupitre = ETAT_PUPITRE_CHOIX_MENU;
        emit sig_stop();
        _aff->afficherMenu();
    break;
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        break;
    case Qt::Key_Return:
        return;
    default:  // autre touche
        break;
    } // sw
}

void CGererPupitre::traiterSaisie()
{
    // machine à états
}
