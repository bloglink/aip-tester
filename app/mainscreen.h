/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       界面框架
*******************************************************************************/
#ifndef MAINSCREEN_H
#define MAINSCREEN_H

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

#include "udpsocket.h"

#include "lqmessagebox.h"


#include "signin.h"
#include "author.h"
#include "system.h"
#include "config.h"
#include "sqlite.h"
#include "tester.h"

#include "resistance.h"
#include "insulation.h"
#include "magnetic.h"
#include "voltage.h"
#include "impulse.h"

#define JUNOIR_INIT 0x00
#define JUNOIR_IDLE 0x01
#define JUNOIR_TEST 0x02
#define JUNOIR_CONF 0x03
#define JUNOIR_STOP 0x04
#define JUNOIR_WAIT 0x05
#define JUNOIR_OVER 0x06
#define JUNOIR_NETS 0x07
#define JUNOIR_NETP 0x08

#define MIDDLE_INIT 0x00
#define MIDDLE_IDLE 0x01
#define MIDDLE_TEST 0x02
#define MIDDLE_CONF 0x03
#define MIDDLE_STOP 0x04
#define MIDDLE_WAIT 0x05
#define MIDDLE_OVER 0x06

#define SENOIR_DCR 0x01
#define SENOIR_MAG 0x02
#define SENOIR_INR 0x03
#define SENOIR_ACW 0x04
#define SENOIR_DCW 0x05
#define SENOIR_IMP 0x06
#define SENOIR_PWR 0x07
#define SENOIR_IND 0x08
#define SENOIR_LCK 0x09
#define SENOIR_LVS 0x0A
#define SENOIR_HAL 0x0B
#define SENOIR_LOD 0x0C
#define SENOIR_NLD 0x0D
#define SENOIR_BEF 0x0E

#define JUNIOR 0
#define MIDDLE 1
#define SENIOR 2

class MainScreen : public QWidget
{
    Q_OBJECT
public:
    explicit MainScreen(QWidget *parent = 0);

signals:
    void sendAppShow(QString win);
    void sendSqlOpen(QString name);
    void sendNetMsg(QJsonObject dat);
    void sendAppCmd(QJsonObject dat);
public slots:
    void openSqlite();
private slots:
    void initUI();
    void initSkin();
    void initPopup();
    void initLayout();
    void initSignIn();
    void initAuthor();
    void initSystem();
    void initConfig();
    void initSqlite();
    void initTester();
    void initSocket();
    void initVersion();
    void initSettings();
    void saveSettings();
    void initTaskThread();
    void sendNetXml(QJsonObject msg);
    void sendNetCmd(QString msg);
    void sendNetShow(QString sourceName);

    double getMasterId(QString masterName);
    double getSourceId(QString sourceName);
    double getAction(double masterId, double sourceId);
    double getGlobalParameter(QString name);
    void recvAppShow(QString win);
    void recvAppMsg(QJsonObject msg);
    void recvNetMsg(QString msg);

    void warnning(QString dat);
    quint8 getState(quint8 i, quint32 s);
    void setState(quint8 i, quint8 s);
    void setShift(quint8 i, quint8 s);
    quint8 getNextTestItem(int t);
    void taskThread();
    void heartThread();
    void juniorThread();
    void middleThread();
    void saveThread();
    void initThread();
    void idleThread();
    void overThread();
    void stopThread();

    QString CurrentSettings();
    void closeEvent(QCloseEvent *e);

private:
    LQMessageBox *popup;
    QStackedWidget *stack;
    SignIn *signin;
    Author *author;
    System *system;
    Config *config;
    Sqlite *sqlite;
    Tester *tester;

    Resistance *dcr;
    Insulation *inr;
    Magnetic *mag;
    Voltage *acw;
    Impulse *imp;

    QString masterName;
    QString sqlName;
    QSqlDatabase db;

    UdpSocket udp;

    quint32 gState;
    quint32 gShift;
    quint8 station;

    QJsonObject configs;
    QJsonObject sState;
    QJsonObject sShift;
};

#endif // MAINSCREEN_H
