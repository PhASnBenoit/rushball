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
    connect(_jeu, &CJeu::sig_pupitre, this, &CIhm::on_pupitre);
}

CIhm::~CIhm()
{
    delete _jeu;
    delete ui;
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

void CIhm::on_pbTest_clicked()
{
  _jeu->play();
}
