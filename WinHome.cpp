/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170422
 * author:      zhaonanlin
 * brief:       开机主页
*******************************************************************************/
#include "WinHome.h"
#include "ui_WinHome.h"

WinHome::WinHome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinHome)
{
    ui->setupUi(this);
    initUI("V-2.1.1.0");
    HomeMode = HOME_FREE;
    isPause = false;
    station = 0x13;
}

WinHome::~WinHome()
{
    thread_sql->quit();
    thread_sql->wait();
    thread_tcp->quit();
    thread_tcp->wait();
    thread_udp->quit();
    thread_udp->wait();
    thread_com->quit();
    thread_com->wait();
    delete ui;
}

bool WinHome::login()
{
    Login login;
    connect(&login, SIGNAL(sendJson(QJsonObject)), this, SLOT(initUdp(QJsonObject)));
    connect(this, SIGNAL(sendNetMsg(QString)), &login, SLOT(recvNetMsg(QString)));
    return login.exec();
}

void WinHome::initUI(QString v)
{
    ui->keybord->setCurrentIndex(0);
    //设置界面风格
    QFile file;
    QString qss;
    file.setFileName(":/source/SoftRainBlue.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);

    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->btnSyst, Qt::Key_1);
    btnGroup->addButton(ui->btnType, Qt::Key_2);
    btnGroup->addButton(ui->btnData, Qt::Key_3);
    btnGroup->addButton(ui->btnTest, Qt::Key_4);
    btnGroup->addButton(ui->btnQuit, Qt::Key_5);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(readButtons(int)));

    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    ini->setValue("/GLOBAL/Version", v);
    this->setWindowTitle(QString("电机综合测试仪%1").arg(v));
    ui->titleVn->setText(v);
}

void WinHome::readButtons(int id)
{
    switch (id) {
    case Qt::Key_1:
        JumpToWindow("WinSyst");
        break;
    case Qt::Key_2:
        JumpToWindow("WinType");
        break;
    case Qt::Key_3:
        JumpToWindow("WinData");
        break;
    case Qt::Key_4:
        JumpToWindow("WinTest");
        break;
    case Qt::Key_5:
        QApplication::closeAllWindows();
        break;
    }
}

void WinHome::initUdp(QJsonObject obj)
{
    QString host = obj.value("host_addr").toString();
    quint16 port = obj.value("host_port").toString().toInt();
    udp.initSocket(host, port);
    connect(this, SIGNAL(transmitJson(QJsonObject)), &udp, SLOT(recvAppJson(QJsonObject)));
    thread_udp = new QThread(this);
    udp.moveToThread(thread_udp);
    thread_udp->start();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(regularTasks()));
    timer->start(5);

    emit transmitJson(obj);

    initCom();
    InitSql();
    InitTcp();

    QTimer::singleShot(1000, this, SLOT(InitWindowsAll()));
}

void WinHome::initCom()
{
    thread_com = new QThread(this);
    com.moveToThread(thread_com);
    connect(thread_com, SIGNAL(started()), &com, SLOT(initCom()));
    thread_com->start();
}

void WinHome::testThread()
{

}

void WinHome::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void WinHome::testInit()
{
    winTest->initItems(station);
    QJsonObject obj;
    obj.insert("TxMessage",QString("6020 %1").arg(station));
    emit transmitJson(obj);

    //    com.send_IO(station, Y10);
    //    readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);

    QStringList testItems = CurrentItems();
    for (int i=0; i < testItems.size(); i++) {
        int cmd = testItems.at(i).toInt();
        status = cmd;
        switch (cmd) {
        case STATUS_DCR:
            testDCR();
            break;
        case STATUS_INR:
            testINR();
            break;
        case STATUS_ACW:
            testACW();
            break;
        case STATUS_IND:
            testIND();
            break;
        case STATUS_HAL:
            testHAL();
            break;
        case STATUS_NLD:
            testNLD();
            break;
        case STATUS_LOD:
            testLOD();
            break;
        default:
            break;
        }
        if (status == STATUS_OVER) {
            QMessageBox::warning(this, "警告", "测试停止", QMessageBox::Ok);
            break;
        }
    }
    if (winTest->updateResult()) {
        com.send_IO(station, Y11 | Y08);  // 绿灯加蜂鸣器
        wait(500);
        com.send_IO(station, Y11);  // 绿灯
    } else {
        com.send_IO(station, Y09 | Y08);  // 红灯加蜂鸣器
        wait(1500);
        com.send_IO(station, Y09);  // 红灯
    }
    status = STATUS_FREE;
}

