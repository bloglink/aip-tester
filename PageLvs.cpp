/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       低启模块
*******************************************************************************/
#include "PageLvs.h"
#include "ui_PageLvs.h"

PageLvs::PageLvs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageLvs)
{
    ui->setupUi(this);
    InitButtons();
    InitSettings();
    TestStatus = "free";
}

PageLvs::~PageLvs()
{
    delete ui;
}

void PageLvs::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExit, Qt::Key_0);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void PageLvs::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        SaveSettings();
        GoToWindow(NULL);
        break;
    default:
        break;
    }
}

void PageLvs::InitSettings()
{
    QSettings *global = new QSettings(INI_PATH, QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    PowerSupply = global->value("PowerSupply", "0").toInt();
    FileInUse = global->value("FileInUse", INI_DEFAULT).toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1").arg(global->value("FileInUse", INI_DEFAULT).toString());
    set = new QSettings(t, QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetLvs");

    QStringList temp = (set->value("Other", "20 0 0.5 10 10 50").toString()).split(" ");
    if (temp.size() >= 6) {
        ui->BoxVolt->setValue(temp.at(0).toDouble());
        ui->BoxTime->setValue(temp.at(1).toDouble());
        ui->BoxCurrMin->setValue(temp.at(2).toDouble());
        ui->BoxCurrMax->setValue(temp.at(3).toDouble());
        ui->BoxPowerMin->setValue(temp.at(4).toDouble());
        ui->BoxPowerMax->setValue(temp.at(5).toDouble());
    }
    qDebug() << QTime::currentTime().toString() << "PageLvs read OK";
}

void PageLvs::SaveSettings()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxVolt->value()));
    temp.append(QString::number(ui->BoxTime->value()));
    temp.append(QString::number(ui->BoxCurrMin->value()));
    temp.append(QString::number(ui->BoxCurrMax->value()));
    temp.append(QString::number(ui->BoxPowerMin->value()));
    temp.append(QString::number(ui->BoxPowerMax->value()));
    set->setValue("Other", (temp.join(" ").toUtf8()));
    qDebug() << QTime::currentTime().toString() << "PageLvs save OK";
}

void PageLvs::ExcuteCanCmd(int addr, QByteArray msg)
{
    if (addr != CAN_ID_PWR)
        return;
    if (TestStatus == "free")
        return;
    TimeOut = 0;
    quint8 cmd = (quint8)msg.at(0);
    switch (cmd) {
    case 0x00:
        ReadCanCmdStatus(msg);
        break;
    case 0x01:
        ReadCanCmdResult(msg);
        break;
    default:
        break;
    }
}

void PageLvs::SendCanCmdStart(quint8 s)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 t = ui->BoxTime->value()*10;
    quint16 v = ui->BoxVolt->value();
    quint8 p = PowerSupply << 4;
    quint8 g = 0x01;
    if (ui->BoxFreq->value() == 60)
        p += 0x02;
    if (s == WIN_ID_OUT14)
        g  <<= 4;
    out << quint16(0x27) << quint8(0x07) << quint8(0x01) << quint8(g)
        << quint8(t/256) << quint8(t%256) << quint8(p+v/256) << quint8(v%256)
        << quint8(0x00) << quint8(0x00);
    emit CanMsg(msg);
}

void PageLvs::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x27) << quint8(0x01) << quint8(0x02);
    emit CanMsg(msg);
}

void PageLvs::ReadCanCmdStatus(QByteArray msg)
{
    int s = quint8(msg.at(1));
    switch (s) {
    case 0x00:
        break;
    case 0x01:
        return;
    case 0x02:
        SendWarnning("FLASH_ERROR");
        break;
    case 0x03:
        SendWarnning("HV_ERROR");
        break;
    case 0x04:
        SendWarnning("WAVE_ERROR");
        break;
    default:
        SendWarnning("UNKONW_ERROR");
        break;
    }
    TestStatus = "free";
}

void PageLvs::ReadCanCmdResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double c = quint16(msg.at(3)*256)+quint8(msg.at(4));
    double p = quint16(msg.at(5)*256)+quint8(msg.at(6));
    Volt.append(v);
    Curr.append(c);
    Power.append(p);

    QString rrr = QString::number(Curr.last()/1000, 'f', 3);
    QString ppp = QString::number(Power.last()/10, 'f', 1);
    QString t = QString("%1A, %2W").arg(rrr).arg(ppp);
    ItemView[0].insert("TestResult", t);

    CalculateResult();
    if (Judge == "NG") {
        ItemView[0].insert("TestJudge", "STOP");
        SendCanCmdStop();
        ClearResults();
    }
    SendTestItems(0);
}

