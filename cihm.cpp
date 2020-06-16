#include "cihm.h"
#include "ui_cihm.h"

CIhm::CIhm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CIhm)
{
    ui->setupUi(this);

    // composition dynamique
    _jeu = new CJeu(this);
    connect(_jeu, &CJeu::sig_erreur, this, &CIhm::on_erreurJeu);
    connect(_jeu, &CJeu::sig_info, this, &CIhm::on_info);
    connect(this, &CIhm::sig_toucheRecue, _jeu, &CJeu::on_toucheRecue);
    connect(this, &CIhm::sig_cibleTouched, _jeu, &CJeu::on_cibleTouchee);  // pour simulation vers CJeu
}

CIhm::~CIhm()
{
    delete _jeu;
    delete ui;
}

void CIhm::keyPressEvent(QKeyEvent *event)
{
    // frontal de réception des touches du pupitre
    // emission d'un signal vers CJeu qui emettra un signal vers CGererPupitre
    // CGererPupitre::on_toucheRecue() traitera la touche et formera la saisie selon le protocole établi.
    // CGererPupitre::on_traiterSaisie() gère la machine à état du pupitre
    int touche = event->key();
    on_info("Une touche est pressée : " + QString::number(touche));
    emit sig_toucheRecue(touche);
    QWidget::keyPressEvent(event); // détournement terminé, compportement par défaut
}

void CIhm::on_erreurJeu(QString mess)
{
    ui->teErreurs->append(mess);
}

void CIhm::on_info(QString mess)
{
    ui->teSuivi->append(mess);
}

void CIhm::on_pupitre(QString mess)
{
    ui->tePupitre->append(mess);
}

void CIhm::on_pbCibleTouched_clicked()
{
    emit sig_cibleTouched(static_cast<uint8_t>(ui->sbPan->value()),static_cast<uint8_t>(ui->sbCibles->value()));
}
