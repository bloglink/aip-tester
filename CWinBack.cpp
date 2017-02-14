/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       后台设置界面
*******************************************************************************/
#include "CWinBack.h"
#include "ui_CWinBack.h"
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       构造
******************************************************************************/
CWinBack::CWinBack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CWinBack)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       析构
******************************************************************************/
CWinBack::~CWinBack()
{
    delete ui;
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       界面初始化
******************************************************************************/
void CWinBack::WinInit()
{

}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       按钮初始化
******************************************************************************/
void CWinBack::BtnInit()
{
    QButtonGroup *BtnGroup = new QButtonGroup;
    BtnGroup->addButton(ui->BtnExit,Qt::Key_0);
    connect(BtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       按钮功能
******************************************************************************/
void CWinBack::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        emit TransformCmd(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    default:
        break;
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       数据初始化
******************************************************************************/
void CWinBack::DatInit()
{
    global = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    QStringList temp = global->value("ItemEnable","1 2 3 4 6").toString().split(" ");
    if (temp.contains("1"))
        ui->BoxEnableDcr->setChecked(true);
    if (temp.contains("2"))
        ui->BoxEnableMag->setChecked(true);
    if (temp.contains("3"))
        ui->BoxEnableIr->setChecked(true);
    if (temp.contains("4"))
        ui->BoxEnableAcw->setChecked(true);
    if (temp.contains("5"))
        ui->BoxEnableDcw->setChecked(true);
    if (temp.contains("6"))
        ui->BoxEnableImp->setChecked(true);

    temp = global->value("OutEnable","0 1").toString().split(" ");
    if (temp.contains("0"))
        ui->BoxEnableOut13->setChecked(true);
    if (temp.contains("1"))
        ui->BoxEnableOut14->setChecked(true);
    if (temp.contains("2"))
        ui->BoxEnableOut15->setChecked(true);
    if (temp.contains("3"))
        ui->BoxEnableOut16->setChecked(true);
    if (temp.contains("4"))
        ui->BoxEnableOut17->setChecked(true);

    QString t = global->value("Number","V20170106").toString();
    ui->EditNumber->setText(t);

}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       数据保存
******************************************************************************/
void CWinBack::DatSave()
{
    QStringList temp;
    temp.append("0");
    if (ui->BoxEnableDcr->isChecked())
        temp.append("1");
    if (ui->BoxEnableMag->isChecked())
        temp.append("2");
    if (ui->BoxEnableIr->isChecked())
        temp.append("3");
    if (ui->BoxEnableAcw->isChecked())
        temp.append("4");
    if (ui->BoxEnableDcw->isChecked())
        temp.append("5");
    if (ui->BoxEnableImp->isChecked())
        temp.append("6");
    global->setValue("ItemEnable",temp.join(" "));

    temp.clear();
    if (ui->BoxEnableOut13->isChecked())
        temp.append("0");
    if (ui->BoxEnableOut14->isChecked())
        temp.append("1");
    if (ui->BoxEnableOut15->isChecked())
        temp.append("2");
    if (ui->BoxEnableOut16->isChecked())
        temp.append("3");
    if (ui->BoxEnableOut17->isChecked())
        temp.append("4");
    global->setValue("OutEnable",temp.join(" "));

    global->setValue("Number",ui->EditNumber->text());
}

void CWinBack::showEvent(QShowEvent *)
{
    DatInit();
}

void CWinBack::hideEvent(QHideEvent *)
{
    DatSave();
}
