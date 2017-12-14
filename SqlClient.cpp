/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       SQL处理模块
*******************************************************************************/
#include "SqlClient.h"

SqlClient::SqlClient(QObject *parent) : QObject(parent)
{
    count = 0;
}

void SqlClient::DeviceOpen()
{
    QString f = filePath();
    QFile file(f);
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    }
    db = QSqlDatabase::addDatabase("QSQLITE", "SQL");
    db.setDatabaseName(f);
    if (!db.open()) {
        qDebug() << QTime::currentTime().toString() << "SQL Error";
        return;
    }

    QSqlQuery query(db);

    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyyMMdd"); //设置显示格式
    QString cmd = "create table if not exists aip_tail(";
    cmd += "id integer primary key,";
    cmd += "pro_name varchar(10),";
    cmd += "result_num integer,";
    cmd += "result_ok integer,";
    cmd += "result_ng integer,";
    cmd += "result_time varchar(10),";
    cmd += "result_type varchar(10),";
    cmd += "result_percen varchar(10))";

    bool err = query.exec(cmd);
    if(!err)   {
        qDebug()<<"Database Error:"<< db.lastError().text();
        db.close();
        return;
    }
    char Ssql[2048];
    memset(Ssql,0,2048);

    err= query.exec("select *from aip_tail where id = (select max(id) from aip_tail);");
    QString qs1;
    while(query.next()) {
        query.value(0).toInt();
        query.value(2).toInt();
        qs1= query.value(5).toString();
    }
    if(strcmp(qs1.toLocal8Bit().data(),str.toLocal8Bit().data())!=0)
    {
        typeSql("aip9918");
    }
}

