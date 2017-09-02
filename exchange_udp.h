#ifndef EXCHANGEUDP_H
#define EXCHANGEUDP_H

#include <QUrl>
#include <QUuid>
#include <QTimer>
#include <QQueue>
#include <QObject>
#include <QUdpSocket>
#include <QElapsedTimer>
#include <QApplication>
#include <QNetworkInterface>
#include <QJsonObject>
#include <QJsonDocument>

#define LOCAL_PORT 10000
#define VERSION "erp-0.0.0.1"

class ExchangeUdp : public QUdpSocket
{
    Q_OBJECT
public:
    explicit ExchangeUdp(QObject *parent = 0);

signals:
    void sendNetMsg(QString msg);
public slots:
    void initSocket(QString host, quint16 port);
public:
    QQueue <QString> send_queue;
    QQueue <QString> recv_queue;
private slots:
    void readSocket(void);
    void recvAppJson(QJsonObject obj);
private:
    quint16 txPort;
    QHostAddress txHost;
};

#endif // EXCHANGEUDP_H
