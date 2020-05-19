#ifndef CZDC_H
#define CZDC_H
// ZDC : Zone De Données Communes

#include <QObject>
#include <QSharedMemory>

#include "communs.h"  // Définition de la ZDC

class CZdc : public QSharedMemory
{
public:
    CZdc();
    void getCouleurs(uint8_t *tabCouleurs);
    void getCibles(bool *tabCibles);
    void setCouleurs(uint8_t *tabCouleurs);
    void setCibles(bool *tabCibles);

private:
    T_DATAS _datas;
    QString _key;
};

#endif // CZDC_H
