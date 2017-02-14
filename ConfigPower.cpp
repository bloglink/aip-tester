#include "ConfigPower.h"
#include "ui_ConfigPower.h"

ConfigPower::ConfigPower(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigPower)
{
    ui->setupUi(this);
}

ConfigPower::~ConfigPower()
{
    delete ui;
}
