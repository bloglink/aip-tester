/**
  ******************************************************************************
  * @file    WinType.cpp
  * @author  link
  * @version 2.0.2.0
  * @date    2017-02-28
  * @brief   Motor type settings
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "WinType.h"
#include "ui_WinType.h"
/**
  * @brief  Initializes
  * @param  *parent:parent widget
  * @retval None
  */
WinType::WinType(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinType)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    SetInit();
}
/**
  * @brief  Destruct the window
  * @param  None
  * @retval None
  */
WinType::~WinType()
{
    delete ui;
}
/**
  * @brief  Initializes the window
  * @param  None
  * @retval None
  */
void WinType::WinInit()
{
    ItemNames<<"空"<<"电阻"<<"反嵌"<<"绝缘"<<"交耐"<<"直耐"<<"匝间"<<"电感"<<"功率"<<"低启"<<"堵转"
            <<"转速"<<"泄漏"<<"PG"<<"EMF"<<"DCBL";
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabColor->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabColor->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabWire->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabWire->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabProj->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabProj->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabFile->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabTest->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabColor->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabColor->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabWire->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabWire->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabProj->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabProj->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabFile->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabTest->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif

    for (int i=0; i<ui->TabColor->rowCount(); i++) {
        for (int j=0; j<ui->TabColor->columnCount(); j++) {
            ui->TabColor->setItem(i,j,new QTableWidgetItem);
        }
    }
    ui->TabColor->item(0,0)->setBackgroundColor(QColor(255, 255, 255, 255));
    ui->TabColor->item(1,0)->setBackgroundColor(QColor(92, 217, 254, 255));
    ui->TabColor->item(2,0)->setBackgroundColor(QColor(128, 181, 154, 255));
    ui->TabColor->item(3,0)->setBackgroundColor(QColor(185, 0, 254, 255));
    ui->TabColor->item(4,0)->setBackgroundColor(QColor(228, 0, 254, 255));
    ui->TabColor->item(5,0)->setBackgroundColor(QColor(240, 78, 153, 255));
    ui->TabColor->item(6,0)->setBackgroundColor(QColor(254, 121, 115, 255));
    ui->TabColor->item(7,0)->setBackgroundColor(QColor(250, 204, 94, 255));
    ui->TabColor->item(8,0)->setBackgroundColor(QColor(244, 254, 124, 255));
    ui->TabColor->item(9,0)->setBackgroundColor(QColor(149, 237, 124, 255));

    ui->TabColor->item(0,1)->setBackgroundColor(QColor(167, 167, 167, 255));
    ui->TabColor->item(1,1)->setBackgroundColor(QColor(29, 203, 254, 255));
    ui->TabColor->item(2,1)->setBackgroundColor(QColor(87, 151, 154, 255));
    ui->TabColor->item(3,1)->setBackgroundColor(QColor(157, 0, 254, 255));
    ui->TabColor->item(4,1)->setBackgroundColor(QColor(208, 0, 254, 255));
    ui->TabColor->item(5,1)->setBackgroundColor(QColor(234, 0, 114, 255));
    ui->TabColor->item(6,1)->setBackgroundColor(QColor(254, 69, 52, 255));
    ui->TabColor->item(7,1)->setBackgroundColor(QColor(250, 174, 0, 255));
    ui->TabColor->item(8,1)->setBackgroundColor(QColor(242, 254, 55, 255));
    ui->TabColor->item(9,1)->setBackgroundColor(QColor(106, 231, 71, 255));

    ui->TabColor->item(0,2)->setBackgroundColor(QColor(120, 120, 120, 255));
    ui->TabColor->item(1,2)->setBackgroundColor(QColor(25, 172,223, 255));
    ui->TabColor->item(2,2)->setBackgroundColor(QColor(52, 100, 237, 255));
    ui->TabColor->item(3,2)->setBackgroundColor(QColor(117, 0, 215, 255));
    ui->TabColor->item(4,2)->setBackgroundColor(QColor(165, 0, 195, 255));
    ui->TabColor->item(5,2)->setBackgroundColor(QColor(187, 0, 87, 255));
    ui->TabColor->item(6,2)->setBackgroundColor(QColor(254, 0, 0, 255));
    ui->TabColor->item(7,2)->setBackgroundColor(QColor(249, 121, 0, 255));
    ui->TabColor->item(8,2)->setBackgroundColor(QColor(246, 254, 0, 255));
    ui->TabColor->item(9,2)->setBackgroundColor(QColor(76, 205, 28, 255));

    ui->TabColor->item(0,3)->setBackgroundColor(QColor(69, 69, 69, 255));
    ui->TabColor->item(1,3)->setBackgroundColor(QColor(25, 143, 184, 255));
    ui->TabColor->item(2,3)->setBackgroundColor(QColor(44, 78, 200, 255));
    ui->TabColor->item(3,3)->setBackgroundColor(QColor(78, 0, 153, 255));
    ui->TabColor->item(4,3)->setBackgroundColor(QColor(133, 0, 163, 255));
    ui->TabColor->item(5,3)->setBackgroundColor(QColor(155, 0, 75, 255));
    ui->TabColor->item(6,3)->setBackgroundColor(QColor(226, 0, 0, 255));
    ui->TabColor->item(7,3)->setBackgroundColor(QColor(221, 86, 0, 255));
    ui->TabColor->item(8,3)->setBackgroundColor(QColor(197, 196, 0, 255));
    ui->TabColor->item(9,3)->setBackgroundColor(QColor(64, 172, 27, 255));

    ui->TabColor->item(0,4)->setBackgroundColor(QColor(35, 35, 35, 255));
    ui->TabColor->item(1,4)->setBackgroundColor(QColor(19, 112, 144, 255));
    ui->TabColor->item(2,4)->setBackgroundColor(QColor(27, 50, 134, 255));
    ui->TabColor->item(3,4)->setBackgroundColor(QColor(64, 0, 124, 255));
    ui->TabColor->item(4,4)->setBackgroundColor(QColor(105, 0, 128, 255));
    ui->TabColor->item(5,4)->setBackgroundColor(QColor(122, 0, 60, 255));
    ui->TabColor->item(6,4)->setBackgroundColor(QColor(172, 0, 0, 255));
    ui->TabColor->item(7,4)->setBackgroundColor(QColor(166, 68, 0, 255));
    ui->TabColor->item(8,4)->setBackgroundColor(QColor(146, 149, 0, 255));
    ui->TabColor->item(9,4)->setBackgroundColor(QColor(49, 134, 17, 255));

    ui->TabColor->item(0,5)->setBackgroundColor(QColor(0, 0, 0, 255));
    ui->TabColor->item(1,5)->setBackgroundColor(QColor(15, 82, 105, 255));
    ui->TabColor->item(2,5)->setBackgroundColor(QColor(19, 34, 86, 255));
    ui->TabColor->item(3,5)->setBackgroundColor(QColor(53, 3, 100, 255));
    ui->TabColor->item(4,5)->setBackgroundColor(QColor(89, 2, 108, 255));
    ui->TabColor->item(5,5)->setBackgroundColor(QColor(106, 6, 55, 255));
    ui->TabColor->item(6,5)->setBackgroundColor(QColor(152, 2, 2, 255));
    ui->TabColor->item(7,5)->setBackgroundColor(QColor(144, 61, 3, 255));
    ui->TabColor->item(8,5)->setBackgroundColor(QColor(116, 118, 2, 255));
    ui->TabColor->item(9,5)->setBackgroundColor(QColor(0, 255, 0, 255));

    ui->BoxType->setView(new QListView);
    ui->BoxTestNG->setView(new QListView(this));
    ui->Type->setCurrentIndex(0);
    ui->Other->setCurrentIndex(0);
}
/**
  * @brief  Initializes the buttons
  * @param  None
  * @retval None
  */
