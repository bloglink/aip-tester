/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       界面框架
*******************************************************************************/
#include "mainscreen.h"

MainScreen::MainScreen(QWidget *parent) : QWidget(parent)
{
    initUI();
    openSqlite();
    initSocket();
    initSettings();
    initTaskThread();
}

void MainScreen::openSqlite()
{
    sqlName = "action";
    db = QSqlDatabase::addDatabase("QSQLITE", sqlName);
    db.setDatabaseName("aip.db");
    if (db.open()) {
        emit sendSqlOpen(db.connectionName());
    } else {
        qDebug() << "db open fail";
    }
}

void MainScreen::initUI()
{
    initSkin();
    initPopup();
    initLayout();
    initSignIn();
    initAuthor();
    initSystem();
    initConfig();
    initSqlite();
    initTester();
    initVersion();
}

void MainScreen::initSkin()
{
    QFile file;
    QString qss;
    file.setFileName("./skins/soft_rain.qss");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);
}

void MainScreen::initPopup()
{
    popup = new LQMessageBox(this, "", "", QMessageBox::Ok);
    popup->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Popup);
    popup->setStyleSheet("QDialog{border:2px solid cyan;}");
}

void MainScreen::initLayout()
{
    stack = new QStackedWidget(this);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(stack);
    this->setLayout(layout);
}

void MainScreen::initSignIn()
{
    signin = new SignIn(this);
    signin->setObjectName("signin");
    connect(this, SIGNAL(sendAppShow(QString)), signin, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), signin, SLOT(recvSqlOpen(QString)));
    connect(signin, SIGNAL(sendAppMsg(QJsonObject)), this, SLOT(recvAppMsg(QJsonObject)));
    stack->addWidget(signin);
}

void MainScreen::initAuthor()
{
    author = new Author(this);
    author->setObjectName("author");
    connect(this, SIGNAL(sendAppShow(QString)), author, SLOT(recvAppShow(QString)));
    connect(author, SIGNAL(sendAppShow(QString)), this, SLOT(recvAppShow(QString)));
    stack->addWidget(author);
}

void MainScreen::initSystem()
{
    system = new System(this);
    system->setObjectName("system");
    connect(this, SIGNAL(sendAppShow(QString)), system, SLOT(recvAppShow(QString)));
    connect(system, SIGNAL(sendAppShow(QString)), this, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), system, SLOT(recvSqlOpen(QString)));
    connect(system, SIGNAL(sendAppMsg(QJsonObject)), this, SLOT(recvAppMsg(QJsonObject)));
    stack->addWidget(system);
}