void SqlClient::typeSql(QString qs)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyyMMdd"); //设置显示格式
    QSqlQuery query(db);

    char Ssql[2048];
    memset(Ssql,0,2048);
    sprintf(Ssql,"insert into  aip_tail(id,pro_name,result_num,result_ok,result_ng,result_time,result_type,result_percen)values(null,'%s',%d,%d,%d,'%s','%s','%s')"
            ,"总数",0,0,0,str.toLocal8Bit().data(),qs.toLocal8Bit().data(),"0");
    bool   err=query.exec(Ssql);

    memset(Ssql,0,2048);
    sprintf(Ssql,"insert into  aip_tail(id,pro_name,result_num,result_ok,result_ng,result_time,result_type,result_percen)values(null,'%s',%d,%d,%d,'%s','%s','%s')"
            ,"电阻",0,0,0,str.toLocal8Bit().data(),qs.toLocal8Bit().data(),"0");
    err=query.exec(Ssql);

    memset(Ssql,0,2048);
    sprintf(Ssql,"insert into aip_tail(id,pro_name,result_num,result_ok,result_ng,result_time,result_type,result_percen)values(null,'%s',%d,%d,%d,'%s','%s','%s')"
            ,"反嵌",0,0,0,str.toLocal8Bit().data(),qs.toLocal8Bit().data(),"0");
    err=query.exec(Ssql);

    memset(Ssql,0,2048);
    sprintf(Ssql,"insert into aip_tail(id,pro_name,result_num,result_ok,result_ng,result_time,result_type,result_percen)values(null,'%s',%d,%d,%d,'%s','%s','%s')"
            ,"磁旋",0,0,0,str.toLocal8Bit().data(),qs.toLocal8Bit().data(),"0");
    err=query.exec(Ssql);

    memset(Ssql,0,2048);
    sprintf(Ssql,"insert into aip_tail(id,pro_name,result_num,result_ok,result_ng,result_time,result_type,result_percen)values(null,'%s',%d,%d,%d,'%s','%s','%s')"
            ,"绝缘",0,0,0,str.toLocal8Bit().data(),qs.toLocal8Bit().data(),"0");
    err=query.exec(Ssql);

    memset(Ssql,0,2048);
    sprintf(Ssql,"insert into aip_tail(id,pro_name,result_num,result_ok,result_ng,result_time,result_type,result_percen)values(null,'%s',%d,%d,%d,'%s','%s','%s')"
            ,"交耐",0,0,0,str.toLocal8Bit().data(),qs.toLocal8Bit().data(),"0");
    err=query.exec(Ssql);

    memset(Ssql,0,2048);
    sprintf(Ssql,"insert into aip_tail(id,pro_name,result_num,result_ok,result_ng,result_time,result_type,result_percen)values(null,'%s',%d,%d,%d,'%s','%s','%s')"
            ,"直耐",0,0,0,str.toLocal8Bit().data(),qs.toLocal8Bit().data(),"0");
    err=query.exec(Ssql);

    memset(Ssql,0,2048);
    sprintf(Ssql,"insert into aip_tail(id,pro_name,result_num,result_ok,result_ng,result_time,result_type,result_percen)values(null,'%s',%d,%d,%d,'%s','%s','%s')"
            ,"匝间",0,0,0,str.toLocal8Bit().data(),qs.toLocal8Bit().data(),"0");
    err=query.exec(Ssql);

    memset(Ssql,0,2048);
    sprintf(Ssql,"insert into aip_tail(id,pro_name,result_num,result_ok,result_ng,result_time,result_type,result_percen)values(null,'%s',%d,%d,%d,'%s','%s','%s')"
            ,"电感",0,0,0,str.toLocal8Bit().data(),qs.toLocal8Bit().data(),"0");
    err=query.exec(Ssql);

    memset(Ssql,0,2048);
    sprintf(Ssql,"insert into aip_tail(id,pro_name,result_num,result_ok,result_ng,result_time,result_type,result_percen)values(null,'%s',%d,%d,%d,'%s','%s','%s')"
            ,"功率",0,0,0,str.toLocal8Bit().data(),qs.toLocal8Bit().data(),"0");
    err=query.exec(Ssql);

    memset(Ssql,0,2048);
    sprintf(Ssql,"insert into aip_tail(id,pro_name,result_num,result_ok,result_ng,result_time,result_type,result_percen)values(null,'%s',%d,%d,%d,'%s','%s','%s')"
            ,"转向",0,0,0,str.toLocal8Bit().data(),qs.toLocal8Bit().data(),"0");
    err=query.exec(Ssql);

    memset(Ssql,0,2048);
    sprintf(Ssql,"insert into aip_tail(id,pro_name,result_num,result_ok,result_ng,result_time,result_percen)values(null,'%s',%d,%d,%d,'%s','%s','%s')"
            ,"低启",0,0,0,str.toLocal8Bit().data(),qs.toLocal8Bit().data(),"0");
    err=query.exec(Ssql);

    memset(Ssql,0,2048);
    sprintf(Ssql,"insert into aip_tail(id,pro_name,result_num,result_ok,result_ng,result_time,result_type,result_percen)values(null,'%s',%d,%d,%d,'%s','%s','%s')"
            ,"堵转",0,0,0,str.toLocal8Bit().data(),qs.toLocal8Bit().data(),"0");
    err=query.exec(Ssql);
}

