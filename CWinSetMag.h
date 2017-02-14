#ifndef CWINSETMAG_H
#define CWINSETMAG_H

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
public slots:
    void DisplayInit(void);
    void ExcuteCmd(int id, QByteArray msg);
    void CmdCheckState(void);
    void CmdStartTest(quint8 pos);
    void CmdStartSample(void);
    void CmdStopTest(void);
    void CmdConfigure(void);
    bool WaitTestOver(void);
    void DisplayWave(QByteArray msg);
public:
    QStringList ListItem;
    QStringList ListPara;
    QStringList ListResult;
    QStringList ListJudge;
    QList<Waveform *> WaveMag;
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void DatSave(void);
    void ItemClick(int r, int c);
    void ItemChange(QString msg);
    void UpdateWave(QByteArray msg);
    void UpdateState(QByteArray msg);
    void UpdateTestData(QByteArray msg);
    void UpdateResult(QByteArray msg);
    void UpdateJudge(QByteArray msg);
    void Delay(int ms);
    void UpdateDir();
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
    QList<QDoubleSpinBox*> Max;
    QList<int> WaveNumber;
    QList<int> Area;
    QList<int> Freq;
};

#endif // CWINSETMAG_H
