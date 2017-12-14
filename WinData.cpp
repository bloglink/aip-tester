/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170424
 * author:      zhaonanlin
 * brief:       数据管理模块
*******************************************************************************/
#include "WinData.h"
#include "ui_WinData.h"

WinData::WinData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinData)
{
    ui->setupUi(this);
    initUI();
    InitButtons();
    InitSqlite();
}

WinData::~WinData()
{
    delete ui;
}

void WinData::initUI()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->TabTestData->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabDetail->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabDetail->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabTestData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabDetail->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabDetail->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
}

void WinData::initDB()
{
    QString fileName = "./nandflash/aip.db";
#ifdef __arm__
    QProcess *cmd = new QProcess(this);
    cmd->start("df -h");
    cmd->waitForFinished();
    QStringList s = QString(cmd->readAll()).split("\n");
    for (int i=0; i < s.size(); i++) {
        if (s.at(i).contains("/mnt/sdcard")) {
            fileName = "/mnt/sdcard/aip.db";
            break;
        }
    }
#endif
    QFile file(fileName);
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    }
    db = QSqlDatabase::addDatabase("QSQLITE", "WinData");
    db.setDatabaseName(fileName);
    if (!db.open()) {
        SendWarnning(tr("数据库打开失败"));
        return;
    }
}

void WinData::initData()
{
    items << tr("总数")
          << tr("电阻") << tr("反嵌") << tr("磁旋") << tr("绝缘")
          << tr("交耐") << tr("直耐") << tr("匝间") << tr("电感")
          << tr("功率") << tr("转向") << tr("低启") << tr("堵转");

    QSqlQuery query(db);

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
        SendWarnning(tr("数据表创建失败"));
        db.close();
        return;
    }
    err= query.exec("select *from aip_tail where id = (select max(id) from aip_tail);");
    if(!err)   {
        SendWarnning(tr("数据表获取失败"));
        db.close();
        return;
    }
    QString t1;
    while(query.next())
        t1= query.value(5).toString();
    if (t1 != QDateTime::currentDateTime().toString("yyyyMMdd")) {
        typeSql("aip9918");
    }
}

void WinData::initSql(QString sql)
{
    QString cmd = "insert into aip_tail(";
    cmd += "id,pro_name,result_num,result_ok,result_ng,result_time,result_type,result_percen)";
    cmd += "pro_name varchar(10),";
    cmd += "result_num integer,";
    cmd += "result_ok integer,";
    cmd += "result_ng integer,";
    cmd += "result_time varchar(10),";
    cmd += "result_type varchar(10),";
    cmd += "result_percen varchar(10))";
    qDebug() << sql;
}

void WinData::typeSql(QString qs)
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

void WinData::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("数据异常:\n%1").arg(s));
    emit SendVariant(hash);
}

void WinData::InitButtons()
{
    QButtonGroup *BtnGroup = new QButtonGroup;
    BtnGroup->addButton(ui->BtnExit, Qt::Key_0);
    BtnGroup->addButton(ui->BtnShow, Qt::Key_1);
    BtnGroup->addButton(ui->BtnCheck, Qt::Key_2);
    BtnGroup->addButton(ui->BtnClear, Qt::Key_3);
    connect(BtnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void WinData::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        GoToWindow(NULL);
        break;
    case Qt::Key_1:
        ExportToUsb();
        break;
    case Qt::Key_2:
        ReadSqliteAll();
        break;
    case Qt::Key_3:
        ClearSqlite();
        break;
    default:
        break;
    }
}

