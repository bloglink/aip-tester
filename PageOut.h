#ifndef PAGEOUT_H
#define PAGEOUT_H

#include <QTime>
#include <QDebug>
#include <QTimer>
#include <QWidget>
#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QMessageBox>
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
    void SendCommand(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void ReadMessage(quint16 addr,quint16 cmd,QByteArray data);
    void ExcuteCanCmd(quint16 id,QByteArray msg);
    void SendCanCmdStatus(quint16 pos);
    void SendWinCmdStart(void);

    void SendCanCmdConfig(QByteArray msg);
    void ReadCanCmdStatus(quint16 addr,QByteArray msg);
    void ReadCanCmdStart(quint16 addr);
    void ReadCanCmdStop(quint16 addr);

    bool WaitTestOver(quint16 t);
    void Delay(int ms);
private:
    bool Testing;
    bool isStop;
    quint16 TimeOut;
    quint16 Pos;
    QTimer *Timer;
    quint8 Mode;
};

#endif // PAGEOUT_H
