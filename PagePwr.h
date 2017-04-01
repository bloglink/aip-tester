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

#define PWR_ROW 6

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
    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void InitTestItems(void);
    void SendTestItems(void);
    void SendTestItemsAllError(void);
    void SendCanCmdStatus(void);
    void SendCanCmdStart(void);
    void SendCanCmdStop(void);
    void SendItemTemp(void);
    void SendTestJudge(void);
    void SendItemJudge(void);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdResult(QByteArray msg);
    void ReadCanCmdDir(QByteArray msg);
    void CalculateResult(void);
    void ClearResults(void);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
private:
    QSettings *set;
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
    quint8 TestRow;
    quint8 Mode;
    quint8 PowerSupply;
    QString dir;
    quint8 isTestDir;
};

#endif // PAGEPWR_H
