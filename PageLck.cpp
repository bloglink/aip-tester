#include "PageLck.h"
#include "ui_PageLck.h"

PageLck::PageLck(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageLck)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
    Mode = LCK_FREE;
}

PageLck::~PageLck()
{
    delete ui;
}

void PageLck::InitWindows()
{

}

void PageLck::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSample,Qt::Key_0);
    btnGroup->addButton(ui->BtnExit,Qt::Key_2);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}

void PageLck::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        Mode = LCK_SAMPLE;
        Mode = LCK_FREE;
        break;
    case Qt::Key_2:
        emit SendCommand(ADDR,CMD_JUMP,NULL);
        break;
    default:
        break;
    }
}

void PageLck::InitSettings()
{
    QSettings *global = new QSettings(INI_PATH,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");

    //当前使用的测试项目
    QString t = QString("./config/%1").arg(global->value("FileInUse",INI_DEFAULT).toString());
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("PageLck");

    ui->BoxVolt->setValue(set->value("Voltage","220").toDouble());
    ui->BoxTime->setValue(set->value("TimeUse","1").toDouble());
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

void PageLck::SaveSettings()
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

void PageLck::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_LCK && addr != CAN_ID_PWR)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(msg);
        break;
    case CMD_CHECK:
        break;
    case CMD_START:
        SendCanCmdStart(msg.toInt());
        break;
    case CMD_STOP:
        SendCanCmdStop();
        break;
    case CMD_INIT:
        InitSettings();
        InitTestItems();
        break;
    default:
        break;
    }
}

void PageLck::ExcuteCanCmd(QByteArray msg)
{
    if (Mode == LCK_FREE)
        return;
    TimeOut = 0;
    if (msg.size() == 4 && (quint8)msg.at(0) == 0x00)
        ReadCanCmdStatus(msg);
    if (msg.size() == 7 && (quint8)msg.at(0) == 0x01) {
        if (Mode == LCK_TEST)
            ReadCanCmdResult(msg);
        if (Mode == LCK_SAMPLE)
            ReadCanCmdSample(msg);
    }
}

void PageLck::InitTestItems()
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
    emit SendCommand(ADDR,CMD_INIT_ITEM,Items.join("\n").toUtf8());
}


void PageLck::ReadCanCmdStatus(QByteArray)
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
    emit SendCommand(ADDR,CMD_ITEM,s.join("@").toUtf8());

    Volt.clear();
    Curr.clear();
    Power.clear();
}

void PageLck::ReadCanCmdSample(QByteArray msg)
{

}

void PageLck::TestSample(void)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x27)<<quint8(0x05)<<quint8(0x03)
      <<quint8(ui->BoxGrade->value())<<quint8(ui->BoxTime->value()/10)
     <<quint8(int(ui->BoxVolt)/256)<<quint8(int(ui->BoxVolt)%256);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void PageLck::TestSampleOver()
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

void PageLck::SendCanCmdStart(quint8 pos)
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
    emit SendCommand(ADDR,CMD_CAN,msg);
    Testing = true;
    if(!WaitTimeOut(100)) {
        Testing = false;
        emit SendCommand(ADDR,CMD_JUDGE,"NG");
        for (int i=0; i<Items.size(); i++) {
            QStringList s = QString(Items.at(i)).split("@");
            if (s.at(2) == " ")
                s[2] = "---";
            if (s.at(3) == " ")
                s[3] = "NG";
            emit SendCommand(ADDR,CMD_ITEM,s.join("@").toUtf8());
        }
    }
}

void PageLck::ReadCanCmdResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double c = quint16(msg.at(3)*256)+quint8(msg.at(4));
    double p = quint16(msg.at(5)*256)+quint8(msg.at(6));
    Volt.append(v);
    Curr.append(c);
    Power.append(p);
}

void PageLck::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x27)<<quint8(0x01)<<quint8(0x02);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

bool PageLck::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (Mode != LCK_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageLck::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

void PageLck::showEvent(QShowEvent *)
{
    InitSettings();
}

void PageLck::hideEvent(QHideEvent *)
{
    SaveSettings();
}
/*********************************END OF FILE**********************************/
