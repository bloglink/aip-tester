#ifndef TEST_H
#define TEST_H

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
class Test;
}

class Test : public QWidget
{
    Q_OBJECT

public:
    explicit Test(QWidget *parent = 0);
    ~Test();

private:
    Ui::Test *ui;

signals:
    void sendMessage(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void initShow(void);
    void initButton(void);
    void judgeButton(int win);
    void initSettings(void);
    void saveSettings(void);
    void initItems(QByteArray msg);
    void showItem(QString item);
    void showJudge(QString judge);
    void showWaveItem(QByteArray msg);
    void showWaveByte(QByteArray msg);
    void showWaveTest(QByteArray msg);
    void showTemperature(QByteArray msg);
    void showStation(QByteArray msg);
    void showTime(void);
    void clearWave(void);
    void ItemClick(int r, int c);
    void readMessage(quint16 addr,quint16 cmd,QByteArray dataA);
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
private:
    QTimer *timer;
    QList<Waveform *>wave;
    QString motor_type;
    QStringList Items;
};

#endif // TEST_H
