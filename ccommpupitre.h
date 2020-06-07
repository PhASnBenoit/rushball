#ifndef CCOMMPUPITRE_H
#define CCOMMPUPITRE_H

#include <QObject>

class CCommPupitre : public QObject
{
    Q_OBJECT
public:
    explicit CCommPupitre(QObject *parent = nullptr);

signals:
        void sig_arret();

public slots:
};

#endif // CCOMMPUPITRE_H
