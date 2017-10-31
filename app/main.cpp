/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       权限管理
*******************************************************************************/

#include <QApplication>
#include "mainscreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainScreen w;
    w.showMaximized();

    return a.exec();
}
