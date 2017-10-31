/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电阻配置
*******************************************************************************/
#ifndef RESISTANCE_H
#define RESISTANCE_H

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

class Resistance : public QWidget
{
    Q_OBJECT
public:
    explicit Resistance(QWidget *parent = 0);
signals:
    void sendAppShow(QString win);
    void sendAppMsg(QJsonObject obj);
private slots:
    void initUI();
    void initView();
    void initButton();
    void initLayout();
    void initDelegate();
    void readSettings();
    void autoInput();
    void autoCal();
    void back();
    void recvAppShow(QString win);
    void recvSqlOpen(QString sql);
    void recvAppCmd(QJsonObject msg);
private:
    QTableView *view;
    LQStandardItem *mView;

    QSpinBox *nounSpinBox;
    QSpinBox *tempSpinBox;
    QCheckBox *compensation;
    QDoubleSpinBox *timeBox;

    QSpinBox *minSpinBox;
    QSpinBox *maxSpinBox;


    QHBoxLayout *btnLayout;

    QString sqlName;
    QStringList itemNames;
};

#endif // RESISTANCE_H
