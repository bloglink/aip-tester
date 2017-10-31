/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       横河电参表读取程序
*******************************************************************************/
#include "wt330.h"

WT330::WT330(QObject *parent) : QObject(parent)
{
    com = NULL;
    timeOut = 0;
    wt = WT330_INIT;
}

bool WT330::open(QString portName)
{
    com = new QSerialPort(portName, this);
    if (com->open(QIODevice::ReadWrite)) {
        com->setBaudRate(9600);
        com->setParity(QSerialPort::NoParity);
        com->setDataBits(QSerialPort::Data8);
        com->setStopBits(QSerialPort::OneStop);
        com->setFlowControl(QSerialPort::NoFlowControl);
        com->setDataTerminalReady(true);
        com->setRequestToSend(false);
        return true;
    } else {
        return false;
    }
}

bool WT330::close()
{
    if (com == NULL) {
        return false;
    } else {
        com->close();
        return true;
    }
}

bool WT330::writeLength(quint8 len)
{
    if (com == NULL || !com->isOpen()) {
        return false;
    } else {
        QByteArray num = QString(":NUMERIC:NORMAL:NUMBER %1").arg(len).toUtf8();
        num.append(0x0A);
        com->write(num);  // 设置读取长度
        return true;
    }
}

bool WT330::writeCommand()
{
    if (com == NULL || !com->isOpen()) {
        return false;
    } else {
        QByteArray num = QString(":NUMERIC:NORMAL:VALUE?").toUtf8();
        num.append(0x0A);
        com->write(num);  // 读取电参
        return true;
    }
}

bool WT330::thread()
{
    if (com == NULL || !com->isOpen())
        return false;
    switch (wt) {
    case WT330_INIT:
        writeLength(WT330_LENGTH);
        wt = WT330_IDLE;
        break;
    case WT330_IDLE:
        tmp.clear();
        timeOut = 0;
        writeCommand();
        wt = WT330_READ;
        break;
    case WT330_READ:
        if (com->bytesAvailable() > 0)
            tmp.append(com->readAll());
        if (QString(tmp).split(",").size() >= WT330_LENGTH) {
            pwr.append(tmp);
            if (pwr.size() > 5)
                pwr.removeFirst();
            tmp.clear();
            wt = WT330_IDLE;
        }
        break;
    default:
        break;
    }
    timeOut++;
    if (timeOut > 100)
        return false;
    else
        return true;
}

QStringList WT330::readMeter()
{
    QStringList xx;
    if (pwr.isEmpty())
        return xx;
    QList<double> x;
    for (int i=0; i < pwr.size(); i++) {
        QStringList t = pwr.at(i).split(",");
        if (t.size() >= WT330_LENGTH)
            x.append(t.at(1).toDouble());
        else
            x.append(0);
    }
    double max = 0;
    for (int i=0; i < x.size(); i++)
        max = qMax(max, x.at(i));
    int t = x.indexOf(max);
    if (t >= 0)
        xx = pwr.at(t).split(",");
    pwr.clear();
    return xx;
}
