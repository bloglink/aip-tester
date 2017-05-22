/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       堵转模块
*******************************************************************************/
#include "PageLck.h"
#include "ui_PageLck.h"

PageLck::PageLck(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageLck)
{
    ui->setupUi(this);
    InitButtons();
    InitSettings();
    TestStatus = "free";
}

PageLck::~PageLck()
{
    delete ui;
}

void PageLck::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSample, Qt::Key_0);
    btnGroup->addButton(ui->BtnExit, Qt::Key_2);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void PageLck::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        ui->BtnExit->setEnabled(false);
        for (int i=0; i < ui->BoxSampleTimes->value(); i++) {
            ui->BtnSample->setText(QString("采样中%1").arg(i+1));
            TestStatus = "sample";
            SendCanCmdStart(WIN_ID_OUT13);
            if (!WaitTimeOut(1000)) {
                SendWarnning(tr("采样失败"));
                ClearResults();
                ui->BtnSample->setText(QString("左工位采样"));
                ui->BtnExit->setEnabled(true);
                TestStatus = "free";
                return;
            }
            TestStatus = "free";
            Delay(ui->BoxSampleDelay->value()*1000);
        }
        CalculateSample();
        ClearResults();
        ui->BtnSample->setText(QString("左工位采样"));
        ui->BtnExit->setEnabled(true);
        break;
    case Qt::Key_2:
        SaveSettings();
        GoToWindow(NULL);
        break;
    default:
        break;
    }
}

void PageLck::InitSettings()
{
    QSettings *global = new QSettings(INI_PATH, QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    PowerSupply = global->value("PowerSupply", "0").toInt();
    FileInUse = global->value("FileInUse", INI_DEFAULT).toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(FileInUse);
    set = new QSettings(t, QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("PageLck");

    ui->BoxVolt->setValue(set->value("Voltage", "220").toDouble());
    ui->BoxTime->setValue(set->value("TimeUse", "1").toDouble());
    ui->BoxGrade->setValue(set->value("Grade", "1").toDouble());
    ui->BoxVoltMax->setValue(set->value("VoltMax", "255").toDouble());
    ui->BoxVoltMin->setValue(set->value("VoltMin", "1").toDouble());
    ui->BoxCurrMax->setValue(set->value("CurrentMax", "5.000").toDouble());
    ui->BoxCurrMin->setValue(set->value("CurrentMin", "0.001").toDouble());
    ui->BoxPowerMax->setValue(set->value("PowerMax", "1275").toDouble());
    ui->BoxPowerMin->setValue(set->value("PowerMin", "0.1").toDouble());
    ui->BoxFreq->setValue(set->value("Freq", "1").toDouble());
    ui->BoxCurr->setValue(set->value("Current", "0.001").toDouble());
    ui->BoxPower->setValue(set->value("Power", "1").toDouble());
    qDebug() << QTime::currentTime().toString() << "PageLck read OK";
}

void PageLck::SaveSettings()
{
    set->setValue("Voltage", QString::number(ui->BoxVolt->value()));
    set->setValue("TimeUse", QString::number(ui->BoxTime->value()));
    set->setValue("Grade", QString::number(ui->BoxGrade->value()));
    set->setValue("VoltMax", QString::number(ui->BoxVoltMax->value()));
    set->setValue("VoltMin", QString::number(ui->BoxVoltMin->value()));
    set->setValue("CurrentMax", QString::number(ui->BoxCurrMax->value()));
    set->setValue("CurrentMin", QString::number(ui->BoxCurrMin->value()));
    set->setValue("PowerMax", QString::number(ui->BoxPowerMax->value()));
    set->setValue("PowerMin", QString::number(ui->BoxPowerMin->value()));
    set->setValue("Freq", QString::number(ui->BoxFreq->value()));
    set->setValue("Current", QString::number(ui->BoxCurr->value()));
    set->setValue("Power", QString::number(ui->BoxPower->value()));
    qDebug() << QTime::currentTime().toString() << "PageLck save OK";
}

void PageLck::ExcuteCanCmd(int addr, QByteArray msg)
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

void PageLck::SendCanCmdStart(quint8 s)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 t = ui->BoxTime->value()*100;
    quint16 v = ui->BoxVolt->value();
    quint8 p = PowerSupply << 4;
    quint8 g = 0x01;
    if (ui->BoxFreq->value() == 60)
        p += 0x02;
    if (s == WIN_ID_OUT14)
        g  <<= 4;
    out << quint16(0x27) << quint8(0x05) << quint8(0x03) << quint8(g)
        << quint8(t%256) << quint8(p+v/256) << quint8(v%256);
    emit CanMsg(msg);
}



void PageLck::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x27) << quint8(0x01) << quint8(0x02);
    emit CanMsg(msg);
}

