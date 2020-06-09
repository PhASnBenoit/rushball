#ifndef CIHM_H
#define CIHM_H

#include <QMainWindow>

#include "cjeu.h"

namespace Ui {
class CIhm;
}

class CIhm : public QMainWindow
{
    Q_OBJECT

public:
    explicit CIhm(QWidget *parent = nullptr);
    ~CIhm();

private:
    Ui::CIhm *ui;
    CJeu *_jeu;  // composition dynamique

private slots:
    void on_erreurJeu(QString mess);
    void on_info(QString mess);
    void on_pupitre(QString mess);
    void on_pbTest_clicked();
};

#endif // CIHM_H
