/**
  ******************************************************************************
  * @file    SqlData.cpp
  * @author  link
  * @version 2.0.0.0
  * @date    2017-02-27
  * @brief   Sql thread
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "SqlData.h"
/**
  * @brief  Initializes
  * @param  parent:parent object
  * @retval None
  */
SqlData::SqlData(QObject *parent) : QObject(parent)
{

}
/**
  * @brief  Open sql database
  * @param  None
  * @retval None
  */
void SqlData::DeviceOpen()
{
    db = QSqlDatabase::addDatabase("QSQLITE","SQL");
    db.setDatabaseName(SQL_PATH);
    if(db.open())
        qDebug()<<QTime::currentTime().toString()<<"打开数据库OK";
    else
        qDebug()<<QTime::currentTime().toString()<<"打开数据库Error";
}
/**
  * @brief  Close sql database
  * @param  None
  * @retval None
  */
void SqlData::DeviceQuit()
{
    db.close();
}
/**
  * @brief  insert sql data
  * @param  msg:data to insert
  * @retval None
  */
void SqlData::Write(QByteArray msg)
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

