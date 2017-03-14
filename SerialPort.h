#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QObject>
#include <QStringList>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "define.h"

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = 0);

signals:
    void SendCommand(quint16 addr,quint16 cmd,QByteArray msg);
private slots:
    void OpenSerial(void);
    void CloseSerial(void);
    void ReadSerial(void);
private:
    QSerialPort *com;
    QTimer *timer;
};

#endif // SERIALPORT_H
