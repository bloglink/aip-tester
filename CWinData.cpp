/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2017.01.05
 * brief:       数据管理界面
 * date:        2017.01.06
 * brief:       QSqlTableModel有256条限制,直接使用QSqlQuery,QSqlTableModel只作显示
*******************************************************************************/
#include "CWinData.h"
#include "ui_CWinData.h"
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.05
 * brief:       构造
******************************************************************************/
CWinData::CWinData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CWinData)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    SqlInit();
    DatInit();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.05
 * brief:       析构
******************************************************************************/
CWinData::~CWinData()
{
    delete ui;
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.05
 * brief:       界面初始化
 * date:        2017.01.06
 * brief:       加入版本控制,最大ID初始化
******************************************************************************/
void CWinData::WinInit()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabTestData->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabDetail->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabDetail->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabTestData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabDetail->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabDetail->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.05
 * brief:       按钮初始化
******************************************************************************/
void CWinData::BtnInit()
{
    QButtonGroup *BtnGroup = new QButtonGroup;
    BtnGroup->addButton(ui->BtnExit,Qt::Key_0);
    BtnGroup->addButton(ui->BtnShow,Qt::Key_1);
    BtnGroup->addButton(ui->BtnCheck,Qt::Key_2);
    BtnGroup->addButton(ui->BtnClear,Qt::Key_3);
    connect(BtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.05
 * brief:       按钮功能
******************************************************************************/
void CWinData::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        emit TransformCmd(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    case Qt::Key_1:
        if (ui->WidgetShow->currentIndex() == 0)
            ui->WidgetShow->setCurrentIndex(1);
        else
            ui->WidgetShow->setCurrentIndex(0);
        break;
    case Qt::Key_2:
        DatInit();
        break;
    case Qt::Key_3:
        DeleteAll();
        break;
    default:
        break;
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       数据库初始化
******************************************************************************/
void CWinData::SqlInit()
{
    QFile file("./data/aip.db");
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    }
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./data/aip.db");
    db.open();

    QSqlQuery query(db);
    query.prepare("select count(*) from sqlite_master where type='table' and name='TestData'");
    query.exec();
    if(query.next() && query.value(0).toInt() == 0)
    {
        query.prepare("create table TestData (id int primary key,item text,para text,result text,judge text,b1 text,b2 text,b3 text)");
        query.exec();
    }
    query.exec("select max(id) from TestData");
    while(query.next())
        MaxId = query.value(0).toInt();
    model = new QSqlTableModel(this);
    model->setTable("TestData");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->TabTestData->setModel(model);

    global = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    //可使用的测试项目
    QStringList temp = (global->value("ItemEnable","0 1 2 3 4 6").toString()).split(" ");
    int row = temp.size();
    if (temp.contains("2")) {
        row++;
    }
    ui->TabDetail->setRowCount(row);
    int t = 0;
    if (temp.contains("0")) {
        for (int i=0; i<5; i++) {
            Amount.append(new QTableWidgetItem);
            Amount.at(i)->setTextAlignment(Qt::AlignCenter);
            ui->TabDetail->setItem(t,i,Amount.at(i));
            Amount.at(i)->setText("0");

        }
        Amount.at(0)->setText("总数");
        t++;
    }
    if (temp.contains("1")) {
        for (int i=0; i<5; i++) {
            Dcr.append(new QTableWidgetItem);
            Dcr.at(i)->setTextAlignment(Qt::AlignCenter);
            ui->TabDetail->setItem(t,i,Dcr.at(i));
            Dcr.at(i)->setText("0");
        }
        Dcr.at(0)->setText("电阻");
        t++;
    }
    if (temp.contains("2")) {
        for (int i=0; i<5; i++) {
            Mag.append(new QTableWidgetItem);
            Mag.at(i)->setTextAlignment(Qt::AlignCenter);
            ui->TabDetail->setItem(t,i,Mag.at(i));
            Mag.at(i)->setText("0");
        }
        Mag.at(0)->setText("反嵌");
        t++;
        for (int i=0; i<5; i++) {
            Dir.append(new QTableWidgetItem);
            Dir.at(i)->setTextAlignment(Qt::AlignCenter);
            ui->TabDetail->setItem(t,i,Dir.at(i));
            Dir.at(i)->setText("0");
        }
        Dir.at(0)->setText("磁旋");
        t++;
    }
    if (temp.contains("3")) {
        for (int i=0; i<5; i++) {
            Inr.append(new QTableWidgetItem);
            Inr.at(i)->setTextAlignment(Qt::AlignCenter);
            ui->TabDetail->setItem(t,i,Inr.at(i));
            Inr.at(i)->setText("0");
        }
        Inr.at(0)->setText("绝缘");
        t++;
    }
    if (temp.contains("4")) {
        for (int i=0; i<5; i++) {
            Acw.append(new QTableWidgetItem);
            Acw.at(i)->setTextAlignment(Qt::AlignCenter);
            ui->TabDetail->setItem(t,i,Acw.at(i));
            Acw.at(i)->setText("0");
        }
        Acw.at(0)->setText("交耐");
        t++;
    }
    if (temp.contains("6")) {
        for (int i=0; i<5; i++) {
            Imp.append(new QTableWidgetItem);
            Imp.at(i)->setTextAlignment(Qt::AlignCenter);
            ui->TabDetail->setItem(t,i,Imp.at(i));
            Imp.at(i)->setText("0");
        }
        Imp.at(0)->setText("匝间");
        t++;
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.05
 * brief:       数据初始化
 * date:        2017.01.06
 * brief:       增加详细表格初始化
******************************************************************************/
void CWinData::DatInit()
{
    ui->DateEdit2->setDate(QDate::currentDate());
    QSqlQuery query(db);
    if (ui->EditType->text().isEmpty()) {
        query.prepare("select para,judge from TestData where item >= :date1 and item <= :date2");
        query.bindValue(":date1",ui->DateEdit1->date());
        query.bindValue(":date2",ui->DateEdit2->date());
    }
    query.exec();
    int a1 = 0;
    int a2 = 0;
    int dcr1 = 0;
    int dcr2 = 0;
    int mag1 = 0;
    int mag2 = 0;
    int dir1 = 0;
    int dir2 = 0;
    int inr1 = 0;
    int inr2 = 0;
    int acw1 = 0;
    int acw2 = 0;
    int imp1 = 0;
    int imp2 = 0;
    while(query.next()) {
        a1++;
        if (query.value(0).toString().contains("DCR"))
            dcr1++;
        if (query.value(0).toString().contains("MAG"))
            mag1++;
        if (query.value(0).toString().contains("DIR"))
            dir1++;
        if (query.value(0).toString().contains("INR"))
            inr1++;
        if (query.value(0).toString().contains("ACW"))
            acw1++;
        if (query.value(0).toString().contains("IMP"))
            imp1++;

        if (!query.value(1).toString().contains("OK"))
            a2++;
        if (query.value(1).toString().contains("DCR"))
            dcr2++;
        if (query.value(1).toString().contains("MAG"))
            mag2++;
        if (query.value(1).toString().contains("DIR"))
            dir2++;
        if (query.value(1).toString().contains("INR"))
            inr2++;
        if (query.value(1).toString().contains("ACW"))
            acw2++;
        if (query.value(1).toString().contains("IMP"))
            imp2++;
    }
    Amount.at(1)->setText(QString::number(a1));
    Amount.at(2)->setText(QString::number(a1-a2));
    Amount.at(3)->setText(QString::number(a2));
    if (a1 == 0)
        Amount.at(4)->setText(QString::number(0));
    else
        Amount.at(4)->setText(QString::number(a2*100/a1));
    Dcr.at(1)->setText(QString::number(dcr1));
    Dcr.at(2)->setText(QString::number(dcr1-dcr2));
    Dcr.at(3)->setText(QString::number(dcr2));
    if (dcr1 == 0)
        Dcr.at(4)->setText(QString::number(0));
    else
        Dcr.at(4)->setText(QString::number(dcr2*100/dcr1));
    Mag.at(1)->setText(QString::number(mag1));
    Mag.at(2)->setText(QString::number(mag1-mag2));
    Mag.at(3)->setText(QString::number(mag2));
    if (mag1 == 0)
        Mag.at(4)->setText(QString::number(0));
    else
        Mag.at(4)->setText(QString::number(mag2*100/mag1));
    Dir.at(1)->setText(QString::number(dir1));
    Dir.at(2)->setText(QString::number(dir1-dir2));
    Dir.at(3)->setText(QString::number(dir2));
    if (dir1 == 0)
        Dir.at(4)->setText(QString::number(0));
    else
        Dir.at(4)->setText(QString::number(dir2*100/dir1));
    Inr.at(1)->setText(QString::number(inr1));
    Inr.at(2)->setText(QString::number(inr1-inr2));
    Inr.at(3)->setText(QString::number(inr2));
    if (inr1 == 0)
        Inr.at(4)->setText(QString::number(0));
    else
        Inr.at(4)->setText(QString::number(inr2*100/inr1));
    Acw.at(1)->setText(QString::number(acw1));
    Acw.at(2)->setText(QString::number(acw1-acw2));
    Acw.at(3)->setText(QString::number(acw2));
    if (acw1 == 0)
        Acw.at(4)->setText(QString::number(0));
    else
        Acw.at(4)->setText(QString::number(acw2*100/acw1));
    Imp.at(1)->setText(QString::number(imp1));
    Imp.at(2)->setText(QString::number(imp1-imp2));
    Imp.at(3)->setText(QString::number(imp2));
    if (imp1 == 0)
        Imp.at(4)->setText(QString::number(0));
    else
        Imp.at(4)->setText(QString::number(imp2*100/imp1));

    ui->BarAmount->setValue(100-Amount.at(4)->text().toInt());
    ui->BarDcr->setValue(100-Dcr.at(4)->text().toInt());
    ui->BarMag->setValue(100-Mag.at(4)->text().toInt());
    ui->BarDir->setValue(100-Dir.at(4)->text().toInt());
    ui->BarInr->setValue(100-Inr.at(4)->text().toInt());
    ui->BarAcw->setValue(100-Acw.at(4)->text().toInt());
    ui->BarImp->setValue(100-Imp.at(4)->text().toInt());

    model->select();
    while(model->canFetchMore())
    {
        model->fetchMore();
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.05
 * brief:       插入总数据
 * date:        2017.01.06
 * brief:       QSqlTableModel有256条限制，直接使用QSqlQuery
******************************************************************************/
void CWinData::InsertItem(QString para, QString r, QString j)
{
    QSqlQuery query(db);
    MaxId++;
    query.prepare("insert into TestData (id,item,para,result,judge)""values(:t0,:t1,:t2,:t3,:t4)");
    query.bindValue(":t0",MaxId);
    query.bindValue(":t1",QDate::currentDate());
    query.bindValue(":t2",para);
    query.bindValue(":t3",r);
    query.bindValue(":t4",j);
    query.exec();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.05
 * brief:       插入详细数据
 * date:        2017.01.06
 * brief:       QSqlTableModel有256条限制，直接使用QSqlQuery
******************************************************************************/
void CWinData::InsertRow(QString item,QString para,QString r,QString j)
{
    QSqlQuery query(db);
    MaxId++;
    query.prepare("insert into TestData(id,item,para,result,judge)""values(:t0,:t1,:t2,:t3,:t4)");
    query.bindValue(":t0",MaxId);
    query.bindValue(":t1",item);
    query.bindValue(":t2",para);
    query.bindValue(":t3",r);
    query.bindValue(":t4",j);
    query.exec();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       开始事务操作
******************************************************************************/
void CWinData::SubmitStart()
{
    db.transaction();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       提交
******************************************************************************/
void CWinData::SubmitAll()
{
    db.commit();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.05
 * brief:       清空表
******************************************************************************/
void CWinData::DeleteAll()
{
    int nRowCnt = model->rowCount();
    while (nRowCnt > 0)
    {
        nRowCnt--;
        model->removeRow(nRowCnt);
    }
    model->database().transaction();
    model->submitAll();
    model->database().commit();
    DatInit();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.05
 * brief:       显示
******************************************************************************/
void CWinData::showEvent(QShowEvent *)
{
//    DatInit();
}
