/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       电阻模块
*******************************************************************************/
#ifndef PAGEDCR_H
#define PAGEDCR_H

#include <qmath.h>
#include <QUuid>
#include <QDate>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QComboBox>
#include <QSettings>
#include <QListView>
#include <QShowEvent>
#include <QHeaderView>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>

#include "define.h"
#include "PageNum.h"

#define  ResCu    0.0039  //  铜材料
#define  ResCu_Al 0.0041  //  铜铝材料
#define  ResAl    0.0043  //  铝材料

#define MAX_ROW 8

namespace Ui {
class PageDcr;
}

class PageDcr : public QWidget
{
    Q_OBJECT

public:
    explicit PageDcr(QWidget *parent = 0);
    ~PageDcr();

private:
    Ui::PageDcr *ui;

signals:
    void SendVariant(QVariantHash s);
    void CanMsg(QByteArray msg);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int id);
    void InitSettings(void);
    void SaveSettings(void);
    void AutoCalculateMinAndMax(void);
    bool CheckSetting(void);
    void ItemClick(int r, int c);
    void ItemChange(QString msg);
    void ExcuteCanCmd(int addr, QByteArray msg);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdResult(QByteArray msg);
    void ReadOffset(QByteArray msg);


    void SendCanCmdStatus(void);
    void SendCanCmdStart(quint8 pos);
    void SendCanCmdStop(void);
    void SendCanCmdConfig(void);
    void SendAlarm(quint8 addr);
    void SendCanCmdPwr(quint8 pos);

    double CalculateOffset(double t, quint8 num);
    void CalculateBalance(void);
    int CalculateGear(int row);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    QString CurrentSettings(void);
    QString CurrentPorwer(void);
    bool IsPowerOn(void);
    void AutoChangeMetal(int index);
    void AutoChangeUnit(int index);
    virtual void showEvent(QShowEvent*);
    void ReadVariant(QVariantHash s);
    void GoToWindow(QString w);
    void SendWarnning(QString s);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendTestItems(int num);
    void SendSafeCheck(void);
    void SendTestPause(void);
    void SendTestSave(void);
    void SendTemperature(QByteArray msg);
    void TestThread(QVariantHash hash);
private:
    quint16 TimeOut;
    PageNum *input;
    QStringList Items;
    QList<double> Results;
    QString Judge;

    QList<QTableWidgetItem*> Enable;
    QList<QTableWidgetItem*> Terminal1;
    QList<QTableWidgetItem*> Terminal2;
    QList<QComboBox*> Metal;
    QList<QComboBox*> Unit;
    QList<QDoubleSpinBox*> Min;
    QList<QDoubleSpinBox*> Max;
    QList<QDoubleSpinBox*> Std;
    QList<QDoubleSpinBox*> Offset;
    QList<QDoubleSpinBox*> OffsetR;
    quint8 stat;
    QList<double> pwr;
    QList<QVariantHash> ItemView;
    QString TestStatus;
};

#endif // PAGEDCR_H
