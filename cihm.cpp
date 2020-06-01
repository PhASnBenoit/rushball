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