void WinHome::testDCR()
{
//    com.send_IO(station, Y10);
//    readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);

    QJsonObject obj;
    obj.insert("TxMessage","6006 DCR");
    emit transmitJson(obj);
    waitTimeOut(STATUS_DCR);

//    com.send_IO(station, Y10);
//    readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);
}

void WinHome::testINR()
{
    //    bool cylinder = false;
    //    com.send_IO(station, Y10);
    //    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);
    //    if (!cylinder) {
    //        status = STATUS_OVER;
    //        return;
    //    }
    //    wait(100);
    //    com.send_IO(station, Y03 | Y10);
    //    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_TARGET);
    //    wait(100);

    QJsonObject obj;
    obj.insert("TxMessage","6006 IR");
    emit transmitJson(obj);
    waitTimeOut(STATUS_INR);

    //    com.send_IO(station, Y10);
    //    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);
    //    if (!cylinder) {
    //        status = STATUS_OVER;
    //        return;
    //    }
    //    wait(500);
}

void WinHome::testACW()
{
//    bool cylinder = false;
//    com.send_IO(station, Y10);  // 气缸全部归位
//    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);
//    if (!cylinder) {
//        status = STATUS_OVER;
//        return;
//    }
//    wait(100);
//    com.send_IO(station, Y03 | Y10);  // 气缸4动作
//    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_TARGET);
//    if (!cylinder) {
//        status = STATUS_OVER;
//        return;
//    }
//    wait(100);

    QJsonObject obj;
    obj.insert("TxMessage","6006 ACW");
    emit transmitJson(obj);
    waitTimeOut(STATUS_ACW);

//    com.send_IO(station, Y10);  // 气缸全部归位
//    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);
//    if (!cylinder) {
//        status = STATUS_OVER;
//        return;
//    }
//    wait(500);
}

void WinHome::testIND()
{
    bool cylinder = false;
    com.send_IO(station, Y10);  // 气缸全部归位
    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);

    QJsonObject obj;
    obj.insert("TxMessage","6006 IND");
    emit transmitJson(obj);
    waitTimeOut(STATUS_INR);

    com.send_IO(station, Y10);  // 气缸全部归位
    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(500);
}

void WinHome::testHAL()
{
    QJsonObject obj;
    obj.insert("TxMessage","6006 HALL");
    emit transmitJson(obj);
    waitTimeOut(STATUS_HAL);
}

void WinHome::testNLD()
{
    //    bool cylinder = false;
    //    com.send_IO(station, Y10);  // 气缸全部归位
    //    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);
    //    if (!cylinder) {
    //        status = STATUS_OVER;
    //        return;
    //    }
    //    wait(100);

    //    com.send_IO(station, Y02 | Y10);  // 气缸3压紧
    //    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_TARGET | X04_ORIGIN);
    //    if (!cylinder) {
    //        status = STATUS_OVER;
    //        return;
    //    }
    //    wait(100);

    QJsonObject obj;
    obj.insert("TxMessage","6006 NOLAOD");
    emit transmitJson(obj);
    waitTimeOut(STATUS_NLD);

    //    com.send_IO(station, Y10);  // 气缸全部归位
    //    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);
    //    if (!cylinder) {
    //        status = STATUS_OVER;
    //        return;
    //    }
    //    wait(100);
}

