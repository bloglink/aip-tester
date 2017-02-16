/********************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       绝缘设置界面
 * date:        2017.01.13
 * brief:       加入求平均
********************************************************************************/
#include "CWinSetAcw.h"
#include "ui_CWinSetAcw.h"
/********************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       构造
********************************************************************************/
CWinSetAcw::CWinSetAcw(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CWinSetAcw)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
    Testing = false;
    isCheckOk = false;
}
/********************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       析构
********************************************************************************/
CWinSetAcw::~CWinSetAcw()
{
    delete ui;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       初始化界面
*******************************************************************************/
void CWinSetAcw::WinInit()
{
    ui->BoxArc->setView(new QListView(this));
    ui->BoxFrequcy->setView(new QListView(this));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       初始化按键
*******************************************************************************/
void CWinSetAcw::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExitAcw,Qt::Key_2);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       按键功能
*******************************************************************************/
void CWinSetAcw::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        break;
    case Qt::Key_1:
        break;
    case Qt::Key_2:
        emit TransformCmd(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    default:
        break;
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       数据初始化
*******************************************************************************/
void CWinSetAcw::DatInit()
{
    QSettings *global = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    FileInUse = global->value("FileInUse","default.ini").toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(FileInUse);
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetAcw");

    QStringList temp = (set->value("Other","500 0 5.00 3 0 0 0").toString()).split(" ");
    if (temp.size() >= 7) {
        ui->BoxVoltage->setValue(temp.at(0).toDouble());
        ui->BoxMin->setValue(temp.at(1).toDouble());
        ui->BoxMax->setValue(temp.at(2).toDouble());
        ui->BoxTime->setValue(temp.at(3).toDouble());
        ui->BoxArc->setCurrentIndex(temp.at(4).toInt());
        ui->BoxFrequcy->setCurrentIndex(temp.at(5).toInt());
        ui->BoxOffset->setValue(temp.at(6).toInt());
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       数据保存
*******************************************************************************/
void CWinSetAcw::DatSave()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxVoltage->value()));
    temp.append(QString::number(ui->BoxMin->value()));
    temp.append(QString::number(ui->BoxMax->value()));
    temp.append(QString::number(ui->BoxTime->value()));
    temp.append(QString::number(ui->BoxArc->currentIndex()));
    temp.append(QString::number(ui->BoxFrequcy->currentIndex()));
    temp.append(QString::number(ui->BoxOffset->value()));
    set->setValue("Other",(temp.join(" ").toUtf8()));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.15
 * brief:       命令处理
*******************************************************************************/
void CWinSetAcw::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_ACW && addr != CAN_ID_IR)
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
 * date:        2016.12.20
 * brief:       CAN命令处理
*******************************************************************************/
void CWinSetAcw::ExcuteCanCmd(QByteArray msg)
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
 * date:       2016.12.20
 * brief:      更新显示
 * date:       2017.02.15
 * brief:      修改显示方式
*******************************************************************************/
void CWinSetAcw::TestInit()
{
    Items.clear();
    QStringList s;
    QString U1 = QString::number(ui->BoxVoltage->value());
    QString M1 = ui->BoxMin->text();
    QString M2 = ui->BoxMax->text();
    s.append(QString(tr("交耐")));
    s.append(QString("%1V,%2~%3mA").arg(U1).arg(M1).arg(M2));
    s.append(" ");
    s.append(" ");
    Items.append(s.join("@"));
    emit TransformCmd(ADDR,WIN_CMD_SHOW,Items.join("\n").toUtf8());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       检测状态
 * date:        2017.02.15
 * brief:       取消功能,移到绝缘模块
*******************************************************************************/
void CWinSetAcw::TestCheck()
{
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       更新状态
 * date:        2017.01.13
 * brief:       清理测试结果
 * date:        2017.02.15
 * brief:       增加求平均
*******************************************************************************/
void CWinSetAcw::TestCheckOk(QByteArray )
{
    Testing = false;
    if (Volt.isEmpty() || Curr.isEmpty())
        return;
    double vv = 0;
    double rr = 0;
    for (int i=0; i<Volt.size(); i++) {
        vv += Volt.at(i);
        rr += Curr.at(i);
    }
    vv /= Volt.size();
    rr /= Curr.size();
    if (abs(vv-ui->BoxVoltage->value()) <5)
        vv = ui->BoxVoltage->value();
    QString t = QString("%1V,%2mA").arg(vv).arg(rr/100);
    QString judge = "OK";

    if (rr/100>ui->BoxMax->value() || rr/100<ui->BoxMin->value()) {
        Judge = "NG";
        judge = "NG";
    }

    QStringList s = QString(Items.at(0)).split("@");
    if (s.at(2) == " ")
        s[2] = t;
    if (s.at(3) == " ")
        s[3] = judge;
    emit TransformCmd(ADDR,WIN_CMD_ITEM,s.join("@").toUtf8());

    Volt.clear();
    Curr.clear();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       开始测试
 * date:        2017.02.15
 * brief:       增加超时判断
*******************************************************************************/
void CWinSetAcw::TestStart(quint8 pos)
{
    if (Testing)
        return;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x05)<<quint8(0x01)<<quint8(0x05)<<quint8(0x00)
      <<quint8(pos)<<quint8(0x00);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Testing = true;
    Judge = "OK";
    if(!WaitTestOver(100)) {
        Testing = false;
        Judge = "NG";
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
    QStringList s;
    s.append("交耐");
    s.append(QDate::currentDate().toString("yyyy-MM-dd"));
    s.append(FileInUse);
    s.append(Judge);
    emit TransformCmd(ADDR,WIN_CMD_JUDGE,s.join("@").toUtf8());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       更新测试数据
 * date:        2017.01.13
 * brief:       求平均
*******************************************************************************/
void CWinSetAcw::TestResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double tt = quint16(msg.at(3)*256)+quint8(msg.at(4));
    Volt.append(v);
    Curr.append(tt);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       停止测试
*******************************************************************************/
void CWinSetAcw::TestStop()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x01)<<quint8(0x02);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Testing = false;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       配置
*******************************************************************************/
void CWinSetAcw::TestConfig()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    int freq = ui->BoxFrequcy->currentText().toInt();
    int volt = ui->BoxVoltage->value();
    int time = ui->BoxTime->value()*10;
    int min = ui->BoxMin->value()*100;
    int max = ui->BoxMax->value()*100;
    out<<quint16(0x23)<<quint8(0x08)<<quint8(0x03)<<quint8(0x00)<<quint8(0x05)
      <<quint8(0x00)<<quint8(0x00)<<quint8(0xff)<<quint8(0xff)<<quint8(freq);
    out<<quint16(0x23)<<quint8(0x08)<<quint8(0x04)<<quint8(0x00)<<quint8(volt/256)
      <<quint8(volt%256)<<quint8(time/256)<<quint8(time%256)<<quint8(min/256)<<quint8(min%256);
    out<<quint16(0x23)<<quint8(0x07)<<quint8(0x05)<<quint8(0x00)
      <<quint8(max/256)<<quint8(max%256)<<quint8(0x00)<<quint8(0x03)<<quint8(0x0A);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.20
 * brief:      等待测试结束
 * date:       2017.02.15
 * brief:      去除超时处理
*******************************************************************************/
bool CWinSetAcw::WaitTestOver(quint16 t)
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
 * date:        2016.12.20
 * brief:       延时
*******************************************************************************/
void CWinSetAcw::Delay(int ms)
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
void CWinSetAcw::showEvent(QShowEvent *)
{
    DatInit();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.20
 * brief:       退出保存
*******************************************************************************/
void CWinSetAcw::hideEvent(QHideEvent *)
{
    DatSave();
}
/*******************************************************************************
 *                                  END
*******************************************************************************/
