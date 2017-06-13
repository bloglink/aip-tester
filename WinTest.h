/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       测试显示模块
*******************************************************************************/
#ifndef WINTEST_H
#define WINTEST_H

#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QSettings>
#include <QKeyEvent>
#include <QHeaderView>
#include <QButtonGroup>
#include <QDataStream>
#include <QElapsedTimer>

#include "define.h"
#include "Waveform.h"
#include "WaveBox.h"

#include "qcustomplot.h"

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
    void SendVariant(QVariantHash s);
    void SendCommand(quint16 addr, quint16 cmd, QByteArray data);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int win);
    void InitSettings(void);
    void SaveSettings(void);
    void ShowTime(void);
    void ShowCode(void);
    void ClearWave(void);
    void ClickItem(int r,  int c);
    QString BarCode1(void);
    QString BarCode2(void);
    void showEvent(QShowEvent *);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void ReadVariant(QVariantHash s);
    void GoToWindow(QString w);
    void InitTest(QVariantHash hash);
    void ItemShow(void);
    void UpdateItem(QVariantHash hash);
    void ViewItem(QVariantHash hash);
    void UpdateWave(QVariantHash hash);
    void UpdateJudge(QString judge);
    void WaveView(QVariantHash s);
    void WaveHide(void);
    void SendContrl(QString s);
private:
    QTimer *timer;
    QList<WaveBox *>wave;
    QString motor_type;
    QStringList Items;
    QString code;
    QTimer *codeTimer;
    QList<QVariantHash> ItemView;
    QString TestStatus;

};

#endif // WINTEST_H
