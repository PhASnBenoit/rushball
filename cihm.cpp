#include "cihm.h"
#include "ui_cihm.h"


CIhm::CIhm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CIhm)
{
    ui->setupUi(this);
    _jeu = new CJeu();
}

CIhm::~CIhm()
{
    delete  _jeu;
    delete ui;
}

void CIhm::on_pbGo_clicked()
{
    emit sig_play();
}

void CIhm::on_erreurJeu(QString mess)
{
    ui->teSuivi->append(mess);
}

void CIhm::on_info(QString mess)
{
    ui->teSuivi->append(mess);
}

void CIhm::on_pupitre(QString mess)
{
    ui->teSuivi->append(mess);
}

void CIhm::keyPressEvent(QKeyEvent *event)
{
    // frontal de réception des touches du pupitre
    // emission d'un signal vers CJeu qui emettra un signal vers CGererPupitre
    // CGererPupitre::on_toucheRecue() traitera la touche et formera la saisie selon le protocole établi.
    // CGererPupitre::on_traiterSaisie() gère la machine à état du pupitre
    setWindowTitle(tr("KeyPressEvent!"));
    int touche = event->key();
    on_info("Une touche est pressée : " + QString::number(touche));
    emit sig_toucheRecue(touche);
    QWidget::keyPressEvent(event); // détournement terminé, compportement par défaut
}

//void CIhm::keyReleaseEvent(QKeyEvent *event) {
//  setWindowTitle(tr("KeyReleaseEvent!"));
//}
