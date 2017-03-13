#ifndef PAGEIMP_H
#define PAGEIMP_H

#include <QDate>
#include <QWidget>
#include <QListView>
#include <QSettings>
#include <QComboBox>
#include <QMessageBox>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>
#include "define.h"
#include "Waveform.h"
#include "PageNum.h"

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

private:
    Ui::PageImp *ui;

signals:
    void SendCommand(quint16 addr,quint16 cmd,QByteArray data);
public:
    QList<Waveform *> WaveImp;
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void BtnJudge(int id);
    void InitSettings(void);
    void InitStation(void);
    void SaveSettings(void);
    void ItemClick(int r, int c);
    void ItemChange(QString msg);
    void BlockClick(int x);

    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(int id, QByteArray msg);
    void InitTestItems(void);
    void SendTestItemsAllError(void);
    void SendCanCmdStatus(void);
    void SendCanCmdSampleAuto();
    void SendCanCmdSample(quint16 row);
    void SendCanCmdStart(quint8 station);
    void SendCanCmdStop(void);
    void SendCanCmdConfig(void);
    void SendTestJudge(void);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdSample(QByteArray msg);
    void ReadCanCmdWave(QByteArray msg);
    void ReadCanCmdWaveOk(QByteArray msg);
    void ReadCanCmdWaveStart(QByteArray msg);
    void CalculateResult(QByteArray msg);
    void CalculateAvarageWave(void);
    void SendWave(QByteArray msg);
    int CalculateGear(int row);

    bool WaitTimeOut(quint16 t);
    void Delay(int ms);

    void AutoChangeVolt(void);

    virtual void showEvent(QShowEvent*);
    virtual void hideEvent(QHideEvent*);
private:
    QSettings *set;
    quint16 AvrCount;
    quint16 TimeOut;
    PageNum *input;
    quint8 CurrentWave;
    QStringList Items;
    QString JudgeAll;
    QString FileInUse;

    quint8 station;

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
    QList<int> Freq;
    QList<int> Block0;
    QList<int> Block1;
    QList<int> VoltTest;
    quint8 ImpMode;


};

#endif // PAGEIMP_H
