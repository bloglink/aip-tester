#include "CanClient.h"

CanClient::CanClient(QObject *parent) : QObject(parent)
{
    s = 0;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.09
 * brief:       打开设备
*******************************************************************************/
void CanClient::DeviceOpen()
{
    qDebug()<<QTime::currentTime().toString()<<"打开CAN口";
#ifdef __arm__
    struct sockaddr_can     addr;
    struct ifreq            ifr;
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);  /*打开套接字*/
    if (s < 0)
        return;
    strcpy(ifr.ifr_name, "can0" );
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0)
        return;

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        return;
#else
    s = CAN_DeviceOpen((DWORD)ACUSB_132B,(DWORD)0,NULL);
    if (s <= 0)
        return;
    tagCAN_InitConfig InitMsg;
    InitMsg.dwAccCode = 0;
    InitMsg.dwAccMask = 0xffffffff;
    InitMsg.nFilter = 0;
    InitMsg.bMode = 0;
    InitMsg.nBtrType = 1;
    InitMsg.dwBtr[0] = 0x00;
    InitMsg.dwBtr[1] = 0x1C;
    InitMsg.dwBtr[2] = 0;
    InitMsg.dwBtr[3] = 0;
    int ret = CAN_ChannelStart(s,0,&InitMsg);
    if (ret <= 0)
        return;
#endif
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(readAll()));
    timer->start(50);

    qDebug()<<QTime::currentTime().toString()<<"打开CAN口OK";
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.09
 * brief:       关闭设备
*******************************************************************************/
void CanClient::DeviceQuit()
{
    if (s <= 0)
        return;
    timer->stop();
#ifdef __arm__
    close(s);
#else
    CAN_ChannelStop(s,0);
    CAN_DeviceClose(s);
#endif
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.09
 * brief:       发送一帧数据
 * date:        2017.01.06
 * brief:       解决自发自收问题TxMsg
*******************************************************************************/
bool CanClient::DeviceSend()
{
    if (s <= 0)
        return false;
#ifdef __arm__
    int nbytes;
    nbytes = write(s, &TxMsg, sizeof(struct can_frame));
    if (nbytes != sizeof(struct can_frame))
        return false;
    else
        return true;
#else
    if (CAN_ChannelSend(s,0,&TxMsg,1) > 0)
        return true;
    else
        return false;
#endif
    return true;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.09
 * brief:       读取一帧数据
*******************************************************************************/
bool CanClient::DeviceRead()
{
#ifdef __arm__
    int nbytes;
    int ret;
    fd_set rdfds;
    struct timeval tv;

    FD_ZERO(&rdfds);
    FD_SET(s, &rdfds);
    tv.tv_sec  = 0;
    tv.tv_usec = 0;
    ret = select(s+1, &rdfds, NULL, NULL, &tv);
    if (ret <= 0){
        return false;
    }
    if (FD_ISSET(s, &rdfds)){
        nbytes = read(s, &TxMsg, sizeof(struct can_frame));
        return true;
    }
    return false;
#else
    if (!BytesAvalible())
        return false;
    CAN_ChannelReceive(s,0,&TxMsg,1,1);
    return true;
#endif
    return true;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.09
 * brief:       缓冲区数据
*******************************************************************************/
bool CanClient::BytesAvalible()
{
#ifndef __arm__
    if (s <= 0)
        return false;
    if (CAN_GetReceiveCount(s,0) > 0)
        return true;
    else
        return false;
#endif
    return true;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.09
 * brief:       读取所有数据
*******************************************************************************/
void CanClient::readAll()
{
    QByteArray msg;
    QDataStream in(&msg, QIODevice::ReadWrite);
    in.setVersion(QDataStream::Qt_4_8);
#ifdef __arm__
    while (DeviceRead()) {
        in << quint16(TxMsg.can_id)<<quint8(TxMsg.can_dlc);
        for (int i=0; i<TxMsg.can_dlc; i++)
            in << TxMsg.data[i];
    }
#else
    while (DeviceRead()) {
        in << quint16(TxMsg.uID)<<quint8(TxMsg.nDataLen);
        for (int i=0; i<TxMsg.nDataLen; i++)
            in << TxMsg.arryData[i];
    }
    CAN_ClearReceiveBuffer(s,0);
#endif
    if (!msg.isEmpty())
        emit GetCanData(msg);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.09
 * brief:       发送数据
*******************************************************************************/
void CanClient::WriteAll(QByteArray msg)
{
    quint16 id;
    quint8 dlc;
    quint8 dat;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    while(!out.atEnd()) {
        out >> id >> dlc;
#ifdef __arm__
        TxMsg.can_id = id;
        TxMsg.can_dlc = dlc;
        for (int i=0; i<dlc; i++) {
            out >> dat;
            TxMsg.data[i] = dat;
        }
#else
        TxMsg.uID = id;
        TxMsg.nDataLen = dlc;
        TxMsg.bExternFlag = 0;
        TxMsg.bRemoteFlag = 0;
        TxMsg.nSendType = 0;
        for (int i=0; i<dlc; i++) {
            out >> dat;
            TxMsg.arryData[i] = dat;
        }
#endif
        DeviceSend();
    }
}
