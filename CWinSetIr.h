#ifndef CWINSETIR_H
#define CWINSETIR_H

#include <QDebug>
#include <QWidget>
#include <QListView>
#include <QSettings>
#include <QComboBox>
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
public slots:
    void DisplayInit(void);
    void ExcuteCmd(QByteArray msg);
    void CmdCheckState(void);
    void CmdStartTest(quint8 pos);
    void CmdStopTest(void);
    void CmdConfigure(void);
    void CmdOnOff(quint8 port);
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
    void UpdateResult(QByteArray msg);
    void UpdateJudge(QByteArray msg);
    void UpdateState(QByteArray msg);
    void UpdateTestData(QByteArray msg);
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

};

#endif // CWINSETIR_H
