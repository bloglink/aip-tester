#ifndef CWINSETIR_H
#define CWINSETIR_H

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
#include "main.h"

namespace Ui {
class CWinSetIr;
}

class CWinSetIr : public QWidget
{
    Q_OBJECT

public:
    explicit CWinSetIr(QWidget *parent = 0);
    ~CWinSetIr();

private:
    Ui::CWinSetIr *ui;
signals:
    void TransformCmd(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void DatSave(void);

    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void TestInit(void);
    void TestCheck(void);
    void TestCheckOk(QByteArray msg);
    void TestStart(quint8 pos);
    void TestResult(QByteArray msg);
    void TestStop(void);
    void TestConfig(void);
    void TestAlarm(quint8 port);

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
    QList<double> Res;
    QStringList Items;
    QString Judge;
    QString FileInUse;

};

#endif // CWINSETIR_H
