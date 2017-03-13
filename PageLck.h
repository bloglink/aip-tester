#ifndef PAGELCK_H
#define PAGELCK_H

#include <QDebug>
#include <QWidget>
#include <QListView>
#include <QSettings>
#include <QComboBox>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include "define.h"

#define LCK_FREE        0
#define LCK_INIT        4
#define LCK_TEST        1
#define LCK_SAMPLE      2

namespace Ui {
class PageLck;
}

class PageLck : public QWidget
{
    Q_OBJECT

public:
    explicit PageLck(QWidget *parent = 0);
    ~PageLck();

private:
    Ui::PageLck *ui;

signals:
    void SendMessage(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void BtnJudge(int id);
    void InitSettings(void);
    void SaveSettings(void);

    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void InitTestItems(void);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdSample(QByteArray msg);
    void TestSample(void);
    void TestSampleOver(void);
    void SendCanCmdStart(quint8 pos);
    void ReadCanCmdResult(QByteArray msg);
    void SendCanCmdStop(void);

    bool WaitTimeOut(quint16 t);
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

    quint8 Mode;
};

#endif // PAGELCK_H