void SqlClient::viewSql(QStringList qs)
{
    int restype=0;
    int total=0,resok=0,resng=0;
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyyMMdd"); //设置显示格式

    QSqlQuery query(db);
    char Ssql[2048];
    memset(Ssql,0,2048);
    QString qs1;QString qs2;

    qs1=qs.at(0);
    qs2=qs.at(1);

    sprintf(Ssql,"select *from aip_tail where result_time='%s' and result_type='%s'"\
            ,str.toLocal8Bit().data(),qs2.toLocal8Bit().data());
    query.exec(Ssql);
    query.next();
    restype=query.value(0).toInt();
    if(restype==0)
        typeSql(qs2);

    sprintf(Ssql,"select *from aip_tail where pro_name='%s' and result_time='%s' and result_type='%s';",qs1.toLocal8Bit().data()\
            ,str.toLocal8Bit().data(),qs2.toLocal8Bit().data());
    bool err= query.exec(Ssql);
    while(query.next()) {
        total=query.value(2).toInt();
        resok=query.value(3).toInt();
        resng=query.value(4).toInt();
    }
    total++;
    //更新总数
    memset(Ssql,0,2048);
    sprintf(Ssql,"update aip_tail  set result_num=%d where pro_name='%s' and result_time='%s' and result_type='%s'",total,qs1.toLocal8Bit().data(),\
            str.toLocal8Bit().data(),qs2.toLocal8Bit().data());
    err= query.exec(Ssql);
    if(qs.at(2)=="OK") {
        resok++;
        memset(Ssql,0,2048);
        sprintf(Ssql,"update aip_tail set result_ok=%d where pro_name='%s' and result_time='%s' and result_type='%s'",resok,qs1.toLocal8Bit().data(), \
                str.toLocal8Bit().data(),qs2.toLocal8Bit().data());
        err= query.exec(Ssql);
    } else if(qs.at(2)=="NG") {
        resng++;
        memset(Ssql,0,2048);
        sprintf(Ssql,"update aip_tail set result_ng=%d where pro_name='%s' and result_time='%s' and result_type='%s'",resng,qs1.toLocal8Bit().data(),\
                str.toLocal8Bit().data(),qs2.toLocal8Bit().data());
        err= query.exec(Ssql);
    }

}

void SqlClient::DeviceQuit()
{
    db.close();
}

void SqlClient::Write(QByteArray msg)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyyMMdd"); //设置显示格式

    QDateTime dateTime;
    QString dateTime_str = dateTime.currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QSqlQuery query(db);
    char sSql[2048];
    memset(sSql,0,2048);
    sprintf(sSql,"select max(id) from aip_%s",str.toLocal8Bit().data());
    bool  err=query.exec(sSql);
    query.next();
    int max = query.value(0).toInt();

    /************************/
    if (msg.startsWith("code")) {
        code = msg.remove(0, 4);
        qDebug() << code;
        return;
    }
    QStringList dat = QString(msg).split("@");
    if (dat.size() < 3)
        return;

    ResData str_ResData;

    if (dat.size() == 3) {
        if(dat.at(0)=="总数") {
            count++;
            str_ResData.m_item=dat.at(0);
            str_ResData.m_para=dat.at(1);
            str_ResData.m_result=dat.at(2);
            dateList.append(str_ResData);
            for(int i=0;i<dateList.size();i++) {
                ResData m_strData;
                m_strData = dateList.at(i);

                memset(sSql,0,2048);
                max++;
                sprintf(sSql,"insert into aip_%s(id,item,para,result,judge,b1,b2,b3)values(%d,'%s','%s','%s','%s','%s','%d','%s')",\
                        str.toLocal8Bit().data(),max,m_strData.m_item.toLocal8Bit().data(),m_strData.m_para.toLocal8Bit().data()\
                        ,m_strData.m_result.toLocal8Bit().data(),m_strData.m_judge.toLocal8Bit().data(),str_ResData.m_para.toLocal8Bit().data()\
                        ,count,dateTime_str.toLocal8Bit().data());
                err= query.exec(sSql);

                memset(sSql,0,2048);
            }
            dateList.clear();
        }
        viewSql(dat);
    } else {
        str_ResData.m_item=dat.at(0);
        str_ResData.m_para=dat.at(1);
        str_ResData.m_result=dat.at(2);
        str_ResData.m_judge=dat.at(3);
        dateList.append(str_ResData);
    }
}

QString SqlClient::filePath()
{
#ifdef __arm__
    QProcess *cmd = new QProcess(this);
    cmd->start("df -h");
    cmd->waitForFinished();
    QStringList s = QString(cmd->readAll()).split("\n");
    for (int i=0; i < s.size(); i++) {
        if (s.at(i).contains("/mnt/sdcard")) {
            return "/mnt/sdcard/aip.db";
        }
    }
#endif
    return "./nandflash/aip.db";
}
/*********************************END OF FILE**********************************/


