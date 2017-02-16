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
    //    DatInit();
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
    if(query.next() && query.value(0).toInt() == 0) {
        query.prepare("create table TestData (id int primary key,item text,para text,result text,judge text,b1 text,b2 text,b3 text)");
        query.exec();
    }
    model = new QSqlTableModel(this);
    model->setTable("TestData");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->TabTestData->setModel(model);
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
    model->select();
    while(model->canFetchMore()) {
        model->fetchMore();
    }
    ui->TabDetail->setRowCount(0);
    ItemInit("总数");
    ItemInit("反嵌");
    ItemInit("磁旋");
    ItemInit("绝缘");
    ItemInit("交耐");
    ItemInit("匝间");
    ItemInit("电感");
}

void CWinData::ItemInit(QString item)
{

    QSqlQuery query(db);
    if (ui->EditType->text().isEmpty()) {
        query.prepare("select judge from TestData where para>=:date1 and para<=:date2 and item=:item");
        query.bindValue(":date1",ui->DateEdit1->date());
        query.bindValue(":date2",ui->DateEdit2->date());
        query.bindValue(":item",item);
    } else {
        query.prepare("select judge from TestData where para>=:date1 and para<=:date2 and item=:item and result=:type");
        query.bindValue(":date1",ui->DateEdit1->date());
        query.bindValue(":date2",ui->DateEdit2->date());
        query.bindValue(":item",item);
        query.bindValue(":type",ui->EditType->text());
    }
    query.exec();
    int t1 = 0;
    int t2 = 0;
    while(query.next()) {
        if (query.value(0).toString() == "OK")
            t1++;
        if (query.value(0).toString() == "NG")
            t2++;
    }
    if (t1==0 && t2==0)
        return;
    int row = ui->TabDetail->rowCount();
    ui->TabDetail->setRowCount(row+1);
    for (int i=0; i<5; i++) {
        ui->TabDetail->setItem(row,i,new QTableWidgetItem);
        ui->TabDetail->item(row,i)->setTextAlignment(Qt::AlignCenter);
    }
    ui->TabDetail->item(row,0)->setText(item);
    ui->TabDetail->item(row,1)->setText(QString::number(t1+t2));
    ui->TabDetail->item(row,2)->setText(QString::number(t1));
    ui->TabDetail->item(row,3)->setText(QString::number(t2));
    ui->TabDetail->item(row,4)->setText(QString::number(t2*100/(t1+t2)));
    ui->TabDetail->item(row,4)->setText(QString("%1%").arg(t2*100/(t1+t2)));
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
    DatInit();
}
