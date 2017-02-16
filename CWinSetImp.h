#ifndef CWINSETIMP_H
#define CWINSETIMP_H

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
public:
    QList<Waveform *> WaveImp;
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void DatSave(void);
    void VoltEdit(void);

    void ItemClick(int r, int c);
    void ItemChange(QString msg);
    void BlockClick(int x);



    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(int id, QByteArray msg);
    void TestInit(void);
    void TestCheck(void);
    void TestCheckOk(QByteArray msg);
    void TestSampleAuto();
    void TestSample(quint16 row);
    void TestStart(quint8 pos);
    void TestResult(QByteArray msg);
    void TestWave(QByteArray msg);
    void TestWaveShow(QByteArray msg);

    void TestStop(void);
    void TestConfig(void);
    int TestGear(int row);

    bool WaitTestOver(quint16 t);
    void Delay(int ms);

    virtual void showEvent(QShowEvent*);
    virtual void hideEvent(QHideEvent*);
private:
    QSettings *set;
    bool isCheckOk;
    bool Testing;
    bool Sampling;
    quint16 TimeOut;
    InputNum *input;
    quint8 CurrentWave;
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
    QList<int> Freq;
    QList<int> Block0;
    QList<int> Block1;
};

#endif // CWINSETIMP_H
