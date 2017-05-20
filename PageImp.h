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
#include "define.h"
#include "Waveform.h"
#include "PageNum.h"
#include "WaveBox.h"

#define MAX_ROW 8
#define IMP_FREE        0
#define IMP_INIT        4
#define IMP_TEST        1
#define IMP_SAMPLE      2
#define IMP_SAMPLE_ADD  5
#define IMP_SAMPLE_OTHER 3

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
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int id);
    void InitSettings(void);
    void InitStation(void);
    void SaveSettings(void);
    void ItemClick(int r,  int c);
    void ItemChange(QString msg);
    void BlockClick(int x);
    void ExcuteCanCmd(int id, QByteArray msg);
    void SendCanCmdStatus(void);
    void SendCanCmdSampleAuto();
    void SendCanCmdSample(quint16 row);
    void SendCanCmdStart(quint8 stat);
    void SendCanCmdConfig(void);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdResult(QByteArray msg);
    void ReadCanCmdWaveOk(void);
    void CalculateResult(void);
    void CalculateAvarageWave(void);
    int CalculateGear(int row);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    void AutoChangeVolt(void);
    void showEvent(QShowEvent*);

    void ReadVariant(QVariantHash s);
    void GoToWindow(QString w);
    void SendWarnning(QString s);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendTestWavesAllEmpty(void);
    void SendTestItems(int num);
    void SendTestPause(void);
    void SendTestSave(void);
    void WaveView(QVariantHash s);
    void WaveClick(QVariantHash s);
    void TestThread(QVariantHash hash);
private:
    Ui::PageImp *ui;
    QList<WaveBox *> WaveImp;
    QSettings *set;
    quint16 AvrCount;
    quint16 TimeOut;
    PageNum *input;
    quint8 TestRow;
    QStringList Items;
    QString Judge;
    QString FileInUse;

    QList<QTableWidgetItem*> Enable;
    QList<QTableWidgetItem*> Terminal1;
    QList<QTableWidgetItem*> Terminal2;
    QList<QDoubleSpinBox*> Volt;
    QList<QDoubleSpinBox*> Time;
    QList<QDoubleSpinBox*> Flut;
    QList<QDoubleSpinBox*> Phase;
    QList<QDoubleSpinBox*> Area;
    QList<QDoubleSpinBox*> Diff;
    QList<int> WaveNumber;
    QList<int> Origin;
    QList<int> Terminal;

    QList<int> Block0;
    QList<int> Block1;
    QList<int> FreqL;
    QList<int> VoltL;
    QList<int> FreqR;
    QList<int> VoltR;
    QStringList WaveLeft;
    QStringList WaveRight;
    quint8 ImpMode;

    QList<QVariantHash> ItemView;
    QByteArray wave;
    QString TestStatus;
    QList<WaveBox *> WaveMag;
    quint8 stat;
};

#endif // PAGEIMP_H
