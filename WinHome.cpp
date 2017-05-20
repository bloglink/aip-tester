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
    QWidget(parent, Qt::FramelessWindowHint),
    ui(new Ui::WinHome)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitVersion("V-2.1.1.170520");
    InitThreadAll();
    isPause = false;
    TestStatus = "free";
}

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
    thread_all->quit();
    thread_all->wait();
    delete ui;
}

void WinHome::InitThreadAll()
{
    QTimer *timer = new QTimer(this);
    InitCan();
    InitSql();
    InitTcp();
    InitUdp();
    InitSerial();
    timer->singleShot(50, this, SLOT(InitWindowsAll()));
}

void WinHome::InitWindows()
{
#ifdef __arm__
    ui->btnQuit->hide();
#endif
    ui->keybord->setCurrentIndex(0);
    //设置界面风格
    QFile file;
    QString qss;
    file.setFileName(":/source/SoftRainBlue.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);
}

void WinHome::InitWindowsAll()
{
    WinBack *winBack = new WinBack(this);
    ui->desktop->addWidget(winBack);
    winBack->setObjectName("WinBack");
    connect(winBack, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(winBack, SIGNAL(CanMsg(QByteArray)), this, SLOT(ExcuteCanCmd(QByteArray)));
    connect(this, SIGNAL(CanMsg(int, QByteArray)), winBack, SLOT(ExcuteCanCmd(int, QByteArray)));
    SendTestDebug("Initialize WinBack OK");

    WinSyst *winSyst = new WinSyst(this);
    ui->desktop->addWidget(winSyst);
    winSyst->setObjectName("WinSyst");
    connect(winSyst, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), winSyst, SLOT(ReadVariant(QVariantHash)));
    SendTestDebug("Initialize WinSyst OK");

    WinType *winType = new WinType(this);
    ui->desktop->addWidget(winType);
    winType->setObjectName("WinType");
    connect(winType, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), winType, SLOT(ReadVariant(QVariantHash)));
    SendTestDebug("Initialize WinType OK");

    WinData *winData = new WinData(this);
    ui->desktop->addWidget(winData);
    winData->setObjectName("WinData");
    connect(winData, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), winData, SLOT(ReadVariant(QVariantHash)));
    SendTestDebug("Initialize WinData OK");

    WinTest *winTest = new WinTest(this);
    ui->desktop->addWidget(winTest);
    winTest->setObjectName("WinTest");
    connect(winTest, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), winTest, SLOT(ReadVariant(QVariantHash)));
    SendTestDebug("Initialize WinTest OK");

    PageDcr *pageDcr = new PageDcr(this);
    ui->desktop->addWidget(pageDcr);
    pageDcr->setObjectName("PageDcr");
    connect(pageDcr, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), pageDcr, SLOT(ReadVariant(QVariantHash)));
    connect(pageDcr, SIGNAL(CanMsg(QByteArray)), this, SLOT(ExcuteCanCmd(QByteArray)));
    connect(this, SIGNAL(CanMsg(int, QByteArray)), pageDcr, SLOT(ExcuteCanCmd(int, QByteArray)));
    SendTestDebug("Initialize PageDcr OK");

    PageMag *pageMag = new PageMag(this);
    ui->desktop->addWidget(pageMag);
    pageMag->setObjectName("PageMag");
    connect(pageMag, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), pageMag, SLOT(ReadVariant(QVariantHash)));
    connect(pageMag, SIGNAL(CanMsg(QByteArray)), this, SLOT(ExcuteCanCmd(QByteArray)));
    connect(this, SIGNAL(CanMsg(int, QByteArray)), pageMag, SLOT(ExcuteCanCmd(int, QByteArray)));
    SendTestDebug("Initialize PageMag OK");

    PageInr *pageInr = new PageInr(this);
    ui->desktop->addWidget(pageInr);
    pageInr->setObjectName("PageInr");
    connect(pageInr, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), pageInr, SLOT(ReadVariant(QVariantHash)));
    connect(pageInr, SIGNAL(CanMsg(QByteArray)), this, SLOT(ExcuteCanCmd(QByteArray)));
    connect(this, SIGNAL(CanMsg(int, QByteArray)), pageInr, SLOT(ExcuteCanCmd(int, QByteArray)));
    SendTestDebug("Initialize PageInr OK");

    PageAcw *pageAcw = new PageAcw(this);
    ui->desktop->addWidget(pageAcw);
    pageAcw->setObjectName("PageAcw");
    connect(pageAcw, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), pageAcw, SLOT(ReadVariant(QVariantHash)));
    connect(pageAcw, SIGNAL(CanMsg(QByteArray)), this, SLOT(ExcuteCanCmd(QByteArray)));
    connect(this, SIGNAL(CanMsg(int, QByteArray)), pageAcw, SLOT(ExcuteCanCmd(int, QByteArray)));
    SendTestDebug("Initialize PageAcw OK");

    PageImp *pageImp = new PageImp(this);
    ui->desktop->addWidget(pageImp);
    pageImp->setObjectName("PageImp");
    connect(pageImp, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), pageImp, SLOT(ReadVariant(QVariantHash)));
    connect(pageImp, SIGNAL(CanMsg(QByteArray)), this, SLOT(ExcuteCanCmd(QByteArray)));
    connect(this, SIGNAL(CanMsg(int, QByteArray)), pageImp, SLOT(ExcuteCanCmd(int, QByteArray)));
    SendTestDebug("Initialize PageImp OK");

    PageInd *pageInd = new PageInd(this);
    ui->desktop->addWidget(pageInd);
    pageInd->setObjectName("PageInd");
    connect(pageInd, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), pageInd, SLOT(ReadVariant(QVariantHash)));
    connect(pageInd, SIGNAL(CanMsg(QByteArray)), this, SLOT(ExcuteCanCmd(QByteArray)));
    connect(this, SIGNAL(CanMsg(int, QByteArray)), pageInd, SLOT(ExcuteCanCmd(int, QByteArray)));
    SendTestDebug("Initialize PageInd OK");

    PagePwr *pagePwr = new PagePwr(this);
    ui->desktop->addWidget(pagePwr);
    pagePwr->setObjectName("PagePwr");
    connect(pagePwr, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), pagePwr, SLOT(ReadVariant(QVariantHash)));
    connect(pagePwr, SIGNAL(CanMsg(QByteArray)), this, SLOT(ExcuteCanCmd(QByteArray)));
    connect(this, SIGNAL(CanMsg(int, QByteArray)), pagePwr, SLOT(ExcuteCanCmd(int, QByteArray)));
    SendTestDebug("Initialize PagePwr OK");

    PageLvs *pageLvs = new PageLvs(this);
    ui->desktop->addWidget(pageLvs);
    pageLvs->setObjectName("PageLvs");
    connect(pageLvs, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), pageLvs, SLOT(ReadVariant(QVariantHash)));
    connect(pageLvs, SIGNAL(CanMsg(QByteArray)), this, SLOT(ExcuteCanCmd(QByteArray)));
    connect(this, SIGNAL(CanMsg(int, QByteArray)), pageLvs, SLOT(ExcuteCanCmd(int, QByteArray)));
    SendTestDebug("Initialize PageLvs OK");

    PageLck *pageLck = new PageLck(this);
    ui->desktop->addWidget(pageLck);
    pageLck->setObjectName("PageLck");
    connect(pageLck, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), pageLck, SLOT(ReadVariant(QVariantHash)));
    connect(pageLck, SIGNAL(CanMsg(QByteArray)), this, SLOT(ExcuteCanCmd(QByteArray)));
    connect(this, SIGNAL(CanMsg(int, QByteArray)), pageLck, SLOT(ExcuteCanCmd(int, QByteArray)));
    SendTestDebug("Initialize PageLck OK");

    PageOut *pageOut = new PageOut(this);
    ui->desktop->addWidget(pageOut);
    pageOut->setObjectName("PageOut");
    connect(pageOut, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), pageOut, SLOT(ReadVariant(QVariantHash)));
    connect(pageOut, SIGNAL(CanMsg(QByteArray)), this, SLOT(ExcuteCanCmd(QByteArray)));
    connect(this, SIGNAL(CanMsg(int, QByteArray)), pageOut, SLOT(ExcuteCanCmd(int, QByteArray)));
    SendTestDebug("Initialize PageOut OK");

    ReadStatusAll();
}

