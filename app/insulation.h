/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       绝缘配置
*******************************************************************************/
#ifndef INSULATION_H
#define INSULATION_H

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

class Insulation : public QWidget
{
    Q_OBJECT
public:
    explicit Insulation(QWidget *parent = 0);
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

#endif // INSULATION_H
