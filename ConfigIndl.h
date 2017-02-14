#ifndef CONFIGINDL_H
#define CONFIGINDL_H

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
public slots:
    void DisplayInit(void);
    void ExcuteCmd(QByteArray msg);
    void CmdCheckState(void);
    void CmdStartTest(quint8 pos);
    void CmdStopTest(void);
    void CmdConfigure(void);
    bool WaitTestOver(void);
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
    void ItemClick(int r,int c);
    void ItemChange(QString msg);
    int Gear(int row);
    int Mode(int row);
    void UpdateState(QByteArray msg);
    void UpdateTestData(QByteArray msg);
    void UpdateResult(QByteArray msg);
    void UpdateJudge(QByteArray msg);
    void UpdateUnbalance(void);
    void Delay(int ms);
    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray msg);
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
private:
    QSettings *set;
    bool isCheckOk;
    bool Testing;
    quint16 TimeOut;
    InputNum *input;

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
