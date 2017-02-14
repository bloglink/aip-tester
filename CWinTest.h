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
    void DisplayItem(QStringList item);
    void DisplayPara(QStringList para);
    void DisplayResult(QStringList result);
    void DisplayJudge(QStringList judge);
    void DisplayWaveItem(QByteArray msg);
    void DisplayWaveByte(QByteArray msg);
    void DisplayWaveTest(QByteArray msg);
    void DisplayTemp(QByteArray msg);
    void DisplayPos(QByteArray msg);
    void DisplayState(QByteArray msg);
    void DisplayAmount(QStringList amount);
    void WaveClear(void);
public:
    QStringList ListItem;
    QStringList ListPara;
    QStringList ListResult;
    QStringList ListJudge;
private slots:
    void WinInit(void);
    void KeyInit(void);
    void KeyJudge(int win);
    void DatInit(void);
    void DatSave(void);
    void DisplayTime(void);
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
    bool TestOn;

};

#endif // CWINTEST_H
