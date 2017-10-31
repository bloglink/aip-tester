/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       UDP处理模块
*******************************************************************************/
#ifndef SocketUdp_H
#define SocketUdp_H

#include <QObject>
#include <QUdpSocket>
#include <QJsonObject>

#define CMD_CONNECT 100
#define CMD_COMMAND 101

#define TIME_OUT 500

class UdpSocket : public QUdpSocket
{
    Q_OBJECT
public:
    explicit UdpSocket(QObject *parent = 0);

signals:
    void sendNetMsg(QString msg);
public:
    quint32 txTime;
private slots:
    void initSocket(QString host, QString port);
    void recvNetMsg();
    void writeSocket(QString cmd);
    void recvAppMsg(QJsonObject msg);
private:
    quint16 txPort;
    QHostAddress txHost;
};

#endif // SocketUdp_H
