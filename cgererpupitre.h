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
    void traiterSaisie();

signals:
    void sig_stop();
    void sig_start();
    void sig_afficherScores();


public slots:

};

#endif // CGERERPUPITRE_H
