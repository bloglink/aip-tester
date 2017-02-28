/**
  ******************************************************************************
  * @file    system.cpp
  * @author  link
  * @version 2.0.1.0
  * @date    2017-02-28
  * @brief   System settings
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "System.h"
#include "ui_System.h"
/**
  * @brief  Initializes
  * @param  *parent:parent widget
  * @retval None
  */
System::System(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::System)
{
    ui->setupUi(this);
    initShow();
    initButton();
}
/**
  * @brief  Destruct the window
  * @param  None
  * @retval None
  */
System::~System()
{
    delete ui;
}
/**
  * @brief  Initializes the window
  * @param  None
  * @retval None
  */
void System::initShow()
{
    ui->BoxUser->setView(new QListView(this));
    ui->BoxMode->setView(new QListView(this));
    ui->BoxStyle->setView(new QListView(this));
    password.clear();
}
/**
  * @brief  Initializes the buttons
  * @param  None
  * @retval None
  */
void System::initButton()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnOk,Qt::Key_0);
    btnGroup->addButton(ui->BtnCancel,Qt::Key_1);
    btnGroup->addButton(ui->BtnPassword,Qt::Key_2);
    btnGroup->addButton(ui->BtnSystPasswordOK,Qt::Key_3);
    btnGroup->addButton(ui->BtnSystPasswordExit,Qt::Key_4);
    btnGroup->addButton(ui->BtnExit,Qt::Key_4);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(judgeButton(int)));
}
/**
  * @brief  Button functions
  * @param  id:button id
  * @retval None
  */
void System::judgeButton(int id)
{
    switch (id) {
    case Qt::Key_0:
        if (ui->EditPassword->text() == "aip9981")
            emit sendMessage(ADDR,WIN_CMD_SWITCH,"WinBack");
        else if (ui->EditPassword->text() == password)
            ui->StackWinSyst->setCurrentIndex(0);
        else
            emit sendMessage(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    case Qt::Key_1:
        emit sendMessage(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    case Qt::Key_2:
        ui->StackWinSyst->setCurrentIndex(2);
        break;
    case Qt::Key_3:
        modifyPassword();
        break;
    case Qt::Key_4:
        emit sendMessage(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    default:
        break;
    }
}
/**
  * @brief  Initializes the data
  * @param  None
  * @retval None
  */
void System::initSettings()
{
    qDebug()<<QTime::currentTime().toString()<<"读取系统配置";

    QSettings *settings = new QSettings(GLOBAL_PATH,QSettings::IniFormat);
    settings->setIniCodec("GB18030");
    settings->beginGroup("GLOBAL");

    ui->BoxUser->setCurrentIndex(settings->value("User","0").toInt());
    ui->BoxMode->setCurrentIndex(settings->value("Mode","0").toInt());
    ui->BoxStyle->setCurrentIndex(settings->value("Style","0").toInt());
    password = settings->value("Password","").toString();
    ui->EditPassword->clear();
    qDebug()<<QTime::currentTime().toString()<<"读取系统配置OK";
}
/**
  * @brief  Save the data
  * @param  None
  * @retval None
  */
void System::saveSettings()
{
    qDebug()<<QTime::currentTime().toString()<<"保存系统配置";
    QSettings *settings = new QSettings(GLOBAL_PATH,QSettings::IniFormat);
    settings->setIniCodec("GB18030");
    settings->beginGroup("GLOBAL");
    settings->setValue("User",QString::number(ui->BoxUser->currentIndex()));
    settings->setValue("Mode",QString::number(ui->BoxMode->currentIndex()));
    settings->setValue("Style",QString::number(ui->BoxStyle->currentIndex()));
    settings->setValue("Password",password);
    qDebug()<<QTime::currentTime().toString()<<"保存系统配置OK";
}
/**
  * @brief  Judge and save the password
  * @param  None
  * @retval None
  */
void System::modifyPassword()
{
    QString old = ui->EditPwdOld->text();
    QString new1 = ui->EditPwdNew->text();
    QString new2 = ui->EditPwdNewR->text();
    if ( old == password && new1 == new2)
        password = new1;
    saveSettings();
}
/**
  * @brief  Excute command
  * @param  addr:target address;cmd:command to excute;msg:command param
  * @retval None
  */
void System::readMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr!=ADDR)
        return;
    switch (cmd) {
    case WIN_CMD_DEBUG:
        ui->TextDebug->insertPlainText(msg);
        ui->TextDebug->moveCursor(QTextCursor::End);
        break;
    default:
        break;
    }
}
/**
  * @brief  Initializes data when show
  * @param  None
  * @retval None
  */
void System::showEvent(QShowEvent *)
{
    initSettings();
    ui->StackWinSyst->setCurrentIndex(1);
}
/**
  * @brief  Save data when hide
  * @param  None
  * @retval None
  */
void System::hideEvent(QHideEvent *)
{
    saveSettings();
}
/*********************************END OF FILE**********************************/
