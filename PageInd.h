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

#define MAX_ROW 8

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
    void SendCommand(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void InitializesWindow(void);
    void InitializesButton(void);
    void BtnJudge(int id);
    void InitializesSetting(void);
    void SaveSetting(void);
    void CalculateAuto(void);
    void ItemClick(int r,int c);
    void ItemChange(QString msg);
    int CalculateGear(int row);
    int CalculateMode(int row);

    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void InitializesItem(void);
    void SendStatusCmd(void);
    void ReadStatus(QByteArray msg);
    void SendStartCmd(quint8 pos);
    void ReadResult(QByteArray msg);
    void ReadOffset(QByteArray msg);
    void SendStopCmd(void);
    void SendConfigCmd(void);

    bool WaitTestOver(quint16 t);
    bool WaitOffset(quint16 t);
    void Delay(int ms);
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
private:
    QSettings *set;
    bool isCheckOk;
    bool Testing;
    bool Offsetting;
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
    QList<QDoubleSpinBox*> Offset;

    Resultunion  Result1;
    Resultunion  Result2;
};

#endif // PAGEIND_H