void WinData::InitSqlite()
{
    QString f = filePath();
    QFile file(f);
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    }
    db = QSqlDatabase::addDatabase("QSQLITE", "WinData");
    db.setDatabaseName(f);
    if (!db.open()) {
        qDebug() << QTime::currentTime().toString() << "SQL Error";
        return;
    }

    QSqlQuery query(db);

    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyyMMdd"); //设置显示格式

    QString cmd = "create table if not exists aip_pool(";
    cmd += "id integer primary key,";
    cmd += "filedata varchar(10),";
    cmd += "filetime varchar(10))";

    bool err= query.exec(cmd);
    if(!err) {
        query.clear();
        db.close();
        return;
    }
    char sSql[2048];
    memset(sSql,0,2048);
    QString sq;
    sprintf(sSql,"select *from aip_pool where filetime='%s'",str.toLocal8Bit().data());
    err =query.exec(sSql);
    while(query.next())  {
        sq = query.value(2).toString();
    }
    qDebug()<<sq;
    qDebug()<<str;
    if(strcmp(sq.toLocal8Bit().data(),str.toLocal8Bit().data())!=0) {
        memset(sSql,0,2048);
        QString b1="aip_"+str;
        sprintf(sSql,"insert into  aip_pool(id,filedata,filetime)values(null,'%s','%s')"
                ,b1.toLocal8Bit().data(),str.toLocal8Bit().data());
        bool   err=query.exec(sSql);
        memset(sSql,0,2048);
        //        sprintf(sSql,0,2048);
        sprintf(sSql,"create table if not exists aip_%s(id integer primary key,item varchar(20),para varchar(20),result varchar(20),judge varchar(20)\
                ,b1 varchar(20),b2 integer,b3 varchar(20));",str.toLocal8Bit().data());
        err=query.exec(sSql);
    }
}

void WinData::ReadSqliteAll()
{
    ui->TabDetail->setRowCount(0);
    ReadView(tr("总数"));
    ReadView(tr("电阻"));
    ReadView(tr("反嵌"));
    ReadView(tr("磁旋"));
    ReadView(tr("绝缘"));
    ReadView(tr("交耐"));
    ReadView(tr("匝间"));
    ReadView(tr("电感"));
    ReadView(tr("功率"));
    ReadView(tr("PG"));
    ReadView(tr("低启"));
    ReadView(tr("堵转"));
    QStringList n;
    for (int i=0; i < ui->TabDetail->rowCount(); i++)
        n.append(ui->TabDetail->item(i, 0)->text());
    QStringList v;
    for (int i=0; i < ui->TabDetail->rowCount(); i++)
        v.append(ui->TabDetail->item(i, 4)->text());
    ui->TextHistogram->setNames(n);
    ui->TextHistogram->setValues(v);
    ui->TextHistogram->update();

    qDebug() << QTime::currentTime().toString() << "WinData read OK";
}

