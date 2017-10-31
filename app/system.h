/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       系统界面
*******************************************************************************/
#ifndef PERMISSION_H
#define PERMISSION_H

#include <QFile>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QStackedWidget>
#include <QApplication>
#include <QMessageBox>

#include <QSqlQuery>
#include <QSqlDatabase>

#include "signin.h"
#include "global.h"
#include "master.h"
#include "author.h"
#include "action.h"
#include "source.h"
#include "backup.h"
#include "logger.h"

#include "iobrd.h"

class System : public QWidget
{
    Q_OBJECT

public:
    explicit System(QWidget *parent = 0);
    ~System();
signals:
    void sendAppShow(QString win);
    void sendSqlOpen(QString name);
    void sendAppMsg(QJsonObject obj);
public slots:
    void setIobrd(QString s);
    void setMasterName(QString name);
private slots:
    void initUI();
    void initLayout();
    void initGlobal();
    void initIobrd();
    void initMaster();
    void initSource();
    void initAction();
    void initBackup();
    void initLogger();
    void initLogout();
    void clickButton();
    double getMasterId(QString masterName);
    double getSourceId(QString sourceName);
    double getAction(double masterId, double sourceId);
    void recvAppShow(QString win);
    void recvSqlOpen(QString sql);
private:
    QString masterName;
    QString sqlName;
    QSqlDatabase db;
    QStackedWidget *stack;
    QVBoxLayout *btnLayout;
    QList<QPushButton*> buttons;

    Global *global;
    Master *master;
    Action *action;
    Source *source;
    Backup *backup;
    Logger *logger;

    Iobrd *iobrd;
};

#endif // PERMISSION_H
