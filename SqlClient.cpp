/**
  ******************************************************************************
  * @file    SqlClient.cpp
  * @author  link
  * @version 2.0.0.0
  * @date    2017-02-27
  * @brief   Sql thread
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "SqlClient.h"

SqlClient::SqlClient(QObject *parent) : QObject(parent)
{

}

void SqlClient::DeviceOpen()
{
    QFile file(SQL_PATH);
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    }

    db = QSqlDatabase::addDatabase("QSQLITE","SQL");
    db.setDatabaseName(SQL_PATH);
    if(!db.open())
        qDebug()<<QTime::currentTime().toString()<<"SQL Error";

    QSqlQuery query(db);
    query.prepare("select count(*) from sqlite_master where type='table' and name='TestData'");
    query.exec();
    if(query.next() && query.value(0).toInt() == 0) {
        query.prepare("create table TestData (id int primary key,item text,para text,result text,judge text,b1 text,b2 text,b3 text)");
        query.exec();
    }
}

void SqlClient::DeviceQuit()
{
    db.close();
}

void SqlClient::Write(QByteArray msg)
{
    QStringList dat = QString(msg).split("@");
    if (dat.size() < 3)
        return;

    QSqlQuery query(db);

    query.exec("select max(id) from TestData");
    query.next();
    int max = query.value(0).toInt();

    query.prepare("insert into TestData (id,item,para,result,judge)""values(:t0,:t1,:t2,:t3,:t4)");
    query.bindValue(":t0",max+1);
    if (dat.size() == 3) {
        query.bindValue(":t1",dat.at(0));
        query.bindValue(":t2",QDate::currentDate());
        query.bindValue(":t3",dat.at(1));
        query.bindValue(":t4",dat.at(2));
    }
    if (dat.size() == 4) {
        query.bindValue(":t1",dat.at(0));
        query.bindValue(":t2",dat.at(1));
        query.bindValue(":t3",dat.at(2));
        query.bindValue(":t4",dat.at(3));
    }
    query.exec();
}
/*********************************END OF FILE**********************************/


