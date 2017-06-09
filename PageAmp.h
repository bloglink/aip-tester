#ifndef PAGEAMP_H
#define PAGEAMP_H

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QDataStream>
#include <QElapsedTimer>
#include "define.h"

#define AMP_FREE        0
#define AMP_INIT        4
#define AMP_TEST        1

class PageAmp : public QObject
{
    Q_OBJECT
public:
    explicit PageAmp(QObject *parent = 0);

signals:
    void SendVariant(QVariantHash s);
    void CanMsg(QByteArray msg);
private slots:
    void ExcuteCanCmd(int addr, QByteArray msg);
    void SendCanCmdStatus(void);
    void SendWarnning(QString s);
    void SendError(QString s);
    void ReadCanCmdStatus(QByteArray msg);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    void ReadVariant(QVariantHash s);
private:
    quint8 Mode;
    quint16 TimeOut;
    QString TestStatus;
};

#endif // PAGEAMP_H
