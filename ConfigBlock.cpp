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

void ConfigBlock::BtnInit()
{

}

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

    ui->BoxVoltage->setValue(set->value("Voltage","1").toDouble());
    ui->BoxTimeUse->setValue(set->value("TimeUse","10").toDouble());
    ui->BoxGrade->setValue(set->value("Grade","1").toDouble());
    ui->BoxVoltMax->setValue(set->value("VoltMax","255").toDouble());
    ui->BoxVoltMin->setValue(set->value("VoltMin","1").toDouble());
    ui->BoxCurrentMax->setValue(set->value("CurrentMax","5.000").toDouble());
    ui->BoxCurrentMin->setValue(set->value("CurrentMin","0.001").toDouble());
    ui->BoxPowerMax->setValue(set->value("PowerMax","1275").toDouble());
    ui->BoxPowerMin->setValue(set->value("PowerMin","0.1").toDouble());
    ui->BoxFreq->setValue(set->value("Freq","1").toDouble());
    ui->BoxCurrent->setValue(set->value("Current","0.001").toDouble());
    ui->BoxPower->setValue(set->value("Power","1").toDouble());
}

void ConfigBlock::DatSave()
{
    set->setValue("Voltage",QString::number(ui->BoxVoltage->value()));
    set->setValue("TimeUse",QString::number(ui->BoxTimeUse->value()));
    set->setValue("Grade",QString::number(ui->BoxGrade->value()));
    set->setValue("VoltMax",QString::number(ui->BoxVoltMax->value()));
    set->setValue("VoltMin",QString::number(ui->BoxVoltMin->value()));
    set->setValue("CurrentMax",QString::number(ui->BoxCurrentMax->value()));
    set->setValue("CurrentMin",QString::number(ui->BoxCurrentMin->value()));
    set->setValue("PowerMax",QString::number(ui->BoxPowerMax->value()));
    set->setValue("PowerMin",QString::number(ui->BoxPowerMin->value()));
    set->setValue("Freq",QString::number(ui->BoxFreq->value()));
    set->setValue("Current",QString::number(ui->BoxCurrent->value()));
    set->setValue("Power",QString::number(ui->BoxPower->value()));
}



void ConfigBlock::DisplayInit()
{
    ListItem.clear();
    ListPara.clear();
    ListResult.clear();
    ListJudge.clear();
    QString U1 = QString::number(ui->BoxVoltMin->value());
    QString U2 = QString::number(ui->BoxVoltMax->value());
    QString C1 = QString::number(ui->BoxCurrentMin->value());
    QString C2 = QString::number(ui->BoxCurrentMax->value());
    QString P1 = QString::number(ui->BoxPowerMin->value());
    QString P2 = QString::number(ui->BoxPowerMax->value());
    ListItem.append(QString(tr("堵转")));
    ListPara.append(QString("%1~%2V,%3~%4mA,%5~%6W").arg(U1).arg(U2).arg(C1).arg(C2).arg(P1).arg(P2));
    ListResult.append(" ");
    ListJudge.append(" ");
}

void ConfigBlock::ExcuteCmd(QByteArray msg)
{

}

void ConfigBlock::CmdCheckState()
{

}

void ConfigBlock::CmdStartTest(quint8 pos)
{

}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       定频采集一个波形
*******************************************************************************/
void ConfigBlock::CmdSample(void)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x27)<<quint8(0x05)<<quint8(0x03)
      <<quint8(ui->BoxGrade->value())<<quint8(ui->BoxTimeUse->value()/10)
       <<quint8(int(ui->BoxVoltage)/256)<<quint8(int(ui->BoxVoltage)%256);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
}
void ConfigBlock::SampleOver()
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
void ConfigBlock::CmdStopTest()
{

}

void ConfigBlock::CmdConfigure()
{

}

bool ConfigBlock::WaitTestOver()
{
//    TimeOut = 0;
//    while (Testing) {
//        Delay(10);
//        TimeOut++;
//        if (TimeOut > 50) {
//            for (int i=0; i<ListResult.size(); i++) {
//                if (ListResult.at(i) == " ") {
//                    ListResult[i] = "---";
//                }
//            }
//            for (int i=0; i<ListJudge.size(); i++) {
//                if (ListJudge.at(i) == " ") {
//                    ListJudge[i] = "NG";
//                }
//            }
//            Testing = false;
//            emit TransformCmd(ADDR,WIN_CMD_RESULT,NULL);
//            return false;
//        }
//    }
    return true;
}

void ConfigBlock::showEvent(QShowEvent *)
{
    DatInit();
}

void ConfigBlock::hideEvent(QHideEvent *)
{
    DatSave();
}
