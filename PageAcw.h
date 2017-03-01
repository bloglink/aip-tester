#ifndef PAGEACW_H
#define PAGEACW_H

#include <QDate>
#include <QDebug>
#include <QWidget>
#include <QListView>
#include <QSettings>
#include <QComboBox>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include "define.h"

namespace Ui {
class PageAcw;
}

class PageAcw : public QWidget
{
    Q_OBJECT

public:
    explicit PageAcw(QWidget *parent = 0);
    ~PageAcw();

private:
    Ui::PageAcw *ui;

signals:
    void SendMessage(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void DatSave(void);

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

    virtual void showEvent(QShowEvent*);
    virtual void hideEvent(QHideEvent*);
private:
    QSettings *set;
    bool isCheckOk;
    bool Testing;
    quint16 TimeOut;
    QList<double> Volt;
    QList<double> Curr;
    QStringList Items;
    QString Judge;
    QString FileInUse;
};

#endif // PAGEACW_H
