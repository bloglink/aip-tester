/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170422
 * author:      zhaonanlin
 * brief:       开机主页
*******************************************************************************/
#include "desktop.h"
#include "ui_desktop.h"

Desktop::Desktop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Desktop)
{
    ui->setupUi(this);
    InitWin();
    InitCan();
    TestStatus = "free";
}

Desktop::~Desktop()
{
    delete ui;
}

void Desktop::InitWin()
{
#ifdef __arm__
    ui->btnQuit->hide();
//    this->setWindowFlags(Qt::FramelessWindowHint);
#endif
    ui->keybord->setCurrentIndex(0);
    //设置界面风格
    QFile file;
    QString qss;
    file.setFileName(":/source/SoftRainBlue.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);
}

void Desktop::InitCan()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ReadCan()));
    if (!can.DeviceOpen())
        qDebug() << "can open fail";
    else
        timer->start(50);
    count = 0;
}

void Desktop::ReadCan()
{
    if (can.DeviceRead()) {
        count++;
        qDebug() <<count << can.GetAddress() << can.GetMessage().toHex();
    }
}
