#include "ConfigLvs.h"
#include "ui_ConfigLvs.h"

ConfigLvs::ConfigLvs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigLvs)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
}

ConfigLvs::~ConfigLvs()
{
    delete ui;
}

void ConfigLvs::WinInit()
{

}

void ConfigLvs::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExit,Qt::Key_0);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}

void ConfigLvs::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        emit TransformCmd(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    default:
        break;
    }
}

void ConfigLvs::DatInit()
{
    QSettings *global = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");

    //当前使用的测试项目
    QString t = QString("./config/%1").arg(global->value("FileInUse","default.ini").toString());
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetLvs");

    QStringList temp = (set->value("Other","20 0 0.5 10 10 50").toString()).split(" ");
    if (temp.size() >= 6) {
        ui->BoxVolt->setValue(temp.at(0).toDouble());
        ui->BoxTime->setValue(temp.at(1).toDouble());
        ui->BoxCurrMin->setValue(temp.at(2).toDouble());
        ui->BoxCurrMax->setValue(temp.at(3).toDouble());
        ui->BoxPowerMin->setValue(temp.at(4).toInt());
        ui->BoxPowerMax->setValue(temp.at(5).toInt());
    }

}

void ConfigLvs::DatSave()
{

}