void PageLck::SendItemJudge()
{
    int num = ui->BoxFreq->value();
    if (Volt.size() < num)
        return;
    if (Curr.size() < num)
        return;
    if (Power.size() < num)
        return;
    double vv = Volt.at(num)/10;
    double rr = Curr.at(num)/1000;
    double pp = Power.at(num)/10;

    QString t = QString("%1V, %2A, %3W").arg(vv).arg(rr).arg(pp);

    if (rr > ui->BoxCurrMax->value() || rr < ui->BoxCurrMin->value() )
        Judge = "NG";
    if (pp > ui->BoxPowerMax->value() || pp < ui->BoxPowerMin->value() )
        Judge = "NG";
    ItemView[0].insert("TestResult", t);
    ItemView[0].insert("TestJudge", Judge);
    SendTestItems(0);
    ClearResults();
}

void PageLck::CalculateSample()
{
    int SampleTimes = ui->BoxSampleTimes->value();
    int Num = Power.size()/SampleTimes;

    int Power1 = 0;
    int Variance = 0;
    QList<int> Variances;
    for (int k=0; k < SampleTimes; k++) {
        Power1 = 0;
        Variance = 0;
        for (int i=0; i < Num; i++)
            Power1 += Power.at(i*SampleTimes+k);
        Power1 = Power1/Num;   // 得到平均数
        for (int i=0; i < Num; i++) // 求取方差
            Variance += ((Power1-Power.at(i*SampleTimes+k)))*((Power1-Power.at(i*SampleTimes+k)));
        Variance = Variance / Num;
        Variances.append(Variance);
    }
    Variances.removeAt(Variances.size()-1);
    Variances.removeAt(1);
    Variances.removeAt(0);
    int Trough = Variances.at(0);
    int Trough_Point = 2;

    for (int i=0; i < Variances.size()-1; i++) {  // 寻找测试点
        if ((Trough > Variances.at(i+1)) && (Trough > 0)) {
            Trough = Variances.at(i+1);
            Trough_Point = i+3;
        } else if (Trough < Variances.at(i+1)) {
            break;
        } else if (Trough == 0) {
            break;
        }
    }
    ui->BoxFreq->setValue(Trough_Point+1);

    double Current1 = 0;
    for (int i=0; i < Num; i++)
        Current1 += Curr.at(i*SampleTimes+Trough_Point);

    Current1 = Current1/Num;
    ui->BoxCurr->setValue(Current1/1000);

    double pwr_average = 0; // 功率平均数
    for (int i=0; i < Num; i++)
        pwr_average += Power.at(i*SampleTimes+Trough_Point);
    pwr_average = pwr_average/Num;
    ui->BoxPower->setValue(pwr_average/10);
}

void PageLck::ReadCanCmdStatus(QByteArray msg)
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

void PageLck::ReadCanCmdResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double c = quint16(msg.at(3)*256)+quint8(msg.at(4));
    double p = quint16(msg.at(5)*256)+quint8(msg.at(6));
    Volt.append(v);
    Curr.append(c);
    Power.append(p);
}

void PageLck::ClearResults()
{
    Volt.clear();
    Curr.clear();
    Power.clear();
}

