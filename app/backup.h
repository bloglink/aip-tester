/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       后台管理
*******************************************************************************/
#ifndef BACKUP_H
#define BACKUP_H

#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>

#include <QSqlQuery>
#include <QSqlDatabase>

#include "lqsnowflake.h"
#include "lqreadonlyitem.h"
#include "lqsqltablemodel.h"

class Backup : public QWidget
{
    Q_OBJECT
public:
    explicit Backup(QWidget *parent = 0);

private slots:
    void initUI();
    void initView();
    void initLayout();
    void createSqlite();
    void updateSqlite();
    void updateBackup(QModelIndex index);
    void insertSqlite();
    void recvAppShow(QString win);
    void recvSqlOpen(QString sql);
private:
    QString sqlName;
    LQSnowflake snow;
    QTableView *view;
    LQSqlTableModel *mView;
};

#endif // BACKUP_H
