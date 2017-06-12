#ifndef DESKTOP_H
#define DESKTOP_H

#include <QTime>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QThread>
#include <QSettings>
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
#include "PageAmp.h"

#include "SqlClient.h"
#include "TcpClient.h"
#include "UdpClient.h"
#include "SerialPort.h"
#include "sockeckcan.h"
#include "warnning.h"

namespace Ui {
class Desktop;
}

class Desktop : public QWidget
{
    Q_OBJECT
signals:
    void SendVariant(QVariantHash s);
    void SendCanCmdDcr(int addr, QByteArray msg);
    void SendCanCmdMag(int addr, QByteArray msg);
    void SendCanCmdInr(int addr, QByteArray msg);
    void SendCanCmdAcw(int addr, QByteArray msg);
    void SendCanCmdImp(int addr, QByteArray msg);
    void SendCanCmdInd(int addr, QByteArray msg);
    void SendCanCmdPwr(int addr, QByteArray msg);
    void SendCanCmdLvs(int addr, QByteArray msg);
    void SendCanCmdLck(int addr, QByteArray msg);
    void SendCanCmdOut(int addr, QByteArray msg);
    void SendCanCmdAmp(int addr, QByteArray msg);
    void CanMsg(int addr, QByteArray msg);
    void message(QByteArray msg);
    void PutCanData(QByteArray msg);
    void WriteSql(QByteArray msg);
    void SendCommand(quint16 addr, quint16 cmd, QByteArray data);
public:
    explicit Desktop(QWidget *parent = 0);
    ~Desktop();
private slots:
    void InitWin(void);
    void InitCan(void);
    void ReadCan(void);
    void Warnnings(QVariantHash s);
    void Delay(int ms);

    void InitWindowsAll(void);
    void JumpToWindow(QByteArray win);
    void ReadButtons(int id);
    void InitSql(void);
    void InitTcp(void);
    void InitUdp(void);
    void InitBtn(void);
    void ReadCanCmd(QByteArray msg);

    void ReadMessage(quint16 addr, quint16 cmd, QByteArray data);
    void ReadStatusAll(void);
    void SendTestRestart(void);
    void SendTestPause(void);

    QString GetWinName(int n);
    QString CurrentSettings(void);
    QStringList CurrentItems(void);
    QStringList EnableItems(void);
    QStringList EnableOutput(void);
    int CurrentStartMode(void);
    int CurrentPauseMode(void);
    int CurrentAlarmTime(QString msg);
    int CurrentReStartMode(void);
    int CurrentDelay(void);
    QString CurrentUser(void);

    void ReadVariant(QVariantHash s);
    void SendTestStatus(QString msg);
    void SendTestSave(void);
    void ReadTestSave(QVariantHash s);
    void SendTestAlarm(QString msg);
    void SendTestDebug(QString msg);
    void SendTestJudge(QString msg);
    void SendTestInit(QVariantHash hash);
    void SendTestStop(QVariantHash hash);
    bool IsStartModeRight(QVariantHash hash);
    void ExcuteCanCmd(QByteArray msg);
    void TestThread(QVariantHash hash);
private:
    Ui::Desktop *ui;
    QString TestStatus;
    SockeckCan can;
    QTimer *timer;

    QList<int> previous_window;
    QString Judge;

    QThread *thread_sql;
    QThread *thread_tcp;
    QThread *thread_udp;
    QThread *thread_btn;
    SqlClient sql;
    TcpClient tcp;
    UdpClient udp;
    SerialPort btn;
    QByteArray stat;
    Warnning *msgBox;
    quint16 Current_Test_Item;
    QStringList TempItems;
    bool isPause;
    QVariantHash TestHash;
};

#endif // DESKTOP_H
