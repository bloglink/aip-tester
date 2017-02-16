#ifndef CONFIGINDL_H
#define CONFIGINDL_H

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

#include "main.h"
#include "InputNum.h"

union  Resultunion // 接收电感数据
{
    float   Result;
    uint8_t dat[4];
};


namespace Ui {
class ConfigIndl;
}

class ConfigIndl : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigIndl(QWidget *parent = 0);
    ~ConfigIndl();

private:
    Ui::ConfigIndl *ui;
signals:
    void TransformCmd(quint16 addr,quint16 cmd,QByteArray data);
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

    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray msg);
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
    InputNum *input;
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

#endif // CONFIGINDL_H