void MainScreen::initConfig()
{
    config = new Config(this);
    config->setObjectName("config");
    connect(this, SIGNAL(sendAppShow(QString)), config, SLOT(recvAppShow(QString)));
    connect(config, SIGNAL(sendAppShow(QString)), this, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), config, SLOT(recvSqlOpen(QString)));
    connect(config, SIGNAL(sendAppMsg(QJsonObject)), this, SLOT(recvAppMsg(QJsonObject)));
    connect(this, SIGNAL(sendAppCmd(QJsonObject)), config, SLOT(recvAppCmd(QJsonObject)));
    connect(config, SIGNAL(updateSettings()), this, SLOT(initSettings()));
    stack->addWidget(config);

    dcr = new Resistance(this);
    dcr->setObjectName("dcr");
    connect(this, SIGNAL(sendAppShow(QString)), dcr, SLOT(recvAppShow(QString)));
    connect(dcr, SIGNAL(sendAppShow(QString)), this, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), dcr, SLOT(recvSqlOpen(QString)));
    connect(dcr, SIGNAL(sendAppMsg(QJsonObject)), this, SLOT(recvAppMsg(QJsonObject)));
    connect(this, SIGNAL(sendAppCmd(QJsonObject)), dcr, SLOT(recvAppCmd(QJsonObject)));
    stack->addWidget(dcr);

    inr = new Insulation(this);
    inr->setObjectName("ir");
    connect(this, SIGNAL(sendAppShow(QString)), inr, SLOT(recvAppShow(QString)));
    connect(inr, SIGNAL(sendAppShow(QString)), this, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), inr, SLOT(recvSqlOpen(QString)));
    connect(inr, SIGNAL(sendAppMsg(QJsonObject)), this, SLOT(recvAppMsg(QJsonObject)));
    connect(this, SIGNAL(sendAppCmd(QJsonObject)), inr, SLOT(recvAppCmd(QJsonObject)));
    stack->addWidget(inr);

    mag = new Magnetic(this);
    mag->setObjectName("mag");
    connect(this, SIGNAL(sendAppShow(QString)), mag, SLOT(recvAppShow(QString)));
    connect(mag, SIGNAL(sendAppShow(QString)), this, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), mag, SLOT(recvSqlOpen(QString)));
    connect(mag, SIGNAL(sendAppMsg(QJsonObject)), this, SLOT(recvAppMsg(QJsonObject)));
    connect(this, SIGNAL(sendAppCmd(QJsonObject)), mag, SLOT(recvAppCmd(QJsonObject)));
    stack->addWidget(mag);

    acw = new Voltage(this);
    acw->setObjectName("acw");
    connect(this, SIGNAL(sendAppShow(QString)), acw, SLOT(recvAppShow(QString)));
    connect(acw, SIGNAL(sendAppShow(QString)), this, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), acw, SLOT(recvSqlOpen(QString)));
    connect(acw, SIGNAL(sendAppMsg(QJsonObject)), this, SLOT(recvAppMsg(QJsonObject)));
    connect(this, SIGNAL(sendAppCmd(QJsonObject)), acw, SLOT(recvAppCmd(QJsonObject)));
    stack->addWidget(acw);

    imp = new Impulse(this);
    imp->setObjectName("imp");
    connect(this, SIGNAL(sendAppShow(QString)), imp, SLOT(recvAppShow(QString)));
    connect(imp, SIGNAL(sendAppShow(QString)), this, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), imp, SLOT(recvSqlOpen(QString)));
    connect(imp, SIGNAL(sendAppMsg(QJsonObject)), this, SLOT(recvAppMsg(QJsonObject)));
    connect(this, SIGNAL(sendAppCmd(QJsonObject)), imp, SLOT(recvAppCmd(QJsonObject)));
    stack->addWidget(imp);
}

void MainScreen::initSqlite()
{
    sqlite = new Sqlite(this);
    sqlite->setObjectName("sqlite");
    connect(this, SIGNAL(sendAppShow(QString)), sqlite, SLOT(recvAppShow(QString)));
    connect(sqlite, SIGNAL(sendAppShow(QString)), this, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), sqlite, SLOT(recvSqlOpen(QString)));
    stack->addWidget(sqlite);
}

void MainScreen::initTester()
{
    tester = new Tester(this);
    tester->setObjectName("tester");
    connect(this, SIGNAL(sendAppShow(QString)), tester, SLOT(recvAppShow(QString)));
    connect(tester, SIGNAL(sendAppShow(QString)), this, SLOT(recvAppShow(QString)));
    connect(tester, SIGNAL(sendAppMsg(QJsonObject)), this, SLOT(recvAppMsg(QJsonObject)));
    stack->addWidget(tester);
}

void MainScreen::initSocket()
{
    connect(this, SIGNAL(sendNetMsg(QJsonObject)), &udp, SLOT(recvAppMsg(QJsonObject)));
    connect(&udp, SIGNAL(sendNetMsg(QString)), this, SLOT(recvNetMsg(QString)));
}

void MainScreen::initVersion()
{
    this->setWindowTitle("二代真空测试仪试用版V0.1");
}

void MainScreen::initSettings()
{
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");

    QStringList groups = ini->childGroups();
    for (int i=0; i < groups.size(); i++) {
        QJsonObject obj;
        ini->beginGroup(groups.at(i));
        QStringList keys = ini->allKeys();
        for (int j=0; j < keys.size(); j++) {
            obj.insert(keys.at(j), ini->value(keys.at(j)).toString());
        }
        configs.insert(groups.at(i), obj);
        ini->endGroup();
    }
    emit sendAppCmd(configs);
}

void MainScreen::saveSettings()
{
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");

    QStringList keys = configs.keys();
    for (int i=0; i < keys.size(); i++) {
        QJsonObject obj = configs.value(keys.at(i)).toObject();
        QStringList temp = obj.keys();
        ini->beginGroup(keys.at(i));
        for (int i=0; i < temp.size(); i++) {
            ini->setValue(temp.at(i), obj.value(temp.at(i)).toString());
        }
        ini->endGroup();
    }
}