void WinData::ReadView(QString item)
{
    QSqlQuery query(db);
    int mm=0, j=0,z=0,n=0;

    QString wee=ui->EditType->text();
    qDebug()<<wee;
    if (ui->EditType->text().isEmpty())
    {
        query.prepare("select result_num from aip_tail where result_time>=:d1 and result_time<=:d2 and pro_name=:item");
        query.bindValue(":d1", ui->DateEdit1->date().toString("yyyyMMdd"));
        query.bindValue(":d2", ui->DateEdit2->date().toString("yyyyMMdd"));
        query.bindValue(":item", item);
        query.exec();
        while (query.next())
        {
            mm=query.value(0).toInt();    // 总数
            z=mm+z;
        }
        if (mm == 0 && z==0)
        {
            return;
        }
        mm=0;
        query.prepare("select result_ok from aip_tail where result_time>=:d1 and result_time<=:d2 and pro_name=:item");
        query.bindValue(":d1", ui->DateEdit1->date().toString("yyyyMMdd"));
        query.bindValue(":d2", ui->DateEdit2->date().toString("yyyyMMdd"));
        query.bindValue(":item", item);
        query.exec();
        while (query.next())
        {
            mm=query.value(0).toInt();    // OK
            j=mm+j;
        }
        mm=0;
        query.prepare("select result_ng from aip_tail where result_time>=:d1 and result_time<=:d2 and pro_name=:item");
        query.bindValue(":d1", ui->DateEdit1->date().toString("yyyyMMdd"));
        query.bindValue(":d2", ui->DateEdit2->date().toString("yyyyMMdd"));
        query.bindValue(":item", item);
        query.exec();
        while (query.next())
        {
            mm=query.value(0).toInt();    // NG
            n=mm+n;
        }
    } else {
        // 总数
        query.prepare("select result_num from aip_tail where result_time>=:d1 and result_time<=:d2 and result_type=:type and pro_name=:item");
        query.bindValue(":d1", ui->DateEdit1->date().toString("yyyyMMdd"));
        query.bindValue(":d2", ui->DateEdit2->date().toString("yyyyMMdd"));
        query.bindValue(":item", item);
        query.bindValue(":type",ui->EditType->text());
        query.exec();
        while (query.next())
        {
            mm=query.value(0).toInt();    // 总数
            z=mm+z;
        }
        if (mm == 0 && z==0)
        {
            return;
        }
        mm=0;
        query.prepare("select result_ok from aip_tail where result_time>=:d1 and result_time<=:d2 and pro_name=:item and result_type=:type");
        query.bindValue(":d1", ui->DateEdit1->date().toString("yyyyMMdd"));
        query.bindValue(":d2", ui->DateEdit2->date().toString("yyyyMMdd"));
        query.bindValue(":item", item);
        query.bindValue(":type", ui->EditType->text());
        query.exec();
        while (query.next())
        {
            mm=query.value(0).toInt();    // OK
            j=mm+j;
        }
        mm=0;
        query.prepare("select result_ng from aip_tail where result_time>=:d1 and result_time<=:d2 and pro_name=:item and result_type=:type");
        query.bindValue(":d1", ui->DateEdit1->date().toString("yyyyMMdd"));
        query.bindValue(":d2", ui->DateEdit2->date().toString("yyyyMMdd"));
        query.bindValue(":item", item);
        query.bindValue(":type", ui->EditType->text());
        query.exec();
        while (query.next())
        {
            mm=query.value(0).toInt();    // NG
            n=mm+n;
        }
    }


    int row = ui->TabDetail->rowCount();
    ui->TabDetail->setRowCount(row+1);
    for (int i=0; i < 5; i++) {
        ui->TabDetail->setItem(row, i, new QTableWidgetItem);
        ui->TabDetail->item(row, i)->setTextAlignment(Qt::AlignCenter);
    }
    ui->TabDetail->item(row, 0)->setText(item);   //
    ui->TabDetail->item(row, 1)->setText(QString::number(z));
    ui->TabDetail->item(row, 2)->setText(QString::number(j));
    ui->TabDetail->item(row, 3)->setText(QString::number(n));
    ui->TabDetail->item(row, 4)->setText(QString::number(n*100/z));
    ui->TabDetail->item(row, 4)->setText(QString("%1%").arg(j*100/z));
}

void WinData::ClearSqlite()
{
    QSqlQuery query(db);
    query.prepare("delete from TestData");
    query.exec();
    ReadSqliteAll();
}

