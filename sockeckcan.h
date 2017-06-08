#ifndef SOCKECKCAN_H
#define SOCKECKCAN_H

#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QObject>
#include <QLibrary>
#include <QDataStream>
#include <QElapsedTimer>
#include "unistd.h"
#ifdef __arm__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/netlink.h>
#include <errno.h>
#include <netinet/in.h>
#include <net/if.h>
#include <fcntl.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#else
#include "ICANCmd.h"
#endif

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
