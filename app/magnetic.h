/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       反嵌配置
*******************************************************************************/
#ifndef MAGNETIC_H
#define MAGNETIC_H

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

#include "lib/qcustomplot.h"

#define MAG_WAVE 121

class Magnetic : public QWidget
{
    Q_OBJECT
public:
    explicit Magnetic(QWidget *parent = 0);

signals:
    void sendAppShow(QString win);
    void sendAppMsg(QJsonObject obj);
public slots:
    void drawWave(QString w);
    void parameters(QString w);
private slots:
    void initUI();
    void initView();
    void initWave();
    void initButton();
    void initLayout();
    void initDelegate();
    void readSettings();
    void waveL();
    void waveR();
    void back();
    void recvAppShow(QString win);
    void recvSqlOpen(QString sql);
    void recvAppCmd(QJsonObject msg);
private:
    QTableView *view;
    LQStandardItem *mView;

    QComboBox *turnComboBox;

    QString sqlName;
    QStringList itemNames;

    QList<QCustomPlot*> waves;
    QList<QCPGraph*> graphs;

    QHBoxLayout *btnLayout;

    QMap<int, QString> waveStrings;

    QStringList areasL;
    QStringList freqsL;
    QStringList areasR;
    QStringList freqsR;
    int station;
};

#endif // MAGNETIC_H
