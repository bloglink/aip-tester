/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       关于界面
*******************************************************************************/
#ifndef AUTHOR_H
#define AUTHOR_H

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QPushButton>

class Author : public QWidget
{
    Q_OBJECT
public:
    explicit Author(QWidget *parent = 0);
signals:
    void sendAppShow(QString win);
private slots:
    void initUI();
    void clickButton();
    void recvAppShow(QString win);
};

#endif // AUTHOR_H