QList<QString> WinData::Ex_head(int b,QString qs)
{
    QSqlQuery query(db);
    m_head.clear();
    m_vhead.clear();
    m_vhead.append(tr("电阻1"));
    m_vhead.append(tr("电阻2"));
    m_vhead.append(tr("电阻3"));
    m_vhead.append(tr("电阻4"));
    m_vhead.append(tr("电阻5"));
    m_vhead.append(tr("电阻6"));
    m_vhead.append(tr("电阻7"));
    m_vhead.append(tr("电阻8"));
    m_vhead.append(tr("反嵌1"));
    m_vhead.append(tr("反嵌2"));
    m_vhead.append(tr("反嵌3"));
    m_vhead.append(tr("反嵌4"));
    m_vhead.append(tr("反嵌5"));
    m_vhead.append(tr("反嵌6"));
    m_vhead.append(tr("反嵌7"));
    m_vhead.append(tr("反嵌8"));
    m_vhead.append(tr("磁旋"));
    m_vhead.append(tr("绝缘1"));
    m_vhead.append(tr("绝缘2"));
    m_vhead.append(tr("绝缘3"));
    m_vhead.append(tr("绝缘4"));
    m_vhead.append(tr("交耐1"));
    m_vhead.append(tr("交耐2"));
    m_vhead.append(tr("交耐3"));
    m_vhead.append(tr("交耐4"));
    m_vhead.append(tr("功率1"));
    m_vhead.append(tr("功率2"));
    m_vhead.append(tr("功率3"));
    m_vhead.append(tr("功率4"));
    m_vhead.append(tr("功率5"));
    m_vhead.append(tr("功率6"));
    m_vhead.append(tr("堵转"));
    m_vhead.append(tr("低启"));
    m_vhead.append(tr("匝间1"));
    m_vhead.append(tr("匝间2"));
    m_vhead.append(tr("匝间3"));
    m_vhead.append(tr("匝间4"));
    m_vhead.append(tr("匝间5"));
    m_vhead.append(tr("匝间6"));
    m_vhead.append(tr("匝间7"));
    m_vhead.append(tr("匝间8"));
    m_vhead.append(tr("PG1"));
    m_vhead.append(tr("PG2"));
    m_vhead.append(tr("PG3"));
    m_vhead.append(tr("电感1"));
    m_vhead.append(tr("电感2"));
    m_vhead.append(tr("电感3"));
    m_vhead.append(tr("电感4"));
    m_vhead.append(tr("电感5"));
    m_vhead.append(tr("电感6"));
    m_vhead.append(tr("电感7"));
    m_vhead.append(tr("电感8"));
    char sSql[2048];

    for(int i=0;i<m_vhead.size();i++)
    {
        memset(sSql,0,2048);
        sprintf(sSql,"select *from aip_%d where item='%s' and b1='%s'",b,m_vhead.at(i).toLocal8Bit().data(),qs.toLocal8Bit().data());
        bool   err=query.exec(sSql);
        qDebug()<<sSql;
        qDebug()<<err;
        query.next();
        QString total;
        total.clear();
        total =query.value(1).toString();
        if(!total.isEmpty())
        {
            qDebug()<<total;
            m_head.append(total);
        }
    }
    return m_head;
}