void WinHome::JumpToWindow(QByteArray win)
{
    int WinCurrent = ui->desktop->currentIndex();
    if (win.isEmpty()) { //空代表返回
        ui->desktop->setCurrentIndex(previous_window.last());
        previous_window.removeLast();
        return;
    }
    for (int i=0; i < ui->desktop->count(); i++) {
        if (ui->desktop->widget(i)->objectName() == win) {
            ui->desktop->setCurrentIndex(i);
            break;
        }
    }
    previous_window.append(WinCurrent);
    if (previous_window.size() > 10) { // 最大嵌套10层
        previous_window.removeFirst();
    }
}

void WinHome::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->btnSyst, Qt::Key_1);
    btnGroup->addButton(ui->btnType, Qt::Key_2);
    btnGroup->addButton(ui->btnData, Qt::Key_3);
    btnGroup->addButton(ui->btnTest, Qt::Key_4);
    btnGroup->addButton(ui->btnQuit, Qt::Key_5);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void WinHome::ReadButtons(int id)
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

void WinHome::InitVersion(QString v)
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    ini->setValue("/GLOBAL/Version", v);
    this->setWindowTitle(QString("电机综合测试仪%1").arg(v));
    ui->titleVn->setText(v);
}

void WinHome::InitCan()
{
    thread_can = new QThread(this);
    can.moveToThread(thread_can);
    connect(thread_can, SIGNAL(started()), &can, SLOT(DeviceOpen()));
    connect(thread_can, SIGNAL(finished()), &can, SLOT(DeviceQuit()));
    connect(this, SIGNAL(PutCanData(QByteArray)), &can, SLOT(WriteAll(QByteArray)));
    connect(&can, SIGNAL(GetCanData(QByteArray)), this, SLOT(ReadCanCmd(QByteArray)));
    thread_can->start();
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

void WinHome::InitUdp()
{
    thread_udp = new QThread(this);
    udp.moveToThread(thread_udp);
    connect(thread_udp, SIGNAL(started()), &udp, SLOT(Init()));
    connect(thread_udp, SIGNAL(finished()), &udp, SLOT(Quit()));
    connect(&udp, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), &udp, SLOT(ReadVariant(QVariantHash)));
    thread_udp->start();
}

