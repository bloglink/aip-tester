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
        ui->BoxPowerMin->setValue(temp.at(4).toDouble());
        ui->BoxPowerMax->setValue(temp.at(5).toDouble());
    }

}

void ConfigLvs::DatSave()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxVolt->value()));
    temp.append(QString::number(ui->BoxTime->value()));
    temp.append(QString::number(ui->BoxCurrMin->value()));
    temp.append(QString::number(ui->BoxCurrMax->value()));
    temp.append(QString::number(ui->BoxPowerMin->value()));
    temp.append(QString::number(ui->BoxPowerMax->value()));
    set->setValue("Other",(temp.join(" ").toUtf8()));
}

void ConfigLvs::UpdateState(QByteArray msg)
{
    if (!isCheckOk) {
        isCheckOk = true;
        emit TransformCmd(ADDR,WIN_CMD_DEBUG,"INDL check ok\n");
    }
    if (Testing)
        Testing = false;
}

void ConfigLvs::UpdateTestData(QByteArray msg)
{
    qDebug()<<msg;
}

void ConfigLvs::UpdateResult(QByteArray msg)
{
    for (int i=0; i<ListResult.size(); i++) {
        if (ListResult.at(i) == " ") {
            ListResult[i] = msg;
            break;
        }
    }
}

void ConfigLvs::UpdateJudge(QByteArray msg)
{
    for (int i=0; i<ListJudge.size(); i++) {
        if (ListJudge.at(i) == " ") {
            ListJudge[i] = msg;
            break;
        }
    }
}

void ConfigLvs::UpdateUnbalance()
{

}

void ConfigLvs::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}
void ConfigLvs::DisplayInit()
{
    ListItem.clear();
    ListPara.clear();
    ListResult.clear();
    ListJudge.clear();
    ListItem.append(QString(tr("低启")));
    double M1 = ui->BoxCurrMin->value();
    double M2 = ui->BoxCurrMax->value();
    double Q1 = ui->BoxPowerMin->value();
    double Q2 = ui->BoxPowerMax->value();
    ListPara.append(QString("%1~%2,%3~%4").arg(M1).arg(M2).arg(Q1).arg(Q2));
    ListResult.append(" ");
    ListJudge.append(" ");
}

void ConfigLvs::ExcuteCmd(QByteArray msg)
{

}

void ConfigLvs::CmdCheckState()
{

}

void ConfigLvs::CmdStartTest(quint8 pos)
{

}

void ConfigLvs::CmdStopTest()
{

}

void ConfigLvs::CmdConfigure()
{

}

bool ConfigLvs::WaitTestOver()
{
    return true;
}
