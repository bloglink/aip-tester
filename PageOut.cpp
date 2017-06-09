/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       输出模块
*******************************************************************************/
#include "PageOut.h"
#include "ui_PageOut.h"

PageOut::PageOut(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageOut)
{
    ui->setupUi(this);
    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(SendWinCmdStart()));
}

PageOut::~PageOut()
{
    delete ui;
}

void PageOut::ExcuteCanCmd(int addr, QByteArray msg)
{
    if (quint8(msg.at(0)) == 0x00)
        ReadCanCmdStatus(addr, msg);
    if (quint8(msg.at(0)) == 0x01 && quint8(msg.at(1) == 0x01))
        ReadCanCmdStart(addr);
    if (quint8(msg.at(0)) == 0x01 && quint8(msg.at(1) == 0x00))
        ReadCanCmdStop(addr);
}

void PageOut::SendCanCmdStatus(quint16 pos)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(pos) << quint8(0x01) << quint8(0x00);
    emit CanMsg(msg);
}

void PageOut::SendWinCmdStart()
{
    Timer->stop();
    SendContrl("StartTest");
}

void PageOut::SendCanCmdConfig()
{
    quint8 StartMode = CurrentStartMode();
    if (StartMode != 1 && StartMode != 2)
        return;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x13) << quint8(0x02) << quint8(0x03) << quint8(StartMode);
    out << quint16(0x14) << quint8(0x02) << quint8(0x03) << quint8(StartMode);
    emit CanMsg(msg);
}

void PageOut::ReadCanCmdStatus(quint16 addr, QByteArray msg)
{
    QString t;
    if (addr == WIN_ID_OUT13)
        t = tr("左工位");
    if (addr == WIN_ID_OUT14)
        t = tr("右工位");
    int s = quint8(msg.at(1));
    switch (s) {
    case 0x00:
        break;
    case 0x01:
        SendWarnning(t + " UNIVALID");
        return;
    default:
        SendWarnning(t + " UNKONW_ERROR");
        break;
    }
    TestStatus = "free";
}

void PageOut::ReadCanCmdStart(quint16 addr)
{
    if (addr == CAN_ID_13OUT)
        stat = WIN_ID_OUT13;
    if (addr == CAN_ID_14OUT)
        stat = WIN_ID_OUT14;
    TestStatus = "free";
    Timer->start(10);
}

void PageOut::ReadCanCmdStop(quint16 addr)
{
    if (addr == CAN_ID_13OUT && stat == WIN_ID_OUT13)
        SendContrl("StopTest");
    if (addr == CAN_ID_14OUT && stat == WIN_ID_OUT14)
        SendContrl("StopTest");
}

bool PageOut::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (TestStatus != "free") {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageOut::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void PageOut::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "PageOut" && s.value("TxAddress") != "WinHome")
        return;
    if (s.value("TxCommand") == "TestInit")
        SendCanCmdConfig();

    if (s.value("TxCommand") == "CheckStatus") {
        TestStatus = "init";
        QString w;
        if (s.value("Station").toString() == "left") {
            stat = WIN_ID_OUT13;
            w = tr("左工位超时");
        }
        if (s.value("Station").toString() == "right") {
            stat = WIN_ID_OUT14;
            w = tr("右工位超时");
        }
        SendCanCmdStatus(stat);
        if (!WaitTimeOut(30))
            SendWarnning(w);
        TestStatus = "free";
    }
    if (s.value("TxCommand") == "TestAlarm") {
        quint8 t = 0x00;
        if (s.value("TxMessage").toString().contains("LEDR"))
            t = 0x01;
        if (s.value("TxMessage").toString().contains("LEDG"))
            t = 0x02;
        if (s.value("TxMessage").toString().contains("LEDY"))
            t = 0x03;
        SendAlarm(t);
    }
}

void PageOut::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("输出异常:\n%1").arg(s));
    emit SendVariant(hash);
}

void PageOut::SendContrl(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", s);
    if (stat == WIN_ID_OUT13)
        hash.insert("Station", "left");
    if (stat == WIN_ID_OUT14)
        hash.insert("Station", "right");
    hash.insert("StartMode", "out");
    emit SendVariant(hash);
}

void PageOut::SendAlarm(quint8 addr)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(stat) << quint8(0x02) << quint8(0x02) << quint8(addr);
    emit CanMsg(msg);
}

int PageOut::CurrentStartMode()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    return ini->value("/GLOBAL/Mode", "0").toInt();
}
/*********************************END OF FILE**********************************/
