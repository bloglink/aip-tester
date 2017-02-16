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
    Testing = false;
    isCheckOk = false;
}

ConfigLvs::~ConfigLvs()
{
    delete ui;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       初始化界面
*******************************************************************************/
void ConfigLvs::WinInit()
{

}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       初始化按键
*******************************************************************************/
void ConfigLvs::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExit,Qt::Key_0);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       按键功能
*******************************************************************************/
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
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       数据初始化
*******************************************************************************/
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
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       数据保存
*******************************************************************************/
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
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       命令处理
*******************************************************************************/
void ConfigLvs::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_LVS && addr != CAN_ID_PWR)
        return;
    switch (cmd) {
    case CAN_DAT_GET:
        ExcuteCanCmd(msg);
        break;
    case CAN_CMD_CHECK:
        TestCheck();
        break;
    case CAN_CMD_START:
        TestStart(msg.toInt());
        break;
    case CAN_CMD_STOP:
        TestStop();
        break;
    case CAN_CMD_INIT:
        DatInit();
        TestInit();
        TestConfig();
        break;
    default:
        break;
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       CAN命令处理
*******************************************************************************/
void ConfigLvs::ExcuteCanCmd(QByteArray msg)
{
    if (!Testing)
        return;
    TimeOut = 0;
    if (msg.size() == 4 && (quint8)msg.at(0) == 0x00)
        TestCheckOk(msg);
    if (msg.size() == 7 && (quint8)msg.at(0) == 0x01)
        TestResult(msg);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.16
 * brief:      更新显示
*******************************************************************************/
void ConfigLvs::TestInit()
{
    Items.clear();
    QStringList s;
    s.append(QString(tr("低启")));

    double M1 = ui->BoxCurrMin->value();
    double M2 = ui->BoxCurrMax->value();
    double Q1 = ui->BoxPowerMin->value();
    double Q2 = ui->BoxPowerMax->value();
    s.append(QString("%1~%2,%3~%4").arg(M1).arg(M2).arg(Q1).arg(Q2));
    s.append(" ");
    s.append(" ");
    Items.append(s.join("@"));
    emit TransformCmd(ADDR,WIN_CMD_SHOW,Items.join("\n").toUtf8());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       检测状态
*******************************************************************************/
void ConfigLvs::TestCheck()
{

}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       更新状态,求平均并清理测试结果
*******************************************************************************/
void ConfigLvs::TestCheckOk(QByteArray )
{
    Testing = false;

    if (Volt.isEmpty() || Curr.isEmpty() || Power.isEmpty())
        return;
    double vv = 0;
    double rr = 0;
    double pp = 0;
    for (int i=0; i<Volt.size(); i++) {
        vv += Volt.at(i);
        rr += Curr.at(i);
        pp += Power.at(i);
    }
    vv /= Volt.size();
    rr /= Curr.size();
    pp /= Power.size();
    if (abs(vv-ui->BoxVolt->value()) <5)
        vv = ui->BoxVolt->value();
    QString t = QString("%1V,%2mA,%3W").arg(vv).arg(rr/10).arg(pp);
    QString judge = "OK";

    if (rr/100>ui->BoxCurrMax->value() || rr/10<ui->BoxCurrMin->value() )
        judge = "NG";
    if (pp>ui->BoxPowerMax->value() || pp<ui->BoxPowerMin->value() )
        judge = "NG";
    QStringList s = QString(Items.at(0)).split("@");
    if (s.at(2) == " ")
        s[2] = t;
    if (s.at(3) == " ")
        s[3] = judge;
    emit TransformCmd(ADDR,WIN_CMD_ITEM,s.join("@").toUtf8());

    Volt.clear();
    Curr.clear();
    Power.clear();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       开始测试
*******************************************************************************/
void ConfigLvs::TestStart(quint8 pos)
{
    if (Testing)
        return;
    qDebug()<<pos;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 t = ui->BoxTime->value()*10;
    quint16 v = ui->BoxVolt->value()*10;
    out<<quint16(0x27)<<quint8(0x07)<<quint8(0x01)<<quint8(0x01)
      <<quint8(t/256)<<quint8(t%256)<<quint8(v/256)<<quint8(v%256)
     <<quint8(0x00)<<quint8(0x00);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Testing = true;
    if(!WaitTestOver(100)) {
        Testing = false;
        emit TransformCmd(ADDR,WIN_CMD_JUDGE,"NG");
        for (int i=0; i<Items.size(); i++) {
            QStringList s = QString(Items.at(i)).split("@");
            if (s.at(2) == " ")
                s[2] = "---";
            if (s.at(3) == " ")
                s[3] = "NG";
            emit TransformCmd(ADDR,WIN_CMD_ITEM,s.join("@").toUtf8());
        }
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       更新测试数据
*******************************************************************************/
void ConfigLvs::TestResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double c = quint16(msg.at(3)*256)+quint8(msg.at(4));
    double p = quint16(msg.at(5)*256)+quint8(msg.at(6));
    Volt.append(v);
    Curr.append(c);
    Power.append(p);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       停止测试
*******************************************************************************/
void ConfigLvs::TestStop()
{

}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       配置
*******************************************************************************/
void ConfigLvs::TestConfig()
{

}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.16
 * brief:      等待测试结束
*******************************************************************************/
bool ConfigLvs::WaitTestOver(quint16 t)
{
    TimeOut = 0;
    while (Testing) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       延时
*******************************************************************************/
void ConfigLvs::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       更新显示
*******************************************************************************/
void ConfigLvs::showEvent(QShowEvent *)
{
    DatInit();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       退出保存
*******************************************************************************/
void ConfigLvs::hideEvent(QHideEvent *)
{
    DatSave();
}
