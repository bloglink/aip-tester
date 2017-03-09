/**
  ******************************************************************************
  * @file    WinHome.cpp
  * @author  link
  * @version 2.0.1.0
  * @date    2017-02-28
  * @brief   Home page of the motor comprehensive tester
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "WinHome.h"
#include "ui_WinHome.h"
/**
  * @brief  Initializes
  * @param  parent:parent widget
  * @retval None
  */
WinHome::WinHome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinHome)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
}
/**
  * @brief  Destruct the window
  * @param  None
  * @retval None
  */
WinHome::~WinHome()
{
    thread_can->quit();
    thread_can->wait();
    thread_sql->quit();
    thread_sql->wait();
    thread_tcp->quit();
    thread_tcp->wait();
    thread_udp->quit();
    thread_udp->wait();
    delete ui;
}

void WinHome::Init()
{
    QTimer *timer = new QTimer(this);
    CanInit();
    SqlInit();
    TcpInit();
    UdpInit();
    timer->singleShot(50,this,SLOT(WinInitAll()));
}
/**
  * @brief  Initializes title version and style
  * @param  None
  * @retval None
  */
void WinHome::WinInit()
{
    ui->titleVn->hide();

    //设置界面风格
    QFile file;
    QString qss;
    file.setFileName(":/source/SoftRainBlue.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);

    Testing = false;
    isCheckOk = false;
}
/**
  * @brief  Initializes all window used
  * @param  None
  * @retval None
  */
void WinHome::WinInitAll()
{
    qDebug()<<QTime::currentTime().toString()<<"初始化所有窗口";

    TestText("Initialize WinBack\n");
    WinBack *winBack = new WinBack(this);
    ui->desktop->addWidget(winBack);
    winBack->setObjectName("WinBack");
    connect(winBack,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize WinBack OK\n");

    TestText("Initialize WinSyst\n");
    WinSyst *winSyst = new WinSyst(this);
    ui->desktop->addWidget(winSyst);
    winSyst->setObjectName("WinSyst");
    connect(winSyst,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(SendMessage(quint16,quint16,QByteArray)),winSyst,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize WinSyst OK\n");

    TestText("Initialize WinType\n");
    WinType *winType = new WinType(this);
    ui->desktop->addWidget(winType);
    winType->setObjectName("WinType");
    connect(winType,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize WinType OK\n");

    TestText("Initialize WinData\n");
    WinData *winData = new WinData(this);
    ui->desktop->addWidget(winData);
    winData->setObjectName("WinData");
    connect(winData,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize WinData OK\n");

    TestText("Initialize WinTest\n");
    WinTest *winTest = new WinTest(this);
    ui->desktop->addWidget(winTest);
    winTest->setObjectName("WinTest");
    connect(winTest,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(SendMessage(quint16,quint16,QByteArray)),winTest,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize WinTest OK\n");

    TestText("Initialize PageDcr\n");
    PageDcr *pageDcr = new PageDcr(this);
    ui->desktop->addWidget(pageDcr);
    pageDcr->setObjectName("PageDcr");
    connect(pageDcr,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(SendMessage(quint16,quint16,QByteArray)),pageDcr,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize PageDcr OK\n");

    TestText("Initialize PageMag\n");
    PageMag *pageMag = new PageMag(this);
    ui->desktop->addWidget(pageMag);
    pageMag->setObjectName("PageMag");
    connect(pageMag,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(SendMessage(quint16,quint16,QByteArray)),pageMag,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize PageMag OK\n");

    TestText("Initialize PageInr\n");
    PageInr *pageInr = new PageInr(this);
    ui->desktop->addWidget(pageInr);
    pageInr->setObjectName("PageInr");
    connect(pageInr,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(SendMessage(quint16,quint16,QByteArray)),pageInr,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize PageInr OK\n");

    TestText("Initialize PageAcw\n");
    PageAcw *pageAcw = new PageAcw(this);
    ui->desktop->addWidget(pageAcw);
    pageAcw->setObjectName("PageAcw");
    connect(pageAcw,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(SendMessage(quint16,quint16,QByteArray)),pageAcw,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize PageAcw OK\n");

    TestText("Initialize PageImp\n");
    PageImp *pageImp = new PageImp(this);
    ui->desktop->addWidget(pageImp);
    pageImp->setObjectName("PageImp");
    connect(pageImp,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(SendMessage(quint16,quint16,QByteArray)),pageImp,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize PageImp OK\n");

    TestText("Initialize PageInd\n");
    PageInd *pageInd = new PageInd(this);
    ui->desktop->addWidget(pageInd);
    pageInd->setObjectName("PageInd");
    connect(pageInd,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(SendMessage(quint16,quint16,QByteArray)),pageInd,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize PageInd OK\n");

    TestText("Initialize PagePwr\n");
    PagePwr *pagePwr = new PagePwr(this);
    ui->desktop->addWidget(pagePwr);
    pagePwr->setObjectName("PagePwr");
    connect(pagePwr,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(SendMessage(quint16,quint16,QByteArray)),pagePwr,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize PagePwr OK\n");

    TestText("Initialize PageLvs\n");
    PageLvs *pageLvs = new PageLvs(this);
    ui->desktop->addWidget(pageLvs);
    pageLvs->setObjectName("PageLvs");
    connect(pageLvs,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(SendMessage(quint16,quint16,QByteArray)),pageLvs,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize PageLvs OK\n");

    TestText("Initialize PageLck\n");
    PageLck *pageLck = new PageLck(this);
    ui->desktop->addWidget(pageLck);
    pageLck->setObjectName("PageLck");
    connect(pageLck,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(SendMessage(quint16,quint16,QByteArray)),pageLck,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize PageLck OK\n");

    TestText("Initialize PageOut\n");
    PageOut *pageOut = new PageOut(this);
    ui->desktop->addWidget(pageOut);
    pageOut->setObjectName("PageOut");
    connect(pageOut,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(SendMessage(quint16,quint16,QByteArray)),pageOut,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    TestText("Initialize PageOut OK\n");

    qDebug()<<QTime::currentTime().toString()<<"初始化所有窗口OK";

    TestCheck();

}
/**
  * @brief  Jump to another window
  * @param  msg:window name
  * @retval None
  */
void WinHome::WinJump(QByteArray win)
{
    int WinCurrent = ui->desktop->currentIndex();
    if (win.isNull()) { //空代表返回
        ui->desktop->setCurrentIndex(previous_window.last());
        previous_window.removeLast();
        return;
    }
    for (int i=0; i<ui->desktop->count(); i++) {
        if (ui->desktop->widget(i)->objectName() == win) {
            ui->desktop->setCurrentIndex(i);
            break;
        }
    }
    previous_window.append(WinCurrent);
    if (previous_window.size()>10) { //最大嵌套10层
        previous_window.removeFirst();
    }
}
/**
  * @brief  Initializes the buttons
  * @param  None
  * @retval None
  */
void WinHome::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->btnSyst,Qt::Key_1);
    btnGroup->addButton(ui->btnType,Qt::Key_2);
    btnGroup->addButton(ui->btnData,Qt::Key_3);
    btnGroup->addButton(ui->btnTest,Qt::Key_4);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/**
  * @brief  Button functions
  * @param  id:button id
  * @retval None
  */
void WinHome::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_1:
        WinJump("WinSyst");
        break;
    case Qt::Key_2:
        WinJump("WinType");
        break;
    case Qt::Key_3:
        WinJump("WinData");
        break;
    case Qt::Key_4:
        WinJump("WinTest");
        break;
    case Qt::Key_5:
        QApplication::closeAllWindows();
        break;
    }
}
/**
  * @brief  Initializes data
  * @param  None
  * @retval None
  */
void WinHome::DatInit()
{
    QString v = "V-2.1.0.2";
    QSettings *global = new QSettings(INI_PATH,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    global->setValue("Version",v);
    this->setWindowTitle(QString("电机综合测试仪%1").arg(v));
    ui->titleVn->setText(v);
}
/**
  * @brief  Initializes can thread
  * @param  None
  * @retval None
  */
void WinHome::CanInit()
{
    thread_can = new QThread(this);
    can.moveToThread(thread_can);
    connect(thread_can,SIGNAL(started()),&can,SLOT(DeviceOpen()));
    connect(thread_can,SIGNAL(finished()),&can,SLOT(DeviceQuit()));
    connect(this,SIGNAL(PutCanData(QByteArray)),&can,SLOT(WriteAll(QByteArray)));
    connect(&can,SIGNAL(GetCanData(QByteArray)),this,SLOT(CanThread(QByteArray)));
    thread_can->start();
}
/**
  * @brief  Initializes sql thread
  * @param  None
  * @retval None
  */
void WinHome::SqlInit()
{
    thread_sql = new QThread(this);
    sql.moveToThread(thread_sql);
    connect(thread_sql,SIGNAL(started()),&sql,SLOT(DeviceOpen()));
    connect(thread_sql,SIGNAL(finished()),&sql,SLOT(DeviceQuit()));
    connect(this,SIGNAL(WriteSql(QByteArray)),&sql,SLOT(Write(QByteArray)));
    thread_sql->start();
}
/**
  * @brief  Initializes tcp thread
  * @param  None
  * @retval None
  */
void WinHome::TcpInit()
{
    thread_tcp = new QThread(this);
    tcp.moveToThread(thread_tcp);
    connect(thread_tcp,SIGNAL(started()),&tcp,SLOT(TcpInit()));
    connect(thread_tcp,SIGNAL(finished()),&tcp,SLOT(TcpQuit()));
    connect(&tcp,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    thread_tcp->start();
}

void WinHome::UdpInit()
{
    thread_udp = new QThread(this);
    udp.moveToThread(thread_udp);
    connect(thread_udp,SIGNAL(started()),&udp,SLOT(UdpInit()));
    connect(thread_udp,SIGNAL(finished()),&udp,SLOT(UdpQuit()));
    connect(&udp,SIGNAL(SendMessage(quint16,quint16,QByteArray)),this,
            SLOT(ReadMessage(quint16,quint16,QByteArray)));
    thread_udp->start();
}
/**
  * @brief  Can data read
  * @param  msg:can data
  * @retval None
  */
void WinHome::CanThread(QByteArray msg)
{
    if (!msg.isEmpty()) {
        quint16 id;
        quint8 dlc;
        quint8 dat;
        QByteArray cmd;
        QDataStream in(&msg, QIODevice::ReadWrite);
        in.setVersion(QDataStream::Qt_4_8);

        while(!in.atEnd()) {
            in >> id >> dlc;
            cmd.clear();
            for (int i=0; i<dlc; i++) {
                in >> dat;
                cmd.append(dat);
            }
            emit SendMessage(id,CMD_CAN,cmd);
        }
    }
}
/**
  * @brief  Excute command
  * @param  addr:target address;cmd:command to excute;msg:command param
  * @retval None
  */
void WinHome::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    switch (cmd) {
    case CMD_JUMP:
        WinJump(msg);
        break;
    case CMD_INIT:
        TestInit();
        break;
    case CMD_STATUS:
        TestCheck();
        break;
    case CMD_INIT_ITEM:
        Items.append(QString(msg).split("\n"));
        break;
    case CMD_JUDGE:
        TestSaveJudge(msg);
        break;
    case CMD_ITEM:
        emit WriteSql(msg);
    case CMD_TEMP:
    case CMD_WAVE_BYTE:
    case CMD_WAVE_TEST:
    case CMD_WAVE_ITEM:
    case CMD_WAVE_HIDE:
        emit SendMessage(WIN_ID_TEST,cmd,msg);
        break;
    case CMD_DEBUG:
        TestText(msg);
        break;
    case CMD_WAVE:
        emit SendMessage(ADDR,CMD_WAVE,msg);
        break;
    case CMD_CAN:
        emit PutCanData(msg);
        break;
    case CMD_START:
        TestStart(msg);
        break;
    case CMD_STOP:
        emit SendMessage(ADDR,CMD_STOP,msg);
        Testing = false;
        break;
    case CMD_NET:
        ui->IconNet->setPixmap(QPixmap(":/source/wifi.png"));
        break;
    case CMD_SQL:
        ui->IconNet->setPixmap(QPixmap(":/source/radio_1.png"));
        break;
    default:
        qDebug()<<addr<<cmd<<msg;
        break;
    }
}
/**
  * @brief  Initializes before test
  * @param  None
  * @retval None
  */
void WinHome::TestInit()
{
    qDebug()<<QTime::currentTime().toString()<<"初始化测试";

    Items.clear();
    //全局配置
    QSettings *settings_g = new QSettings(INI_PATH,QSettings::IniFormat);
    settings_g->setIniCodec("GB18030");
    settings_g->beginGroup("GLOBAL");
    motor_type = settings_g->value("FileInUse",INI_DEFAULT).toString();
    motor_type.remove(".ini");
    //当前配置
    QString n = QString("./config/%1.ini").arg(motor_type);
    QSettings *settings_c = new QSettings(n,QSettings::IniFormat);
    settings_c->setIniCodec("GB18030");

    ItemToTest = settings_c->value("/GLOBAL/ProjToTest","1").toString().split(" ");
    PauseMode = settings_c->value("/GLOBAL/TestNG","1").toInt();

    emit SendMessage(WIN_ID_OUT13,CMD_INIT,NULL);//设定启动方式

    if (ItemToTest.isEmpty())
        return;

    for (int i=0; i<ItemToTest.size(); i++) {
        emit SendMessage(ItemToTest.at(i).toInt(),CMD_INIT,NULL);
    }
    emit SendMessage(WIN_ID_TEST,CMD_INIT,Items.join("\n").toUtf8());//初始化测试界面

    qDebug()<<QTime::currentTime().toString()<<"初始化测试OK";
}
/**
  * @brief  Check the board status
  * @param  None
  * @retval None
  */
void WinHome::TestCheck()
{
    qDebug()<<QTime::currentTime().toString()<<"开机自检";
    if (Testing)
        return;
    Testing = true;

    //全局配置
    QSettings *settings_g = new QSettings(INI_PATH,QSettings::IniFormat);
    settings_g->setIniCodec("GB18030");
    settings_g->beginGroup("GLOBAL");

    QStringList t = (settings_g->value("ItemEnable","0 1 2 3 4 6").toString()).split(" ");
    for (int i=0; i<t.size(); i++) {
        emit SendMessage(t.at(i).toInt(),CMD_CHECK,NULL);
    }
    QStringList s = (settings_g->value("OutEnable","0").toString()).split(" ");
    for (int i=0; i<s.size(); i++) {
        emit SendMessage(WIN_ID_OUT13,CMD_CHECK,s.at(i).toUtf8());
    }
    qDebug()<<QTime::currentTime().toString()<<"开机自检OK";

    ui->titleVn->show();
    ui->Text->hide();

    Testing = false;
    if (!isCheckOk) {
        isCheckOk = true;
        Delay(1000);
        WinJump("WinTest");
    }
}
/**
  * @brief  Test thread
  * @param  data:station to test
  * @retval None
  */
void WinHome::TestStart(QByteArray station)
{
    WaitTestOver(100);
    if (Testing)
        return;
    if (ui->desktop->currentWidget()->objectName() != "WinTest")
        return;
    Testing = true;
    ItemJudge = "OK";

    TestInit();

    emit SendMessage(WIN_ID_TEST,CMD_START,station);

    QByteArray msg;
    msg.append(0x02 | 0x00);
    emit SendMessage(ADDR,CMD_ALARM,msg);

    for (int i=0; i<ItemToTest.size(); i++) {
        emit SendMessage(ItemToTest.at(i).toInt(),CMD_START,station);
        if (!Testing)
            break;
    }
    TestSaveItem();
    if (ItemJudge == "NG") {
        msg.clear();
        msg.append(0x08 | 0x01);
        emit SendMessage(ADDR,CMD_ALARM,msg);
        Delay(500);
        msg.clear();
        msg.append(0x08 | 0x00);
        emit SendMessage(ADDR,CMD_ALARM,msg);
    } else {
        msg.clear();
        msg.append(0x04 | 0x01);
        emit SendMessage(ADDR,CMD_ALARM,msg);
        Delay(200);
        msg.clear();
        msg.append(0x04 | 0x00);
        emit SendMessage(ADDR,CMD_ALARM,msg);
    }
    emit SendMessage(WIN_ID_TEST,CMD_JUDGE,ItemJudge.toUtf8());
    Testing = false;
}
/**
  * @brief  Save test data
  * @param  None
  * @retval None
  */
void WinHome::TestSaveItem()
{
    QStringList s;
    s.append("总数");
    s.append(motor_type);
    s.append(ItemJudge);
    emit WriteSql(s.join("@").toUtf8());
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       测试结果判定保存
******************************************************************************/
void WinHome::TestSaveJudge(QByteArray msg)
{
    emit WriteSql(msg);
    QStringList s = QString(msg).split("@");
    if (s.size() < 3)
        return;
    if (s.at(2) == "NG")
        ItemJudge = "NG";
    if (s.at(2) == "NG" && PauseMode != 1)
        TestPause();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       不合格警告
******************************************************************************/
void WinHome::TestPause()
{
    if(QMessageBox::warning(this,"此项目不合格", "是否继续",
                            QMessageBox::Yes,QMessageBox::No)==QMessageBox::No)
        Testing = false;
}

void WinHome::TestText(QByteArray msg)
{
    if (!ui->Text->isHidden()) {
        ui->Text->insertPlainText(msg);
        ui->Text->moveCursor(QTextCursor::EndOfBlock);
        Delay(1);
    }
    emit SendMessage(ADDR,CMD_DEBUG,msg);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.18
 * brief:      等待测试结束
*******************************************************************************/
bool WinHome::WaitTestOver(quint16 t)
{
    int TimeOut = 0;
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
 * date:       2016.12.30
 * brief:      延时
*******************************************************************************/
void WinHome::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

void WinHome::showEvent(QShowEvent *)
{
    if (!isCheckOk) {
        Init();
    }
}
/*********************************END OF FILE**********************************/
