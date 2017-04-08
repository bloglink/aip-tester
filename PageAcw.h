#ifndef PAGEACW_H
#define PAGEACW_H

#include <qmath.h>
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

#define ACW_FREE        0
#define ACW_INIT        4
#define ACW_TEST        1

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
    void SendCommand(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int id);
    void InitSettings(void);
    void SaveSettings(void);
    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdResult(QByteArray msg);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendTestItemTemp(void);
    void SendTestItem(void);
    void SendTestJudge(void);
    void SendCanCmdStart(quint8 pos);
    void SendCanCmdStop(void);
    void SendCanCmdConfig(void);
    void ClearResults(void);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    QString CurrentSettings(void);
    virtual void showEvent(QShowEvent*);
private:
    quint16 TimeOut;
    QList<double> Volt;
    QList<double> Curr;
    QStringList Items;
    QString Judge;
    quint8 Mode;
};

#endif // PAGEACW_H
