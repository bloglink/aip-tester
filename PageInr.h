#ifndef PAGEINR_H
#define PAGEINR_H

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
#include "define.h"

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
    void SendMessage(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void InitializesWindow(void);
    void InitializesButton(void);
    void BtnJudge(int id);
    void InitializesSetting(void);
    void SaveSetting(void);

    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void InitializesItem(void);
    void SendStatusCmd(void);
    void ReadStatus(QByteArray msg);
    void SendStartCmd(quint8 pos);
    void ReadResult(QByteArray msg);
    void SendStopCmd(void);
    void SendConfigCmd(void);
    void SendAlarmCmd(quint8 port);

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

#endif // PAGEINR_H
