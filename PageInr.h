#ifndef PAGEINR_H
#define PAGEINR_H

#include <qmath.h>
#include <QDate>
#include <QDebug>
#include <QWidget>
#include <QListView>
#include <QSettings>
#include <QComboBox>
#include <QMessageBox>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>
#include <QCheckBox>
#include "define.h"


#define INR_FREE        0
#define INR_INIT        4
#define INR_TEST        1
#define INR_ROW         4


namespace Ui {
class PageInr;
}

class PageInr : public QWidget
{
    Q_OBJECT

public:
    explicit PageInr(QWidget *parent = 0);
    ~PageInr();

private:
    Ui::PageInr *ui;

signals:
    void SendCommand(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int id);
    void InitSettings(void);
    void SaveSettings(void);
    void ItemClick(int r, int c);
    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdResult(QByteArray msg);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendTestItemTemp(void);
    void SendTestItem(void);
    void SendTestItemError(QString s);
    void SendTestJudge(void);
    void SendCanCmdStatus(void);
    void SendCanCmdStart(quint8 pos);
    void SendCanCmdStop(void);
    void SendCanCmdConfig(int row);
    void ClearResults(void);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    quint16 GetTeminal(int r, int c);
    void InitInput(int r, int c);
    void EnsureInput(void);
    QString CurrentSettings(void);
    bool EnablePhase(void);
    virtual void showEvent(QShowEvent*);
private:
    QSettings *ini;

    QList<double> Volt;
    QList<double> Res;

    QString Judge;
    QString FileInUse;
    QStringList Items;

    quint8 Mode;
    quint16 TimeOut;
    QList<QTableWidgetItem*> Enable;
    QList<QTableWidgetItem*> Terminal1;
    QList<QTableWidgetItem*> Terminal2;
    QList<QDoubleSpinBox*> Vol;
    QList<QDoubleSpinBox*> Min;
    QList<QDoubleSpinBox*> Max;
    QList<QDoubleSpinBox*> Time;
    QList<QDoubleSpinBox*> Offset;
    quint8 TestRow;
    QList<QCheckBox*> Check;
};

#endif // PAGEINR_H