void WinHome::InitSerial()
{
    thread_all = new QThread(this);
    btn.moveToThread(thread_all);
    connect(thread_all, SIGNAL(started()), &btn, SLOT(OpenSerial()));
    connect(thread_all, SIGNAL(finished()), &btn, SLOT(CloseSerial()));

    connect(&btn, SIGNAL(SendVariant(QVariantHash)), this, SLOT(ReadVariant(QVariantHash)));
    connect(this, SIGNAL(SendVariant(QVariantHash)), &btn, SLOT(ReadVariant(QVariantHash)));
    thread_all->start();
}

void WinHome::ReadCanCmd(QByteArray msg)
{
    if (!msg.isEmpty()) {
        quint16 id;
        quint8 dlc;
        quint8 dat;
        QByteArray cmd;
        QDataStream in(&msg,  QIODevice::ReadWrite);
        in.setVersion(QDataStream::Qt_4_8);

        while (!in.atEnd()) {
            in >> id >> dlc;
            cmd.clear();
            for (int i=0; i < dlc; i++) {
                in >> dat;
                cmd.append(dat);
            }
            if (id == CAN_ID_DCR && quint8(cmd.at(0)) == 0x09)
                ReadButtonBox(cmd);
            else
                emit CanMsg(id, cmd);
        }
    }
}