void MainScreen::initTaskThread()
{
    gState = 0;
    gShift = 0;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(taskThread()));
    timer->start(10);
}

void MainScreen::sendNetXml(QJsonObject msg)
{
    QStringList keys = msg.keys();
    for (int i=0; i < keys.size(); i++) {
        QDomDocument doc;
        QDomElement root;
        root = doc.createElement(keys.at(i));
        doc.appendChild(root);

        QJsonObject temp = msg.value(keys.at(i)).toObject();
        QStringList key = temp.keys();
        for (int t=0; t < key.size(); t++) {
            QDomElement element = doc.createElement(key.at(t));
            QDomText text = doc.createTextNode(temp.value(key.at(t)).toString());
            element.appendChild(text);
            root.appendChild(element);
        }
        sendNetCmd(QString("6002 %1").arg(QString(doc.toByteArray())));
    }
}

void MainScreen::sendNetCmd(QString msg)
{
    QJsonObject xx;
    xx.insert("txCommand", CMD_COMMAND);
    xx.insert("txMessage", msg);
    emit sendNetMsg(xx);
}

void MainScreen::sendNetShow(QString win)
{
    if (win == "tester") {
        sendNetCmd("6008");
        if (getGlobalParameter("testMode") == 3)
            sendNetCmd("6052 1");
        else
            sendNetCmd("6052 0");
    } else if (win == "config") {
        sendNetCmd("6004 Conf");
    } else if (win == "dcr") {
        sendNetCmd("6004 DCR");
    } else if (win == "mag") {
        sendNetCmd("6004 MAG");
    } else if (win == "ir") {
        sendNetCmd("6004 IR");
    } else if (win == "acw") {
        sendNetCmd("6004 ACW");
    } else if (win == "imp") {
        sendNetCmd("6004 IMP");
        if (getGlobalParameter("testMode") == 3)
            sendNetCmd("6052 1");
        else
            sendNetCmd("6052 0");
    }
}

double MainScreen::getMasterId(QString masterName)
{
    double id = 0;
    QSqlQuery query(QSqlDatabase::database(sqlName));
    query.prepare("select id from master where name = :name");
    query.bindValue(":name", masterName);
    query.exec();
    if (query.next()) {
        id = query.value(0).toDouble();
    } else {
        qDebug() << "get master id fail" << masterName;
    }
    return id;
}

double MainScreen::getSourceId(QString sourceName)
{
    double id = 0;
    QSqlQuery query(QSqlDatabase::database(sqlName));
    query.prepare("select id from source where name = :name");
    query.bindValue(":name", sourceName);
    query.exec();
    if (query.next()) {
        id = query.value(0).toDouble();
    } else {
        qDebug() << "get source id fail" << sourceName;
    }
    return id;
}

double MainScreen::getAction(double masterId, double sourceId)
{
    double id = 0;
    QSqlQuery query(QSqlDatabase::database(sqlName));
    query.prepare("select action from action where master = :master and source = :source");
    query.bindValue(":master", masterId);
    query.bindValue(":source", sourceId);
    query.exec();
    if (query.next()) {
        id = query.value(0).toDouble();
    } else {
        qDebug() << "get action fail";
    }
    return id;
}

double MainScreen::getGlobalParameter(QString name)
{
    double id = 0;
    QSqlQuery query(QSqlDatabase::database(sqlName));
    query.prepare("select parameter from global where name = :name");
    query.bindValue(":name", name);
    query.exec();
    if (query.next()) {
        id = query.value(0).toDouble();
    } else {
        qDebug() << "get global parameter fail" << name;
    }
    return id;
}

void MainScreen::recvAppShow(QString sourceName)
{
    double masterId = getMasterId(masterName);
    double sourceId = getSourceId(sourceName);
    double actionValue = getAction(masterId, sourceId);

    if (actionValue != Qt::Checked) {
        QMessageBox::warning(this, "", "您无权限进行此操作", QMessageBox::Ok);
        return;
    }

    for (int i=0; i < stack->count(); i++) {
        if (stack->widget(i)->objectName() == sourceName) {
            stack->setCurrentIndex(i);
            emit sendAppShow(sourceName);
            break;
        }
    }
    sendNetShow(sourceName);
}

