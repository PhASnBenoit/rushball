#ifndef CCOMMAFFICHAGE_H
#define CCOMMAFFICHAGE_H

#include <QObject>

class CCommAffichage : public QObject
{
    Q_OBJECT
public:
    explicit CCommAffichage(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CCOMMAFFICHAGE_H