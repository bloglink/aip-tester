#ifndef PAGEDCR_H
#define PAGEDCR_H

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
    void SendMessage(quint16 addr,quint16 cmd,QByteArray data);
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

    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void TestInit(void);
    void TestCheck(void);
    void TestStatus(QByteArray msg);
    void TestStart(quint8 pos);
    void TestWait(void);
    void TestJudge(void);
    void TestResult(QByteArray msg);
    double TestOffset(double t,quint8 num);
    void TestStop(void);
    void TestConfig(void);
    int TestGear(int row);
    void TestVersion(void);
    void TestDelay(int ms);
    bool TestTimeOut(quint16 t);

    void MetalChange(int index);
    void UnitChange(int index);
    void MinResChange(double x);
    void MaxResChange(double x);
    void StdResChange(double x);
    virtual void showEvent(QShowEvent*);
    virtual void hideEvent(QHideEvent*);
private:
    QSettings *set;
    bool isCheckOk;
    bool Testing;
    quint16 TimeOut;
    PageNum *input;
    QStringList Items;
    QList<double> Results;
    QString Judge;
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
