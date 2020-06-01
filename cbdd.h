#ifndef CBDD_H
#define CBDD_H

#include <QObject>
#include <QDebug>

class CBdd : public QObject
{
    Q_OBJECT
public:
    explicit CBdd(QObject *parent = nullptr);
    bool verifierParamsConnexion(QString login, QString mdp);

signals:

public slots:
};

#endif // CBDD_H
