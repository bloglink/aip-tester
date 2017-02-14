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
#ifdef __arm__
    //启用输入法
    QWSInputMethod *p_ipmethod =new IPMethod;
    QWSServer::setCurrentInputMethod(p_ipmethod);
    connect(this,SIGNAL(Serial_SendString(QString)),p_ipmethod,SLOT(Preedit(QString)));
    connect(this,SIGNAL(Serial_SendEnter()),p_ipmethod,SLOT(Confirm()));
    connect(this,SIGNAL(Serial_SendBackspace()),p_ipmethod,SLOT(Backspace()));
    connect(this,SIGNAL(Serial_SendEscape()),p_ipmethod,SLOT(Escape()));
    connect(this,SIGNAL(Serial_Upspace()),p_ipmethod,SLOT(Upspace()));
    connect(this,SIGNAL(Serial_Downspace()),p_ipmethod,SLOT(Downspace()));
    connect(this,SIGNAL(Serial_Tabspace()),p_ipmethod,SLOT(Tabspace()));
    connect(this,SIGNAL(Serial_BackTabspace()),p_ipmethod,SLOT(BackTabspace()));
#endif
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
    //反嵌设置
    WinSetMag = new CWinSetMag(this);
    ui->Desktop->addWidget(WinSetMag);
    WinSetMag->setObjectName("反嵌");
    connect(WinSetMag,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //绝缘设置
    WinSetIr = new CWinSetIr(this);
    ui->Desktop->addWidget(WinSetIr);
    WinSetIr->setObjectName("绝缘");
    connect(WinSetIr,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //交耐设置
    WinSetAcw = new CWinSetAcw(this);
    ui->Desktop->addWidget(WinSetAcw);
    WinSetAcw->setObjectName("交耐");
    connect(WinSetAcw,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //匝间设置
    WinSetImp = new CWinSetImp(this);
    ui->Desktop->addWidget(WinSetImp);
    WinSetImp->setObjectName("匝间");
    connect(WinSetImp,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    //电感设置
    WinSetIndl = new ConfigIndl(this);
    ui->Desktop->addWidget(WinSetIndl);
    WinSetIndl->setObjectName("电感");
    connect(WinSetIndl,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
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
    WinSwitch("堵转");

    //打开CAN口
    CanThreadInit();
    //设置风格样式
    WinSets->SysStyle();

    Testing = false;
    isChecked = false;
    Pos = 0x13;

    QTimer::singleShot(300,this,SLOT(CheckThread()));
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       界面切换
******************************************************************************/
void MainPage::WinSwitch(QByteArray msg)
{
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

    QStringList tt;
    for (int i=0; i<WinData->Amount.size(); i++)
        tt.append(WinData->Amount.at(i)->text());
    tt.removeFirst();
    WinTest->DisplayAmount(tt);
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
    case WIN_CMD_SHOW:
        DisplayInit();
    case WIN_CMD_RESULT:
        DisplayResult();
        break;
    case WIN_CMD_TEMP:
        WinTest->DisplayTemp(msg);
        break;
    case WIN_WAVE_BYTE:
        WinTest->DisplayWaveByte(msg);
        break;
    case WIN_WAVE_TEST:
        WinTest->DisplayWaveTest(msg);
        break;
    case WIN_WAVE_ITEM:
        WinTest->DisplayWaveItem(msg);
        break;
    case WIN_CMD_DEBUG:
        if (ui->Desktop->currentWidget()->objectName() == "WinHome")
            WinHome->ExcuteCmd(msg);
        WinSets->ExcuteCmd(msg);
        break;
    case WIN_CMD_WAVE:
        DisplayWave(msg);
        break;
    case CAN_DAT_GET:
        break;
    case CAN_DAT_PUT:
        emit PutCanData(msg);
        break;
    case CTRL_CMD_START:
        TestThread();
        break;
    case CTRL_CMD_STOP:
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
            switch (id) {
            case CAN_ID_DLR:
                WinSetDcr->ExcuteCmd(cmd);
                WinSetMag->ExcuteCmd(id,cmd);
                break;
            case CAN_ID_DLR_WAVE:
                WinSetMag->ExcuteCmd(id,cmd);
                break;
            case CAN_ID_IR:
                WinSetIr->ExcuteCmd(cmd);
                WinSetAcw->ExcuteCmd(cmd);
                break;
            case CAN_ID_IMP:
            case CAN_ID_IMP_WAVE:
                WinSetImp->ExcuteCmd(id,cmd);
                break;
            case CAN_ID_INDL:
                WinSetIndl->ExcuteCmd(cmd);
                break;
            case CAN_ID_13OUT:
            case CAN_ID_14OUT:
                OutExcuteCmd(id,cmd);
                break;
            default:
                break;
            }
        }
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.03
 * brief:       自检流程
******************************************************************************/
void MainPage::CheckThread()
{
    if (Testing)
        return;
    Testing = true;
    QStringList t = (global->value("ItemEnable","0 1 2 3 4 6").toString()).split(" ");
    if (t.contains("1") || t.contains("2")) {
        WinSetDcr->CmdCheckState();
        if (!WinSetDcr->WaitTestOver()) {
            ExcuteCmd(ADDR,WIN_CMD_DEBUG,"DCR time out\n");
            QMessageBox::warning(this,"警告","电阻板异常",QMessageBox::Ok);
        }
    }
    if (t.contains("3") || t.contains("4")) {
        WinSetIr->CmdCheckState();
        if (!WinSetIr->WaitTestOver()) {
            ExcuteCmd(ADDR,WIN_CMD_DEBUG,"IR time out\n");
            QMessageBox::warning(this,"警告","绝缘板异常",QMessageBox::Ok);
        }
    }
    if (t.contains("6")) {
        WinSetImp->CmdCheckState();
        if (!WinSetImp->WaitTestOver()) {
            ExcuteCmd(ADDR,WIN_CMD_DEBUG,"IMP time out\n");
            QMessageBox::warning(this,"警告","匝间板异常",QMessageBox::Ok);
        }
    }
    t = (global->value("ItemEnable","0").toString()).split(" ");
    if (t.contains("0")) {
        OutTesting = true;
        QByteArray msg;
        QDataStream out(&msg, QIODevice::ReadWrite);
        out.setVersion(QDataStream::Qt_4_8);
        out<<quint16(0x13)<<quint8(0x01)<<quint8(0x00);
        ExcuteCmd(ADDR,CAN_DAT_PUT,msg);
        if (!OutWaitOver()) {
            ExcuteCmd(ADDR,WIN_CMD_DEBUG,"OUT13 time out\n");
            QMessageBox::warning(this,"警告","输出板13异常",QMessageBox::Ok);
        }
    }
    if (t.contains("1")) {
        OutTesting = true;
        QByteArray msg;
        QDataStream out(&msg, QIODevice::ReadWrite);
        out.setVersion(QDataStream::Qt_4_8);
        out<<quint16(0x14)<<quint8(0x01)<<quint8(0x00);
        ExcuteCmd(ADDR,CAN_DAT_PUT,msg);
        if (!OutWaitOver()) {
            ExcuteCmd(ADDR,WIN_CMD_DEBUG,"OUT14 time out\n");
            QMessageBox::warning(this,"警告","输出板14异常",QMessageBox::Ok);
        }
    }
    Testing = false;
    WinSwitch("WinTest");
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       测试流程
 * date:        2017.01.17
 * brief:       增加报警输出
******************************************************************************/
void MainPage::TestThread()
{
    if (Testing)
        return;
    WinSetIr->CmdOnOff(0x02);
    DisplayInit();
    Testing = true;
    ItemToTest = set->value("/GLOBAL/ProjToTest","").toString().split(" ");
    int pause = set->value("/GLOBAL/TestNG","0").toInt();
    for (int i=0; i<ItemToTest.size(); i++) {
        switch (ItemToTest.at(i).toInt()) {
        case 0:
            break;
        case 1:
            WinSetDcr->DisplayInit();
            WinSetDcr->CmdStartTest(Pos);
            if (!WinSetDcr->WaitTestOver())
                ExcuteCmd(ADDR,WIN_CMD_DEBUG,"DCR time out\n");

            if (WinSetDcr->ListJudge.contains("NG") && pause == 0)
                DisplayWarnning();
            break;
        case 2:
            WinTest->WaveClear();
            WinSetMag->DisplayInit();
            WinSetMag->CmdStartTest(Pos);
            WinSetMag->WaitTestOver();
            if (WinSetMag->ListJudge.contains("NG") && pause == 0)
                DisplayWarnning();
            break;
        case 3:
            WinSetIr->DisplayInit();
            WinSetIr->CmdStartTest(Pos);
            WinSetIr->WaitTestOver();
            if (WinSetIr->ListJudge.contains("NG") && pause == 0)
                DisplayWarnning();
            break;
        case 4:
            WinSetAcw->DisplayInit();
            WinSetAcw->CmdStartTest(Pos);
            WinSetAcw->WaitTestOver();
            if (WinSetAcw->ListJudge.contains("NG") && pause == 0)
                DisplayWarnning();
            break;
        case 5:
            break;
        case 6:
            WinTest->WaveClear();
            WinSetImp->DisplayInit();
            WinSetImp->CmdStartTest(Pos);
            WinSetImp->WaitTestOver();
            if (WinSetImp->ListJudge.contains("NG") && pause == 0)
                DisplayWarnning();
            break;
        case 7:
            WinSetIndl->DisplayInit();
            WinSetIndl->CmdStartTest(Pos);
            WinSetIndl->WaitTestOver();
            if (WinSetIndl->ListJudge.contains("NG") && pause == 0)
                DisplayWarnning();
            break;
        default:
            break;
        }
        if (!Testing)
            break;
    }
    TestSave();
    Testing = false;
    QStringList t;
    t.append("1");
    if (WinTest->ListJudge.contains("NG")) {
        WinTest->DisplayState("NG");
        WinSetIr->CmdOnOff(0x09);
        Delay(500);
        WinSetIr->CmdOnOff(0x08);
        t.append("0");
        t.append("1");
    } else {
        WinTest->DisplayState("OK");
        WinSetIr->CmdOnOff(0x05);
        Delay(200);
        WinSetIr->CmdOnOff(0x04);
        t.append("1");
        t.append("0");

    }
    WinTest->DisplayAmount(t);
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
    QStringList item;
    QStringList judge;
    if (!WinTest->ListItem.filter("电阻").isEmpty()) {
        item.append("DCR");
        if (WinSetDcr->ListJudge.contains("NG"))
            judge.append("DCR");
    }
    if (!WinTest->ListItem.filter("反嵌").isEmpty()) {
        item.append("MAG");
        if (WinSetMag->ListJudge.contains("NG"))
            judge.append("MAG");
    }
    if (!WinTest->ListItem.filter("磁旋").isEmpty()) {
        item.append("DIR");
        if (WinSetMag->ListJudge.last() == "NG")
            judge.append("DIR");
    }
    if (!WinTest->ListItem.filter("绝缘").isEmpty()) {
        item.append("INR");
        if (WinSetIr->ListJudge.contains("NG"))
            judge.append("INR");
    }
    if (!WinTest->ListItem.filter("交耐").isEmpty()) {
        item.append("ACW");
        if (WinSetAcw->ListJudge.contains("NG"))
            judge.append("ACW");
    }
    if (!WinTest->ListItem.filter("匝间").isEmpty()) {
        item.append("IMP");
        if (WinSetImp->ListJudge.contains("NG"))
            judge.append("IMP");
    }
    QString type = global->value("FileInUse","default.ini").toString();
    if (judge.isEmpty())
        judge.append("OK");

    WinData->SubmitStart();
    WinData->InsertItem(item.join(" "),type.remove(".ini"),judge.join(" "));
    for (int i=0; i<WinTest->ListItem.size(); i++) {
        QString t1 = WinTest->ListItem.at(i);
        QString t2 = WinTest->ListPara.at(i);
        QString t3 = WinTest->ListResult.at(i);
        QString t4 = WinTest->ListJudge.at(i);
        WinData->InsertRow(t1,t2,t3,t4);
    }
    WinData->SubmitAll();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       初始化显示
 * date:        2017.01.05
 * brief:       显示测试数量
******************************************************************************/
void MainPage::DisplayInit()
{
    QStringList item;
    QStringList para;
    WinTest->WaveClear();
    WinSets->CmdConfigure();

    ItemToTest = set->value("/GLOBAL/ProjToTest","").toString().split(" ");
    for (int i=0; i<ItemToTest.size(); i++) {
        switch (ItemToTest.at(i).toInt()) {
        case 0:
            break;
        case 1:
            WinSetDcr->DisplayInit();
            item.append(WinSetDcr->ListItem);
            para.append(WinSetDcr->ListPara);
            WinSetDcr->CmdConfigure();
            break;
        case 2:
            WinSetMag->DisplayInit();
            item.append(WinSetMag->ListItem);
            para.append(WinSetMag->ListPara);
            WinSetMag->CmdConfigure();
            break;
        case 3:
            WinSetIr->DisplayInit();
            item.append(WinSetIr->ListItem);
            para.append(WinSetIr->ListPara);
            WinSetIr->CmdConfigure();
            break;
        case 4:
            WinSetAcw->DisplayInit();
            item.append(WinSetAcw->ListItem);
            para.append(WinSetAcw->ListPara);
            WinSetAcw->CmdConfigure();
            break;
        case 5:
            break;
        case 6:
            WinSetImp->DisplayInit();
            item.append(WinSetImp->ListItem);
            para.append(WinSetImp->ListPara);
            WinSetImp->CmdConfigure();
            break;
        case 7:
            WinSetIndl->DisplayInit();
            item.append(WinSetIndl->ListItem);
            para.append(WinSetIndl->ListPara);
            //            WinSetIndl->CmdConfigure();
            break;
        default:
            break;
        }
    }
    WinTest->DisplayItem(item);
    WinTest->DisplayPara(para);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       显示测试结果
******************************************************************************/
void MainPage::DisplayResult()
{
    QStringList item;
    QStringList para;
    ItemToTest = set->value("/GLOBAL/ProjToTest","").toString().split(" ");
    for (int i=0; i<ItemToTest.size(); i++) {
        switch (ItemToTest.at(i).toInt()) {
        case 0:
            break;
        case 1:
            item.append(WinSetDcr->ListResult);
            para.append(WinSetDcr->ListJudge);
            break;
        case 2:
            item.append(WinSetMag->ListResult);
            para.append(WinSetMag->ListJudge);
            break;
        case 3:
            item.append(WinSetIr->ListResult);
            para.append(WinSetIr->ListJudge);
            break;
        case 4:
            item.append(WinSetAcw->ListResult);
            para.append(WinSetAcw->ListJudge);
            break;
        case 5:
            break;
        case 6:
            item.append(WinSetImp->ListResult);
            para.append(WinSetImp->ListJudge);
            break;
        case 7:
            item.append(WinSetIndl->ListResult);
            para.append(WinSetIndl->ListJudge);
            break;
        default:
            break;
        }
    }
    WinTest->DisplayResult(item);
    WinTest->DisplayJudge(para);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       显示波形
******************************************************************************/
void MainPage::DisplayWave(QByteArray msg)
{
    WinTest->WaveClear();
    if (msg.contains("反嵌"))
        WinSetMag->DisplayWave(msg);
    if (msg.contains("匝间"))
        WinSetImp->DisplayWave(msg);
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
            WinTest->DisplayPos("左");
            isStop = false;
        }
        if (id == CAN_ID_14OUT) {
            Pos = 0x14;
            WinTest->DisplayPos("右");
            isStop = false;
        }
        QTimer::singleShot(10,this,SLOT(TestThread()));
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
