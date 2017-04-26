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
    void ItemClickPG(int r,int c);
    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(quint16 addr, QByteArray msg);
    void InitTestItems(void);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendCanCmdStatus(void);
    void SendCanCmdStart(quint8 s);
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
    void SendWave(QByteArray msg);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    QString CurrentSettings(void);
    QString CurrentPorwer(void);
    bool IsPGEnable(void);
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
    QList<QDoubleSpinBox*> PGUpperMin;
    QList<QDoubleSpinBox*> PGUpperMax;
    QList<QDoubleSpinBox*> PGLowerMin;
    QList<QDoubleSpinBox*> PGLowerMax;
    QList<QDoubleSpinBox*> PGDutyMin;
    QList<QDoubleSpinBox*> PGDutyMax;
    QList<QDoubleSpinBox*> PGFreqMin;
    QList<QDoubleSpinBox*> PGFreqMax;
    QList<QDoubleSpinBox*> PGCurrMin;
    QList<QDoubleSpinBox*> PGCurrMax;
    QList<QTableWidgetItem*> PGGrade;
    QList<double> PGCurrs;
    QList<double> PGUppers;
    QList<double> PGLowers;
    QList<double> PGFreqAvr;
    QList<double> PGDutyAvr;
    QByteArray wave;
    QString PGJudge;

    QStringList PGWaveItem;
    QByteArray wave1;
    QByteArray wave2;
    QByteArray wave3;
};

#endif // PAGEPWR_H
