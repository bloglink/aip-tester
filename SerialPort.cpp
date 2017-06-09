/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170418
 * author:      zhaonanlin
 * brief:       串口通信模块
*******************************************************************************/
#include "SerialPort.h"

SerialPort::SerialPort(QObject *parent) : QObject(parent)
{
    fd = -1;
    com = NULL;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ReadSerial()));
}

void SerialPort::OpenSerial()
{
    if (com  ==  NULL)
        com = new QSerialPort("/dev/ttyS3", this);
    if (com->isOpen())
        com->close();
    if (com->open(QIODevice::ReadWrite)) {
        com->setBaudRate(115200);     //波特率
        com->setParity(QSerialPort::NoParity);
        com->setDataBits(QSerialPort::Data8);
        com->setStopBits(QSerialPort::OneStop);
        com->setFlowControl(QSerialPort::NoFlowControl);
        com->setDataTerminalReady(true);
        com->setRequestToSend(false);
        timer->start(50);
    }
#ifdef __arm__
    fd = open("/dev/em335x_pwm2", O_RDWR);
    if (fd < 0) {
        qDebug() << "pwm2 open fail";
        return;
    }
#endif
}

void SerialPort::CloseSerial()
{
    timer->stop();
    if (com != NULL && com->isOpen())
        com->close();
}

void SerialPort::ReadSerial()
{
    QByteArray cmd = com->readAll();
    if (cmd.isEmpty())
        return;
    QStringList msg;
    msg.append(QString::number(0x13));
    msg.append(QString::number(0x00));
    if (cmd.size() == 4 && quint8(cmd.at(1) == 0x32))
        SendContrl("StartTest");
    if (cmd.size() == 4 && quint8(cmd.at(1) == 0x31)) {
        SendContrl("StopTest");
        com->write("LED1");
    }
}

void SerialPort::StartBeep()
{
#ifdef __arm__
    struct pwm_config_info conf;
    conf.freq = 2000;
    conf.duty = 100;
    conf.polarity = POLARITY;
    conf.count = 0;
    write(fd, &conf, sizeof(struct pwm_config_info));
#endif
}

void SerialPort::StopBeep()
{
#ifdef __arm__
    struct pwm_config_info conf;
    memset(&conf, 0, sizeof(struct pwm_config_info));
    write(fd, &conf, sizeof(struct pwm_config_info));
#endif
}

void SerialPort::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "WinHome")
        return;
    if (s.value("TxCommand") == "TestAlarm") {
        if (s.value("TxMessage").toString().contains("LEDY"))
            com->write("LEDY");
        if (s.value("TxMessage").toString().contains("LEDG"))
            com->write("LEDG");
        if (s.value("TxMessage").toString().contains("LEDR"))
            com->write("LEDR");
        if (s.value("TxMessage").toString().contains("BEEP"))
            StartBeep();
        else
            StopBeep();
    }
}

void SerialPort::SendContrl(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", s);
    hash.insert("Station", "left");
    hash.insert("StartMode", "btn");
    emit SendVariant(hash);
}
