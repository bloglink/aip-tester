#ifndef WINHOME_H
#define WINHOME_H

#include <QTime>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QWidget>
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
public slots:
    void WinInitAll(void);
private slots:
    void WinInit(void);
    void WinJump(QByteArray win);
    void BtnInit(void);
    void BtnJudge(int id);

    void ReadMessage(quint16 addr,quint16 cmd,QByteArray data);
    void TestInit(void);
    void TestCheck(void);
    void TestStart(QByteArray msg);
    void TestSaveItem(void);
    void TestSaveJudge(QByteArray msg);
    void TestPause(void);
    bool WaitTestOver(quint16 t);
    void Delay(int ms);
private:
    QList<int> previous_window;
    QString motor_type;
    bool Testing;
    bool isCheckOk;
    QString ItemJudge;
    QStringList ItemToTest;
    int PauseMode;
    QStringList Items;
};

#endif // WINHOME_H
