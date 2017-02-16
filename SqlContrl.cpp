#include "SqlContrl.h"

SqlContrl::SqlContrl(QObject *parent) : QObject(parent)
{
    SqlName = "SqlContrl";
}

void SqlContrl::DeviceOpen()
{
    db = QSqlDatabase::addDatabase("QSQLITE",SqlName);
    db.setDatabaseName(SQL);
    if(db.open())
        qDebug()<<"db open";
    else
        qDebug()<<"db error";
}

void SqlContrl::DeviceQuit()
{
    db.close();
}

void SqlContrl::PutItem(QByteArray msg)
{
    QStringList dat = QString(msg).split("@");
    if (dat.size() < 4)
        return;

    QSqlQuery query(db);

    query.exec("select max(id) from TestData");
    query.next();
    int max = query.value(0).toInt();

    query.prepare("insert into TestData (id,item,para,result,judge)""values(:t0,:t1,:t2,:t3,:t4)");
    query.bindValue(":t0",max+1);
    query.bindValue(":t1",dat.at(0));
    query.bindValue(":t2",dat.at(1));
    query.bindValue(":t3",dat.at(2));
    query.bindValue(":t4",dat.at(3));
    query.exec();
}

