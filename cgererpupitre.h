#ifndef CGERERPUPITRE_H
#define CGERERPUPITRE_H

#include <QObject>
#include <QTextStream>
#include "communs.h"

class CGererPupitre : public QObject
{
    Q_OBJECT
public:
    explicit CGererPupitre(QObject *parent = nullptr);
    ~CGererPupitre();
    void on_toucheRecue(int touche);

private:
    int _etatPupitre;
    void traiterSaisie(int touche);
    int _touches[5];  // sauvgarde d'un ensemble de touches
    int _ind; // indice de sauvegarde touche
    int _noJ;
    uint16_t _score;

signals:
    void sig_stop();
    void sig_start();

    void sig_info(QString mess);
    void sig_erreur(QString mess);

    void sig_affScores();
    void sig_affQuelJoueur();
    void sig_affJoueurChoisi(int noJ);
    void sig_affMenu();
    void sig_affDureeTexte(int duree, QString texte);
    void sig_affValScore(int val);

    void sig_appliquerPenalite(int noJ);
    void sig_appliquerChangeJoueur(int noJoueur);
    void sig_appliquerChangeScore(int noJoueur, int val);

public slots:

};

#endif // CGERERPUPITRE_H
