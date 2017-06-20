/*******************************************************************************
 * Copyright [2016]  < 青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170417
 * author:      zhaonanlin
 * brief:       匝间测试模块
*******************************************************************************/
#ifndef PAGEIMP_H
#define PAGEIMP_H

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

#include "delegate.h"
#include "qcustomplot.h"

#define IMP_MAX 8

namespace Ui {
class PageImp;
}

class PageImp : public QWidget
{
    Q_OBJECT

public:
    explicit PageImp(QWidget *parent = 0);
    ~PageImp();

signals:
    void SendVariant(QVariantHash s);
    void CanMsg(QByteArray msg);
private slots:
    void InitWin(void);
    void InitSet(void);
    void SaveSet(void);
    void InitItems(void);
    void WaveChange(QModelIndex m);
    void ExcuteCanCmd(int id, QByteArray msg);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdResult(QByteArray msg);
    void ReadCanCmdWaveOk(void);
    void ReadCanCmdBack(QByteArray msg);
    void SendCanCmdStatus(void);
    void SendCanCmdSampleAuto();
    void SendCanCmdSample(quint16 row);
    void SendCanCmdStart(quint8 stat);
    void SendCanCmdConfig(void);
    void SendCanCmdDebug();
    void SendCanCmdCode();
    void SendCanCmdVersion();

    void CalculateResult(void);
    int CalculateGear(int row);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);

    void ReadVariant(QVariantHash s);
    void GoToWindow(QString w);
    void SendWarnning(QString s);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendTestWavesAllEmpty(void);
    void SendTestItems(int num);
    void SendTestPause(void);
    void SendTestSave(void);
    void TestThread(QVariantHash hash);
    void showEvent(QShowEvent*);

    void on_btn1_clicked();

    void on_btn2_clicked();

    void on_btn3_clicked();

private:
    Ui::PageImp *ui;
    StandardItemModel *m;
    QSettings *ini;
    QList<QCustomPlot *> Waves;

    quint16 AvrCount;
    quint16 TimeOut;
    quint8 TestRow;
    QStringList Items;
    QString Judge;
    QString FileInUse;

    QList<int> WaveNumber;
    QList<int> Origin;
    QList<int> Terminal;

    QList<int> Block0;
    QList<int> Block1;
    QList<int> FreqL;
    QList<int> VoltL;
    QList<int> FreqR;
    QList<int> VoltR;
    QStringList WaveL;
    QStringList WaveR;
    quint8 ImpMode;

    QList<QVariantHash> ItemView;
    QByteArray wave;
    QString TestStatus;
    quint8 stat;
};

#endif // PAGEIMP_H