void MainScreen::recvAppMsg(QJsonObject msg)
{
    int cmd = msg.value("txCommand").toInt();
    switch (cmd) {
    case CMD_CONNECT:
        masterName = msg.value("username").toString();
        system->setMasterName(masterName);
        tester->setMasterName(masterName);
        msg.insert("txMessage", "6000");
        sendNetMsg(msg);
        recvAppShow("author");
        break;
    case CMD_START:
        setShift(JUNIOR, JUNOIR_TEST);
        setShift(MIDDLE, JUNOIR_INIT);
        qDebug() << "recv start" << getState(JUNIOR, gShift) << getState(MIDDLE, gShift);
        station = msg.value("txMessage").toInt();
        break;
    case CMD_STOP:
        setShift(JUNIOR, JUNOIR_STOP);
        break;
    case MAG_WAVE:
        msg.insert("txCommand", CMD_COMMAND);
        sendNetMsg(msg);
        break;
    case IMP_WAVE:
        msg.insert("txCommand", CMD_COMMAND);
        sendNetMsg(msg);
        break;
    case IOBRD:
        msg.insert("txCommand", CMD_COMMAND);
        sendNetMsg(msg);
        break;
    default:
        QStringList keys = msg.keys();
        for (int i=0; i < keys.size(); i++)
            configs.insert(keys.at(i), msg.value(keys.at(i)).toObject());
        saveSettings();
        sendNetXml(msg);
        break;
    }
}

void MainScreen::recvNetMsg(QString msg)
{
    int a = msg.indexOf(" ");
    int cmd = msg.mid(0, a).toInt();
    QString dat = msg.mid(a+1, msg.size());
    switch (cmd) {
    case 6001:  // 自检信息
        if (!dat.isEmpty())
            warnning(QString("板卡错误:%1").arg(dat));
        recvAppShow("author");
        break;
    case 6005:  // 上传配置
        //        if (dat.contains("NG"))
        //            isNG = true;
        tester->updateItems(dat);
        break;
    case 6007:  // 单项测试完成
        setShift(MIDDLE, JUNOIR_OVER);
        qDebug() << "recv over" << getState(JUNIOR, gShift) << getState(MIDDLE, gShift);
        break;
    case 6015:  // 空载启动完成
        //        QTimer::singleShot(10, this, SLOT(readNoLoadStart()));
        break;
    case 6019:
        //        power = dat.split(" ");
        break;
    case 6021:
        //        tester->updateWave(dat);
        break;
    case 6032:
        tester->updateTemp(dat);
        break;
    case 6033:
        //        loadStopEnable = false;
        //        QTimer::singleShot(10, this, SLOT(readNoLoadStop()));
        break;
    case 6035:
        mag->parameters(dat);
        break;
    case 6037:
    case 6059:
        system->setIobrd(msg);
        break;
    case 6039:
        mag->drawWave(dat);
        break;
    case 6040:
        tester->saveMAG(dat);
        tester->drawMAG(dat);
        break;
    case 6042:
        imp->drawWave(dat);
        break;
    case 6043:
        tester->saveIMP(dat);
        if (station == 0x13)
            tester->saveStd(imp->readWaveL(dat));
        else
            tester->saveStd(imp->readWaveR(dat));
        tester->drawIMP(dat);
        if (station == 0x13)
            tester->drawStd(imp->readWaveL(dat));
        else
            tester->drawStd(imp->readWaveR(dat));
        break;
    case 6053:
        if (getGlobalParameter("testMode") == 3)
            sendNetCmd("6052 1");
        else
            sendNetCmd("6052 0");
        break;
    case 6055:
        imp->parameters(dat);
        break;
    case 6060:
        setShift(JUNIOR, JUNOIR_NETS);
        setShift(MIDDLE, JUNOIR_INIT);
        qDebug() << "recv start" << getState(JUNIOR, gShift) << getState(MIDDLE, gShift);
        station = dat.toInt();
        break;
    case 6061:
        setShift(JUNIOR, JUNOIR_NETP);
        break;
    case 6062:
        recvAppShow("author");
        warnning("真空泵已关闭,进入待机状态");
        break;
    case 9032:
        //        readStartL(false);
        //        readStartR(false);
        //        QMessageBox::warning(this, "警告", "失去对设备的连接", QMessageBox::Ok);
        break;
    default:
        break;
    }
}

void MainScreen::taskThread()
{
    heartThread();
    juniorThread();
}