void WinHome::testLOD()
{
    bool cylinder = false;

    com.send_IO(station, Y10);  // 气缸全部归位
    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);

    com.send_IO(station, Y00 | Y10);  // 气缸1上升
    cylinder = readCylinderL(X01_TARGET | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);

    com.send_IO(station, Y00 | Y01 | Y10);  // 气缸2夹紧
    cylinder = readCylinderL(X01_TARGET | X02_TARGET | X03_ORIGIN | X04_ORIGIN);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);

    com.send_IO(station, Y00 | Y01 | Y02 | Y10);  // 气缸3压紧
    cylinder = readCylinderL(X01_TARGET | X02_TARGET | X03_TARGET | X04_ORIGIN);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);

    QJsonObject obj;
    obj.insert("TxMessage","6006 LOAD");
    emit transmitJson(obj);
    wait(1500);

    com.pre_speed();
    int load = 1*2446;

    for (int i=0; i < 11; i++) {
        com.add_speed(load/10*i);
        wait(100);
    }
    com.readPlc();
    quint16 speed = com.speed;
    quint16 torque = com.torque;
    QString s = QString("转速:%1,转矩:%2\n").arg(speed).arg(torque);
    QMessageBox::warning(this, "伺服", s, QMessageBox::Ok);
    wait(1500);
    for (int i=0; i < 11; i++) {
        com.add_speed(load/10*(10-i));
        wait(100);
    }

    wait(1500);
    com.end_speed();
    wait(100);

    com.send_IO(station, Y00 | Y02 | Y10);  // 气缸2松开
    cylinder = readCylinderL(X01_TARGET | X02_ORIGIN | X03_TARGET | X04_ORIGIN);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);

    com.send_IO(station, Y02 | Y10);  // 气缸1归位
    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_TARGET | X04_ORIGIN);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);

    com.send_IO(station, Y10);  // 气缸全部归位
    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);
}

void WinHome::testStop()
{
    QJsonObject obj;
    obj.insert("TxMessage",QString("6022 %1").arg(station));
    emit transmitJson(obj);
    status = STATUS_OVER;
}

void WinHome::testStopAction()
{
    bool cylinder = false;
    com.send_IO(station, Y00 | Y02 | Y10);  // 气缸2松开
    cylinder = readCylinderL(X01_TARGET | X02_ORIGIN | X03_TARGET | X04_ORIGIN);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);

    com.send_IO(station, Y02 | Y10);  // 气缸1归位
    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_TARGET | X04_ORIGIN);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);

    com.send_IO(station, Y10);  // 气缸全部归位
    cylinder = readCylinderL(X01_ORIGIN | X02_ORIGIN | X03_ORIGIN | X04_ORIGIN);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);
}

void WinHome::testTimeOut()
{
    QJsonObject obj;
    obj.insert("TxMessage",QString("6026"));
    emit transmitJson(obj);
    status = STATUS_OVER;
}

void WinHome::recvIOMsg(QString msg)
{
    if (msg == "StartL") {
        if (status != STATUS_FREE)
            return;
        status = STATUS_PREP;
        station = 0x13;
        QTimer::singleShot(10, this, SLOT(testThread()));
    }
    if (msg == "StartR") {
        if (status != STATUS_FREE)
            return;
        status = STATUS_PREP;
        station = 0x14;
        QTimer::singleShot(10, this, SLOT(testThread()));
    }
    if (msg == "StopL" || msg == "StopR") {
        testStop();
        testTimeOut();
    }
}

bool WinHome::readCylinderL(quint16 s)
{
    quint16 timeOut = 0x0000;
    while (1) {
        if ((station == 0x13) && (com.hexL & 0xFF00) == s)
            return true;
        if ((station == 0x14) && (com.hexR & 0xFF00) == s)
            return true;
        wait(10);
        timeOut++;
        if (timeOut > 500) {
            QString temp = QString("气缸到位超时:%1!=%2").
                    arg(s, 4, 16, QChar('0')).arg(com.hexL, 4, 16, QChar('0'));
            QMessageBox::warning(this, "气缸", temp, QMessageBox::Ok);
            return false;
        }
    }
}

bool WinHome::waitTimeOut(quint16 s)
{
    status = s;
    timeOut = 0;
    while (1) {
        wait(10);
        timeOut++;
        if (timeOut > 2000) {
            QMessageBox::warning(this, "超时", QString("测试超时:%1").arg(s), QMessageBox::Ok);
            testTimeOut();
            return false;
        }
        if (status != s)
            return true;
    }
}

