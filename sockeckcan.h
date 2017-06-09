#ifndef SOCKECKCAN_H
#define SOCKECKCAN_H

#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QObject>
#include <QLibrary>
#include <QDataStream>
#include <QElapsedTimer>
#include "ICANCmd.h"

class SockeckCan : public QObject
{
    Q_OBJECT
public:
    explicit SockeckCan(QObject *parent = 0);

public slots:
    bool DeviceOpen(void);
    bool DeviceQuit(void);
    bool DeviceSend(quint16 addr, QByteArray msg);
    bool DeviceRead(void);
    quint16 GetAddress(void);
    QByteArray GetMessage(void);
private:
    int s;
#ifdef __arm__
    can_frame TxMsg;
#else
    tagCAN_DataFrame TxMsg;
#endif
};

#endif // SOCKECKCAN_H
