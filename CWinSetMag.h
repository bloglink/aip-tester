#ifndef CWINSETMAG_H
#define CWINSETMAG_H

#include <QDate>
#include <QMessageBox>
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
class CWinSetMag;
}

class CWinSetMag : public QWidget
{
    Q_OBJECT

public:
    explicit CWinSetMag(QWidget *parent = 0);
    ~CWinSetMag();

private:
    Ui::CWinSetMag *ui;

signals:
    void TransformCmd(quint16 addr,quint16 cmd,QByteArray data);
public:
    QList<Waveform *> WaveMag;
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void DatSave(void);
    void ItemClick(int r, int c);
    void ItemChange(QString msg);

    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(int id, QByteArray msg);
    void TestInit(void);
    void TestCheck(void);
    void TestCheckOk(QByteArray msg);
    void TestSample(void);
    void TestStart(quint8 pos);
    void TestResult(QByteArray msg);
    void TestWave(QByteArray msg);
    void TestWaveShow(QByteArray msg);
    void TestDir();
    void TestStop(void);
    void TestConfig(void);

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
    QList<QDoubleSpinBox*> Max;
    QList<int> WaveNumber;
    QList<int> Area;
    QList<int> Freq;
};

#endif // CWINSETMAG_H
