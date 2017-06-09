/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       低启模块
*******************************************************************************/
#ifndef PAGELVS_H
#define PAGELVS_H

#include <QUuid>
#include <QTime>
#include <QDebug>
#include <QWidget>
#include <QLineEdit>
#include <QShowEvent>
#include <QComboBox>
#include <QSettings>
#include <QListView>
#include <QHeaderView>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>
#include "define.h"
#include "qmath.h"

#define LVS_FREE        0
#define LVS_INIT        4
#define LVS_TEST        1

namespace Ui {
class PageLvs;
}

class PageLvs : public QWidget
{
    Q_OBJECT

public:
    explicit PageLvs(QWidget *parent = 0);
    ~PageLvs();

private:
    Ui::PageLvs *ui;

signals:
    void SendVariant(QVariantHash s);
    void CanMsg(QByteArray msg);
private slots:
    void InitButtons(void);
    void ReadButtons(int id);
    void InitSettings(void);
    void SaveSettings(void);
    void ExcuteCanCmd(int addr, QByteArray msg);
    void SendCanCmdStart(quint8 s);
    void SendCanCmdStop(void);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdResult(QByteArray msg);
    void CalculateResult(void);
    void ClearResults(void);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    virtual void showEvent(QShowEvent *);

    void ReadVariant(QVariantHash s);
    void GoToWindow(QString w);
    void SendWarnning(QString s);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendTestItems(int num);
    void SendTestPause(void);
    void SendTestSave(void);
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

#endif // PAGELVS_H
