#ifndef CWINSETDCR_H
#define CWINSETDCR_H

#include <cmath>
#include <QDate>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QComboBox>
#include <QSettings>
#include <QListView>
#include <QMessageBox>
#include <QHeaderView>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>

#include "main.h"
#include "InputNum.h"

namespace Ui {
class CWinSetDcr;
}

class CWinSetDcr : public QWidget
{
    Q_OBJECT

public:
    explicit CWinSetDcr(QWidget *parent = 0);
    ~CWinSetDcr();

private:
    Ui::CWinSetDcr *ui;

signals:
    void TransformCmd(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void DatSave(void);
    void DatAuto(void);
    bool DatStdd(void);
    void ItemClick(int r,int c);
    void ItemChange(QString msg);

    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void TestInit(void);
    void TestCheck(void);
    void TestCheckOk(QByteArray msg);
    void TestStart(quint8 pos);
    void TestResult(QByteArray msg);
    double TestOffset(double t,quint8 num);
    void TestStop(void);
    void TestConfig(void);
    int TestGear(int row);
    void TestVersion(void);
    bool WaitTestOver(quint16 t);
    void Delay(int ms);

    virtual void showEvent(QShowEvent*);
    virtual void hideEvent(QHideEvent*);
private:
    QSettings *set;
    bool isCheckOk;
    bool Testing;
    quint16 TimeOut;
    InputNum *input;
    QStringList Items;
    QList<double> Results;
    QString Judge;
    QString FileInUse;

    QTimer *Timer;

    QList<QTableWidgetItem*> Enable;
    QList<QTableWidgetItem*> Terminal1;
    QList<QTableWidgetItem*> Terminal2;
    QList<QComboBox*> Metal;
    QList<QComboBox*> Unit;
    QList<QDoubleSpinBox*> Min;
    QList<QDoubleSpinBox*> Max;
    QList<QDoubleSpinBox*> Std;
};

#endif // CWINSETDCR_H
