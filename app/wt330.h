/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       横河电参表读取程序
*******************************************************************************/
#ifndef WT330_H
#define WT330_H

#include <QTimer>
#include <QDebug>
#include <QObject>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>

#define WT330_INIT 0x00
#define WT330_IDLE 0x01
#define WT330_READ 0x02

#define WT330_LENGTH 30

class WT330 : public QObject
{
    Q_OBJECT
public:
    explicit WT330(QObject *parent = 0);

public slots:
    bool open(QString portName);
    bool close();
    bool thread();
    QStringList readMeter();
private slots:
    bool writeLength(quint8 len);
    bool writeCommand();
private:
    QSerialPort *com;
    QStringList pwr;
    QByteArray tmp;
    quint8 wt;
    quint16 timeOut;
};

#endif // WT330_H
