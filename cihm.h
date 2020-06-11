#ifndef CIHM_H
#define CIHM_H

#include <QMainWindow>
#include <QKeyEvent>

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

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::CIhm *ui;
    CJeu *_jeu;  // composition dynamique

signals:
    void sig_toucheRecue(int touche); // vers CJeu

private slots:
    void on_erreurJeu(QString mess);
    void on_info(QString mess);
    void on_pupitre(QString mess);
    void on_pbTest_clicked();
};

#endif // CIHM_H
