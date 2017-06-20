/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       反嵌模块
*******************************************************************************/
#ifndef PAGEMAG_H
#define PAGEMAG_H

#include <QUuid>
#include <QDate>
#include <QWidget>
#include <QListView>
#include <QSettings>
#include <QComboBox>
#include <QShowEvent>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>

#include "qcustomplot.h"
#include "delegate.h"

#define MAX_ROW 8
#define MAG_MAX 8

#define MAG_FREE        0
#define MAG_INIT        4
#define MAG_TEST        1
#define MAG_SAMPLE      2

namespace Ui {
class PageMag;
}

class PageMag : public QWidget
{
    Q_OBJECT

public:
    explicit PageMag(QWidget *parent = 0);
    ~PageMag();

private:
    Ui::PageMag *ui;

signals:
    void SendVariant(QVariantHash s);
    void CanMsg(QByteArray msg);

private slots:
    void InitWin(void);
    void InitSet(void);
    void SaveSet(void);
    void InitItems(void);
    void ExcuteCanCmd(int addr, QByteArray msg);
    void SendCanCmdSample(quint8 s);
    void SendCanCmdStart(quint8 s);
    void SendCanCmdConfig(quint8 s);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdResult(QByteArray msg);
    void ReadCanCmdSample(QByteArray msg);
    void ReadCanCmdWave(QByteArray msg);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);


    void ReadVariant(QVariantHash s);
    void GoToWindow(QString w);
    void SendWarnning(QString s);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendTestWavesAllEmpty(void);
    void SendTestItems(int num);
    void SendTestItemsAll(void);
    void SendTestPause(void);
    void SendTestSave(void);
    void TestThread(QVariantHash hash);
    virtual void showEvent(QShowEvent*);
    void on_btn1_clicked();

    void on_btn2_clicked();

private:
    StandardItemModel *m;
    QSettings *ini;

    quint16 TimeOut;
    quint8 TestRow;
    QStringList Items;
    QString Judge;

    QList<int> AreaL;
    QList<int> FreqL;
    QList<int> AreaR;
    QList<int> FreqR;

    quint8 stat;
    quint8 MagMode;

    QList<QVariantHash> ItemView;
    QByteArray wave;
    QString TestStatus;
    QList<QCustomPlot *> Waves;
};

#endif // PAGEMAG_H
