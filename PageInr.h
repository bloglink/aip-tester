/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       绝缘模块
*******************************************************************************/
#ifndef PAGEINR_H
#define PAGEINR_H

#include <qmath.h>
#include <QUuid>
#include <QDate>
#include <QDebug>
#include <QWidget>
#include <QListView>
#include <QSettings>
#include <QComboBox>
#include <QShowEvent>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>
#include <QCheckBox>
#include "define.h"


#define INR_FREE        0
#define INR_INIT        4
#define INR_TEST        1
#define INR_ROW         4


namespace Ui {
class PageInr;
}

class PageInr : public QWidget
{
    Q_OBJECT

public:
    explicit PageInr(QWidget *parent = 0);
    ~PageInr();

private:
    Ui::PageInr *ui;

signals:
    void SendVariant(QVariantHash s);
    void CanMsg(QByteArray msg);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int id);
    void InitSettings(void);
    void SaveSettings(void);
    void ItemClick(int r,  int c);
    void ExcuteCanCmd(int addr, QByteArray msg);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdResult(QByteArray msg);

    void SendCanCmdStatus(void);
    void SendCanCmdStart(quint8 pos);
    void SendCanCmdStop(void);
    void SendCanCmdConfig(int row);
    void ClearResults(void);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    quint16 GetTeminal(int r,  int c);
    void InitInput(int r,  int c);
    void EnsureInput(void);
    QString CurrentSettings(void);
    bool EnablePhase(void);
    virtual void showEvent(QShowEvent*);

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
    QSettings *ini;

    QList<double> Volt;
    QList<double> Res;

    QString Judge;
    QString FileInUse;
    QStringList Items;

    quint8 Mode;
    quint16 TimeOut;
    QList<QTableWidgetItem*> Enable;
    QList<QTableWidgetItem*> Terminal1;
    QList<QTableWidgetItem*> Terminal2;
    QList<QComboBox*> Vol;
    QList<QDoubleSpinBox*> Min;
    QList<QDoubleSpinBox*> Max;
    QList<QDoubleSpinBox*> Time;
    QList<QDoubleSpinBox*> Offset;
    quint8 TestRow;
    QList<QCheckBox*> Check;

    quint8 stat;
    QList<QVariantHash> ItemView;
    QString TestStatus;
};

#endif // PAGEINR_H
