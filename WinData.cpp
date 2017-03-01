/**
  ******************************************************************************
  * @file    WinData.cpp
  * @author  link
  * @version 2.0.3.0
  * @date    2017-02-28
  * @brief   Show sql data
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "WinData.h"
#include "ui_WinData.h"
/**
  * @brief  Initializes
  * @param  parent:parent widget
  * @retval None
  */
WinData::WinData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinData)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    SqlInit();
}
/**
  * @brief  Destruct the window
  * @param  None
  * @retval None
  */
WinData::~WinData()
{
    delete ui;
}
/**
  * @brief  Initializes the window
  * @param  None
  * @retval None
  */
void WinData::WinInit()
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
/**
  * @brief  Initializes the buttons
  * @param  None
  * @retval None
  */
void WinData::BtnInit()
{
    QButtonGroup *BtnGroup = new QButtonGroup;
    BtnGroup->addButton(ui->BtnExit,Qt::Key_0);
    BtnGroup->addButton(ui->BtnShow,Qt::Key_1);
    BtnGroup->addButton(ui->BtnCheck,Qt::Key_2);
    BtnGroup->addButton(ui->BtnClear,Qt::Key_3);
    connect(BtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/**
  * @brief  Button functions
  * @param  id:button id
  * @retval None
  */
void WinData::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        emit SendMessage(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    case Qt::Key_1:
        if (ui->WidgetShow->currentIndex() == 0)
            ui->WidgetShow->setCurrentIndex(1);
        else
            ui->WidgetShow->setCurrentIndex(0);
        break;
    case Qt::Key_2:
        SqlRead();
        break;
    case Qt::Key_3:
        SqlClear();
        break;
    default:
        break;
    }
}
/**
  * @brief  Initializes sql data
  * @param  None
  * @retval None
  */
void WinData::SqlInit()
{
    qDebug()<<QTime::currentTime().toString()<<"连接数据库";

    QFile file(SQL_PATH);
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    }
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SQL_PATH);
    if (!db.open()) {
        qDebug()<<QTime::currentTime().toString()<<"连接数据库Error";
        return;
    }

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

    qDebug()<<QTime::currentTime().toString()<<"连接数据库OK";
}
/**
  * @brief  Initializes the data
  * @param  None
  * @retval None
  */
void WinData::SqlRead()
{
    qDebug()<<QTime::currentTime().toString()<<"读取数据库";

    ui->DateEdit2->setDate(QDate::currentDate());
    model->select();
    while(model->canFetchMore()) {
        model->fetchMore();
    }
    ui->TabDetail->setRowCount(0);
    SqlQuery("总数");
    SqlQuery("电阻");
    SqlQuery("反嵌");
    SqlQuery("磁旋");
    SqlQuery("绝缘");
    SqlQuery("交耐");
    SqlQuery("匝间");
    SqlQuery("电感");
    QStringList n;
    for (int i=0; i<ui->TabDetail->rowCount(); i++)
        n.append(ui->TabDetail->item(i,0)->text());
    QStringList v;
    for (int i=0; i<ui->TabDetail->rowCount(); i++)
        v.append(ui->TabDetail->item(i,4)->text());
    ui->TextHistogram->setNames(n);
    ui->TextHistogram->setValues(v);
    ui->TextHistogram->update();

    qDebug()<<QTime::currentTime().toString()<<"读取数据库OK";
}
/**
  * @brief  Initializes the item data
  * @param  item:query the item from sql
  * @retval None
  */
void WinData::SqlQuery(QString item)
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
    ui->TabDetail->item(row,4)->setText(QString("%1%").arg(t1*100/(t1+t2)));
}
/**
  * @brief  Clear the sql
  * @param  None
  * @retval None
  */
void WinData::SqlClear()
{
    QSqlQuery query(db);
    query.prepare("delete from TestData");
    query.exec();
    SqlRead();
}
/**
  * @brief  Initializes data when show
  * @param  None
  * @retval None
  */
void WinData::showEvent(QShowEvent *)
{
    SqlRead();
}
/*********************************END OF FILE**********************************/