void MainScreen::heartThread()
{
    if (udp.txTime <= TIME_OUT) {
        udp.txTime++;
        if (udp.txTime == TIME_OUT/2)
            sendNetCmd("6030");
        if (udp.txTime >= TIME_OUT) {
            udp.txTime = 0;
            warnning("网络连接中断");
        }
    }
}

void MainScreen::juniorThread()
{
    quint8 state = getState(JUNIOR, gState);
    switch (state) {
    case JUNOIR_INIT:
        setState(JUNIOR, JUNOIR_IDLE);
        break;
    case JUNOIR_IDLE:
        if (getState(JUNIOR, gShift) == JUNOIR_TEST) {
            setState(JUNIOR, JUNOIR_TEST);
            setState(MIDDLE, JUNOIR_INIT);
            setState(SENIOR, getNextTestItem(0));
            sendNetCmd(QString("6020 %1").arg(station));
            tester->initItems(station);
        }
        if (getState(JUNIOR, gShift) == JUNOIR_NETS) {
            setState(JUNIOR, JUNOIR_TEST);
            setState(MIDDLE, JUNOIR_INIT);
            setState(SENIOR, getNextTestItem(0));
            tester->initItems(station);
        }
        break;
    case JUNOIR_TEST:
        if (getState(JUNIOR, gShift) == JUNOIR_STOP)
            setState(JUNIOR, JUNOIR_STOP);
        else if (getState(JUNIOR, gShift) == JUNOIR_NETP)
            setState(JUNIOR, JUNOIR_OVER);
        else
            middleThread();
        break;
    case JUNOIR_STOP:
        stopThread();
        break;
    case JUNOIR_WAIT:
        middleThread();
        break;
    case JUNOIR_OVER:
        saveThread();
        break;
    default:
        setState(JUNIOR, JUNOIR_IDLE);
        break;
    }
}

void MainScreen::middleThread()
{
    quint8 state = getState(MIDDLE, gState);
    quint8 next = getNextTestItem(getState(SENIOR, gState));

    switch (state) {
    case MIDDLE_INIT:
        initThread();
        break;
    case MIDDLE_IDLE:
        idleThread();
        break;
    case MIDDLE_TEST:
        if (getState(MIDDLE, gShift) == JUNOIR_OVER) {
            setState(MIDDLE, JUNOIR_WAIT);
        }
        break;
    case MIDDLE_WAIT:
        setState(MIDDLE, JUNOIR_OVER);
        break;
    case MIDDLE_OVER:
        if (next == 0 || getState(JUNIOR, gShift) == JUNOIR_STOP) {
            setState(JUNIOR, JUNOIR_OVER);
        } else {
            setState(MIDDLE, JUNOIR_INIT);
            setState(SENIOR, next);
            setShift(MIDDLE, JUNOIR_INIT);
        }
        break;
    default:
        break;
    }
}

void MainScreen::saveThread()
{
    bool ok;
    QString xx;
    if (getState(JUNIOR, gShift) == JUNOIR_STOP) {
        ok = tester->updateResult(1);
    } else {
        ok = tester->updateResult(0);
    }

    xx.append(CurrentSettings());
    xx.append("@");
    xx.append(tester->readNumb());
    xx.append("@");
    xx.append(masterName);
    xx.append("@");
    if (ok) {
        xx.append("OK");
    } else {
        xx.append("NG");
    }
    QJsonObject objs;
    objs.insert("title", xx);
    objs.insert("content", tester->readResult());
    sqlite->saveSql(objs);

    setState(JUNIOR, JUNOIR_INIT);
    setState(MIDDLE, JUNOIR_INIT);
    setState(SENIOR, JUNOIR_INIT);
    gShift = 0;
    qDebug() << "save" << getState(JUNIOR, gShift) << getState(MIDDLE, gShift);;
}

void MainScreen::initThread()
{
    quint8 state = getState(SENIOR, gState);
    switch (state) {
    default:
        setState(MIDDLE, MIDDLE_IDLE);
        break;
    }
}

