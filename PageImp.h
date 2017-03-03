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
    void SendMessage(quint16 addr,quint16 cmd,QByteArray data);
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



    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
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
    PageNum *input;
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

#endif // PAGEIMP_H