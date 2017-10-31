/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       权限管理界面
*******************************************************************************/
#include "action.h"

Action::Action(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Action::initUI()
{
    initView();
    initLayout();
}

void Action::initView()
{
    master = new QTableView(this);
    master->setItemDelegate(new LQReadOnlyItem(this));
    master->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(master, SIGNAL(clicked(QModelIndex)), this, SLOT(updateSource(QModelIndex)));

    source = new QTableView(this);
    source->setItemDelegate(new LQReadOnlyItem(this));
    source->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(source, SIGNAL(clicked(QModelIndex)), this, SLOT(changeAction(QModelIndex)));
}

void Action::initLayout()
{
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(master, 0, 0);
    layout->addWidget(source, 0, 1);

    this->setLayout(layout);
}

void Action::createSqlite()
{
    QSqlQuery query(QSqlDatabase::database(sqlName));
    //    query.exec("drop table action");
    QString cmd;
    cmd = "create table if not exists action (";
    cmd += "id integer primary key, master integer, source integer, action integer)";
    if (!query.exec(cmd)) {
        qDebug() << "create table action Error";
    }
}

void Action::updateSqlite()
{
    mAction = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mAction->setTable("action");
    mAction->select();

    mMaster = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mMaster->setTable("master");
    mMaster->select();

    master->setModel(mMaster);
    master->hideColumn(0);
    master->hideColumn(2);
//    master->hideRow(0);

    mSource = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mSource->setTable("source");
    mSource->select();
    mSource->setCheckable(1);
    source->setModel(mSource);
    source->hideColumn(0);
}

void Action::updateSource(QModelIndex index)
{
    QSqlQuery query(QSqlDatabase::database(sqlName));

    double masterId = getMasterId(index);
    QList<double> sourceIds;
    QList<double> actionValues;

    query.prepare("select source, action from action where master=:master");
    query.bindValue(":master", masterId);
    query.exec();
    while (query.next()) {
        sourceIds.append(query.value(0).toDouble());
        actionValues.append(query.value(1).toDouble());
    }

    for (int i=0; i < mSource->rowCount(); i++) {
        QModelIndex indexSource = mSource->index(i, 1);
        double sourceId = getSourceId(indexSource);
        if (sourceIds.contains(sourceId)) {
            int row = sourceIds.indexOf(sourceId);
            mSource->setData(indexSource, actionValues.at(row), Qt::CheckStateRole);
        } else {
            mSource->setData(indexSource, Qt::Unchecked, Qt::CheckStateRole);
        }
    }
    source->update();
}

void Action::changeAction(QModelIndex index)
{
    QSqlQuery query(QSqlDatabase::database(sqlName));

    double masterId = getMasterId(master->currentIndex());
    double sourceId = getSourceId(index);
    double actionId = snow.getId();
    double actionValue = mSource->data(index, Qt::CheckStateRole).toInt();

    query.prepare("select id from action where master=:master and source=:source");
    query.bindValue(":master", masterId);
    query.bindValue(":source", sourceId);
    query.exec();
    if (query.next()) {
        actionId = query.value(0).toDouble();
    }

    query.prepare("replace into action values(?,?,?,?)");
    query.bindValue(0, actionId);
    query.bindValue(1, masterId);
    query.bindValue(2, sourceId);
    query.bindValue(3, actionValue);
    query.exec();
}

double Action::getMasterId(QModelIndex index)
{
    QModelIndex indexMaster = mMaster->index(index.row(), 0);
    double id = mMaster->data(indexMaster, Qt::DisplayRole).toDouble();
    return id;
}

double Action::getSourceId(QModelIndex index)
{
    QModelIndex indexMaster = mSource->index(index.row(), 0);
    double id = mSource->data(indexMaster, Qt::DisplayRole).toDouble();
    return id;
}

void Action::recvAppShow(QString win)
{
    if (win == this->objectName()) {
        mAction->select();
        mMaster->select();
        mSource->select();
    }
}

void Action::recvSqlOpen(QString sql)
{
    sqlName = sql;
    createSqlite();
    updateSqlite();
}
