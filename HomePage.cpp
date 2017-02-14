/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.11.04
 * brief:       开机主界面
*******************************************************************************/
#include "HomePage.h"
#include "ui_HomePage.h"
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.11.04
  * brief:      初始化
******************************************************************************/
HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.11.04
  * brief:      界面析构
******************************************************************************/
HomePage::~HomePage()
{
    delete ui;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.11.04
  * brief:      界面显示初始化
******************************************************************************/
void HomePage::WinInit()
{
//    ui->titleVn->hide();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.11.04
  * brief:      按钮功能初始化
******************************************************************************/
void HomePage::BtnInit()
{
    QButtonGroup *BtnGroup = new QButtonGroup;
    BtnGroup->addButton(ui->KeyHomeSyst,Qt::Key_1);
    BtnGroup->addButton(ui->KeyHomeConf,Qt::Key_2);
    BtnGroup->addButton(ui->KeyHomeData,Qt::Key_3);
    BtnGroup->addButton(ui->KeyHomeTest,Qt::Key_4);
//    BtnGroup->addButton(ui->KeyHomeExit,Qt::Key_5);
    connect(BtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.11.04
  * brief:      按钮功能
******************************************************************************/
void HomePage::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_1:
        emit TransformCmd(ADDR,WIN_CMD_SWITCH,"WinSyst");
        break;
    case Qt::Key_2:
        emit TransformCmd(ADDR,WIN_CMD_SWITCH,"WinType");
        break;
    case Qt::Key_3:
        emit TransformCmd(ADDR,WIN_CMD_SWITCH,"WinData");
        break;
    case Qt::Key_4:
        emit TransformCmd(ADDR,WIN_CMD_SWITCH,"WinTest");
        break;
    case Qt::Key_5:
        QApplication::closeAllWindows();
        break;
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.11.04
  * brief:      数据初始化
******************************************************************************/
void HomePage::DatInit()
{
    set = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("GLOBAL");
    if (TimeJudge())
        set->setValue("DateTime",QDateTime::currentDateTime());
    else
        TimeError();
    set->setValue("SYS_VERSION",ui->titleVn->text());
    set->setValue("SYS_NUMBER","AIP000001");
    set->setValue("DEV_USER","guest");
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.11.04
  * brief:      判断时间是否丢失
******************************************************************************/
bool HomePage::TimeJudge()
{
    if (set->value("DateTime").toDateTime().isNull())
        return true;
    if (set->value("DateTime").toDateTime() < QDateTime::currentDateTime())
        return true;
    return false;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.11.04
  * brief:      时间丢失处理
******************************************************************************/
void HomePage::TimeError()
{
    qDebug()<<"TimeError";
}

void HomePage::hideEvent(QHideEvent *)
{
    ui->Text->hide();
    ui->KeyHomeSyst->setEnabled(true);
    ui->KeyHomeConf->setEnabled(true);
    ui->KeyHomeData->setEnabled(true);
    ui->KeyHomeTest->setEnabled(true);
}

void HomePage::ExcuteCmd(QByteArray msg)
{
    ui->Text->insertPlainText(msg);
}
