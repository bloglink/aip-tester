#ifndef CWINTEST_H
#define CWINTEST_H

#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QSettings>
#include <QHeaderView>
#include <QButtonGroup>
#include <QDataStream>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QMessageBox>

#include "Waveform.h"
#include "main.h"

namespace Ui {
class CWinTest;
}

class CWinTest : public QWidget
{
    Q_OBJECT

public:
    explicit CWinTest(QWidget *parent = 0);
    ~CWinTest();

private:
    Ui::CWinTest *ui;

signals:
    void TransformCmd(quint16 addr,quint16 cmd,QByteArray data);
public slots:
    void ShowItems(QStringList item);
    void ShowItem(QString item);
    void ShowJudge(QString judge);
    void ShowWaveItem(QByteArray msg);
    void ShowWaveByte(QByteArray msg);
    void ShowWaveTest(QByteArray msg);
    void ShowTemp(QByteArray msg);
    void ShowPos(QByteArray msg);
    void ShowAmount(QStringList amount);
    void ShowTime(void);
    void WaveClear(void);
private slots:
    void WinInit(void);
    void KeyInit(void);
    void KeyJudge(int win);
    void DatInit(void);
    void DatSave(void);

    void ItemClick(int r, int c);
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
private:
    QTimer *timer;
    QList<Waveform *>wave;
    QButtonGroup *btnGroup;

    QSettings *set;
    QSettings *global;

    QList<QByteArray> WaveMAG;
    QList<QByteArray> WaveIMP;

    bool TestBusy;
    bool TestRunning;
    quint16 TestStep;
    quint16 TestTime;

};

#endif // CWINTEST_H
