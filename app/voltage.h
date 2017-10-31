/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       交耐配置
*******************************************************************************/
#ifndef VOLTAGE_H
#define VOLTAGE_H

#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QSettings>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QJsonObject>

#include "lqcombobox.h"
#include "lqspinbox.h"
#include "lqdoublespinbox.h"
#include "lqstandarditem.h"

class Voltage : public QWidget
{
    Q_OBJECT
public:
    explicit Voltage(QWidget *parent = 0);
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
    void back();
    void recvAppShow(QString win);
    void recvSqlOpen(QString sql);
    void recvAppCmd(QJsonObject msg);

private:
    QTableView *view;
    LQStandardItem *mView;
    QStringList itemNames;
    QHBoxLayout *btnLayout;
    QString sqlName;
};

#endif // VOLTAGE_H
