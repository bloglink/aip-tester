#ifndef PAGEMAG_H
#define PAGEMAG_H

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
#include "define.h"
#include "Waveform.h"
#include "PageNum.h"
#define MAX_ROW 8
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
    void SendMessage(quint16 addr,quint16 cmd,QByteArray data);
public:
    QList<Waveform *> WaveMag;
private slots:
    void InitializesWindow(void);
    void InitializesButton(void);
    void BtnJudge(int id);
    void InitializesSetting(void);
    void SaveSetting(void);
    void ItemClick(int r, int c);
    void ItemChange(QString msg);

    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(int id, QByteArray msg);
    void InitializesItems(void);
    void SendStatusCmd(void);
    void ReadStatus(QByteArray msg);
    void SendSampleCmd(void);
    void SendStartCmd(quint8 pos);
    void ReadResult(QByteArray msg);
    void ReadWave(QByteArray msg);
    void ReadWaveOk(QByteArray msg);
    void SendWave(QByteArray msg);
    void TestDir();
    void SendStopCmd(void);
    void SendConfigCmd(void);

    bool WaitTestTimeOut(quint16 t);
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
    QList<QDoubleSpinBox*> Max;
    QList<int> WaveNumber;
    QList<int> Area;
    QList<int> Freq;

    quint8 station;
};

#endif // PAGEMAG_H
