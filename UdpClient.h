#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QDir>
#include <QFile>
#include <QTime>
#include <QObject>
#include <QSettings>
#include <QUdpSocket>
#include <QStringList>

#include "define.h"

class UdpClient : public QUdpSocket
{
    Q_OBJECT
public:
    explicit UdpClient(QObject *parent = 0);

signals:
    void SendCommand(quint16 addr,quint16 cmd,QByteArray msg);

private slots:
    void UdpInit(void);
    void UdpQuit(void);
    void DatInit(void);
    void DatSave(void);
    void GetCommand(QByteArray msg);
    void ReadAllFrame(void);
private:
    QString Number;
    QString Types;
    quint16 Command;
    QByteArray Param;
    QString FileInUse;
    QString TxMsg;
};

#endif // UDPCLIENT_H
