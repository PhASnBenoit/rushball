#ifndef CCOMMAFFICHAGE_H
#define CCOMMAFFICHAGE_H

#include <QObject>


class CCommAffichage : public QObject
{
    Q_OBJECT
public:
    explicit CCommAffichage(QObject *parent = nullptr, char modeFinJeu = 'S');
    void afficherBienvenue(int duree);
    void afficherTypeJeu(int duree);
    void afficherMenu();
    void afficherMenuSelected(int duree, QString optionMenu);
    void afficherTextePermanent(QString texte);
    void afficherSortieMenu(int duree);
private:
    char _modeFinJeu; // pousavoir si score ou temps

signals:

public slots:
    void on_afficherScores(uint8_t aQuiLeTour);

};

#endif // CCOMMAFFICHAGE_H
