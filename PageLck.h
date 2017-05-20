/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       堵转模块
*******************************************************************************/
#ifndef PAGELCK_H
#define PAGELCK_H

#include <QUuid>
#include <QTime>
#include <QDebug>
#include <QWidget>
#include <QListView>
#include <QSettings>
#include <QComboBox>
#include <QShowEvent>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include "define.h"

#define LCK_FREE        0
#define LCK_INIT        4
#define LCK_TEST        1
#define LCK_SAMPLE      2

namespace Ui {
class PageLck;
}

class PageLck : public QWidget
{
    Q_OBJECT

public:
    explicit PageLck(QWidget *parent = 0);
    ~PageLck();

private:
    Ui::PageLck *ui;

signals:
    void SendVariant(QVariantHash s);
    void CanMsg(QByteArray msg);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void BtnJudge(int id);
    void InitSettings(void);
    void SaveSettings(void);

    void ExcuteCanCmd(int addr, QByteArray msg);

    void ReadCanCmdStatus(QByteArray msg);
    void SendCanCmdStart(quint8 s);
    void ReadCanCmdResult(QByteArray msg);
    void SendCanCmdStop(void);
    void SendItemJudge(void);
    void SendTestJudge(void);
    void CalculateSample(void);
    void ClearResults(void);
    QString CurrentPorwer(void);

    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    virtual void showEvent(QShowEvent *);
    void ReadVariant(QVariantHash s);
    void GoToWindow(QString w);
    void SendWarnning(QString s);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendTestItems(void);
    void TestThread(QVariantHash hash);
private:
    QSettings *set;
    quint16 TimeOut;
    QStringList Items;
    QList<double> Volt;
    QList<double> Curr;
    QList<double> Power;

    quint8 Mode;
    QString Judge;
    QString FileInUse;
    quint8 PowerSupply;

    quint8 stat;
    QList<QVariantHash> ItemView;
    QString TestStatus;
};

#endif // PAGELCK_H