void WinHome::InitSql()
{
    thread_sql = new QThread(this);
    sql.moveToThread(thread_sql);
    connect(thread_sql, SIGNAL(started()), &sql, SLOT(DeviceOpen()));
    connect(thread_sql, SIGNAL(finished()), &sql, SLOT(DeviceQuit()));
    connect(this, SIGNAL(WriteSql(QByteArray)), &sql, SLOT(Write(QByteArray)));
    thread_sql->start();
}

void WinHome::InitTcp()
{
    thread_tcp = new QThread(this);
    tcp.moveToThread(thread_tcp);
    connect(thread_tcp, SIGNAL(started()), &tcp, SLOT(TcpInit()));
    connect(thread_tcp, SIGNAL(finished()), &tcp, SLOT(TcpQuit()));
    connect(&tcp, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    thread_tcp->start();
}

void WinHome::regularTasks()
{
    while (!udp.recv_queue.isEmpty()) {
        QString msg = udp.recv_queue.dequeue();
        int a = msg.indexOf(" ");
        int cmd = msg.mid(0, a).toInt();
        QString dat = msg.mid(a+1, msg.size());
        switch (cmd) {
        case 6001:  // 自检信息
            emit sendNetMsg("6001");
            break;
        case 6005:
            winTest->updateItems(dat);
            break;
        case 6007:  // 单项测试完成
            status = STATUS_FREE;
        default:
            qDebug() << cmd << dat;
            break;
        }
    }
}





void WinHome::InitWindowsAll()
{
    WinBack *winBack = new WinBack(this);
    ui->desktop->addWidget(winBack);
    winBack->setObjectName("WinBack");
    connect(winBack, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(winBack, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), winBack,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize WinBack OK\n");

    WinSyst *winSyst = new WinSyst(this);
    ui->desktop->addWidget(winSyst);
    winSyst->setObjectName("WinSyst");
    connect(winSyst, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(winSyst, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), winSyst,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize WinSyst OK\n");

    WinType *winType = new WinType(this);
    ui->desktop->addWidget(winType);
    winType->setObjectName("WinType");
    connect(winType, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(winType, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize WinType OK\n");

    WinData *winData = new WinData(this);
    ui->desktop->addWidget(winData);
    winData->setObjectName("WinData");
    connect(winData, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(winData, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize WinData OK\n");

    winTest = new WinTest(this);
    ui->desktop->addWidget(winTest);
    winTest->setObjectName("WinTest");
    connect(winTest, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), winTest, SLOT(recvAppShow(QString)));
    connect(winTest, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), winTest,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize WinTest OK\n");

    PageDcr *pageDcr = new PageDcr(this);
    ui->desktop->addWidget(pageDcr);
    pageDcr->setObjectName("PageDcr");
    connect(pageDcr, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pageDcr, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pageDcr,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PageDcr OK\n");

    PageInr *pageInr = new PageInr(this);
    ui->desktop->addWidget(pageInr);
    pageInr->setObjectName("PageInr");
    connect(pageInr, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pageInr, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pageInr,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PageInr OK\n");

    PageAcw *pageAcw = new PageAcw(this);
    ui->desktop->addWidget(pageAcw);
    pageAcw->setObjectName("PageAcw");
    connect(pageAcw, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pageAcw, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pageAcw,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PageAcw OK\n");

    PageInd *pageInd = new PageInd(this);
    ui->desktop->addWidget(pageInd);
    pageInd->setObjectName("PageInd");
    connect(pageInd, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pageInd, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pageInd,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PageInd OK\n");

    ReadStatusAll();
}

void WinHome::JumpToWindow(QByteArray win)
{
    if (HomeMode == HOME_TEST)
        return;
    int WinCurrent = ui->desktop->currentIndex();
    if (win.isNull()) { //空代表返回
        emit transmitShow(ui->desktop->widget(previous_window.last())->objectName());
        ui->desktop->setCurrentIndex(previous_window.last());
        previous_window.removeLast();
        return;
    }
    for (int i=0; i < ui->desktop->count(); i++) {
        if (ui->desktop->widget(i)->objectName() == win) {
            emit transmitShow(win);
            ui->desktop->setCurrentIndex(i);
            break;
        }
    }
    previous_window.append(WinCurrent);
    if (previous_window.size() > 10) { // 最大嵌套10层
        previous_window.removeFirst();
    }
}





void WinHome::ReadMessage(quint16 addr,  quint16 cmd,  QByteArray msg)
{
    if (addr != ADDR) {
        emit SendCommand(addr, cmd, msg);
        return;
    }
    switch (cmd) {
    case CMD_JUMP:
        JumpToWindow(msg);
        break;
    case CMD_INIT:
        InitTestItems();
        break;
    case CMD_STATUS:
        ReadStatusAll();
        break;
    case CMD_INIT_ITEM:
        if (isPause) {
            TempItems.append(QString(msg).split("\n"));
            break;
        }
        Items.append(QString(msg).split("\n"));
        break;
    case CMD_JUDGE:
        SaveItemJudge(msg);
        break;
    case CMD_ITEM:
        emit WriteSql(msg);
        emit SendCommand(ADDR, cmd, msg);
        break;
    case CMD_ITEM_TEMP:
        emit SendCommand(ADDR, CMD_ITEM, msg);
        break;
    case CMD_TEMP:
    case CMD_WAVE_BYTE:
    case CMD_WAVE_TEST:
    case CMD_WAVE_ITEM:
    case CMD_WAVE_HIDE:
        emit SendCommand(WIN_ID_TEST, cmd, msg);
        break;
    case CMD_DEBUG:
        ShowLogMessage(msg);
        break;
    case CMD_WAVE:
        emit SendCommand(ADDR, CMD_WAVE, msg);
        break;
    case CMD_CAN:
        emit PutCanData(msg);
        break;
    case CMD_START:
        if (HomeMode != HOME_FREE)
            break;
        if (QString(msg).split(" ").size() < 2)
            return;
        if (QString(msg).split(" ").at(1).toInt() != CurrentStartMode())
            return;
        StartTest(QString(msg).split(" ").at(0).toUtf8());
        break;
    case CMD_STOP:
        ItemJudge = "NG";
        SendButtonBox("Ok");
        if (msg.contains("DCR"))
            ShowLogMessage(msg);
        emit SendCommand(ADDR, CMD_STOP, msg);
        if (HomeMode != HOME_FREE)
            HomeMode = HOME_STOP;
        else
            InitTestItems();
        break;
    case CMD_NET:
        ui->IconNet->setPixmap(QPixmap(":/source/wifi.png"));
        break;
    case CMD_SQL:
        ui->IconNet->setPixmap(QPixmap(":/source/radio_1.png"));
        break;
    case CMD_CODE:
        emit WriteSql(msg);
        break;
    default:
        qDebug() << addr << cmd << msg;
        break;
    }
}

void WinHome::InitTestItems()
{
    emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x00));
    QStringList n = CurrentItems();
    if (n.isEmpty())
        return;
    Items.clear();
    if (CurrentStartMode() == 1 || CurrentStartMode() == 2) //滑罩启动
        emit SendCommand(WIN_ID_OUT13, CMD_INIT, QString::number(CurrentStartMode()).toUtf8());

    if (!n.contains(QString::number(WIN_ID_DCR)) && n.contains(QString::number(WIN_ID_PWR))) {
        n.insert(0, QString::number(WIN_ID_DCR));
    }
    if (!n.contains(QString::number(WIN_ID_DCR)) && n.contains(QString::number(WIN_ID_LVS))) {
        n.insert(0, QString::number(WIN_ID_DCR));
    }
    if (!n.contains(QString::number(WIN_ID_DCR)) && n.contains(QString::number(WIN_ID_LCK))) {
        n.insert(0, QString::number(WIN_ID_DCR));
    }
    for (int i=0; i < n.size(); i++) {
        emit SendCommand(n.at(i).toInt(), CMD_INIT, NULL);
    }
    emit SendCommand(WIN_ID_TEST, CMD_INIT, Items.join("\n").toUtf8()); //初始化测试界面
}

void WinHome::ReadStatusAll()
{
    if (HomeMode != HOME_FREE)
        return;

    qDebug() << QTime::currentTime().toString() << "WinHome read OK";

    ui->Text->hide();
    ui->keybord->setCurrentIndex(1);

    HomeMode = HOME_FREE;
    if (ui->desktop->currentWidget()->objectName() == "MainPage") {
        Delay(1000);
        JumpToWindow("WinTest");
    }
}

void WinHome::StartTest(QByteArray station)
{
    testInit();
    //    stat = station;
    //    if (CurrentReStartMode() == 1)
    //        stat = QString("%1").arg(0x13).toUtf8();
    //    if (CurrentReStartMode() == 2)
    //        stat = QString("%1").arg(0x14).toUtf8();
    //    if (HomeMode == HOME_TEST)
    //        return;
    //    if (ui->desktop->currentWidget()->objectName() != "WinTest")
    //        return;
    //    HomeMode = HOME_TEST;
    //    ItemJudge = "OK";

    //    InitTestItems();
    //    emit SendCommand(ADDR, CMD_STATUS, "buzy");
    //    emit SendCommand(WIN_ID_TEST, CMD_START, station);
    //    emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x02 | 0x00));

    //    QStringList n = CurrentItems();
    //    if (!n.contains(QString::number(WIN_ID_DCR)) && n.contains(QString::number(WIN_ID_PWR))) {
    //        n.insert(0, QString::number(WIN_ID_DCR));
    //    }
    //    if (!n.contains(QString::number(WIN_ID_DCR)) && n.contains(QString::number(WIN_ID_LVS))) {
    //        n.insert(0, QString::number(WIN_ID_DCR));
    //    }
    //    if (!n.contains(QString::number(WIN_ID_DCR)) && n.contains(QString::number(WIN_ID_LCK))) {
    //        n.insert(0, QString::number(WIN_ID_DCR));
    //    }
    //    for (int i=0; i < n.size(); i++) {
    //        Current_Test_Item = n.at(i).toInt();
    //        emit SendCommand(n.at(i).toInt(), CMD_START, station);
    //        if (HomeMode == HOME_STOP) {
    //            //            QVariantHash hash;
    //            //            hash.insert("TxMessage", tr("测试中断"));
    //            //            Warnning(hash);
    //            break;
    //        }
    //        Delay(10);
    //    }
    //    SaveTestJudge();
    //    if (ItemJudge == "NG") {
    //        emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x08 | 0x01));
    //        Delay(CurrentAlarmTime("NG"));
    //        emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x08 | 0x00));
    //    } else {
    //        emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x04 | 0x01));
    //        Delay(CurrentAlarmTime("OK"));
    //        emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x04 | 0x00));
    //    }
    //    emit SendCommand(WIN_ID_TEST, CMD_JUDGE, ItemJudge.toUtf8());

    //    emit SendCommand(ADDR, CMD_STATUS, "ready");
    //    if (CurrentReStartMode() != 0 && HomeMode != HOME_STOP) {
    //        QTimer *timer = new QTimer(this);
    //        timer->singleShot(CurrentDelay(),  this,  SLOT(ReStartTest()));
    //    }
    //    HomeMode = HOME_FREE;
}

void WinHome::ReStartTest()
{
    switch (CurrentReStartMode()) {
    case 0:
        break;
    case 1:
        StartTest(QString("%1").arg(0x13).toUtf8());
        break;
    case 2:
        StartTest(QString("%1").arg(0x14).toUtf8());
        break;
    case 3:
        StartTest(stat);
        break;
    case 4:
        if (stat.toInt() == 0x13) {
            StartTest(QString("%1").arg(0x14).toUtf8());
            break;
        }
        if (stat.toInt() == 0x14) {
            StartTest(QString("%1").arg(0x13).toUtf8());
            break;
        }
        break;
    default:
        break;
    }
}

void WinHome::SaveTestJudge()
{
    QString s = QString(tr("总数@%1@%2")).arg(CurrentSettings()).arg(ItemJudge);
    emit WriteSql(s.toUtf8());
}

void WinHome::SaveItemJudge(QByteArray msg)
{
    emit WriteSql(msg);
    QStringList s = QString(msg).split("@");
    if (s.size() < 3)
        return;
    if (s.at(2) == "NG")
        ItemJudge = "NG";
    if (s.at(2) == "NG" && CurrentPauseMode() != 1)
        TestPause();
}

void WinHome::TestPause()
{
    isPause = true;
    TempItems.clear();
    QString text = tr("此项目不合格,是否重测");
    PopupBox *box = new PopupBox(this, "", text, QMessageBox::Retry, QMessageBox::Ok);
    connect(this, SIGNAL(SendVariant(QVariant)), box, SLOT(ReadVariant(QVariant)));
    emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x0A | 0x01));
    Delay(CurrentAlarmTime("NG"));
    emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x0A | 0x00));
    int ret = box->exec();
    if (ret == QMessageBox::Retry)
    {
        ItemJudge = "OK";
        if (HomeMode != HOME_STOP)
            emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x02 | 0x00));
        emit SendCommand(Current_Test_Item, CMD_INIT, stat);
        emit SendCommand(WIN_ID_TEST, CMD_ITEM_REPLACE, TempItems.join("\n").toUtf8());
        emit SendCommand(Current_Test_Item, CMD_START, stat);
        Delay(10);
    } else if (ret == QMessageBox::Ok) {
        if (HomeMode != HOME_STOP)
            emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x02 | 0x00));
    }
    isPause = false;
    TempItems.clear();
}