QString PageLck::CurrentPorwer()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/PowerSupply", "0").toString();
    return n;
}

bool PageLck::WaitTimeOut(quint16 t)
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

void PageLck::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void PageLck::showEvent(QShowEvent *e)
{
    ui->BtnExit->setFocus();
    InitSettings();
    e->accept();
}

void PageLck::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "PageLck" && s.value("TxAddress") != "WinHome")
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

void PageLck::GoToWindow(QString w)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "JumpWindow");
    hash.insert("TxMessage", w);
    emit SendVariant(hash);
}

void PageLck::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("堵转异常:\n%1").arg(s));
    emit SendVariant(hash);
}

void PageLck::SendTestItemsAllEmpty()
{
    ItemView.clear();
    QString uid = QUuid::createUuid();
    QString U1 = QString::number(ui->BoxVoltMin->value());
    QString U2 = QString::number(ui->BoxVoltMax->value());
    QString C1 = QString::number(ui->BoxCurrMin->value());
    QString C2 = QString::number(ui->BoxCurrMax->value());
    QString P1 = QString::number(ui->BoxPowerMin->value());
    QString P2 = QString::number(ui->BoxPowerMax->value());
    QVariantHash hash;
    hash.insert("TestEnable", "Y");
    hash.insert("TestItem", tr("堵转"));
    hash.insert("TestPara", tr("%1~%2V %3~%4A %5~%6W").arg(U1).arg(U2).arg(C1).arg(C2).arg(P1).arg(P2));
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

void PageLck::SendTestItemsAllError()
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
    qDebug() << "lck test all error";
}

void PageLck::SendTestItems(int num)
{
    if (Volt.size() < num) {
        SendTestItemsAllError();
        return;
    }

    double vv = Volt.at(num)/10;
    double rr = Curr.at(num)/1000;
    double pp = Power.at(num)/10;

    QString t = QString("%1V %2A %3W").arg(vv).arg(rr).arg(pp);

    if (rr > ui->BoxCurrMax->value() || rr < ui->BoxCurrMin->value() )
        Judge = "NG";
    if (pp > ui->BoxPowerMax->value() || pp < ui->BoxPowerMin->value() )
        Judge = "NG";
    ItemView[0].insert("TestResult", t);
    ItemView[0].insert("TestJudge", Judge);
    ItemView[0].insert("TestResult1", tr("电压:%1V").arg(vv));
    ItemView[0].insert("TestResult2", tr("电流:%1A").arg(rr));
    ItemView[0].insert("TestResult3", tr("功率:%1W").arg(pp));


    QVariantHash hash = ItemView.at(0);
    if (hash.value("TestEnable") == "Y") {
        hash.insert("TxAddress", "WinTest");
        hash.insert("TxCommand", "ItemUpdate");
        emit SendVariant(hash);
    }
}

void PageLck::SendTestPause()
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestPause");
    hash.insert("TxMessage", Judge);
    emit SendVariant(hash);
}

void PageLck::SendTestSave()
{
    for (int i=0; i < ItemView.size(); i++) {
        QVariantHash hash = ItemView.at(i);
        if (hash.value("TestEnable") == "Y") {
            QStringList s;
            s.append(hash.value("TestResult1").toString());
            s.append(hash.value("TestResult2").toString());
            s.append(hash.value("TestResult3").toString());
            hash.insert("TestResult", s.join(","));
            hash.insert("TxAddress", "WinHome");
            hash.insert("TxCommand", "TestSave");
            emit SendVariant(hash);
        }
    }
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestSave");
    hash.insert("ItemName", tr("堵转"));
    hash.insert("TxMessage", Judge);
    emit SendVariant(hash);
}

void PageLck::TestThread(QVariantHash hash)
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
        SendTestItems(ui->BoxFreq->value());
        ClearResults();
    }
    if (Judge == "NG")
        SendTestPause();
    TestStatus = "free";
}
/*********************************END OF FILE**********************************/
