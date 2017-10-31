/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       UDP处理模块
*******************************************************************************/
#include "udpsocket.h"

UdpSocket::UdpSocket(QObject *parent) : QUdpSocket(parent)
{
    txTime = -1;
    txPort = 6000;
    txHost = "192.168.1.56";
}

void UdpSocket::initSocket(QString host, QString port)
{
    txTime = 0;
    txPort = port.toInt();
    txHost = host;

    this->bind(QHostAddress::AnyIPv4, txPort);
    connect(this, SIGNAL(readyRead()), this, SLOT(recvNetMsg()));
}

void UdpSocket::recvNetMsg()
{
    while (this->hasPendingDatagrams()) {
        QByteArray msg;
        msg.resize(this->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        this->readDatagram(msg.data(), msg.size(), &sender, &senderPort);
        qDebug() << "udp recv:" << msg;
        txTime = 0;
        emit sendNetMsg(msg);
    }
}

void UdpSocket::writeSocket(QString cmd)
{
    this->writeDatagram(cmd.toUtf8(), txHost, txPort);
    this->waitForBytesWritten();
    qDebug() << "udp send:" << cmd;
}

void UdpSocket::recvAppMsg(QJsonObject msg)
{
    int cmd = msg.value("txCommand").toInt();
    switch (cmd) {
    case CMD_CONNECT:
        initSocket(msg.value("hostaddr").toString(), msg.value("hostport").toString());
        writeSocket(msg.value("txMessage").toString());
        break;
    case CMD_COMMAND:
        writeSocket(msg.value("txMessage").toString());
        break;
    default:
        break;
    }
}
