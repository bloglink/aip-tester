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
    //低启设置
    WinSetLvs = new ConfigLvs(this);
    ui->Desktop->addWidget(WinSetLvs);
    WinSetLvs->setObjectName("低启");
    connect(WinSetLvs,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //
    //堵转设置
    WinSetBlock = new ConfigBlock(this);
    ui->Desktop->addWidget(WinSetBlock);
    WinSetBlock->setObjectName("堵转");
    connect(WinSetBlock,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    WinSwitch("WinHome");

    //打开CAN口
    CanThreadInit();
    //设置风格样式
    emit TransformCmd(ADDR,WIN_CMD_STYLE,NULL);

    Testing = false;
    isChecked = false;
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

    //当前使用的测试项目
    QString t = QString("./config/%1").arg(global->value("FileInUse","default.ini").toString());
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");

    ItemToTest = set->value("/GLOBAL/ProjToTest","").toString().split(" ");
    isPause = set->value("/GLOBAL/TestNG","false").toBool();

    QStringList tt;
    for (int i=0; i<WinData->Amount.size(); i++)
        tt.append(WinData->Amount.at(i)->text());
    tt.removeFirst();
    WinTest->ShowAmount(tt);
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
    thread.moveToThread(can);
    connect(can,SIGNAL(started()),&thread,SLOT(DeviceOpen()));
    connect(can,SIGNAL(finished()),&thread,SLOT(DeviceQuit()));
    connect(this,SIGNAL(PutCanData(QByteArray)),&thread,SLOT(WriteAll(QByteArray)));
    connect(&thread,SIGNAL(GetCanData(QByteArray)),this,SLOT(CanThread(QByteArray)));
    can->start();
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
        TestJudge = "NG";
        ItemJudge = "NG";
        break;
    case WIN_CMD_ITEM:
        WinTest->ShowItem(msg);
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
    TestJudge = "OK";
    ItemToTest = set->value("/GLOBAL/ProjToTest","").toString().split(" ");
    isPause = set->value("/GLOBAL/TestNG","false").toBool();

    TestInit();
    if (data.toInt() == 0x13)
        WinTest->ShowPos(tr("左").toUtf8());
    if (data.toInt() == 0x14)
        WinTest->ShowPos(tr("右").toUtf8());

    QByteArray msg;
    msg.append(0x02 | 0x00);
    emit TransformCmd(ADDR,CAN_CMD_ALARM,msg);

    for (int i=0; i<ItemToTest.size(); i++) {
        if (ItemJudge == "NG" && isPause)
            qDebug()<<"pause";
        ItemJudge = "OK";
        emit TransformCmd(ItemToTest.at(i).toInt(),CAN_CMD_START,data);
        if (!Testing)
            break;

    }
    TestSave();
    if (TestJudge == "NG") {
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
    WinTest->ShowJudge(TestJudge);
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
    //    QStringList item;
    //    QStringList judge;
    //    if (!WinTest->ListItem.filter("电阻").isEmpty()) {
    //        item.append("DCR");
    //        if (WinSetDcr->ListJudge.contains("NG"))
    //            judge.append("DCR");
    //    }
    //    if (!WinTest->ListItem.filter("反嵌").isEmpty()) {
    //        item.append("MAG");
    //        if (WinSetMag->ListJudge.contains("NG"))
    //            judge.append("MAG");
    //    }
    //    if (!WinTest->ListItem.filter("磁旋").isEmpty()) {
    //        item.append("DIR");
    //        if (WinSetMag->ListJudge.last() == "NG")
    //            judge.append("DIR");
    //    }
    //    if (!WinTest->ListItem.filter("绝缘").isEmpty()) {
    //        item.append("INR");
    //        if (WinSetIr->ListJudge.contains("NG"))
    //            judge.append("INR");
    //    }
    //    if (!WinTest->ListItem.filter("交耐").isEmpty()) {
    //        item.append("ACW");
    //        if (WinSetAcw->ListJudge.contains("NG"))
    //            judge.append("ACW");
    //    }
    //    if (!WinTest->ListItem.filter("匝间").isEmpty()) {
    //        item.append("IMP");
    //        if (WinSetImp->ListJudge.contains("NG"))
    //            judge.append("IMP");
    //    }
    //    QString type = global->value("FileInUse","default.ini").toString();
    //    if (judge.isEmpty())
    //        judge.append("OK");

    //    WinData->SubmitStart();
    //    WinData->InsertItem(item.join(" "),type.remove(".ini"),judge.join(" "));
    //    for (int i=0; i<WinTest->ListItem.size(); i++) {
    //        QString t1 = WinTest->ListItem.at(i);
    //        QString t2 = WinTest->ListPara.at(i);
    //        QString t3 = WinTest->ListResult.at(i);
    //        QString t4 = WinTest->ListJudge.at(i);
    //        WinData->InsertRow(t1,t2,t3,t4);
    //    }
    //    WinData->SubmitAll();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       不合格警告
******************************************************************************/
void MainPage::DisplayWarnning()
{
    if(QMessageBox::warning(this,"此项目不合格", "是否继续",QMessageBox::Yes,QMessageBox::No)==QMessageBox::No)
        Testing = false;
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       输出板信号处理
******************************************************************************/
void MainPage::OutExcuteCmd(quint16 id,QByteArray msg)
{
    if (quint8(msg.at(0)) == 0x00) {
        OutTesting = false;
    }
    if (quint8(msg.at(0)) == 0x01 && quint8(msg.at(1) == 0x01)) {
        if (!isStop || Testing || ui->Desktop->currentWidget()->objectName() != "WinTest")
            return;
        if (id == CAN_ID_13OUT) {
            Pos = 0x13;
            WinTest->ShowPos("左");
            isStop = false;
        }
        if (id == CAN_ID_14OUT) {
            Pos = 0x14;
            WinTest->ShowPos("右");
            isStop = false;
        }
        QTimer::singleShot(10,this,SLOT(TestStart()));
    }
    if (quint8(msg.at(0)) == 0x01 && quint8(msg.at(1) == 0x00)) {
        if (id == CAN_ID_13OUT && Pos == 0x13) {
            isStop = true;
            Testing = false;
        }
        if (id == CAN_ID_14OUT && Pos == 0x14) {
            isStop = true;
            Testing = false;
        }
    }
}

void MainPage::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

bool MainPage::OutWaitOver()
{
    quint8 TimeOut = 0;
    while (OutTesting) {
        Delay(10);
        TimeOut++;
        if (TimeOut > 50) {
            OutTesting = false;
            return false;
        }
    }
    return true;
}
