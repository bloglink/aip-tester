#include "ConfigBlock.h"
#include "ui_ConfigBlock.h"

ConfigBlock::ConfigBlock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigBlock)
{
    ui->setupUi(this);
}

ConfigBlock::~ConfigBlock()
{
    delete ui;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       初始化界面
*******************************************************************************/
void ConfigBlock::WinInit()
{

}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       初始化按键
*******************************************************************************/
void ConfigBlock::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExit,Qt::Key_2);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       按键功能
*******************************************************************************/
void ConfigBlock::BtnJudge(int id)
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
void ConfigBlock::DatInit()
{
    QSettings *global = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");

    //当前使用的测试项目
    QString t = QString("./config/%1").arg(global->value("FileInUse","default.ini").toString());
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("ConfigBlock");

    ui->BoxVolt->setValue(set->value("Voltage","1").toDouble());
    ui->BoxTime->setValue(set->value("TimeUse","10").toDouble());
    ui->BoxGrade->setValue(set->value("Grade","1").toDouble());
    ui->BoxVoltMax->setValue(set->value("VoltMax","255").toDouble());
    ui->BoxVoltMin->setValue(set->value("VoltMin","1").toDouble());
    ui->BoxCurrMax->setValue(set->value("CurrentMax","5.000").toDouble());
    ui->BoxCurrMin->setValue(set->value("CurrentMin","0.001").toDouble());
    ui->BoxPowerMax->setValue(set->value("PowerMax","1275").toDouble());
    ui->BoxPowerMin->setValue(set->value("PowerMin","0.1").toDouble());
    ui->BoxFreq->setValue(set->value("Freq","1").toDouble());
    ui->BoxCurr->setValue(set->value("Current","0.001").toDouble());
    ui->BoxPower->setValue(set->value("Power","1").toDouble());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       数据保存
*******************************************************************************/
void ConfigBlock::DatSave()
{
    set->setValue("Voltage",QString::number(ui->BoxVolt->value()));
    set->setValue("TimeUse",QString::number(ui->BoxTime->value()));
    set->setValue("Grade",QString::number(ui->BoxGrade->value()));
    set->setValue("VoltMax",QString::number(ui->BoxVoltMax->value()));
    set->setValue("VoltMin",QString::number(ui->BoxVoltMin->value()));
    set->setValue("CurrentMax",QString::number(ui->BoxCurrMax->value()));
    set->setValue("CurrentMin",QString::number(ui->BoxCurrMin->value()));
    set->setValue("PowerMax",QString::number(ui->BoxPowerMax->value()));
    set->setValue("PowerMin",QString::number(ui->BoxPowerMin->value()));
    set->setValue("Freq",QString::number(ui->BoxFreq->value()));
    set->setValue("Current",QString::number(ui->BoxCurr->value()));
    set->setValue("Power",QString::number(ui->BoxPower->value()));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       命令处理
*******************************************************************************/
void ConfigBlock::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_BLOCK && addr != CAN_ID_IR)
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
void ConfigBlock::ExcuteCanCmd(QByteArray msg)
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
void ConfigBlock::TestInit()
{
    Items.clear();
    QStringList s;
    QString U1 = QString::number(ui->BoxVoltMin->value());
    QString U2 = QString::number(ui->BoxVoltMax->value());
    QString C1 = QString::number(ui->BoxCurrMin->value());
    QString C2 = QString::number(ui->BoxCurrMax->value());
    QString P1 = QString::number(ui->BoxPowerMin->value());
    QString P2 = QString::number(ui->BoxPowerMax->value());
    s.append(QString(tr("堵转")));
    s.append(QString("%1~%2V,%3~%4mA,%5~%6W").arg(U1).arg(U2).arg(C1).arg(C2).arg(P1).arg(P2));
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
void ConfigBlock::TestCheck()
{

}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       更新状态,求平均并清理测试结果
*******************************************************************************/
void ConfigBlock::TestCheckOk(QByteArray)
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
 * date:        2016.12.28
 * brief:       定频采集一个波形
*******************************************************************************/
void ConfigBlock::TestSample(void)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x27)<<quint8(0x05)<<quint8(0x03)
      <<quint8(ui->BoxGrade->value())<<quint8(ui->BoxTime->value()/10)
       <<quint8(int(ui->BoxVolt)/256)<<quint8(int(ui->BoxVolt)%256);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       采样波形计算
*******************************************************************************/
void ConfigBlock::TestSampleOver()
{
//    int i,k;
//    int Trough,Trough_Point;  // 波谷
//    int average_data=0,variance=0;
//    int wave_avage = wave[0]->Block_Save_Point.size()/Collect_Cycle;
//    pushButton_Count=0;
//    ui->pushButton_3->setDisabled(true);
//    Collect_pwr_one_cycle_average.clear();
//    if(wave[0]->Block_Save_Point.size()!=0)   //  Collect_pwr_one_cycle_average 平均数
//    {
//        Trough=0;
//        for(k=0;k<Collect_Cycle;k++)
//        {
//            average_data=0;
//            variance=0;
//            for(i=0;i<wave_avage;i++)
//            {
//                average_data += wave[0]->Block_Save_Point.at(i*Collect_Cycle+k).toInt();
//            }
//            average_data = average_data/wave_avage;   // 得到平均数

//            for(i=0;i<wave_avage;i++) // 求取方差
//            {
//                variance += ((average_data-wave[0]->Block_Save_Point.at(i*Collect_Cycle+k).toInt()))*((average_data-wave[0]->Block_Save_Point.at(i*Collect_Cycle+k).toInt()));
//            }
//            variance = variance / wave_avage;
//            Collect_pwr_one_cycle_average.append(QString::number(variance));
//        }
//        qDebug()<<"Collect_pwr_one_cycle_average"<<Collect_pwr_one_cycle_average;
//        Collect_pwr_one_cycle_average.removeAt(Collect_pwr_one_cycle_average.size()-1);
//        Collect_pwr_one_cycle_average.removeAt(1);
//        Collect_pwr_one_cycle_average.removeAt(0);
//        Trough = Collect_pwr_one_cycle_average.at(0).toInt();
//        Trough_Point = 2;


//        for(i=0;i<Collect_pwr_one_cycle_average.size()-1;i++) // 寻找测试点
//        {
//            if((Trough>Collect_pwr_one_cycle_average.at(i+1).toInt())&&(Trough>0))
//            {
//                Trough = Collect_pwr_one_cycle_average.at(i+1).toInt();
//                Trough_Point = i+3;
//            }
//            else if(Trough<Collect_pwr_one_cycle_average.at(i+1).toInt())
//            {
//                break;
//            }
//            else if(Trough==0)
//            {
//                break;
//            }
//        }
//        ui->label_cycle->setText(QString(tr("周期:"))+QString::number(Trough_Point+1));

//        double current_average=0;
//        for(i=0;i<wave_avage;i++)
//        {
//            qDebug()<<"Collect_current_data.at(i*Collect_Cycle+Trough_Point).toInt()"<<Collect_current_data.at(i*Collect_Cycle+Trough_Point).toInt();
//            current_average += Collect_current_data.at(i*Collect_Cycle+Trough_Point).toInt();
//        }
//        current_average = current_average/wave_avage;
//        ui->label_current->setText(QString(tr("电流:"))+QString::number(current_average/1000,'f',3));

//        double pwr_average=0; // 功率平均数
//        for(i=0;i<wave_avage;i++)
//        {
//            qDebug()<<"Collect_pwr_data_copy.at(i*Collect_Cycle+Trough_Point).toInt()"<<Collect_pwr_data_copy.at(i*Collect_Cycle+Trough_Point).toInt();
//            pwr_average += Collect_pwr_data_copy.at(i*Collect_Cycle+Trough_Point).toInt();
//        }
//        pwr_average = pwr_average/wave_avage;
//        ui->label_pwr->setText(QString(tr("功率:"))+QString::number(pwr_average/10,'f',1));
//    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       开始测试
*******************************************************************************/
void ConfigBlock::TestStart(quint8 pos)
{
    if (Testing)
        return;
    qDebug()<<pos;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 t = ui->BoxTime->value()*10;
    quint16 v = ui->BoxVolt->value()*10;
    quint8 g = ui->BoxGrade->value();
    out<<quint16(0x27)<<quint8(0x07)<<quint8(0x01)<<quint8(g)
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
void ConfigBlock::TestResult(QByteArray msg)
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
void ConfigBlock::TestStop()
{

}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       配置
*******************************************************************************/
void ConfigBlock::TestConfig()
{

}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.16
 * brief:      等待测试结束
*******************************************************************************/
bool ConfigBlock::WaitTestOver(quint16 t)
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
void ConfigBlock::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       更新显示
*******************************************************************************/
void ConfigBlock::showEvent(QShowEvent *)
{
    DatInit();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       退出保存
*******************************************************************************/
void ConfigBlock::hideEvent(QHideEvent *)
{
    DatSave();
}
