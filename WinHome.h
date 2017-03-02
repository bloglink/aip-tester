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

#include "SqlData.h"
#include "CanSocket.h"

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
    void SendMessage(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void WinInit(void);
    void WinInitAll(void);
    void WinJump(QByteArray win);
    void BtnInit(void);
    void BtnJudge(int id);
    void CanInit(void);
    void SqlInit(void);
    void CanThread(QByteArray msg);

    void ReadMessage(quint16 addr,quint16 cmd,QByteArray data);
    void TestInit(void);
    void TestCheck(void);
    void TestStart(QByteArray msg);
    void TestSaveItem(void);
    void TestSaveJudge(QByteArray msg);
    void TestPause(void);
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
    CanSocket can;
    SqlData sql;
};

#endif // WINHOME_H
