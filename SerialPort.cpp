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
    QByteArray msg = com->readAll();
    if (msg.isEmpty())
        return;
    if (msg.size()==4 && quint8(msg.at(1)==0x32))
        SendCommand(ADDR,CMD_START,QByteArray::fromHex("1300"));
    if (msg.size()==4 && quint8(msg.at(1)==0x31))
        SendCommand(ADDR,CMD_STOP,QByteArray::fromHex("1300"));
}