void WinType::BtnInit()
{
    btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnInsert,Qt::Key_0);
    btnGroup->addButton(ui->BtnDelete,Qt::Key_1);
    btnGroup->addButton(ui->BtnRead,Qt::Key_2);
    btnGroup->addButton(ui->BtnCheck,Qt::Key_3);
    btnGroup->addButton(ui->BtnConfExit,Qt::Key_4);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(judgeButton(int)));

    connect(ui->BoxType,SIGNAL(currentIndexChanged(int)),this,SLOT(judgeButton(int)));

    connect(ui->TabWire,SIGNAL(cellClicked(int,int)),this,SLOT(showWireColor()));
    connect(ui->TabColor,SIGNAL(cellClicked(int,int)),this,SLOT(selectWireColor(int,int)));
    connect(ui->TabTest,SIGNAL(cellClicked(int,int)),this,SLOT(showAvailableItem(int,int)));
    connect(ui->TabProj,SIGNAL(cellClicked(int,int)),this,SLOT(selectItemToTest(int,int)));
}
/**
  * @brief  Button functions
  * @param  id:button id
  * @retval None
  */
void WinType::judgeButton(int id)
{
    switch (id) {
    case 0:
        ui->labelType->setPixmap(QPixmap(":/source/M1S0.png"));
        break;
    case 1:
        ui->labelType->setPixmap(QPixmap(":/source/M1S1.png"));
        break;
    case 2:
        ui->labelType->setPixmap(QPixmap(":/source/M1S2.png"));
        break;
    case 3:
        ui->labelType->setPixmap(QPixmap(":/source/M1S3.png"));
        break;
    case 4:
        ui->labelType->setPixmap(QPixmap(":/source/M1S4.png"));
        break;
    case 5:
        ui->labelType->setPixmap(QPixmap(":/source/M1S1L1.png"));
        break;
    case 6:
        ui->labelType->setPixmap(QPixmap(":/source/M1S1L2.png"));
        break;
    case 7:
        ui->labelType->setPixmap(QPixmap(":/source/M1S1L3.png"));
        break;
    case 8:
        ui->labelType->setPixmap(QPixmap(":/source/None.png"));
        break;
    case Qt::Key_0:
        TypeAdd();
        break;
    case Qt::Key_1:
        TypeDelete();
        break;
    case Qt::Key_2:
        TypeRead();
        break;
    case Qt::Key_3:
        TypeQuery();
        break;
    case Qt::Key_4:
        emit SendMessage(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    default:
        break;
    }
}
/**
  * @brief  Initializes motor type
  * @param  None
  * @retval None
  */
void WinType::TypesInit()
{
    QDir dir("./config");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    FileNames.clear();
    QFileInfoList list = dir.entryInfoList();
    for (int i=0; i<list.size(); i++)
        FileNames.append(list.at(i).fileName());

    if (FileNames.isEmpty())
        FileNames.append("default.ini");

    ui->TabFile->setRowCount(FileNames.size());
    for (int i=0; i<FileNames.size(); i++) {
        ui->TabFile->setItem(i,0,new QTableWidgetItem);
        ui->TabFile->item(i,0)->setTextAlignment(Qt::AlignCenter);
        ui->TabFile->item(i,0)->setText(QString(FileNames.at(i)).remove(".ini"));
    }
}
/**
  * @brief  Initializes items available
  * @param  None
  * @retval None
  */
void WinType::ItemsInit()
{
    QStringList temp;
    QSettings *g_settings = new QSettings(GLOBAL_PATH,QSettings::IniFormat);
    g_settings->setIniCodec("GB18030");
    g_settings->beginGroup("GLOBAL");
    //可使用的测试项目
    temp = (g_settings->value("ItemEnable","0 1 2 3 4 6").toString()).split(" ");
    if (ui->TabProj->rowCount() < temp.size()) {
        ui->TabProj->setRowCount(temp.size());
        for (int i=0; i<temp.size(); i++) {
            ui->TabProj->setItem(i,0,new QTableWidgetItem);
            ui->TabProj->item(i,0)->setTextAlignment(Qt::AlignCenter);
            ui->TabProj->item(i,0)->setText(ItemNames.at(temp.at(i).toInt()));

            ui->TabProj->setItem(i,1,new QTableWidgetItem);
            ui->TabProj->item(i,1)->setTextAlignment(Qt::AlignCenter);
            if (ui->TabProj->item(i,0)->text() == "空")
                ui->TabProj->item(i,1)->setText("返回");
            else
                ui->TabProj->item(i,1)->setText("设置");
        }
    }
}
/**
  * @brief  Initializes settings
  * @param  None
  * @retval None
  */
void WinType::SetInit()
{
    qDebug()<<QTime::currentTime().toString()<<"读取电机型号";

    QStringList temp;
    QSettings *g_settings = new QSettings(GLOBAL_PATH,QSettings::IniFormat);
    g_settings->setIniCodec("GB18030");
    g_settings->beginGroup("GLOBAL");
    //当前使用的测试项目
    FileInUse = g_settings->value("FileInUse","default.ini").toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString n = QString("./config/%1.ini").arg(FileInUse);
    QSettings *c_settings = new QSettings(n,QSettings::IniFormat);
    c_settings->setIniCodec("GB18030");
    c_settings->beginGroup("GLOBAL");
    ui->TextTypeShow->setText(FileInUse.remove(".ini"));

    temp = (c_settings->value("ProjToTest","0").toString().split(" "));
    ui->TabTest->setRowCount(temp.size()+1);
    for (int i=0; i<temp.size()+1; i++) {
        ui->TabTest->setItem(i,0,new QTableWidgetItem);
        ui->TabTest->item(i,0)->setTextAlignment(Qt::AlignCenter);
    }
    for (int i=0; i<temp.size(); i++) {
        ui->TabTest->item(i,0)->setText(ItemNames.at(temp.at(i).toInt()));
    }

    temp = c_settings->value("WireColor").toString().split(" ");
    for (int i=0; i<qMin(temp.size(),ui->TabWire->rowCount()*2); i++) {
        ui->TabWire->item(i/2,i%2)->setBackgroundColor(QColor(temp.at(i)));
    }

    ui->BoxTestNG->setCurrentIndex(c_settings->value("TestNG",0).toInt());
    ui->BoxType->setCurrentIndex(c_settings->value("WinType","0").toInt());
    qDebug()<<QTime::currentTime().toString()<<"读取电机型号OK";
}
/**
  * @brief  save settings
  * @param  None
  * @retval None
  */
void WinType::SetSave()
{
    qDebug()<<QTime::currentTime().toString()<<"保存电机型号";

    QStringList temp;
    QSettings *g_settings = new QSettings(GLOBAL_PATH,QSettings::IniFormat);
    g_settings->setIniCodec("GB18030");
    g_settings->beginGroup("GLOBAL");
    //当前使用的测试项目
    FileInUse = g_settings->value("FileInUse","default.ini").toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString n = QString("./config/%1.ini").arg(FileInUse);
    QSettings *c_settings = new QSettings(n,QSettings::IniFormat);
    c_settings->setIniCodec("GB18030");
    c_settings->beginGroup("GLOBAL");

    for (int i=0; i<ui->TabTest->rowCount()-1; i++) {
        if (!ui->TabTest->item(i,0)->text().isEmpty()) {
            for (int j=0; j<ItemNames.size(); j++) {
                if (ui->TabTest->item(i,0)->text() == ItemNames.at(j))
                    temp.append(QString::number(j));
            }
        }
    }
    c_settings->setValue("ProjToTest",temp.join(" "));

    temp.clear();
    for (int i=0; i<ui->TabWire->rowCount(); i++) {
        temp.append(ui->TabWire->item(i,0)->backgroundColor().name());
        temp.append(ui->TabWire->item(i,1)->backgroundColor().name());
    }
    c_settings->setValue("WireColor",temp.join(" "));

    c_settings->setValue("TestNG",QString::number(ui->BoxTestNG->currentIndex()));
    c_settings->setValue("WinType",QString::number(ui->BoxType->currentIndex()));
    qDebug()<<QTime::currentTime().toString()<<"保存电机型号OK";
}
/**
  * @brief  Select wire color
  * @param  row,column
  * @retval None
  */
void WinType::selectWireColor(int row, int column)
{
    ui->Type->setCurrentIndex(0);
    QColor color = ui->TabColor->item(row,column)->backgroundColor();
    if (ui->TabWire->currentRow() < 0)
        return;
    ui->TabWire->currentItem()->setBackgroundColor(color);
    ui->TabWire->currentItem()->setSelected(false);
    ui->TabColor->currentItem()->setSelected(false);
}
/**
  * @brief  Show wire color page
  * @param  None
  * @retval None
  */
void WinType::showWireColor()
{
    ui->Type->setCurrentIndex(1);
}
/**
  * @brief  Show items available
  * @param  None
  * @retval None
  */
void WinType::showAvailableItem(int, int)
{
    ui->Other->setCurrentIndex(1);
}
/**
  * @brief  Select item to test
  * @param  None
  * @retval None
  */
void WinType::selectItemToTest(int row, int column)
{
    if (ui->TabTest->currentRow() < 0)
        return;
    if (column > 0) {
        if (ui->TabProj->item(row,0)->text() == tr("空"))
            ui->Other->setCurrentIndex(0);
        if (ui->TabProj->item(row,0)->text() == tr("电阻"))
            emit SendMessage(ADDR,WIN_CMD_SWITCH,"电阻");
        if (ui->TabProj->item(row,0)->text() == tr("反嵌"))
            emit SendMessage(ADDR,WIN_CMD_SWITCH,"反嵌");
        if (ui->TabProj->item(row,0)->text() == tr("绝缘"))
            emit SendMessage(ADDR,WIN_CMD_SWITCH,"绝缘");
        if (ui->TabProj->item(row,0)->text() == tr("交耐"))
            emit SendMessage(ADDR,WIN_CMD_SWITCH,"交耐");
        if (ui->TabProj->item(row,0)->text() == tr("直耐"))
            emit SendMessage(ADDR,WIN_CMD_SWITCH,"直耐");
        if (ui->TabProj->item(row,0)->text() == tr("匝间"))
            emit SendMessage(ADDR,WIN_CMD_SWITCH,"匝间");
        if (ui->TabProj->item(row,0)->text() == tr("电感"))
            emit SendMessage(ADDR,WIN_CMD_SWITCH,"电感");
        if (ui->TabProj->item(row,0)->text() == tr("功率"))
            emit SendMessage(ADDR,WIN_CMD_SWITCH,"功率");
        if (ui->TabProj->item(row,0)->text() == tr("低启"))
            emit SendMessage(ADDR,WIN_CMD_SWITCH,"低启");
        if (ui->TabProj->item(row,0)->text() == tr("堵转"))
            emit SendMessage(ADDR,WIN_CMD_SWITCH,"堵转");
        return;
    }
    if (ui->TabTest->currentRow() == ui->TabTest->rowCount()-1) {
        ui->TabTest->setItem(ui->TabTest->rowCount()-1,0,new QTableWidgetItem);
        ui->TabTest->item(ui->TabTest->rowCount()-1,0)->setTextAlignment(Qt::AlignCenter);
        ui->TabTest->setRowCount(ui->TabTest->rowCount()+1);
    }
    if (row == 0) {
        ui->TabTest->removeRow(ui->TabTest->currentRow());
        return;
    }
    QString t = ui->TabProj->item(row,column)->text();
    ui->TabTest->currentItem()->setText(t);
}
/**
  * @brief  Add motor type
  * @param  None
  * @retval None
  */
void WinType::TypeAdd()
{
    QString t = ui->EditTypeName->text();
    if (t.isEmpty())
        return;
    for (int i=0; i<ui->TabFile->rowCount(); i++) {
        if (ui->TabFile->item(i,0)->text() == t)
            return;
    }
    QSettings *g_settings = new QSettings(GLOBAL_PATH,QSettings::IniFormat);
    g_settings->setIniCodec("GB18030");
    g_settings->beginGroup("GLOBAL");
    //当前使用的测试项目
    FileInUse = g_settings->value("FileInUse","default.ini").toString();
    FileInUse.remove(".ini");
    QString Source = QString("./config/%1.ini").arg(FileInUse);
    QString Target = QString("./config/%1.ini").arg(t);
    QFile::copy(Source,Target);
    SetInit();
}
/**
  * @brief  Delete motor type
  * @param  None
  * @retval None
  */
void WinType::TypeDelete()
{
    QString t = ui->TabFile->currentItem()->text();
    QString Target = QString("./config/%1.ini").arg(t);
    QFile::remove(Target);
    SetInit();
}
/**
  * @brief  Read motor type
  * @param  None
  * @retval None
  */
void WinType::TypeRead()
{
    qDebug()<<QTime::currentTime().toString()<<"调入电机型号";
    if (ui->TabFile->currentRow() < 0) {
        qDebug()<<QTime::currentTime().toString()<<"调入电机型号Error";
        return;
    }
    QString t = ui->TabFile->currentItem()->text();
    t.append(".ini");
    QSettings *g_settings = new QSettings(GLOBAL_PATH,QSettings::IniFormat);
    g_settings->setIniCodec("GB18030");
    g_settings->beginGroup("GLOBAL");
    g_settings->setValue("FileInUse",t);
    qDebug()<<QTime::currentTime().toString()<<"调入电机型号OK";
    SetInit();
}
/**
  * @brief  Query motor type
  * @param  None
  * @retval None
  */
void WinType::TypeQuery()
{
    QString t = ui->EditTypeName->text();
    if (t.isEmpty())
        return;
    for (int i=0; i<FileNames.size(); i++) {
        if (FileNames.at(i).contains(t))
            FileNames.move(i,0);
    }
    for (int i=0; i<qMin(FileNames.size(),ui->TabFile->rowCount()); i++) {
        ui->TabFile->item(i,0)->setText(QString(FileNames.at(i)).remove(".ini"));
    }
}
/**
  * @brief  Initialize when show
  * @param  None
  * @retval None
  */
void WinType::showEvent(QShowEvent *)
{
    TypesInit();
    ItemsInit();
    SetInit();
}
/**
  * @brief  Save settings when hide
  * @param  None
  * @retval None
  */
void WinType::hideEvent(QHideEvent *)
{
    SetSave();
}
/*********************************END OF FILE**********************************/
