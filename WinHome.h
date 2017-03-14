#ifndef WINHOME_H
#define WINHOME_H

#include <QTime>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QThread>
#include <QSettings>
#include <QMessageBox>
#include <QButtonGroup>
#include <QElapsedTimer>

#include "define.h"

#include "WinBack.h"
#include "WinSyst.h"
#include "WinType.h"
#include "WinData.h"
#include "WinTest.h"

#include "PageDcr.h"
#include "PageMag.h"
#include "PageInr.h"
#include "PageAcw.h"
#include "PageImp.h"
#include "PageInd.h"
#include "PagePwr.h"
#include "PageLvs.h"
#include "PageLck.h"
#include "PageOut.h"

#include "SqlClient.h"
#include "CanClient.h"
#include "TcpClient.h"
#include "UdpClient.h"
#include "SerialPort.h"

namespace Ui {
class WinHome;
}

class WinHome : public QWidget
{
    Q_OBJECT

public:
    explicit WinHome(QWidget *parent = 0);
    ~WinHome();

private:
    Ui::WinHome *ui;

signals:
    void PutCanData(QByteArray msg);
    void WriteSql(QByteArray msg);
    void SendCommand(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void Init(void);
    void WinInit(void);
    void WinInitAll(void);
    void WinJump(QByteArray win);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void CanInit(void);
    void SqlInit(void);
    void TcpInit(void);
    void UdpInit(void);
    void SerialInit(void);
    void CanThread(QByteArray msg);

    void ReadMessage(quint16 addr,quint16 cmd,QByteArray data);
    void TestInit(void);
    void TestCheck(void);
    void TestStart(QByteArray msg);
    void TestSaveItem(void);
    void TestSaveJudge(QByteArray msg);
    void TestPause(void);
    void TestText(QByteArray msg);
    bool WaitTestOver(quint16 t);
    void Delay(int ms);

    virtual void showEvent(QShowEvent *);
private:
    QList<int> previous_window;
    QString motor_type;
    bool Testing;
    bool isCheckOk;
    QString ItemJudge;
    QStringList ItemToTest;
    int PauseMode;
    QStringList Items;

    QThread *thread_can;
    QThread *thread_sql;
    QThread *thread_tcp;
    QThread *thread_udp;
    QThread *thread_all;
    CanClient can;
    SqlClient sql;
    TcpClient tcp;
    UdpClient udp;
    SerialPort serial;
};

#endif // WINHOME_H
