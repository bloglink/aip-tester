#include "ConfigPower.h"
#include "ui_ConfigPower.h"

ConfigPower::ConfigPower(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigPower)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
    Testing = false;
    isCheckOk = false;
}

ConfigPower::~ConfigPower()
{
    delete ui;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       初始化界面
*******************************************************************************/
void ConfigPower::WinInit()
{
//    ui->BoxPG->hide();
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabPower->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabPower->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabPG->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabPG->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabPower->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabPower->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabPG->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabPG->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       初始化按键
*******************************************************************************/
void ConfigPower::BtnInit()
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
void ConfigPower::BtnJudge(int id)
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
void ConfigPower::DatInit()
{
    QSettings *global = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");

    //当前使用的测试项目
    QString t = QString("./config/%1").arg(global->value("FileInUse","default.ini").toString());
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetDcr");

    QStringList temp = (set->value("Other","20 0 0.5 10 10 50").toString()).split(" ");
    if (temp.size() >= 6) {
//        ui->BoxStd->setValue(temp.at(0).toDouble());
//        ui->BoxOffset->setValue(temp.at(1).toDouble());
//        ui->BoxTime->setValue(temp.at(2).toDouble());
//        ui->BoxMin->setValue(temp.at(3).toDouble());
//        ui->BoxMax->setValue(temp.at(4).toDouble());
//        ui->BoxUnbalance->setValue(temp.at(5).toDouble());
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       数据保存
*******************************************************************************/
void ConfigPower::DatSave()
{

}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       命令处理
*******************************************************************************/
void ConfigPower::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_PWR && addr != CAN_ID_PWR)
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
void ConfigPower::ExcuteCanCmd(QByteArray msg)
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
void ConfigPower::TestInit()
{
//    Items.clear();
//    Results.clear();
//    QStringList n;
//    for (int row = 0; row<Enable.size(); row++) {
//        QStringList s;
//        QString T1 = Terminal1.at(qMin(row,Terminal1.size()))->text();
//        QString T2 = Terminal2.at(qMin(row,Terminal2.size()))->text();
//        QString U1 = Unit.at(qMin(row,Unit.size()))->currentText();
//        QString M1 = Min.at(qMin(row,Min.size()))->text();
//        QString M2 = Max.at(qMin(row,Max.size()))->text();
//        s.append(QString(tr("电阻%1-%2")).arg(T1).arg(T2));
//        s.append(QString("%1~%2%3").arg(M1).arg(M2).arg(U1));
//        s.append(" ");
//        s.append(" ");
//        Items.append(s.join("@"));
//    }
//    for (int row = 0; row<Enable.size(); row++) {
//        if (Enable.at(row)->text() == "Y") {
//            n.append(Items.at(row));
//        }
//    }
//    if (ui->BoxUnbalance->value() != 0 && Items.size()>=3) {
//        QStringList s;
//        s.append("电阻平衡");
//        s.append(QString("%1%").arg(ui->BoxUnbalance->value()));
//        s.append(" ");
//        s.append(" ");
//        Items.append(s.join("@"));
//        n.append(Items.last());
//    }
//    emit TransformCmd(ADDR,WIN_CMD_SHOW,n.join("\n").toUtf8());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       检测状态
*******************************************************************************/
void ConfigPower::TestCheck()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x27)<<quint8(0x01)<<quint8(0x00);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Testing = true;
    if (!WaitTestOver(100)) {
        Testing = false;
        QMessageBox::warning(this,tr("警告"),tr("功率板异常"),QMessageBox::Ok);
        emit TransformCmd(ADDR,WIN_CMD_DEBUG,"PWR Error\n");
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       更新状态,求平均并清理测试结果
*******************************************************************************/
void ConfigPower::TestCheckOk(QByteArray)
{
    Testing = false;
    if (!isCheckOk) {
        emit TransformCmd(ADDR,WIN_CMD_DEBUG,"Ir check ok\n");
        isCheckOk = true;
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       开始测试
*******************************************************************************/
void ConfigPower::TestStart(quint8 pos)
{
    if (Testing)
        return;
    qDebug()<<pos;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 v = ui->BoxVolt->value()*10;
    out<<quint16(0x27)<<quint8(0x07)<<quint8(0x01)<<quint8(0x01)
      <<quint8(Time/256)<<quint8(Time%256)<<quint8(v/256)<<quint8(v%256)
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
void ConfigPower::TestResult(QByteArray msg)
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
void ConfigPower::TestStop()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x27)<<quint8(0x01)<<quint8(0x02);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Testing = false;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       配置
*******************************************************************************/
void ConfigPower::TestConfig()
{

}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.16
 * brief:      等待测试结束
*******************************************************************************/
bool ConfigPower::WaitTestOver(quint16 t)
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
void ConfigPower::Delay(int ms)
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
void ConfigPower::showEvent(QShowEvent *)
{
    DatInit();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       退出保存
*******************************************************************************/
void ConfigPower::hideEvent(QHideEvent *)
{
    DatSave();
}
