/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       后台管理
*******************************************************************************/
#include "backup.h"

Backup::Backup(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Backup::initUI()
{
    initView();
    initLayout();
}

void Backup::initView()
{
    view = new QTableView(this);
    view->setItemDelegate(new LQReadOnlyItem(this));
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(updateBackup(QModelIndex)));
}

void Backup::initLayout()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    this->setLayout(layout);
}

void Backup::createSqlite()
{
    QSqlQuery query(QSqlDatabase::database(sqlName));
//    query.exec("drop table backup");
    QString cmd;
    cmd = "create table if not exists backup (";
    cmd += "id integer primary key, name text, enable integer)";
    if (!query.exec(cmd)) {
        qDebug() << "create table backup Error";
    }
}

void Backup::updateSqlite()
{
    mView = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mView->setTable("backup");
    mView->select();
    if (mView->rowCount() == 0) {
        insertSqlite();
        mView->select();
    }
    view->setModel(mView);
    view->horizontalHeader()->hide();
    view->hideColumn(2);
    mView->setCheckable(0);

    for (int i=0; i < mView->rowCount(); i++) {
        int check = mView->data(mView->index(i, 2), Qt::DisplayRole).toInt();
        mView->setData(mView->index(i, 0), check, Qt::CheckStateRole);
    }
}

void Backup::updateBackup(QModelIndex index)
{
    if (index.column() == 0) {
        int row = index.row();
        int check = mView->data(index, Qt::CheckStateRole).toInt();
        int id = mView->data(index, Qt::DisplayRole).toInt();
        QString name = mView->data(mView->index(row, 1), Qt::DisplayRole).toString();
        QSqlQuery query(QSqlDatabase::database(sqlName));
        query.prepare("replace into backup values (?,?,?)");
        query.bindValue(0, id);
        query.bindValue(1, name);
        query.bindValue(2, check);
        query.exec();
    }
    mView->select();
}

void Backup::insertSqlite()
{
    QSqlQuery query(QSqlDatabase::database(sqlName));
    QStringList names;
    names << "电阻" << "反嵌" << "绝缘" << "交耐" << "直耐"
          << "匝间" << "电参" << "电感" << "堵转" << "低启"
          << "霍尔" << "负载" << "空载" << "BEMF";
    for (int i=0; i < names.size(); i++) {
        query.prepare("insert into backup values(?,?,?)");
        query.bindValue(0, i+1);
        query.bindValue(1, names.at(i));
        query.bindValue(2, Qt::Checked);
        query.exec();
    }
}

void Backup::recvAppShow(QString win)
{
    if (win == this->objectName()) {
        mView->select();
    }
}

void Backup::recvSqlOpen(QString sql)
{
    sqlName = sql;
    createSqlite();
    updateSqlite();
}