void WinHome::ShowLogMessage(QByteArray msg)
{
    if (!ui->Text->isHidden()) {
        ui->Text->insertPlainText(msg);
        ui->Text->moveCursor(QTextCursor::EndOfBlock);
        Delay(1);
    }
    emit SendCommand(ADDR, CMD_DEBUG, msg);
}

bool WinHome::WaitTimeOut(quint16 t)
{
    int TimeOut = 0;
    while (HomeMode != HOME_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void WinHome::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

QString WinHome::CurrentSettings()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", INI_DEFAULT).toString();
    return n.remove(".ini");
}

QStringList WinHome::CurrentItems()
{
    QString n = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(n, QSettings::IniFormat);
    QString s = ini->value("/GLOBAL/ProjToTest", "1").toString();
    return s.split(" ");
}

QStringList WinHome::EnableItems()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/ItemEnable", INI_DEFAULT).toString();
    return n.split(" ");
}

int WinHome::CurrentStartMode()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    return ini->value("/GLOBAL/Mode", "0").toInt();
}

int WinHome::CurrentPauseMode()
{
    QString n = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(n, QSettings::IniFormat);
    return ini->value("/GLOBAL/TestNG", "1").toInt();
}

int WinHome::CurrentAlarmTime(QString msg)
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    if (msg == "NG")
        return ini->value("/GLOBAL/TimeNG", "0.2").toDouble()*1000;
    else
        return ini->value("/GLOBAL/TimeOK", "0.1").toDouble()*1000;
}