void MainScreen::idleThread()
{
    quint8 state = getState(SENIOR, gState);
    switch (state) {
    case SENOIR_DCR:
        sendNetCmd("6006 DCR");
        break;
    case SENOIR_MAG:
        sendNetCmd("6006 MAG");
        break;
    case SENOIR_INR:
        sendNetCmd("6006 IR");
        break;
    case SENOIR_ACW:
        sendNetCmd("6006 ACW");
        break;
    case SENOIR_DCW:
        sendNetCmd("6006 DCW");
        break;
    case SENOIR_IMP:
        sendNetCmd("6006 IMP");
        break;
    case SENOIR_IND:
        sendNetCmd("6006 IND");
        break;
    case SENOIR_HAL:
        sendNetCmd("6006 HALL");
        break;
    case SENOIR_LOD:
        sendNetCmd("6006 LOAD");
        break;
    case SENOIR_NLD:
        sendNetCmd("6006 NOLOAD");
        break;
    case SENOIR_BEF:
        sendNetCmd("6006 BEMF");
        break;
    default:
        break;
    }
    setState(MIDDLE, MIDDLE_TEST);
    qDebug() << "send start" << getState(JUNIOR, gShift) << getState(MIDDLE, gShift);
}

void MainScreen::stopThread()
{
    quint8 state = getState(SENIOR, gState);
    switch (state) {
    default:
        sendNetCmd(QString("6022 %1").arg(station));
        setState(JUNIOR, JUNOIR_WAIT);
        qDebug() << "send stop" << getState(JUNIOR, gShift) << getState(MIDDLE, gShift);
        break;
    }
}

void MainScreen::overThread()
{
    quint8 state = getState(SENIOR, gState);
    switch (state) {
    case SENOIR_DCR:
        setState(MIDDLE, JUNOIR_IDLE);
        break;
    case SENOIR_MAG:
        setState(MIDDLE, JUNOIR_IDLE);
        break;
    case SENOIR_INR:
        setState(MIDDLE, JUNOIR_IDLE);
        break;
    case SENOIR_ACW:
        setState(MIDDLE, JUNOIR_IDLE);
        break;
    case SENOIR_DCW:
        setState(MIDDLE, JUNOIR_IDLE);
        break;
    case SENOIR_IMP:
        setState(MIDDLE, JUNOIR_IDLE);
        break;
    default:
        setState(MIDDLE, JUNOIR_IDLE);
        break;
    }
}

QString MainScreen::CurrentSettings()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", "Base_File").toString();
    return n.remove(".ini");
}

void MainScreen::closeEvent(QCloseEvent *e)
{
    sendNetCmd("6052 0");
    e->accept();
}

void MainScreen::warnning(QString dat)
{
    if (!dat.isEmpty()) {
        popup->setText(dat);
        popup->exec();
    }
}

quint8 MainScreen::getState(quint8 i, quint32 s)
{
    quint8 w = JUNOIR_INIT;
    switch (i) {
    case JUNIOR:  // 初级状态
        w = s % 256;
        break;
    case MIDDLE:  // 次级状态
        w = (s >> 8) % 256;
        break;
    case SENIOR:  // 末级状态
        w = (s >> 16) % 256;
        break;
    default:
        break;
    }
    return w;
}

void MainScreen::setState(quint8 i, quint8 s)
{
    quint32 ss = s;
    switch (i) {
    case JUNIOR:  // 初级状态
        gState = (gState & 0xFFFFFF00) | ss;
        break;
    case MIDDLE:  // 次级状态
        gState = (gState & 0xFFFF00FF) | (ss << 8);
        break;
    case SENIOR:  // 末级状态
        gState = (gState & 0xFF00FFFF) | (ss << 16);
        break;
    default:
        break;
    }
}

void MainScreen::setShift(quint8 i, quint8 s)
{
    quint32 ss = s;
    switch (i) {
    case JUNIOR:  // 初级状态
        gShift = (gShift & 0xFFFFFF00) | ss;
        break;
    case MIDDLE:  // 次级状态
        gShift = (gShift & 0xFFFF00FF) | (ss << 8);
        break;
    case SENIOR:  // 末级状态
        gShift = (gShift & 0xFF00FFFF) | (ss << 16);
        break;
    default:
        break;
    }
}

quint8 MainScreen::getNextTestItem(int t)
{
    QJsonObject test = configs.value("Sys").toObject();
    QString tests = test.value("Test_Item").toString();
    if (tests.isEmpty())
        return 0;
    QStringList items = tests.split(",");
    int index = items.indexOf(QString::number(t));
    if (index == -1)
        return items.at(0).toInt();
    if (index == items.size() -1)
        return 0;
    else
        return items.at(index + 1).toInt();
}

