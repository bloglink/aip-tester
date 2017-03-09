/**
  ******************************************************************************
  * @file    WinSyst.cpp
  * @author  link
  * @version 2.0.1.0
  * @date    2017-02-28
  * @brief   WinSyst settings
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "WinSyst.h"
#include "ui_WinSyst.h"
/**
  * @brief  Initializes
  * @param  *parent:parent widget
  * @retval None
  */
WinSyst::WinSyst(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinSyst)
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
WinSyst::~WinSyst()
{
    delete ui;
}
/**
  * @brief  Initializes the window
  * @param  None
  * @retval None
  */
void WinSyst::WinInit()
{
    ui->BoxUser->setView(new QListView(this));
    ui->BoxMode->setView(new QListView(this));
    ui->BoxStyle->setView(new QListView(this));
    password.clear();
    file = new QFile("log.txt");
    file->open(QFile::ReadWrite);

    dateTime = ui->EditTime->dateTime();
}
/**
  * @brief  Initializes the buttons
  * @param  None
  * @retval None
  */
void WinSyst::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnOk,Qt::Key_0);
    btnGroup->addButton(ui->BtnCancel,Qt::Key_1);
    btnGroup->addButton(ui->BtnPassword,Qt::Key_2);
    btnGroup->addButton(ui->BtnSystPasswordOK,Qt::Key_3);
    btnGroup->addButton(ui->BtnSystPasswordExit,Qt::Key_4);
    btnGroup->addButton(ui->BtnExit,Qt::Key_4);
    btnGroup->addButton(ui->btnTime,Qt::Key_5);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/**
  * @brief  Button functions
  * @param  id:button id
  * @retval None
  */
void WinSyst::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        if (ui->EditPassword->text() == "aip9918")
            emit SendMessage(ADDR,CMD_JUMP,"WinBack");
        else if (ui->EditPassword->text() == password)
            ui->StackWinSyst->setCurrentIndex(0);
        else
            emit SendMessage(ADDR,CMD_JUMP,NULL);
        break;
    case Qt::Key_1:
        emit SendMessage(ADDR,CMD_JUMP,NULL);
        break;
    case Qt::Key_2:
        ui->StackWinSyst->setCurrentIndex(2);
        break;
    case Qt::Key_3:
        Password();
        break;
    case Qt::Key_4:
        emit SendMessage(ADDR,CMD_JUMP,NULL);
        break;
    case Qt::Key_5:
        SetDateTime();
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
void WinSyst::SetInit()
{
    qDebug()<<QTime::currentTime().toString()<<"读取系统配置";

    QSettings *settings = new QSettings(INI_PATH,QSettings::IniFormat);
    settings->setIniCodec("GB18030");
    settings->beginGroup("GLOBAL");

    ui->BoxUser->setCurrentIndex(settings->value("User","0").toInt());
    ui->BoxMode->setCurrentIndex(settings->value("Mode","0").toInt());
    ui->BoxStyle->setCurrentIndex(settings->value("Style","0").toInt());
    password = settings->value("Password","").toString();
    ui->EditPassword->clear();

    int t = settings->value("AddSeconds","0").toInt();
    if (dateTime.secsTo(QDateTime::currentDateTime()) < t) {
        QDateTime tt = dateTime;
        tt = tt.addSecs(t);
        ui->EditTime->setDateTime(tt);
        SetDateTime();
        QMessageBox::warning(this,tr("警告"),tr("日期丢失,请重新设定日期"),QMessageBox::Ok);
    } else {
        ui->EditTime->setDateTime(QDateTime::currentDateTime());
    }
    qDebug()<<QTime::currentTime().toString()<<"读取系统配置OK";
}
/**
  * @brief  Save the data
  * @param  None
  * @retval None
  */
void WinSyst::SetSave()
{
    qDebug()<<QTime::currentTime().toString()<<"保存系统配置";
    QSettings *settings = new QSettings(INI_PATH,QSettings::IniFormat);
    settings->setIniCodec("GB18030");
    settings->beginGroup("GLOBAL");
    settings->setValue("User",QString::number(ui->BoxUser->currentIndex()));
    settings->setValue("Mode",QString::number(ui->BoxMode->currentIndex()));
    settings->setValue("Style",QString::number(ui->BoxStyle->currentIndex()));
    settings->setValue("Password",password);
    settings->setValue("AddSeconds",dateTime.secsTo(QDateTime::currentDateTime()));
    qDebug()<<QTime::currentTime().toString()<<"保存系统配置OK";
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void WinSyst::SetDateTime()
{
    QString time = ui->EditTime->dateTime().toString("yyyy.MM.dd-hh:mm:ss");
    QProcess::execute(QString("date %1").arg(time));
    QProcess::execute(QString("hwclock -w"));
}
/**
  * @brief  Judge and save the password
  * @param  None
  * @retval None
  */
void WinSyst::Password()
{
    QString old = ui->EditPwdOld->text();
    QString new1 = ui->EditPwdNew->text();
    QString new2 = ui->EditPwdNewR->text();
    if ( old == password && new1 == new2)
        password = new1;
    SetSave();
}
/**
  * @brief  Excute command
  * @param  addr:target address;cmd:command to excute;msg:command param
  * @retval None
  */
void WinSyst::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr!=ADDR)
        return;
    switch (cmd) {
    case CMD_DEBUG:
        WriteLog(msg);
        break;
    default:
        break;
    }
}

void WinSyst::WriteLog(QByteArray msg)
{
    ui->TextDebug->insertPlainText(msg);
    ui->TextDebug->moveCursor(QTextCursor::End);
    QTextStream out(file);

    out.seek(file->size());
    out<<QDateTime::currentDateTime().toString("yyyyMMdd hh:mm ");
    out<<msg;
}
/**
  * @brief  Initializes data when show
  * @param  None
  * @retval None
  */
void WinSyst::showEvent(QShowEvent *)
{
    SetInit();
    ui->StackWinSyst->setCurrentIndex(1);
}
/**
  * @brief  Save data when hide
  * @param  None
  * @retval None
  */
void WinSyst::hideEvent(QHideEvent *)
{
    SetSave();
}
/*********************************END OF FILE**********************************/
