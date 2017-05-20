/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       输出模块
*******************************************************************************/
#ifndef PAGEOUT_H
#define PAGEOUT_H

#include <QTime>
#include <QDebug>
#include <QTimer>
#include <QWidget>
#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QSettings>
#include <QElapsedTimer>
#include "define.h"

#define OUT_FREE        0
#define OUT_INIT        4
#define OUT_TEST        1

namespace Ui {
class PageOut;
}

class PageOut : public QWidget
{
    Q_OBJECT

public:
    explicit PageOut(QWidget *parent = 0);
    ~PageOut();

private:
    Ui::PageOut *ui;

signals:
    void SendVariant(QVariantHash s);
    void CanMsg(QByteArray msg);
private slots:
    void ExcuteCanCmd(int addr, QByteArray msg);
    void SendCanCmdStatus(quint16 pos);
    void SendWinCmdStart(void);

    void SendCanCmdConfig();
    void ReadCanCmdStatus(quint16 addr, QByteArray msg);
    void ReadCanCmdStart(quint16 addr);
    void ReadCanCmdStop(quint16 addr);


    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    void ReadVariant(QVariantHash s);
    void SendWarnning(QString s);
    void SendContrl(QString s);
    void SendAlarm(quint8 addr);


    int CurrentStartMode();
private:
    bool Testing;
    bool isStop;
    quint16 TimeOut;
    quint16 Pos;
    QTimer *Timer;
    quint8 Mode;
    quint8 StartMode;

    quint8 stat;
    QString TestStatus;
};

#endif // PAGEOUT_H
