#ifndef PAGEDCR_H
#define PAGEDCR_H

#include <QUuid>
#include <QFile>
#include <QDebug>
#include <QWidget>
#include <QSettings>
#include <QShowEvent>
#include <QElapsedTimer>

#include <qmath.h>
#include "delegate.h"

#define DCR_MAX 8
#define  ResCu    0.0039  //  铜材料
#define  ResCu_Al 0.0041  //  铜铝材料
#define  ResAl    0.0043  //  铝材料

namespace Ui {
class PageDcr;
}

class PageDcr : public QWidget
{
    Q_OBJECT

public:
    explicit PageDcr(QWidget *parent = 0);
    ~PageDcr();

signals:
    void SendVariant(QVariantHash s);
    void CanMsg(QByteArray msg);
private slots:
    void InitWin(void);
    void InitSet(void);
    void SaveSet(void);
    void InitItems(void);
    void ExcuteCanCmd(int addr, QByteArray msg);
    void ReadCanCmdStatus(QByteArray msg);
    void SendWarnning(QString s);
    void ReadCanCmdResult(QByteArray msg);
    void ReadCanCmdBack(QByteArray msg);
    void SendCanCmdStatus();
    void SendCanCmdStart(quint8 pos);
    void SendCanCmdStop();
    void SendCanCmdConfig();
    void SendCanCmdDebug();
    void SendCanCmdCode();
    void SendCanCmdVersion();
    void SendAlarm(quint8 addr);
    int CalculateGear(int row);
    void ReadVariant(QVariantHash s);
    void GoToWindow(QString w);
    void SendTestItemsAllEmpty();
    void SendTestItemsAllError();
    void SendTestItems(int num);
    void SendTestItemsAll();
    void SendTestPause();
    void SendTestSave();
    void SendTemperature(QByteArray msg);
    void SendSafeCheck(QVariantHash s);
    void TestThread(QVariantHash hash);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    virtual void showEvent(QShowEvent *e);
    void on_btn1_clicked();

    void on_btn2_clicked();

    void on_btn3_clicked();

    void on_btn4_clicked();

private:
    Ui::PageDcr *ui;
    StandardItemModel *m;
    QSettings *ini;
    QString TestStatus;
    quint16 TimeOut;
    QList<QVariantHash> ItemView;
    QString Judge;
    quint8 stat;
};

#endif // PAGEDCR_H
