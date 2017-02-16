#ifndef CONFIGBLOCK_H
#define CONFIGBLOCK_H

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
class ConfigBlock;
}

class ConfigBlock : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigBlock(QWidget *parent = 0);
    ~ConfigBlock();

private:
    Ui::ConfigBlock *ui;

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
    void TestSample(void);
    void TestSampleOver(void);
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
    QStringList Items;
    QList<double> Volt;
    QList<double> Curr;
    QList<double> Power;
};

#endif // CONFIGBLOCK_H
