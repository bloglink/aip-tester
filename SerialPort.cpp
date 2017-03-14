#include "SerialPort.h"

SerialPort::SerialPort(QObject *parent) : QObject(parent)
{
    com = NULL;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(ReadSerial()));
}

void SerialPort::OpenSerial()
{
    qDebug()<<QTime::currentTime().toString()<<"初始化串口";
    if (com == NULL)
        com = new QSerialPort("/dev/ttyS3",this);
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
    qDebug()<<QTime::currentTime().toString()<<"初始化串口OK";
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
    if (cmd.size()==4 && quint8(cmd.at(1)==0x32))
        SendCommand(ADDR,CMD_START,msg.join(" ").toUtf8());
    if (cmd.size()==4 && quint8(cmd.at(1)==0x31))
        SendCommand(ADDR,CMD_START,msg.join(" ").toUtf8());
}

void SerialPort::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR)
        return;
    switch (cmd) {
    case CMD_ALARM:
        SendAlarm(msg);
        break;
    }
}

void SerialPort::SendAlarm(QByteArray msg)
{
    if (!com->isOpen() || msg.isEmpty())
        return;
    if (msg.at(0) & 0x02)
        com->write("LEDY");
    if (msg.at(0) & 0x04)
        com->write("LEDG");
    if (msg.at(0) & 0x08)
        com->write("LEDR");
}

