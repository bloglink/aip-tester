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

WinSyst::WinSyst(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinSyst)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
}

WinSyst::~WinSyst()
{
    delete ui;
}

void WinSyst::InitWindows()
{
    ui->BoxUser->setView(new QListView(this));
    ui->BoxMode->setView(new QListView(this));
    ui->BoxStyle->setView(new QListView(this));
    ui->BoxPower->setView(new QListView(this));
    password.clear();
    file = new QFile("log.txt");
    file->open(QFile::ReadWrite);

    dateTime = ui->EditTime->dateTime();
}

void WinSyst::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnOk,Qt::Key_0);
    btnGroup->addButton(ui->BtnCancel,Qt::Key_1);
    btnGroup->addButton(ui->BtnPassword,Qt::Key_2);
    btnGroup->addButton(ui->BtnSystPasswordOK,Qt::Key_3);
    btnGroup->addButton(ui->BtnSystPasswordExit,Qt::Key_4);
    btnGroup->addButton(ui->BtnExit,Qt::Key_4);
    btnGroup->addButton(ui->btnTime,Qt::Key_5);
    btnGroup->addButton(ui->BtnStatus,Qt::Key_6);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}

void WinSyst::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        if (ui->EditPassword->text() == "aip9918")
            emit SendCommand(ADDR,CMD_JUMP,"WinBack");
        else if (ui->EditPassword->text() == password)
            ui->StackWinSyst->setCurrentIndex(0);
        else
            emit SendCommand(ADDR,CMD_JUMP,NULL);
        break;
    case Qt::Key_1:
        emit SendCommand(ADDR,CMD_JUMP,NULL);
        break;
    case Qt::Key_2:
        ui->StackWinSyst->setCurrentIndex(2);
        break;
    case Qt::Key_3:
        SetPassword();
        break;
    case Qt::Key_4:
        emit SendCommand(ADDR,CMD_JUMP,NULL);
        break;
    case Qt::Key_5:
        SetDateTime();
        break;
    case Qt::Key_6:
        emit SendCommand(ADDR,CMD_STATUS,NULL);
        break;
    default:
        break;
    }
}

void WinSyst::InitSettings()
{
    qDebug()<<QTime::currentTime().toString()<<"读取系统配置";

    QSettings *g_ini = new QSettings(INI_PATH,QSettings::IniFormat);
    g_ini->setIniCodec("GB18030");
    g_ini->beginGroup("GLOBAL");

    ui->BoxUser->setCurrentIndex(g_ini->value("User","0").toInt());
    ui->BoxMode->setCurrentIndex(g_ini->value("Mode","0").toInt());
    ui->BoxStyle->setCurrentIndex(g_ini->value("Style","0").toInt());
    ui->BoxTimeNG->setValue(g_ini->value("TimeNG","0.5").toDouble());
    ui->BoxTimeOK->setValue(g_ini->value("TimeOK","0.2").toDouble());
    ui->BoxPower->setCurrentIndex(g_ini->value("PowerSupply","0").toInt());

    password = g_ini->value("Password","").toString();
    ui->EditPassword->clear();

    int t = g_ini->value("AddSeconds","0").toInt();
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

void WinSyst::SaveSettings()
{
    qDebug()<<QTime::currentTime().toString()<<"保存系统配置";
    QSettings *g_ini = new QSettings(INI_PATH,QSettings::IniFormat);
    g_ini->setIniCodec("GB18030");
    g_ini->beginGroup("GLOBAL");
    g_ini->setValue("User",QString::number(ui->BoxUser->currentIndex()));
    g_ini->setValue("Mode",QString::number(ui->BoxMode->currentIndex()));
    g_ini->setValue("Style",QString::number(ui->BoxStyle->currentIndex()));
    g_ini->setValue("Password",password);
    g_ini->setValue("AddSeconds",dateTime.secsTo(QDateTime::currentDateTime()));
    g_ini->setValue("TimeNG",ui->BoxTimeNG->value());
    g_ini->setValue("TimeOK",ui->BoxTimeOK->value());
    g_ini->setValue("PowerSupply",ui->BoxPower->currentIndex());
    qDebug()<<QTime::currentTime().toString()<<"保存系统配置OK";
}

void WinSyst::SetDateTime()
{
    QString time = ui->EditTime->dateTime().toString("yyyy.MM.dd-hh:mm:ss");
    QProcess::execute(QString("date %1").arg(time));
    QProcess::execute(QString("hwclock -w"));
}

void WinSyst::SetPassword()
{
    QString old = ui->EditPwdOld->text();
    QString new1 = ui->EditPwdNew->text();
    QString new2 = ui->EditPwdNewR->text();
    if ( old == password && new1 == new2)
        password = new1;
    SaveSettings();
}

void WinSyst::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr!=ADDR && addr!=WIN_ID_SYS)
        return;
    switch (cmd) {
    case CMD_DEBUG:
        WriteLog(msg);
        break;
    case CMD_INIT:
        SendWinCmdStartMode();
        break;
    default:
        break;
    }
}

void WinSyst::SendWinCmdStartMode()
{
    QByteArray msg;
    msg.append(quint8(ui->BoxMode->currentIndex()));
    emit SendCommand(WIN_ID_OUT13,CMD_INIT,msg);
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

void WinSyst::showEvent(QShowEvent *)
{
    InitSettings();
    ui->StackWinSyst->setCurrentIndex(1);
}

void WinSyst::hideEvent(QHideEvent *)
{
    SaveSettings();
}
/*********************************END OF FILE**********************************/
