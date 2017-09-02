#include "exchange_udp.h"

ExchangeUdp::ExchangeUdp(QObject *parent) : QUdpSocket(parent)
{
}

void ExchangeUdp::initSocket(QString host, quint16 port)
{
    txPort = port;
    txHost.setAddress(host);

    this->bind(QHostAddress::AnyIPv4, port);
    connect(this, SIGNAL(readyRead()), this, SLOT(readSocket()));
}

void ExchangeUdp::readSocket()
{
    while (this->hasPendingDatagrams()) {
        QByteArray msg;
        msg.resize(this->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        this->readDatagram(msg.data(), msg.size(), &sender, &senderPort);
        recv_queue.enqueue(msg);
        qDebug() << "recv" << msg;
    }
}

void ExchangeUdp::recvAppJson(QJsonObject obj)
{
    QByteArray msg = obj.value("TxMessage").toString().toUtf8();
    this->writeDatagram(msg, txHost, txPort);
    qDebug() << "send" << msg;
}

void ExchangeUdp::recvAppMsg(QByteArray msg)
{
    this->writeDatagram(msg, txHost, txPort);
    qDebug() << "send" << msg;
}
/*********************************END OF FILE**********************************/
