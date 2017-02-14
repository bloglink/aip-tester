#include "CWinDbug.h"
#include "ui_CWinDbug.h"

CWinDbug::CWinDbug(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CWinDbug)
{
    ui->setupUi(this);
}

CWinDbug::~CWinDbug()
{
    delete ui;
}
