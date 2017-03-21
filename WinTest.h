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
    void InitWindows(void);
    void InitButtons(void);
    void JudgeButtons(int win);
    void InitSettings(void);
    void SaveSettings(void);
    void InitItem(QByteArray msg);
    void ShowItem(QString item);
    void ShowJudge(QString judge);
    void ShowWaveItem(QByteArray msg);
    void ShowWaveByte(QByteArray msg);
    void ShowWaveTest(QByteArray msg);
    void ShowTemperature(QByteArray msg);
    void ShowStation(QByteArray msg);
    void ShowTime(void);
    void ClearWave(void);
    void ClickItem(int r, int c);
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