void PageLvs::CalculateResult()
{
    if (Volt.size() < 5 || Curr.size() < 5 || Power.size() < 5)
        return;
    double rr = Curr.last()/1000;
    double pp = Power.last()/10;

    if (rr > ui->BoxCurrMax->value() || rr < ui->BoxCurrMin->value() ||
            pp > ui->BoxPowerMax->value() || pp < ui->BoxPowerMin->value())
        Judge = "NG";
}

void PageLvs::ClearResults()
{
    Volt.clear();
    Curr.clear();
    Power.clear();
}

bool PageLvs::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (TestStatus != "free") {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageLvs::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void PageLvs::showEvent(QShowEvent *e)
{
    ui->BtnExit->setFocus();
    InitSettings();
    e->accept();
}

void PageLvs::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "PageLvs" && s.value("TxAddress") != "WinHome")
        return;
    if (s.value("TxCommand") == "ItemInit") {
        if (s.value("Station").toString() == "left")
            stat = WIN_ID_OUT13;
        if (s.value("Station").toString() == "right")
            stat = WIN_ID_OUT14;
        InitSettings();
        SendTestItemsAllEmpty();
    }
    if (s.value("TxCommand") == "StartTest")
        TestThread(s);
    if (s.value("TxCommand") == "TestStatus") {
        if (TestStatus == "free")
            return;
        if (s.value("TxMessage").toString() == "stop") {
            SendCanCmdStop();
            TestStatus = "stop";
        }
    }
    if (s.value("TxCommand").toString() == "TestSave")
        SendTestSave();
}

void PageLvs::GoToWindow(QString w)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "JumpWindow");
    hash.insert("TxMessage", w);
    emit SendVariant(hash);
}

void PageLvs::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("低启异常:\n%1").arg(s));
    emit SendVariant(hash);
}

void PageLvs::SendTestItemsAllEmpty()
{
    ItemView.clear();
    QString uid = QUuid::createUuid().toString();
    double M1 = ui->BoxCurrMin->value();
    double M2 = ui->BoxCurrMax->value();
    double Q1 = ui->BoxPowerMin->value();
    double Q2 = ui->BoxPowerMax->value();
    QVariantHash hash;
    hash.insert("TestEnable", "Y");
    hash.insert("TestItem", tr("低启"));
    hash.insert("TestPara", tr("%1-%2A %3-%4W").arg(M1).arg(M2).arg(Q1).arg(Q2));
    hash.insert("TestResult", " ");
    hash.insert("TestJudge", " ");
    hash.insert("TestUid", uid);
    ItemView.append(hash);

    for (int i=0; i < ItemView.size(); i++) {
        QVariantHash hash = ItemView.at(i);
        if (hash.value("TestEnable") == "Y") {
            hash.insert("TxAddress", "WinTest");
            hash.insert("TxCommand", "ItemView");
            emit SendVariant(hash);
        }
    }
}

void PageLvs::SendTestItemsAllError()
{
    for (int i=0; i < ItemView.size(); i++) {
        QVariantHash hash = ItemView.at(i);
        if (hash.value("TestEnable") == "Y") {
            hash.insert("TxAddress", "WinTest");
            hash.insert("TxCommand", "ItemUpdate");
            hash.insert("TestResult", "---");
            hash.insert("TestJudge", "NG");
            emit SendVariant(hash);
        }
    }
    qDebug() << "lvs test all error";
}

void PageLvs::SendTestItems(int num)
{
    QVariantHash hash = ItemView.at(num);
    if (hash.value("TestEnable") == "Y") {
        hash.insert("TxAddress", "WinTest");
        hash.insert("TxCommand", "ItemUpdate");
        emit SendVariant(hash);
    }
}

void PageLvs::SendTestPause()
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestPause");
    hash.insert("TxMessage", Judge);
    emit SendVariant(hash);
}

void PageLvs::SendTestSave()
{
    for (int i=0; i < ItemView.size(); i++) {
        QVariantHash hash = ItemView.at(i);
        if (hash.value("TestEnable") == "Y") {
            hash.insert("TxAddress", "WinHome");
            hash.insert("TxCommand", "TestSave");
            emit SendVariant(hash);
        }
    }
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestSave");
    hash.insert("ItemName", tr("低启"));
    hash.insert("TxMessage", Judge);
    emit SendVariant(hash);
}

void PageLvs::TestThread(QVariantHash hash)
{
    Judge = "OK";
    if (hash.value("Station").toString() == "left")
        stat = WIN_ID_OUT13;
    if (hash.value("Station").toString() == "right")
        stat = WIN_ID_OUT14;
    TestStatus = "buzy";
    Judge = "OK";
    SendCanCmdStart(stat);
    if (!WaitTimeOut(100)) {
        Judge = "NG";
        SendTestItemsAllError();
    } else {
        ItemView[0].insert("TestJudge", Judge);
        SendTestItems(0);
        ClearResults();
    }
    if (Judge == "NG")
        SendTestPause();
    TestStatus = "free";
}
/*********************************END OF FILE**********************************/