void WinHome::ReadMessage(quint16 addr,  quint16 cmd,  QByteArray msg)
{
    switch (cmd) {
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
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestInit");
    hash.insert("Station", tr("左"));
    emit SendVariant(hash);

    hash.insert("TxCommand", "ItemInit");
    QStringList n = CurrentItems();
    for (int i=0; i < n.size(); i++) {
        QString s = WinName(n.at(i).toInt());
        hash.insert("TxAddress", s);
        emit SendVariant(hash);
    }

    hash.insert("TxAddress", "WinTest");
    hash.insert("TxCommand", "ItemShow");
    emit SendVariant(hash);
}

void WinHome::ReadStatusAll()
{
    if (TestStatus != "free")
        return;
    TestStatus = "init";

    QVariantHash hash;
    hash.insert("TxCommand", "CheckStatus");
    QStringList t = EnableItems();
    for (int i=0; i < t.size(); i++) {
        hash.insert("TxAddress", WinName(t.at(i).toInt()));
        emit SendVariant(hash);
    }
    QStringList s = EnableOutput();
    for (int i=0; i < s.size(); i++) {
        hash.insert("TxAddress", "PageOut");
        if (s.at(i).toInt() == 0)
            hash.insert("Station", "left");
        if (s.at(i).toInt() == 1)
            hash.insert("Station", "right");
        emit SendVariant(hash);
    }
    qDebug() << QTime::currentTime().toString() << "WinHome read OK";

    ui->Text->hide();
    ui->keybord->setCurrentIndex(1);

    TestStatus = "free";

    HomeMode = HOME_FREE;
    if (ui->desktop->currentWidget()->objectName() == "MainPage") {
        Delay(1000);
        JumpToWindow("WinTest");
    }

}

void WinHome::ReStartTest()
{
    //    switch (CurrentReStartMode()) {
    //    case 0:
    //        break;
    //    case 1:
    //        TestThread(QString("%1").arg(0x13).toUtf8());
    //        break;
    //    case 2:
    //        TestThread(QString("%1").arg(0x14).toUtf8());
    //        break;
    //    case 3:
    //        TestThread(stat);
    //        break;
    //    case 4:
    //        if (stat.toInt() == 0x13) {
    //            TestThread(QString("%1").arg(0x14).toUtf8());
    //            break;
    //        }
    //        if (stat.toInt() == 0x14) {
    //            TestThread(QString("%1").arg(0x13).toUtf8());
    //            break;
    //        }
    //        break;
    //    default:
    //        break;
    //    }
}

void WinHome::SaveTestJudge()
{
    QString s = QString(tr("总数@%1@%2")).arg(CurrentSettings()).arg(Judge);
    emit WriteSql(s.toUtf8());
}

void WinHome::SaveItemJudge(QByteArray msg)
{
    emit WriteSql(msg);
    QStringList s = QString(msg).split("@");
    if (s.size() < 3)
        return;
    if (s.at(2) == "NG")
        Judge = "NG";
    if (s.at(2) == "NG" && CurrentPauseMode() != 1)
        TestPause();
}

void WinHome::TestPause()
{
    SendTestStatus("pause");

    QString text = tr("此项目不合格,是否重测");
    PopupBox *box = new PopupBox(this, "", text, QMessageBox::Retry, QMessageBox::Ok);
    connect(this, SIGNAL(SendVariant(QVariantHash)), box, SLOT(ReadVariant(QVariantHash)));
    int ret = box->exec();
    if (ret == QMessageBox::Retry) {
        QVariantHash hash;
        hash.insert("TxAddress", WinName(Current_Test_Item));
        hash.insert("TxCommand", "ItemInit");
        emit SendVariant(hash);
        hash.insert("TxCommand", "StartTest");
        emit SendVariant(hash);
        Delay(10);
    }
    SendTestStatus("buzy");
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

QString WinHome::WinName(int n)
{
    switch (n) {
    case WIN_ID_DCR:
        return "PageDcr";
    case WIN_ID_MAG:
        return "PageMag";
    case WIN_ID_INR:
        return "PageInr";
    case WIN_ID_ACW:
        return "PageAcw";
    case WIN_ID_IMP:
        return "PageImp";
    case WIN_ID_IND:
        return "PageInd";
    case WIN_ID_PWR:
        return "PagePwr";
    case WIN_ID_LVS:
        return "PageLvs";
    case WIN_ID_LCK:
        return "PageLck";
    default:
        return NULL;
        break;
    }
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

QStringList WinHome::EnableOutput()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/OutEnable", INI_DEFAULT).toString();
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

QString WinHome::CurrentUser()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    int s = ini->value("/GLOBAL/User", "0").toInt();
    if (s == 0)
        return tr("guest");
    else
        return tr("admin");
}

void WinHome::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "WinHome") {
        emit SendVariant(s);
        return;
    }
    if (s.value("TxCommand") == "Warnning")
        Warnning(s);
    if (s.value("TxCommand") == "StartTest")
        TestThread(s);
    if (s.value("TxCommand") == "InitTest")
        InitTest(s);
    if (s.value("TxCommand") == "StopTest")
        StopTest(s);
    if (s.value("TxCommand") == "JumpWindow")
        JumpToWindow(s.value("TxMessage").toByteArray());
    if (s.value("TxCommand") == "ReadStatus")
        ReadStatusAll();
    if (s.value("TxCommand") == "TestPause") {
        Judge = s.value("TxMessage").toString();
        if (CurrentPauseMode() != 1)
            TestPause();
    }
    if (s.value("TxCommand") == "TestSave")
        ReadTestSave(s);
}

