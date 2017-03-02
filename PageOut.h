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
    void SendMessage(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void ReadMessage(quint16 addr,quint16 cmd,QByteArray data);
    void ExcuteCanCmd(quint16 id,QByteArray msg);
    void TestCheck(quint16 pos);
    void TestStart(void);

    void TestConfig(QByteArray msg);

    bool WaitTestOver(quint16 t);
    void Delay(int ms);
private:
    bool Testing;
    bool isStop;
    quint16 TimeOut;
    quint16 Pos;
    QTimer *Timer;
};

#endif // PAGEOUT_H