bool WinData::filecsv(int istime,QString qs,QString filepath)
{
    exportdata.clear();
    m_chead.clear();
    mm_xcode.clear();
    mm_head.clear();

    QSqlQuery query(db);
    char sSql[2048];
    QString tyname=qs;
    sleep(1);
    QFile file(QString("%1/%2_%3.csv").arg(filepath).arg(QDateTime::currentDateTime().toString("yyyy-MM-ddhhmmss")).arg(tyname));
    //QFile file(QString("%1/%2_%3.csv").arg(filepath).arg(istime).arg(tyname));
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this,  "",   "创建失败",  QMessageBox::Ok);
        return false;
    }

    /***********************文件标题栏**********************************/
    mm_head= Ex_head(istime,tyname);
    QString qs4="时间";
    QString qs5="类型";
    QString qs6 ="判定";
    QString qs7="工位";
    QString qs8="操作者";

    file.write(ToGbk(qs4));
    file.write(",");
    file.write(ToGbk(qs5));
    file.write(",");
    file.write(ToGbk(qs6));
    file.write(",");
    file.write(ToGbk(qs7));
    file.write(",");
    file.write(ToGbk(qs8));
    file.write(",");

    for(int z=0;z<mm_head.size();z++)
    {
        QString qs1="测试参数";
        QString qs2="测试结果";
        QString qs3="测试判定";
        qDebug()<<m_head.at(z);
        file.write(ToGbk(m_head.at(z)+qs1));
        file.write(",");
        if(mm_head.at(z).startsWith("匝间"))
        {
            for(int o=0;o<4;o++)
            {
                file.write(ToGbk(m_head.at(z)+qs2));
                file.write(",");
            }
        }
        else if(mm_head.at(z).startsWith("功率"))
        {
            for(int o=0;o<4;o++)
            {
                file.write(ToGbk(m_head.at(z)+qs2));
                file.write(",");
            }
        }
        else
        {
            file.write(ToGbk(m_head.at(z)+qs2));
            file.write(",");
        }
        file.write(ToGbk(m_head.at(z)+qs3));
        file.write(",");
    }
    file.write("\n");
    /********************************************************************/

    memset(sSql,0,2048);
    sprintf(sSql,"select *from aip_%d where b1='%s'",istime,tyname.toLocal8Bit().data());
    query.exec(sSql);
    while(query.next())
    {
        str_ResData.wm_item = query.value(1).toString();
        str_ResData.wm_para= query.value(2).toString();
        str_ResData.wm_result=query.value(3).toString();
        str_ResData.wm_judge=query.value(4).toString();
        //      str_ResData.wm_date=query.value(7).toString();
        exportdata.append(str_ResData);

        //        qDebug()<< str_ResData.wm_item;
        //        qDebug()<< str_ResData.wm_para;
        //        qDebug()<<str_ResData.wm_result;
        //        qDebug()<<str_ResData.wm_judge;

        if( query.value(1).toString()=="总数")
        {
            wm_ResData resdata;
            wm_ResData revdata;
            wm_ResData data;
            for(int u=0;u<mm_head.size();u++)
            {
                mm_xcode.insert(u,revdata);
            }
            file.write(ToGbk(query.value(7).toString()));
            file.write(",");
            file.write(ToGbk(query.value(2).toString()));
            file.write(",");
            file.write(ToGbk(query.value(3).toString()));
            file.write(",");

            int isize=exportdata.size();
            resdata= exportdata.at(isize-2);
            file.write(ToGbk(resdata.wm_para));
            file.write(",");
            file.write(ToGbk(resdata.wm_result));
            file.write(",");
            //           file.write(ToGbk(resdata.wm_judge));
            //           file.write(",");


            for(int z=0;z<mm_head.size();z++)
            {
                for(int m=0;m<exportdata.size()-2;m++)
                {
                    resdata=exportdata.at(m);
                    qDebug()<<resdata.wm_item;
                    if(mm_head.at(z)==resdata.wm_item)
                    {
                        qDebug()<<"@@@@@@@@@@@@@@@@@@";
                        qDebug()<<mm_head.at(z);
                        qDebug()<<resdata.wm_item;
                        qDebug()<<resdata.wm_para;
                        qDebug()<<z;   //3
                        qDebug()<<"@@@@@@@@@@@@@@@@@@";

                        revdata=resdata;
                        qDebug()<<revdata.wm_item;
                        mm_xcode[z]=revdata;
                        break;
                    }
                    else
                    {
                        data.wm_item=mm_head.at(z);
                        qDebug()<<"###########################";
                        qDebug()<< revdata.wm_item;
                        qDebug()<<z;
                        qDebug()<<  mm_head.at(z);
                        qDebug()<<"###########################";
                        mm_xcode[z]=data;
                    }
                }
            }

            /**********************写文件内容*****************************/
            wm_ResData  writeRES;

            for(int xx=0;xx<mm_xcode.size();xx++)
            {
                writeRES =  mm_xcode.at(xx);
                qDebug()<<writeRES.wm_item;
                qDebug()<<writeRES.wm_para;
                file.write(ToGbk(writeRES.wm_para.replace(","," ")));
                file.write(",");
                if(writeRES.wm_item.startsWith("匝间"))
                {
                    QStringList impList=writeRES.wm_result.split(",");
                    qDebug()<<impList.size();

                    for(int i =0;i<impList.size();i++)
                    {
                        file.write(ToGbk(impList.at(i)));
                        file.write(",");
                    }
                    if(  impList.size()<4)
                    {
                        for(int msize=0;msize<4-impList.size();msize++)
                        {
                            file.write(",");
                        }
                    }
                }
                else if(writeRES.wm_item.startsWith("功率"))
                {
                    QStringList impList=writeRES.wm_result.split(",");
                    qDebug()<<impList.size();

                    for(int i =0;i<impList.size();i++)
                    {
                        file.write(ToGbk(impList.at(i)));
                        file.write(",");
                    }
                    if(  impList.size()<4)
                    {
                        for(int msize=0;msize<4-impList.size();msize++)
                        {
                            file.write(",");
                        }
                    }
                }
                else
                {
                    file.write(ToGbk(writeRES.wm_result.replace(","," ")));
                    file.write(",");
                }
                file.write(ToGbk(writeRES.wm_judge));
                file.write(",");
            }
            file.write("\n");
            exportdata.clear();
            mm_xcode.clear();
        }

    }
    file.close();
    mm_head.clear();
    m_chead.clear();
    return true;
}
void WinData::ExportToUsb()
{
    QString path = ReadUsb();
    if (path.isEmpty()) {
        QMessageBox::warning(this, "", "请插入U盘", QMessageBox::Ok);
        return;
    }
    QSqlQuery query(db);
    bool b3=false;
    char sSql[2048];
    memset(sSql,0,2048);
    int  data1=ui->DateEdit1->date().toJulianDay();
    int  data2=ui->DateEdit2->date().toJulianDay();

    for(int idate=data1;idate<=data2;idate++) {
        int i = QDate::fromJulianDay(idate).toString("yyyyMMdd").toInt();
        if (ui->EditType->text().isEmpty()) {
            typeExlist.clear();
            sprintf(sSql,"select *from aip_tail where pro_name='%s' and  result_time='%d'","总数",i);
            query.exec(sSql);
            while(query.next()) {
                QString qsTypeNane =query.value(6).toString();
                typeExlist.append(qsTypeNane);
            }
            //根据当前类型 、时间 导出
            foreach(QString strtype, typeExlist) {    //遍历list链表，foreach按照标准for循环实现
                sprintf(sSql,"select *from aip_tail where pro_name='%s' and result_time=%d and result_type='%s'","总数",i,strtype.toLocal8Bit().data());
                query.exec(sSql);
                query.next();
                int resultnum=   query.value(2).toInt();
                if(resultnum==0)
                    continue;
                b3=  filecsv(i,strtype,path);
            }
        } else {
            //查找当前的测试类型总数，若为0，则不导出
            QString  tyname =ui->EditType->text();
            sprintf(sSql,"select *from aip_tail where pro_name='%s' and  result_time=%d and result_type='%s'","总数",i,tyname.toLocal8Bit().data());
            query.exec(sSql);
            query.next();
            int resultnum=   query.value(2).toInt();
            if(resultnum==0) {
                QMessageBox::warning(this,  "",  "所测型号-"+ tyname+",数据为空！",  QMessageBox::Ok);
                return;
            }
            b3= filecsv(i,tyname,path);
        }
    }
    if(b3)
        QMessageBox::information(this,  "",   "导出成功 ,请等待30S之后，拔出U盘",  QMessageBox::Ok);
}

QString WinData::ReadUsb()
{
    QDir *dir = new QDir;
    dir->setPath("/mnt/usb1");
    if (dir->count() > 2)
        return "/mnt/usb1";
    dir->setPath("/mnt/usb2");
    if (dir->count() > 2)
        return "/mnt/usb2";
    return NULL;
}

QByteArray WinData::ToGbk(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    return gbk->fromUnicode(inStr);
}

void WinData::GoToWindow(QString w)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "JumpWindow");
    hash.insert("TxMessage", w);
    emit SendVariant(hash);
}

QString WinData::filePath()
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

void WinData::showEvent(QShowEvent *e)
{
    ui->DateEdit1->setDate(QDate::currentDate());
    ui->DateEdit2->setDate(QDate::currentDate());
    ReadSqliteAll();
    e->accept();
}
/*********************************END OF FILE**********************************/
