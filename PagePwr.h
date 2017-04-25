#ifndef PAGEPWR_H
#define PAGEPWR_H

#include <QTime>
#include <cmath>
#include <QDebug>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSettings>
#include <QListView>
#include <QMessageBox>
#include <QHeaderView>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>
#include "define.h"

#define PWR_ROW 3
#define PG_ROW 3

#define PWR_FREE        0
#define PWR_INIT        4
#define PWR_TEST        1

namespace Ui {
class PagePwr;
}

class PagePwr : public QWidget
{
    Q_OBJECT

public:
    explicit PagePwr(QWidget *parent = 0);
    ~PagePwr();

private:
    Ui::PagePwr *ui;

signals:
    void SendCommand(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int id);
    void InitSettings(void);
    void SaveSettings(void);
    void ItemClick(int r,int c);
    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(quint16 addr, QByteArray msg);
    void InitTestItems(void);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendCanCmdStatus(void);
    void SendCanCmdStart(void);
    void SendCanCmdStop(void);
    void SendTestItemTemp(void);
    void SendTestJudge(void);
    void SendTestItem(void);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdResult(QByteArray msg);
    void ReadCanCmdDir(QByteArray msg);
    void ReadCanCmdPGCurrs(QByteArray msg);
    void ReadCanCmdPGFreqs(QByteArray msg);
    void ReadCanCmdPGDutys(QByteArray msg);
    void ReadCanCmdPGWaveStart(void);
    void ReadCanCmdPGWaveStop(void);
    void ReadCanCmdPGWave(QByteArray msg);
    void CalculateResult(void);
    void ClearResults(void);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    QString CurrentSettings(void);
    QString CurrentPorwer(void);
    virtual void showEvent(QShowEvent *);
private:
    QSettings *set;
    quint16 TimeOut;
    quint16 Time;
    QStringList Items;
    QStringList PGItems;
    QList<double> Volt;
    QList<double> Curr;
    QList<double> Power;

    QString Judge;

    QList<QTableWidgetItem*> Enable;
    QList<QDoubleSpinBox*> CurrMax;
    QList<QDoubleSpinBox*> CurrMin;
    QList<QDoubleSpinBox*> PowerMax;
    QList<QDoubleSpinBox*> PowerMin;
    QList<QDoubleSpinBox*> CapMax;
    QList<QDoubleSpinBox*> CapMin;
    QList<QComboBox*> TestDir;
    QList<QDoubleSpinBox*> TestTime;
    QList<QTableWidgetItem*> Grade;
    quint8 TestRow;
    quint8 Mode;
    QString dir;
    quint8 isTestDir;

    QList<QTableWidgetItem*> PGEnable;
    QList<QDoubleSpinBox*> PGUpper;
    QList<QDoubleSpinBox*> PGLower;
    QList<QDoubleSpinBox*> PGDuty;
    QList<QDoubleSpinBox*> PGFreq;
    QList<QDoubleSpinBox*> PGCurr;
    QList<QTableWidgetItem*> PGGrade;
    QList<double> PGCurrs;
    QList<double> PGUppers;
    QList<double> PGLowers;
    QList<double> PGFreqMin;
    QList<double> PGFreqAvr;
    QList<double> PGFreqMax;
    QList<double> PGDutyMin;
    QList<double> PGDutyAvr;
    QList<double> PGDutyMax;
    QByteArray wave;
};

#endif // PAGEPWR_H
