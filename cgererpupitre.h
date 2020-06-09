#ifndef CGERERPUPITRE_H
#define CGERERPUPITRE_H

#include <QObject>
#include <QTextStream>

class CGererPupitre : public QObject
{
    Q_OBJECT
public:
    explicit CGererPupitre(QObject *parent = nullptr);
    ~CGererPupitre();

private:

 //   bool _stop;

signals:
    void sig_saisiePupitre(QByteArray chaine);

public slots:
    void on_lirePupitre();
 //   void on_stopLirePupitre();
};

#endif // CGERERPUPITRE_H
