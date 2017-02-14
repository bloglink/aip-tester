#ifndef CWINSETDCR_H
#define CWINSETDCR_H

#include <cmath>
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
public slots:
    void ExcuteCmd(QByteArray msg);
    void CmdCheckState(void);
    void CmdStartTest(quint8 pos);
    void CmdStopTest(void);
    void CmdConfigure(void);
    void CmdCheckVersion(void);
    bool WaitTestOver(quint16 t);
public:
    QStringList ListItem;
    QStringList ListPara;
    QStringList ListResult;
    QStringList ListJudge;
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
    void UpdateState(QByteArray msg);
    void UpdateTestData(QByteArray msg);
    void UpdateResult(QByteArray msg);
    void UpdateJudge(QByteArray msg);
    void UpdateUnbalance(void);
    int Gear(int row);
    double OffsetValue(double t,quint8 num);
    void Delay(int ms);
    virtual void showEvent(QShowEvent*);
    virtual void hideEvent(QHideEvent*);
    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray msg);
    void ShowInit(void);
private:
    QSettings *set;
    bool isCheckOk;
    bool Testing;
    quint16 TimeOut;
    InputNum *input;
    QStringList Items;

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