void WinHome::ReadButtonBox(QByteArray msg)
{
    if (quint8(msg.at(1)) != 0)
        SendButtonBox("Retry");
    if (quint8(msg.at(2)) != 0)
        SendButtonBox("Ok");
}

void WinHome::Warnning(QVariantHash hash)
{
    QString text = hash.value("TxMessage").toString();
    SendTestDebug(text);
    PopupBox *box = new PopupBox(this, "", text, NULL, QMessageBox::Ok);
    connect(this, SIGNAL(SendVariant(QVariantHash)), box, SLOT(ReadVariant(QVariantHash)));
    box->exec();
}

void WinHome::SendButtonBox(QString button)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "BoxButton");
    hash.insert("TxMessage", button);
    emit SendVariant(hash);
}

void WinHome::SendTestStatus(QString msg)
{
    TestStatus = msg;
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestStatus");
    hash.insert("TxMessage", msg);
    emit SendVariant(hash);
}

void WinHome::SendTestSave()
{
    QVariantHash hash;
    QStringList n = CurrentItems();
    for (int i=0; i < n.size(); i++) {
        hash.insert("TxAddress", WinName(n.at(i).toInt()));
        hash.insert("TxCommand", "TestSave");
        emit SendVariant(hash);
    }
    QString v = "总数1";
    v += "@" + CurrentUser();
    v += "@" + tr("code");
    emit WriteSql(v.toUtf8());

    v = "总数";
    v += "@" + CurrentSettings();
    v += "@" + Judge;
    emit WriteSql(v.toUtf8());
}

void WinHome::ReadTestSave(QVariantHash s)
{
    QString v = s.value("ItemName").toString();
    if (s.value("TestResult").toString().isEmpty()) {
        v += "@" + CurrentSettings();
        v += "@" + s.value("TxMessage").toString();
    } else {
        v += "@" + s.value("TestItem").toString();
        v += " " + s.value("TestPara").toString();
        v += "@" + s.value("TestResult").toString();
        v += "@" + s.value("TestJudge").toString();
    }
    emit WriteSql(v.toUtf8());
}

