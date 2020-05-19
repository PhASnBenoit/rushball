#include "cihm.h"
#include "ui_cihm.h"

CIhm::CIhm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CIhm)
{
    ui->setupUi(this);

    // composition dynamique
    _jeu = new CJeu(this);
}

CIhm::~CIhm()
{
    delete _jeu;
    delete ui;
}
