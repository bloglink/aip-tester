#include "sockeckcan.h"

SockeckCan::SockeckCan(QObject *parent) : QObject(parent)
{
    s = 0;
}
bool SockeckCan::DeviceOpen()
{
#ifdef __arm__
    struct sockaddr_can     addr;
    struct ifreq            ifr;
    s = socket(PF_CAN,  SOCK_RAW,  CAN_RAW);  /*打开套接字*/
    if (s < 0)
        return false;
    strcpy(ifr.ifr_name, "can0");
    if (ioctl(s,  SIOCGIFINDEX,  &ifr) < 0)
        return false;

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(s,  (struct sockaddr *)&addr,  sizeof(addr)) < 0)
        return false;
#else
    s = CAN_DeviceOpen((DWORD)ACUSB_132B, (DWORD)0, NULL);
    if (s <= 0)
        return false;
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
    int ret = CAN_ChannelStart(s, 0, &InitMsg);
    if (ret <= 0)
        return false;
#endif
    return true;
}

bool SockeckCan::DeviceQuit()
{
#ifdef __arm__
    close(s);
#else
    CAN_ChannelStop(s, 0);
    CAN_DeviceClose(s);
#endif
    return true;
}

bool SockeckCan::DeviceSend(quint16 addr, QByteArray msg)
{
#ifdef __arm__
    TxMsg.can_id = addr;
    TxMsg.can_dlc = msg.size();
    for (int i=0; i < TxMsg.can_dlc; i++) {
        TxMsg.data[i] = msg.at(i);
    }
    int nbytes;
    nbytes = write(s,  &TxMsg,  sizeof(struct can_frame));
    if (nbytes != sizeof(struct can_frame))
        return false;
    else
        return true;
#else
    TxMsg.uID = addr;
    TxMsg.nDataLen = msg.size();
    TxMsg.bExternFlag = 0;
    TxMsg.bRemoteFlag = 0;
    TxMsg.nSendType = 0;
    for (int i=0; i < TxMsg.nDataLen; i++) {
        TxMsg.arryData[i] = msg.at(i);
    }
    if (CAN_ChannelSend(s, 0, &TxMsg, 1) > 0)
        return true;
    else
        return false;
#endif
}

bool SockeckCan::DeviceRead()
{
#ifdef __arm__
    int ret;
    fd_set rdfds;
    struct timeval tv;

    FD_ZERO(&rdfds);
    FD_SET(s,  &rdfds);
    tv.tv_sec  = 0;
    tv.tv_usec = 0;
    ret = select(s+1,  &rdfds,  NULL,  NULL,  &tv);
    if (ret <= 0) {
        return false;
    }
    if (FD_ISSET(s,  &rdfds)) {
        if (read(s,  &TxMsg,  sizeof(struct can_frame)) != sizeof(struct can_frame))
            return false;
        else
            return true;
    }
#else
    if (CAN_GetReceiveCount(s, 0) <= 0)
        return false;
    if (CAN_ChannelReceive(s, 0, &TxMsg, 1, 1) == 1)
        return true;
#endif
    return false;
}

quint16 SockeckCan::GetAddress()
{
#ifdef __arm__
    return TxMsg.can_id;
#else
    return TxMsg.uID;
#endif
}

QByteArray SockeckCan::GetMessage()
{
    QByteArray msg;
#ifdef __arm__
    for (int i=0; i < TxMsg.can_dlc; i++)
        msg.append(TxMsg.data[i]);
#else
    for (int i=0; i < TxMsg.nDataLen; i++)
        msg.append(TxMsg.arryData[i]);
#endif
    return msg;
}

