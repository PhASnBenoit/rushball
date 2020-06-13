#ifndef CGERERPUPITRE_H
#define CGERERPUPITRE_H

#include <QObject>
#include <QTextStream>
#include "ccommaffichage.h"
#include "communs.h"

class CGererPupitre : public QObject
{
    Q_OBJECT
public:
    explicit CGererPupitre(QObject *parent = nullptr, CCommAffichage *aff = nullptr);
    ~CGererPupitre();
    void on_toucheRecue(int touche);

private:
    int _etatPupitre;
    CCommAffichage *_aff;
    void traiterSaisie(int touche);
    int _touches[5];  // sauvgarde d'un ensemble de touches
    int _ind; // indice de sauvegarde touche

signals:
    void sig_stop();
    void sig_start();
    void sig_reqAffScores();

public slots:

};

#endif // CGERERPUPITRE_H
