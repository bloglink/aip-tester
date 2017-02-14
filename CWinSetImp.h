#ifndef CWINSETIMP_H
#define CWINSETIMP_H

#include <QWidget>
#include <QListView>
#include <QSettings>
#include <QComboBox>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>
#include "main.h"
#include "Waveform.h"
#include "InputNum.h"

namespace Ui {
class CWinSetImp;
}

class CWinSetImp : public QWidget
{
    Q_OBJECT

public:
    explicit CWinSetImp(QWidget *parent = 0);
    ~CWinSetImp();

private:
    Ui::CWinSetImp *ui;

signals:
    void TransformCmd(quint16 addr,quint16 cmd,QByteArray data);
public slots:
    void DisplayInit(void);
    void ExcuteCmd(int id, QByteArray msg);
    void CmdCheckState(void);
    void CmdStartTest(quint8 pos);
    void CmdSample(quint16 row);
    void CmdStopTest(void);
    void CmdConfigure(void);
    bool WaitTestOver(void);
    void CmdStartSample();
    void DisplayWave(QByteArray msg);
public:
    QStringList ListItem;
    QStringList ListPara;
    QStringList ListResult;
    QStringList ListJudge;
    QList<Waveform *> WaveImp;
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void DatSave(void);
    void VoltEdit(void);
    void Delay(int ms);
    void ItemClick(int r, int c);
    void ItemChange(QString msg);
    void BlockClick(int x);
    void UpdateWave(QByteArray msg);
    void UpdateWaveFreq(QByteArray msg);
    void UpdateState(QByteArray msg);
    void UpdateTestData(QByteArray msg);
    void UpdateResult(QByteArray msg);
    void UpdateJudge(QByteArray msg);
    int Gear(int row);
    virtual void showEvent(QShowEvent*);
    virtual void hideEvent(QHideEvent*);
    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray msg);
    void ShowInit(void);
private:
    QSettings *set;
    bool isCheckOk;
    bool Testing;
    bool Sampling;
    quint16 TimeOut;
    InputNum *input;
    quint8 CurrentWave;
    QStringList Items;
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
};

#endif // CWINSETIMP_H