void WinHome::SendTestAlarm(QString msg)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestAlarm");
    if (msg == "OK") {
        hash.insert("TxMessage", "LEDG BEEP");
        emit SendVariant(hash);
        Delay(CurrentAlarmTime("OK"));
        hash.insert("TxMessage", "LEDG");
        emit SendVariant(hash);
    }
    if (msg == "NG") {
        hash.insert("TxMessage", "LEDR BEEP");
        emit SendVariant(hash);
        Delay(CurrentAlarmTime("NG"));
        hash.insert("TxMessage", "LEDR");
        emit SendVariant(hash);
    }
    if (msg == "LEDY") {
        hash.insert("TxMessage", "LEDY");
        emit SendVariant(hash);
    }
    if (msg == "pause") {
        hash.insert("TxMessage", "LEDY LEDR BEEP");
        emit SendVariant(hash);
        Delay(CurrentAlarmTime("NG"));
        hash.insert("TxMessage", "LEDY red");
        emit SendVariant(hash);
    }
}

void WinHome::SendTestDebug(QString msg)
{
    if (!ui->Text->isHidden()) {
        ui->Text->insertPlainText(msg);
        ui->Text->insertPlainText("\n");
        ui->Text->moveCursor(QTextCursor::EndOfBlock);
        Delay(1);
    }
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestDebug");
    hash.insert("TxMessage", msg);
    emit SendVariant(hash);
}

void WinHome::SendTestJudge(QString msg)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinTest");
    hash.insert("TxCommand", "TestJudge");
    hash.insert("TxMessage", msg);
    emit SendVariant(hash);
}

void WinHome::TestThread(QVariantHash hash)
{
    qDebug() << "win test thread" << TestStatus;
    if (ui->desktop->currentWidget()->objectName() != "WinTest")
        return;
    if (TestStatus != "free")
        return;
    if (!IsStartModeRight(hash)) {
        hash.insert("TxMessage", tr("启动方式错误"));
        Warnning(hash);
        return;
    }
    Judge = "OK";
    SendTestStatus("buzy");
    SendTestAlarm("LEDY");
    InitTest(hash);
    QStringList n = CurrentItems();
    for (int i=0; i < n.size(); i++) {
        Delay(10);
        if (TestStatus == "stop") {
            Judge = "NG";
            break;
        }
        Current_Test_Item = n.at(i).toInt();
        hash.insert("TxAddress", WinName(n.at(i).toInt()));
        emit SendVariant(hash);
    }
    SendTestSave();
    SendTestAlarm(Judge);
    SendTestJudge(Judge);
    SendTestStatus("free");
}

void WinHome::InitTest(QVariantHash hash)
{
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestInit");
    emit SendVariant(hash);

    hash.insert("TxCommand", "ItemInit");
    QStringList n = CurrentItems();
    for (int i=0; i < n.size(); i++) {
        QString s = WinName(n.at(i).toInt());
        hash.insert("TxAddress", s);
        emit SendVariant(hash);
    }

    hash.insert("TxAddress", "WinTest");
    hash.insert("TxCommand", "ItemShow");
    emit SendVariant(hash);
}

void WinHome::StopTest(QVariantHash hash)
{
    if (TestStatus != "free") {
        SendTestStatus("stop");
        SendButtonBox("Ok");
        return;
    }
    InitTest(hash);
}

bool WinHome::IsStartModeRight(QVariantHash hash)
{
    QString s;
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    switch (ini->value("/GLOBAL/Mode", "0").toInt()) {
    case 0:
        s = "btn";
        break;
    case 1:
        s = "out";
    case 2:
        s = "out";
        break;
    case 3:
        s = "udp";
        break;
    default:
        break;
    }
    if (hash.value("StartMode").toString() == s)
        return true;
    else
        return false;
}

void WinHome::ExcuteCanCmd(QByteArray msg)
{
    emit PutCanData(msg);
}

/*********************************END OF FILE**********************************/
