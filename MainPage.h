#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QFile>
#include <QList>
#include <QDebug>
#include <QThread>
#include <QWidget>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>


#include "CanSocket.h"
#include "main.h"

#include "HomePage.h"
#include "Settings.h"
#include "CWinType.h"
#include "CWinData.h"
#include "CWinTest.h"
#include "CWinBack.h"

#include "CWinSetDcr.h"
#include "CWinSetMag.h"
#include "CWinSetAcw.h"
#include "CWinSetIr.h"
#include "CWinSetImp.h"
#include "ConfigIndl.h"
#include "ConfigLvs.h"
#include "ConfigBlock.h"

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = 0);
    ~MainPage();

private:
    Ui::MainPage *ui;
signals:
    void PutCanData(QByteArray msg);
    void TransformCmd(quint16 addr,quint16 cmd,QByteArray msg);
private slots:
    void WinInit(void);
    void WinSwitch(QByteArray msg);
    void DatInit(void);
    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray msg);
    void CanThreadInit(void);
    void CanThread(QByteArray msg);
    void CheckThread(void);

    void TestSave(void);
    void TestInit(void);
    void TestThread(void);

//    void DisplayInit(void);
    void DisplayResult(void);
    void DisplayWave(QByteArray msg);
    void DisplayWarnning();
    void OutExcuteCmd(quint16 id, QByteArray msg);
    void Delay(int ms);
    bool OutWaitOver(void);
private:
    QList<int> WinPrevious;
    QSettings *global;
    QSettings *set;
    QThread *can;
    CanSocket thread;
    QTimer *timer;

    QStringList ItemToTest;
    QStringList Items;
    bool Testing;
    quint8 Pos;
    bool isStop;
    bool isChecked;
    bool OutTesting;

    HomePage *WinHome;
    Settings *WinSets;
    CWinType *WinType;
    CWinData *WinData;
    CWinTest *WinTest;
    CWinBack *WinBack;

    CWinSetDcr *WinSetDcr;
    CWinSetMag *WinSetMag;
    CWinSetIr  *WinSetIr;
    CWinSetAcw *WinSetAcw;
    CWinSetImp *WinSetImp;
    ConfigIndl *WinSetIndl;
    ConfigLvs  *WinSetLvs;
    ConfigBlock *WinSetBlock;
};

#endif // MAINPAGE_H
