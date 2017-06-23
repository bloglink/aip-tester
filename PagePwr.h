/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       功率模块
*******************************************************************************/
#ifndef PAGEPWR_H
#define PAGEPWR_H

#include <QUuid>
#include <QTime>
#include <QDebug>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSettings>
#include <QListView>
#include <QShowEvent>
#include <QHeaderView>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>

#include "qmath.h"

#include "delegate.h"

#define PWR_ROW 6
#define PG_ROW 3

#define PWR_MAX 6

#define PWR_FREE        0
#define PWR_INIT        4
#define PWR_TEST        1

namespace Ui {
class PagePwr;
}

class PagePwr : public QWidget
{
    Q_OBJECT

public:
    explicit PagePwr(QWidget *parent = 0);
    ~PagePwr();

private:
    Ui::PagePwr *ui;
    StandardItemModel *m;

signals:
    void SendVariant(QVariantHash s);
    void CanMsg(QByteArray msg);
private slots:
    void InitWin(void);
    void InitSet(void);
    void SaveSet(void);
    void ExcuteCanCmd(int addr,  QByteArray msg);
    void SendCanCmdStatus(void);
    void SendCanCmdStart(quint8 s);
    void SendCanCmdStartSafe(void);
    void SendCanCmdStop(void);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdResult(QByteArray msg);
    void ReadCanCmdDir(QByteArray msg);
    void ReadCanCmdVolt(QByteArray msg);
    void CalculateResult(void);
    void ClearResults(void);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    QString CurrentPorwer(void);
    bool IsPGEnable(void);
    virtual void showEvent(QShowEvent *);

    void ReadVariant(QVariantHash s);
    void GoToWindow(QString w);
    void SendWarnning(QString s);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendPwrItems(int num);
    void SendDirItems(int num);
    void SendPGItems(int num);
    void SendTestPause(void);
    void SendTestSave();
    void TestThread(QVariantHash hash);
    void SafeThread(QVariantHash hash);
    int CurrentPowerDir();
    void on_btn1_clicked();

private:
    QSettings *set;
    quint16 TimeOut;
    quint16 Time;
    QStringList Items;
    QStringList PGItems;
    QList<double> Volt;
    QList<double> Curr;
    QList<double> Power;
    QList<double> CVolt;
    QStringList Dir;

    quint8 TestRow;
    quint8 Mode;
    QString dir;
    quint8 isTestDir;

    quint8 stat;
    QList<QVariantHash> PwrView;
    QList<QVariantHash> DirView;
    QList<QVariantHash> PGView;
    QString TestStatus;
    QString PwrJudge;
    QString DirJudge;
    QString PGJudge;
};

#endif // PAGEPWR_H
