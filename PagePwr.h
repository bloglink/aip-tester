#ifndef PAGEPWR_H
#define PAGEPWR_H


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
    void SendMessage(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void DatSave(void);
    void ItemClick(int r,int c);

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
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
private:
    QSettings *set;
    bool isCheckOk;
    bool Testing;
    quint16 TimeOut;
    quint16 Time;
    QStringList Items;
    QList<double> Volt;
    QList<double> Curr;
    QList<double> Power;

    QString Judge;
    QString FileInUse;

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
};

#endif // PAGEPWR_H
