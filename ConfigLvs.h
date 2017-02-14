#ifndef CONFIGLVS_H
#define CONFIGLVS_H

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
#include "main.h"

namespace Ui {
class ConfigLvs;
}

class ConfigLvs : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigLvs(QWidget *parent = 0);
    ~ConfigLvs();

private:
    Ui::ConfigLvs *ui;
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

    void UpdateState(QByteArray msg);
    void UpdateTestData(QByteArray msg);
    void UpdateResult(QByteArray msg);
    void UpdateJudge(QByteArray msg);
    void UpdateUnbalance(void);
    void Delay(int ms);
private:
    QSettings *set;
    bool isCheckOk;
    bool Testing;
    quint16 TimeOut;
};

#endif // CONFIGLVS_H
