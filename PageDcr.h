#ifndef PAGEDCR_H
#define PAGEDCR_H

#include <qmath.h>
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

#include "define.h"
#include "PageNum.h"

#define  ResCu    0.0039  //  铜材料
#define  ResCu_Al 0.0041  //  铜铝材料
#define  ResAl    0.0043  //  铝材料

#define MAX_ROW 8

namespace Ui {
class PageDcr;
}

class PageDcr : public QWidget
{
    Q_OBJECT

public:
    explicit PageDcr(QWidget *parent = 0);
    ~PageDcr();

private:
    Ui::PageDcr *ui;

signals:
    void SendCommand(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void InitializesWindow(void);
    void InitializesButton(void);
    void BtnJudge(int id);
    void InitializesSetting(void);
    void SaveSetting(void);
    void CalculateAuto(void);
    bool CheckSetting(void);
    void ItemClick(int r,int c);
    void ItemChange(QString msg);

    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void InitializesItems(void);
    void SendStatusCmd(void);
    void ReadStatus(QByteArray msg);
    void ReadOffset(QByteArray msg);
    void SendStartCmd(quint8 pos);
    void WaitTestFinished(void);
    void SendTestJudge(void);
    void ReadResult(QByteArray msg);
    double CalculateOffset(double t,quint8 num);
    void CalculateBalance(void);
    void SendStopCmd(void);
    void SendConfigCmd(void);
    int CalculateGear(int row);
    void SendVersionCmd(void);
    void Delay(int ms);
    bool WaitTestTimeOut(quint16 t);
    bool WaitOffset(quint16 t);

    void AutoChangeMetal(int index);
    void AutoChangeUnit(int index);
    void AutoChangeMin(double x);
    void AutoChangeMax(double x);
    void AutoChangeStd(double x);
    virtual void showEvent(QShowEvent*);
    virtual void hideEvent(QHideEvent*);
private:
    QSettings *set;
    bool isCheckOk;
    bool Testing;
    bool Offsetting;
    quint16 TimeOut;
    PageNum *input;
    QStringList Items;
    QList<double> Results;
    QString JudgeAll;
    QString FileInUse;

    QList<QTableWidgetItem*> Enable;
    QList<QTableWidgetItem*> Terminal1;
    QList<QTableWidgetItem*> Terminal2;
    QList<QComboBox*> Metal;
    QList<QComboBox*> Unit;
    QList<QDoubleSpinBox*> Min;
    QList<QDoubleSpinBox*> Max;
    QList<QDoubleSpinBox*> Std;
    QList<QDoubleSpinBox*> Offset;
};

#endif // PAGEDCR_H
