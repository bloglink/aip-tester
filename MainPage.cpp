/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       后台界面
 * date:        2017.01.04
 * brief:       增加自检流程
 * date:        2017.01.05
 * brief:       显示测试数量
 * date:        2017.01.17
 * brief:       增加报警输出
*******************************************************************************/
#include "MainPage.h"
#include "ui_MainPage.h"
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       构造
******************************************************************************/
MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    WinInit();
    DatInit();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       析构
******************************************************************************/
MainPage::~MainPage()
{
    can->quit();
    can->wait();
    sql->quit();
    sql->wait();
    delete ui;
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       界面初始化
 * date:        2017.01.04
 * brief:       增加自检流程
 * date:        2017.01.05
 * brief:       自检QTimer冲突，去除自检
******************************************************************************/
void MainPage::WinInit()
{
    //    this->setWindowFlags(Qt::FramelessWindowHint);
    //主界面
    WinHome = new HomePage(this);
    ui->Desktop->addWidget(WinHome);
    WinHome->setObjectName("WinHome");
    connect(WinHome,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //系统设置
    WinSets = new Settings(this);
    ui->Desktop->addWidget(WinSets);
    WinSets->setObjectName("WinSyst");
    connect(WinSets,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),WinSets,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //型号管理
    WinType = new CWinType(this);
    ui->Desktop->addWidget(WinType);
    WinType->setObjectName("WinType");
    connect(WinType,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //数据分析
    WinData = new CWinData(this);
    ui->Desktop->addWidget(WinData);
    WinData->setObjectName("WinData");
    connect(WinData,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //测试界面
    WinTest = new CWinTest(this);
    ui->Desktop->addWidget(WinTest);
    WinTest->setObjectName("WinTest");
    connect(WinTest,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //测试界面
    WinBack = new CWinBack(this);
    ui->Desktop->addWidget(WinBack);
    WinBack->setObjectName("WinBack");
    connect(WinBack,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //电阻设置
    WinSetDcr = new CWinSetDcr(this);
    ui->Desktop->addWidget(WinSetDcr);
    WinSetDcr->setObjectName("电阻");
    connect(WinSetDcr,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),WinSetDcr,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //反嵌设置
    WinSetMag = new CWinSetMag(this);
    ui->Desktop->addWidget(WinSetMag);
    WinSetMag->setObjectName("反嵌");
    connect(WinSetMag,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),WinSetMag,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //绝缘设置
    WinSetIr = new CWinSetIr(this);
    ui->Desktop->addWidget(WinSetIr);
    WinSetIr->setObjectName("绝缘");
    connect(WinSetIr,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),WinSetIr,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //交耐设置
    WinSetAcw = new CWinSetAcw(this);
    ui->Desktop->addWidget(WinSetAcw);
    WinSetAcw->setObjectName("交耐");
    connect(WinSetAcw,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),WinSetAcw,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //匝间设置
    WinSetImp = new CWinSetImp(this);
    ui->Desktop->addWidget(WinSetImp);
    WinSetImp->setObjectName("匝间");
    connect(WinSetImp,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),WinSetImp,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //电感设置
    WinSetIndl = new ConfigIndl(this);
    ui->Desktop->addWidget(WinSetIndl);
    WinSetIndl->setObjectName("电感");
    connect(WinSetIndl,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),WinSetIndl,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //功率设置
    WinSetPower = new ConfigPower(this);
    ui->Desktop->addWidget(WinSetPower);
    WinSetPower->setObjectName("功率");
    connect(WinSetPower,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),WinSetPower,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //低启设置
    WinSetLvs = new ConfigLvs(this);
    ui->Desktop->addWidget(WinSetLvs);
    WinSetLvs->setObjectName("低启");
    connect(WinSetLvs,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),WinSetLvs,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //
    //堵转设置
    WinSetBlock = new ConfigBlock(this);
    ui->Desktop->addWidget(WinSetBlock);
    WinSetBlock->setObjectName("堵转");
    connect(WinSetBlock,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),WinSetBlock,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    WinSwitch("WinHome");

    //打开CAN口
    CanThreadInit();
    //打开SQL
    SqlThreadInit();
    //设置风格样式
    emit TransformCmd(ADDR,WIN_CMD_STYLE,NULL);

    Testing = false;
    Pos = 0x13;

    QTimer::singleShot(300,this,SLOT(TestCheck()));
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       界面切换
******************************************************************************/
void MainPage::WinSwitch(QByteArray msg)
{
    if (Testing)
        return;
    int WinCurrent = ui->Desktop->currentIndex();
    if (msg.isNull()) {
        ui->Desktop->setCurrentIndex(WinPrevious.last());
        WinPrevious.removeLast();
        return;
    }
    for (int i=0; i<ui->Desktop->count(); i++) {
        if (ui->Desktop->widget(i)->objectName() == msg) {
            ui->Desktop->setCurrentIndex(i);
            break;
        }
    }
    WinPrevious.append(WinCurrent);
    if (WinPrevious.size()>10) {
        WinPrevious.removeFirst();
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       数据初始化
******************************************************************************/
void MainPage::DatInit()
{
    global = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    FileInUse = global->value("FileInUse","default.ini").toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(FileInUse);
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");

    ItemToTest = set->value("/GLOBAL/ProjToTest","").toString().split(" ");
    PauseMode = set->value("/GLOBAL/TestNG","0").toBool();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.11
 * brief:       启动CAN线程
******************************************************************************/
void MainPage::CanThreadInit()
{
    can = new QThread(this);

    CanCtrl.moveToThread(can);
    connect(can,SIGNAL(started()),&CanCtrl,SLOT(DeviceOpen()));
    connect(can,SIGNAL(finished()),&CanCtrl,SLOT(DeviceQuit()));
    connect(this,SIGNAL(PutCanData(QByteArray)),&CanCtrl,SLOT(WriteAll(QByteArray)));
    connect(&CanCtrl,SIGNAL(GetCanData(QByteArray)),this,SLOT(CanThread(QByteArray)));
    can->start();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       启动SQL线程
******************************************************************************/
void MainPage::SqlThreadInit()
{
    sql = new QThread(this);
    SqlCtrl.moveToThread(sql);
    connect(sql,SIGNAL(started()),&SqlCtrl,SLOT(DeviceOpen()));
    connect(sql,SIGNAL(finished()),&SqlCtrl,SLOT(DeviceQuit()));
    connect(this,SIGNAL(PutSqlData(QByteArray)),&SqlCtrl,SLOT(PutItem(QByteArray)));
    sql->start();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       CAN数据分发
 * date:        2017.01.05
 * brief:       增加启动自检
 * date:        2017.01.11
 * brief:       删除启动自检,改为线程接收
******************************************************************************/
void MainPage::CanThread(QByteArray msg)
{
    if (!msg.isEmpty()) {
        quint16 id;
        quint8 dlc;
        quint8 dat;
        QByteArray cmd;
        QDataStream in(&msg, QIODevice::ReadWrite);
        in.setVersion(QDataStream::Qt_4_8);

        ExcuteCmd(ADDR,WIN_CMD_DEBUG,msg.toHex());
        ExcuteCmd(ADDR,WIN_CMD_DEBUG,"\n");

        while(!in.atEnd()) {
            in >> id >> dlc;
            cmd.clear();
            for (int i=0; i<dlc; i++) {
                in >> dat;
                cmd.append(dat);
            }
            emit TransformCmd(id,CAN_DAT_GET,cmd);
        }
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       命令处理
******************************************************************************/
void MainPage::ExcuteCmd(quint16 addr,quint16 cmd,QByteArray msg)
{
    switch (cmd) {
    case 1000:
        break;
    case WIN_CMD_SWITCH:
        WinSwitch(msg);
        break;
    case WIN_CMD_INIT:
        TestInit();
        break;
    case WIN_CMD_SHOW:
        Items.append(QString(msg).split("\n"));
        break;
    case WIN_CMD_JUDGE:
        TestJudge(msg);
        break;
    case WIN_CMD_ITEM:
        WinTest->ShowItem(msg);
        emit PutSqlData(msg);
        break;
    case WIN_CMD_TEMP:
        WinTest->ShowTemp(msg);
        break;
    case WIN_WAVE_BYTE:
        WinTest->ShowWaveByte(msg);
        break;
    case WIN_WAVE_TEST:
        WinTest->ShowWaveTest(msg);
        break;
    case WIN_WAVE_ITEM:
        WinTest->ShowWaveItem(msg);
        break;
    case WIN_WAVE_HIDE:
        WinTest->WaveClear();
        break;
    case WIN_CMD_DEBUG:
        emit TransformCmd(ADDR,WIN_CMD_DEBUG,msg);
        break;
    case WIN_CMD_WAVE:
        emit TransformCmd(ADDR,WIN_CMD_WAVE,msg);
        break;
    case CAN_DAT_GET:
        break;
    case CAN_DAT_PUT:
        emit PutCanData(msg);
        break;
    case CAN_CMD_READY:
        QTimer::singleShot(10,this,SLOT(TestStart(msg)));
        break;
    case CAN_CMD_START:
        TestStart(msg);
        break;
    case CAN_CMD_STOP:
        Testing = false;
        break;
    default:
        qDebug()<<addr<<cmd<<msg;
        break;
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.03
 * brief:       自检流程
******************************************************************************/
void MainPage::TestCheck()
{
    if (Testing)
        return;
    Testing = true;
    QStringList t = (global->value("ItemEnable","0 1 2 3 4 6").toString()).split(" ");
    for (int i=0; i<t.size(); i++) {
        emit TransformCmd(t.at(i).toInt(),CAN_CMD_CHECK,NULL);
    }
    QStringList s = (global->value("OutEnable","0").toString()).split(" ");
    for (int i=0; i<s.size(); i++) {
        emit TransformCmd(WIN_ID_OUT,CAN_CMD_CHECK,s.at(i).toUtf8());
    }
    Testing = false;
    WinSwitch("WinTest");
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       初始化显示
 * date:        2017.01.05
 * brief:       显示测试数量
******************************************************************************/
void MainPage::TestInit()
{
    Items.clear();//清空项目
    WinTest->WaveClear();//清空波形
    emit TransformCmd(WIN_ID_OUT,CAN_CMD_INIT,NULL);//设定启动方式
    ItemToTest = set->value("/GLOBAL/ProjToTest","").toString().split(" ");
    for (int i=0; i<ItemToTest.size(); i++) {
        emit TransformCmd(ItemToTest.at(i).toInt(),CAN_CMD_INIT,NULL);
    }
    WinTest->ShowItems(Items);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       测试流程
 * date:        2017.01.17
 * brief:       增加报警输出
******************************************************************************/
void MainPage::TestStart(QByteArray data)
{
    if (Testing)
        return;
    if (ui->Desktop->currentWidget()->objectName() != "WinTest")
        return;
    Testing = true;
    ItemJudge = "OK";
    ItemToTest = set->value("/GLOBAL/ProjToTest","").toString().split(" ");
    PauseMode = set->value("/GLOBAL/TestNG","0").toBool();

    TestInit();
    if (data.toInt() == 0x13)
        WinTest->ShowPos(tr("左").toUtf8());
    if (data.toInt() == 0x14)
        WinTest->ShowPos(tr("右").toUtf8());

    QByteArray msg;
    msg.append(0x02 | 0x00);
    emit TransformCmd(ADDR,CAN_CMD_ALARM,msg);

    for (int i=0; i<ItemToTest.size(); i++) {
        emit TransformCmd(ItemToTest.at(i).toInt(),CAN_CMD_START,data);
        if (!Testing)
            break;
    }
    TestSave();
    if (ItemJudge == "NG") {
        msg.clear();
        msg.append(0x08 | 0x01);
        emit TransformCmd(ADDR,CAN_CMD_ALARM,msg);
        Delay(500);
        msg.clear();
        msg.append(0x08 | 0x00);
        emit TransformCmd(ADDR,CAN_CMD_ALARM,msg);
    } else {
        msg.clear();
        msg.append(0x04 | 0x01);
        emit TransformCmd(ADDR,CAN_CMD_ALARM,msg);
        Delay(200);
        msg.clear();
        msg.append(0x04 | 0x00);
        emit TransformCmd(ADDR,CAN_CMD_ALARM,msg);
    }
    WinTest->ShowJudge(ItemJudge);
    Testing = false;
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       测试数据保存
 * date:        2017.01.05
 * brief:       显示测试数量
******************************************************************************/
void MainPage::TestSave()
{
    QStringList s;
    s.append("总数");
    s.append(FileInUse);
    s.append(ItemJudge);
    emit PutSqlData(s.join("@").toUtf8());
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       不合格警告
******************************************************************************/
void MainPage::TestPause()
{
    if(QMessageBox::warning(this,"此项目不合格", "是否继续",QMessageBox::Yes,QMessageBox::No)==QMessageBox::No)
        Testing = false;
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       测试结果判定保存
******************************************************************************/
void MainPage::TestJudge(QByteArray msg)
{
    emit PutSqlData(msg);
    QStringList s = QString(msg).split("@");
    if (s.size() < 3)
        return;
    if (s.at(2) == "NG")
        ItemJudge = "NG";
    if (s.at(2) == "NG" && PauseMode != 1)
        TestPause();
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.30
 * brief:      延时
*******************************************************************************/
void MainPage::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}
