#ifndef PAGEIND_H
#define PAGEIND_H

#include <cmath>
#include <QDate>
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
#include <stdint.h>

#include "define.h"
#include "PageNum.h"
union  Resultunion // 接收电感数据
{
    float   Result;
    uint8_t dat[4];
};

namespace Ui {
class PageInd;
}

class PageInd : public QWidget
{
    Q_OBJECT

public:
    explicit PageInd(QWidget *parent = 0);
    ~PageInd();

private:
    Ui::PageInd *ui;

signals:
    void SendMessage(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void DatSave(void);
    void DatAuto(void);
    void ItemClick(int r,int c);
    void ItemChange(QString msg);
    int TestGear(int row);
    int TestMode(int row);

    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void TestInit(void);
    void TestCheck(void);
    void TestCheckOk(QByteArray msg);
    void TestStart(quint8 pos);
    void TestResult(QByteArray msg);
    void TestStop(void);
    void TestConfig(void);

    bool WaitTestOver(quint16 t);
    void Delay(int ms);
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
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
    QList<QComboBox*> Unit;
    QList<QDoubleSpinBox*> Min;
    QList<QDoubleSpinBox*> Max;
    QList<QDoubleSpinBox*> QMin;
    QList<QDoubleSpinBox*> QMax;
    QList<QDoubleSpinBox*> Std;

    Resultunion  Result1;
    Resultunion  Result2;
};

#endif // PAGEIND_H
