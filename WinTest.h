#ifndef WINTEST_H
#define WINTEST_H

#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QSettings>
#include <QHeaderView>
#include <QButtonGroup>
#include <QDataStream>
#include <QElapsedTimer>

#include "define.h"
#include "Waveform.h"

namespace Ui {
class WinTest;
}

class WinTest : public QWidget
{
    Q_OBJECT

public:
    explicit WinTest(QWidget *parent = 0);
    ~WinTest();

private:
    Ui::WinTest *ui;

signals:
    void SendCommand(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int win);
    void SetInit(void);
    void SetSave(void);
    void ItemInit(QByteArray msg);
    void showItem(QString item);
    void showJudge(QString judge);
    void showWaveItem(QByteArray msg);
    void showWaveByte(QByteArray msg);
    void showWaveTest(QByteArray msg);
    void showTemperature(QByteArray msg);
    void showStation(QByteArray msg);
    void showTime(void);
    void WaveClear(void);
    void ItemClick(int r, int c);
    void ReadMessage(quint16 addr,quint16 cmd,QByteArray dataA);
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
private:
    QTimer *timer;
    QList<Waveform *>wave;
    QString motor_type;
    QStringList Items;
};

#endif // WINTEST_H