int WinHome::CurrentReStartMode()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    return ini->value("/GLOBAL/RestartMode", "0").toInt();
}

int WinHome::CurrentDelay()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    return ini->value("/GLOBAL/TestDelay", "1").toDouble()*1000;
}


void WinHome::ReadVariant(QVariant s)
{
    QVariantHash hash = s.toHash();
    if (hash.value("TxAddress") != "WinHome") {
        emit SendVariant(s);
        return;
    }
    if (hash.value("TxCommand") == "Warnning")
        Warnning(hash);
    if (hash.value("TxCommand") == "Error")
        Error(hash);
}

void WinHome::Error(QVariantHash hash)
{
    QString text = hash.value("TxMessage").toString();
    PopupBox *box = new PopupBox(this, "", text, NULL, QMessageBox::NoButton);
    box->exec();
}

void WinHome::Warnning(QVariantHash hash)
{
    QString text = hash.value("TxMessage").toString();
    PopupBox *box = new PopupBox(this, "", text, NULL, QMessageBox::Ok);
    connect(this, SIGNAL(SendVariant(QVariant)), box, SLOT(ReadVariant(QVariant)));
    box->exec();
}

void WinHome::SendButtonBox(QString button)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "BoxButton");
    hash.insert("TxMessage", button);
    emit SendVariant(QVariant::fromValue(hash));
}

void WinHome::ReadButtonBox(QByteArray msg)
{
    if (quint8(msg.at(1)) != 0)
        SendButtonBox("Retry");
    if (quint8(msg.at(2)) != 0)
        SendButtonBox("Ok");
}
/*********************************END OF FILE**********************************/
