/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       匝间配置
*******************************************************************************/
#ifndef IMPULSE_H
#define IMPULSE_H

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QSettings>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QJsonObject>

#include "lqcombobox.h"
#include "lqspinbox.h"
#include "lqdoublespinbox.h"
#include "lqstandarditem.h"
#include "lqreadonlyitem.h"

#include "lib/qcustomplot.h"

#define IMP_WAVE 501

class Impulse : public QWidget
{
    Q_OBJECT
public:
    explicit Impulse(QWidget *parent = 0);

signals:
    void sendAppShow(QString win);
    void sendAppMsg(QJsonObject obj);
public slots:
    void drawWave(QString w);
    void parameters(QString w);
    QString readWaveL(QString w);
    QString readWaveR(QString w);
private slots:
    void initUI();
    void initView();
    void initWave();
    void initButton();
    void initLayout();
    void initDelegate();
    void readSettings();
    void clickView(QModelIndex index);
    void waveL();
    void waveR();
    void waveO();
    void clearWave();
    void back();
    void recvAppShow(QString win);
    void recvSqlOpen(QString sql);
    void recvAppCmd(QJsonObject msg);
private:
    QTableView *view;
    LQStandardItem *mView;

    QComboBox *vacuoComboBox;

    QString sqlName;
    QStringList itemNames;

    QList<QCustomPlot*> waves;
    QList<QCPGraph*> graphs;

    QMap<int, QString> waveStringL;
    QMap<int, QString> waveStringR;
    QStringList freqsL;
    QStringList freqsR;

    QHBoxLayout *btnLayout;

    QList<QPushButton*> btns;

    int station;
};

#endif // IMPULSE_H
