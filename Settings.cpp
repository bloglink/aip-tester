/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       系统设置界面
*******************************************************************************/
#include "Settings.h"
#include "ui_Settings.h"
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       构造
******************************************************************************/
Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();

    process = new QProcess(this);
    connect(process, SIGNAL(readyRead()), this, SLOT(ReadData()));
    Testing = false;
    isStop = true;

    Timer = new QTimer(this);
    connect(Timer,SIGNAL(timeout()),this,SLOT(TestStart()));
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       析构
******************************************************************************/
Settings::~Settings()
{
    delete ui;
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       界面初始化
******************************************************************************/
void Settings::WinInit()
{
    ui->TabDisk->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabDisk->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->BoxUser->setView(new QListView(this));
    ui->BoxMode->setView(new QListView(this));
    ui->BoxStyle->setView(new QListView(this));
    //    ui->EditIp->setText(IpAddress());
#ifndef __arm__
    ui->TabDisk->hide();
#endif
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       按钮初始化
******************************************************************************/
void Settings::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSystLoginOK,Qt::Key_0);
    btnGroup->addButton(ui->BtnSystLoginExit,Qt::Key_1);
    btnGroup->addButton(ui->BtnSystLoginPassword,Qt::Key_2);
    btnGroup->addButton(ui->BtnSystPasswordOK,Qt::Key_3);
    btnGroup->addButton(ui->BtnSystPasswordExit,Qt::Key_4);
    btnGroup->addButton(ui->BtnExit,Qt::Key_4);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       按钮功能
******************************************************************************/
void Settings::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        if (ui->EditPassword->text() == "aip9981")
            emit TransformCmd(ADDR,WIN_CMD_SWITCH,"WinBack");
        else if (ui->EditPassword->text() == pwd)
            ui->StackWinSyst->setCurrentIndex(2);
        else
            emit TransformCmd(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    case Qt::Key_1:
        emit TransformCmd(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    case Qt::Key_2:
        ui->StackWinSyst->setCurrentIndex(1);
        break;
    case Qt::Key_3:
        Password();
        break;
    case Qt::Key_4:
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
void Settings::DatInit()
{
    global = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    ui->BoxUser->setCurrentIndex(global->value("User","0").toInt());
    ui->BoxMode->setCurrentIndex(global->value("Mode","0").toInt());
    ui->BoxStyle->setCurrentIndex(global->value("Style","0").toInt());
    pwd = global->value("Password","").toString();
    ui->EditPassword->clear();
#ifdef __arm__
    process->start("df -h");
#endif
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       数据保存
******************************************************************************/
void Settings::DatSave()
{
    global->setValue("User",QString::number(ui->BoxUser->currentIndex()));
    global->setValue("Mode",QString::number(ui->BoxMode->currentIndex()));
    global->setValue("Style",QString::number(ui->BoxStyle->currentIndex()));
    global->setValue("Password",pwd);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       显示磁盘
******************************************************************************/
void Settings::ReadData()
{
    quint8 row = 0;
    while (!process->atEnd()) {
        QString s = QLatin1String(process->readLine());
        if (s.startsWith("/") || s.startsWith("ubi") || s.startsWith("tmp")) {
            if (row >= ui->TabDisk->rowCount()) {
                ui->TabDisk->setRowCount(row+1);
                for (int i=0; i<ui->TabDisk->columnCount(); i++) {
                    ui->TabDisk->setItem(row,i,new QTableWidgetItem);
                    ui->TabDisk->item(row,i)->setTextAlignment(Qt::AlignCenter);
                    ui->TabDisk->item(row,i)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                }
            }
            QStringList list = s.split(" ");
            quint8 index = 0;
            for (int i=0; i<list.size(); i++) {
                QString temp = QString(list.at(i)).trimmed();
                if (temp.isEmpty())
                    continue;
                ui->TabDisk->item(row,index)->setText(temp);
                index++;
            }
            row++;
        }
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       修改密码
******************************************************************************/
void Settings::Password()
{
    QString old = ui->EditPwdOld->text();
    QString new1 = ui->EditPwdNew->text();
    QString new2 = ui->EditPwdNewR->text();
    if ( old == pwd && new1 == new2)
        pwd = new1;
    DatSave();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       获取IP
******************************************************************************/
QString Settings::IpAddress()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    for (int i=0; i<list.size(); i++) {
        if (list[i].hardwareAddress().size() != 17)
            continue;
        if (list[i].hardwareAddress() == "00:00:00:00:00:00")
            continue;
        QList<QNetworkAddressEntry> entryList = list.at(i).addressEntries();
        return entryList.at(0).ip().toString();
    }
    return NULL;
}

/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.15
 * brief:       命令处理
*******************************************************************************/
void Settings::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray data)
{
    if (addr!=ADDR && addr!=WIN_ID_OUT && addr!=CAN_ID_13OUT && addr!=CAN_ID_14OUT && addr!=CAN_ID_15OUT)
        return;
    switch (cmd) {
    case WIN_CMD_STYLE:
        TestStyle();
        break;
    case CAN_DAT_GET:
        ExcuteCanCmd(addr,data);
        break;
    case CAN_CMD_CHECK:
        TestCheck(data.toInt());
        break;
    case CAN_CMD_INIT:
        TestConfig();
        break;
    case WIN_CMD_DEBUG:
        ui->TextDebug->insertPlainText(data);
        ui->TextDebug->moveCursor(QTextCursor::End);
        break;
    default:
        break;
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       输出板信号处理
******************************************************************************/
void Settings::ExcuteCanCmd(quint16 id,QByteArray msg)
{
    if (quint8(msg.at(0)) == 0x00) {
        Testing = false;
    }
    if (quint8(msg.at(0)) == 0x01 && quint8(msg.at(1) == 0x01)) {
        if (!isStop || Testing)
            return;
        if (id == CAN_ID_13OUT) {
            Pos = 0x13;
        }
        if (id == CAN_ID_14OUT) {
            Pos = 0x14;
        }
        isStop = false;
        Timer->start(10);
    }
    if (quint8(msg.at(0)) == 0x01 && quint8(msg.at(1) == 0x00)) {
        if (id == CAN_ID_13OUT && Pos == 0x13) {
            isStop = true;
            Testing = false;
            emit TransformCmd(ADDR,CAN_CMD_STOP,NULL);
        }
        if (id == CAN_ID_14OUT && Pos == 0x14) {
            isStop = true;
            Testing = false;
            emit TransformCmd(ADDR,CAN_CMD_STOP,NULL);
        }
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       设置界面风格
******************************************************************************/
void Settings::TestStyle()
{
    //设置界面风格
    QFile file;
    QString qss;
    if (ui->BoxStyle->currentIndex() == 0)
        file.setFileName(":/css/PeacockGreen.css");
    if (ui->BoxStyle->currentIndex() == 1)
        file.setFileName(":/css/SoftRainBlue.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       查询输出板状态
******************************************************************************/
void Settings::TestCheck(quint16 pos)
{
    if (Testing)
        return;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);

    Testing = true;
    QString w;
    switch (pos) {
    case 0:
        out<<quint16(0x13)<<quint8(0x01)<<quint8(0x00);
        w = tr("输出板13异常");
        break;
    case 1:
        out<<quint16(0x14)<<quint8(0x01)<<quint8(0x00);
        w = tr("输出板14异常");
        break;
    case 2:
        out<<quint16(0x15)<<quint8(0x01)<<quint8(0x00);
        w = tr("输出板15异常");
        break;
    default:
        break;
    }
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    if (!WaitTestOver(100)) {
        Testing = false;
        QMessageBox::warning(this,tr("警告"),w,QMessageBox::Ok);
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.17
 * brief:       启动测试
******************************************************************************/
void Settings::TestStart()
{
    Timer->stop();
    emit TransformCmd(ADDR,CAN_CMD_START,QString::number(Pos).toUtf8());
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       设置启动方式
******************************************************************************/
void Settings::TestConfig()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);

    quint8 mode;
    if (ui->BoxMode->currentIndex() == 2)
        mode = 2;
    else
        mode = 0;
    out<<quint16(0x13)<<quint8(0x02)<<quint8(0x03)<<quint8(mode);
    out<<quint16(0x14)<<quint8(0x02)<<quint8(0x03)<<quint8(mode);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      等待测试结束
 * date:       2017.02.15
 * brief:      去除超时处理
*******************************************************************************/
bool Settings::WaitTestOver(quint16 t)
{
    TimeOut = 0;
    while (Testing) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      延时
*******************************************************************************/
void Settings::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       显示
******************************************************************************/
void Settings::showEvent(QShowEvent *)
{
    ui->StackWinSyst->setCurrentIndex(0);
    DatInit();
    connect(ui->BoxStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(TestStyle()));
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       退出保存
******************************************************************************/
void Settings::hideEvent(QHideEvent *)
{
    DatSave();
    disconnect(ui->BoxStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(TestStyle()));
}
