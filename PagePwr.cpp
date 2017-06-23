/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       功率模块
*******************************************************************************/
#include "PagePwr.h"
#include "ui_PagePwr.h"

PagePwr::PagePwr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PagePwr)
{
    ui->setupUi(this);
    InitWin();
    InitSet();
    TestStatus = "free";
}

PagePwr::~PagePwr()
{
    delete ui;
}

void PagePwr::InitWin()
{
    this->setFocus();
    m = new StandardItemModel(PWR_MAX, 10);

    QStringList headerList;
    headerList << tr("功率") << tr("电流下限") << tr("电流上限") << tr("功率下限") << tr("功率上限")
               << tr("容压下限") << tr("容压上限") << tr("转向") << tr("时间") << tr("档位");
    m->setHorizontalHeaderLabels(headerList);

    ui->parameters->setModel(m);
    ui->parameters->setItemDelegateForColumn(0, new EnableDelegate(this));
    ui->parameters->setItemDelegateForColumn(1, new CurrDelegate);
    ui->parameters->setItemDelegateForColumn(2, new CurrDelegate);
    ui->parameters->setItemDelegateForColumn(3, new DcrDelegate);
    ui->parameters->setItemDelegateForColumn(4, new DcrDelegate);
    ui->parameters->setItemDelegateForColumn(5, new DcrDelegate);
    ui->parameters->setItemDelegateForColumn(6, new DcrDelegate);
    ui->parameters->setItemDelegateForColumn(7, new DirDelegate);
    ui->parameters->setItemDelegateForColumn(8, new DcrDelegate);
    ui->parameters->setItemDelegateForColumn(9, new ReadOnlyDelegate);
    ui->parameters->setEditTriggers(QAbstractItemView::CurrentChanged);

#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->parameters->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(5, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(6, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(7, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(8, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(9, QHeaderView::Stretch);
    ui->parameters->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->parameters->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);
    ui->parameters->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
}

void PagePwr::InitSet()
{
    QSettings *g = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString n = g->value("/GLOBAL/FileInUse", "aip9918.ini").toString().remove(".ini");
    QString s = QString("./config/%1.ini").arg(n);
    set = new QSettings(s, QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("PagePwr");

    QStringList temp = (set->value("Other", "220 50 1 5").toString()).split(" ");
    if (temp.size() >= 3) {
        ui->BoxVolt->setValue(temp.at(0).toDouble());
        ui->BoxFreq->setValue(temp.at(1).toDouble());
        ui->BoxCap->setValue(temp.at(2).toDouble());
    }
    //可用
    temp = (QString(set->value("Enable", "Y Y Y N N N").toByteArray())).split(" ");
    for (int i=0; i < qMin(temp.size(), PWR_MAX); i++)
        m->setItem(i,0,new QStandardItem(temp.at(i)));
    //最小电流
    temp = (set->value("CurrMin", "0 0 0 0 0 0").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), PWR_ROW); i++)
        m->setItem(i,1,new QStandardItem(temp.at(i)));
    //最大电流
    temp = (set->value("CurrMax", "5 5 5 5 5 5").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), PWR_ROW); i++)
        m->setItem(i,2,new QStandardItem(temp.at(i)));
    //最小功率
    temp = (set->value("PowerMin", "0 0 0 0 0 0").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), PWR_ROW); i++)
        m->setItem(i,3,new QStandardItem(temp.at(i)));
    //最大功率
    temp = (set->value("PowerMax", "500 500 500 500 500 500").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), PWR_ROW); i++)
        m->setItem(i,4,new QStandardItem(temp.at(i)));
    //最小容压
    temp = (set->value("CapMin", "0 0 0 0 0 0").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), PWR_ROW); i++)
        m->setItem(i,5,new QStandardItem(temp.at(i)));
    //最大容压
    temp = (set->value("CapMax", "500 500 500 500 500 500").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), PWR_ROW); i++)
        m->setItem(i,6,new QStandardItem(temp.at(i)));
    //转向
    QStringList ss;
    ss << tr("不转") <<  tr("正转") << tr("反转");
    temp = (QString(set->value("TestDir", "0 0 0 0 0 0 0 0").toByteArray())).split(" ");
    for (int i=0; i < qMin(temp.size(), PWR_ROW); i++)
        m->setItem(i,7,new QStandardItem(ss.at(QString(temp.at(i)).toInt())));
    //测试时间
    temp = (set->value("TestTime", "1 1 1 1 1 1").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), PWR_ROW); i++)
        m->setItem(i,8,new QStandardItem(temp.at(i)));
    temp.clear();
    temp << "123" << "124" << "125" << "126" << "127" << "128";
    for (int i=0; i < qMin(temp.size(), PWR_ROW); i++)
        m->setItem(i,9,new QStandardItem(temp.at(i)));
}

void PagePwr::SaveSet()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxVolt->value()));
    temp.append(QString::number(ui->BoxFreq->value()));
    temp.append(QString::number(ui->BoxCap->value()));
    set->setValue("Other", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < PWR_MAX; i++)
        temp.append(m->item(i,0)->text());
    set->setValue("Enable", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < PWR_MAX; i++)
        temp.append(m->item(i,1)->text());
    set->setValue("CurrMax", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < PWR_MAX; i++)
        temp.append(m->item(i,2)->text());
    set->setValue("CurrMin", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < PWR_MAX; i++)
        temp.append(m->item(i,3)->text());
    set->setValue("PowerMax", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < PWR_MAX; i++)
        temp.append(m->item(i,4)->text());
    set->setValue("PowerMin", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < PWR_MAX; i++)
        temp.append(m->item(i,5)->text());
    set->setValue("CapMax", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < PWR_MAX; i++)
        temp.append(m->item(i,6)->text());
    set->setValue("CapMin", (temp.join(" ").toUtf8()));
    temp.clear();
    QStringList ss;
    ss << tr("不转") <<  tr("正转") << tr("反转");
    for (int i=0; i < PWR_MAX; i++)
        temp.append(QString::number(ss.lastIndexOf(m->item(i,7)->text())));
    set->setValue("TestDir", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < PWR_MAX; i++)
        temp.append(m->item(i,8)->text());
    set->setValue("TestTime", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < PWR_MAX; i++)
        temp.append(m->item(i,9)->text());
    set->setValue("Grade", (temp.join(" ").toUtf8()));
}

void PagePwr::ExcuteCanCmd(int addr,  QByteArray msg)
{
    quint8 cmd = (quint8)msg.at(0);
    if (TestStatus == "free" && cmd != 0x06)
        return;
    TimeOut = 0;

    switch (cmd) {
    case 0x00:
        ReadCanCmdStatus(msg);
        break;
    case 0x01:
        ReadCanCmdResult(msg);
        break;
    case 0x02:
        ReadCanCmdDir(msg);
        break;
    case 0x03:
        ReadCanCmdVolt(msg);
        break;
    default:
        qDebug() << addr << msg.toHex();
        break;
    }
}
void PagePwr::ReadCanCmdStatus(QByteArray msg)
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

void PagePwr::ReadCanCmdResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double c = quint16(msg.at(3)*256)+quint8(msg.at(4));
    double p = quint16(msg.at(5)*256)+quint8(msg.at(6));
    Volt.append(v);
    Curr.append(c);
    Power.append(p);
    if (TestStatus == "safe" && Volt.size() > 4 && Volt.last() > 50) {
        QVariantHash hash;
        hash.insert("TxAddress", "WinHome");
        hash.insert("TxCommand", "StopTest");
        emit SendVariant(hash);
        SendWarnning(tr("请等待电机转动停止再测试"));
    }
}

void PagePwr::ReadCanCmdDir(QByteArray msg)
{
    if (TestStatus == "safe")
        return;
    QString dir;
    if (quint8(msg.at(5)) == 0x00)
        dir = tr("不转");
    if (quint8(msg.at(5)) == 0x01)
        dir = tr("反转");
    if (quint8(msg.at(5)) == 0x02)
        dir = tr("正转");
    Dir.append(dir);
    if (Dir.size() > 4) {
        DirView[TestRow].insert("TestResult", dir);
        SendDirItems(TestRow);
    }
}

void PagePwr::ReadCanCmdVolt(QByteArray msg)
{
    if (TestStatus == "safe")
        return;
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    CVolt.append(v);

    QString vvv = QString::number(Volt.last()/10, 'f', 1);
    QString rrr = QString::number(Curr.last()/1000, 'f', 3);
    QString ppp = QString::number(Power.last()/10, 'f', 1);
    QString ccc = QString::number(CVolt.last()/10, 'f', 1);
    QString t = QString("%1V,%2A,%3W,%4V").arg(vvv).arg(rrr).arg(ppp).arg(ccc);
    CalculateResult();
    PwrView[TestRow].insert("TestResult", t);
    PwrView[TestRow].insert("TestResult1", tr("%1").arg(vvv));
    PwrView[TestRow].insert("TestResult2", tr("%1").arg(rrr));
    PwrView[TestRow].insert("TestResult3", tr("%1").arg(ppp));
    PwrView[TestRow].insert("TestResult4", tr("%1").arg(ccc));
    PwrView[TestRow].insert("TestJudge", PwrJudge);
    if (PwrJudge == "NG") {
        PwrView[TestRow].insert("TestJudge", PwrJudge);
        SendCanCmdStop();
        ClearResults();
    }
    SendPwrItems(TestRow);
}

void PagePwr::SendCanCmdStatus()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x27) << quint8(0x01) << quint8(0x00);
    emit CanMsg(msg);
}

void PagePwr::SendCanCmdStart(quint8 s)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 v = ui->BoxVolt->value();
    quint16 t = m->item(TestRow,8)->text().toDouble()*10;
    quint8 p = CurrentPorwer().toInt() << 4;
    quint8 vv = 0;
    quint8 g = TestRow+1;
    if (ui->BoxFreq->value() == 60)
        p += 0x02;
    if (s == 0x14)
        g  <<= 4;
    quint8 dir = CurrentPowerDir();
    if (m->item(TestRow,7)->text() == tr("不转"))
        dir = 0;
    out << quint16(0x27) << quint8(0x08) << quint8(0x01) << quint8(g)
        << quint8(t/256) << quint8(t%256) << quint8(p+v/256) << quint8(v%256)
        << quint8(dir) << quint8(vv);
    emit CanMsg(msg);
}

void PagePwr::SendCanCmdStartSafe()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint8 g = TestRow+1;
    if (stat == 0x14)
        g  <<= 4;
    quint8 p = CurrentPorwer().toInt() << 4;
    if (ui->BoxFreq->value() == 60)
        p += 0x02;

    out << quint16(0x27) << quint8(0x08) << quint8(0x01) << quint8(g)
        << quint8(0) << quint8(5) << quint8(p) << quint8(0)
        << quint8(0) << quint8(0);
    emit CanMsg(msg);
}

void PagePwr::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x27) << quint8(0x01) << quint8(0x02);
    emit CanMsg(msg);
}

void PagePwr::CalculateResult()
{
    if (Volt.size() < 5 || Curr.size() < 5 || Power.size() < 5 || CVolt.size() < 5)
        return;
    double rr = Curr.last()/1000;
    double pp = Power.last()/10;
    double cc = CVolt.last()/10;
    if (rr < m->item(TestRow,1)->text().toDouble())
        PwrJudge = "NG";
    if (rr > m->item(TestRow,2)->text().toDouble())
        PwrJudge = "NG";
    if (pp < m->item(TestRow,3)->text().toDouble())
        PwrJudge = "NG";
    if (pp > m->item(TestRow,4)->text().toDouble())
        PwrJudge = "NG";
    if (cc < m->item(TestRow,5)->text().toDouble())
        PwrJudge = "NG";
    if (cc > m->item(TestRow,6)->text().toDouble())
        PwrJudge = "NG";
}

void PagePwr::ClearResults()
{
    Volt.clear();
    Curr.clear();
    Power.clear();
    CVolt.clear();

    Dir.clear();
}

bool PagePwr::WaitTimeOut(quint16 t)
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

void PagePwr::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

QString PagePwr::CurrentPorwer()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/PowerSupply", "0").toString();
    return n;
}

int PagePwr::CurrentPowerDir()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    return ini->value("/GLOBAL/PowerDir", "0").toInt();
}

bool PagePwr::IsPGEnable()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    return ini->value("/GLOBAL/EnablePG", false).toBool();
}

void PagePwr::showEvent(QShowEvent *e)
{
    InitSet();
    e->accept();
}

void PagePwr::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "PagePwr" && s.value("TxAddress") != "WinHome")
        return;
    if (s.value("TxCommand") == "ItemInit") {
        if (s.value("Station").toString() == "left")
            stat = 0x13;
        if (s.value("Station").toString() == "right")
            stat = 0x14;
        InitSet();
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
    if (s.value("TxCommand") == "SafeCheck")
        SafeThread(s);
    if (s.value("TxCommand") == "CheckStatus") {
        TestStatus = "init";
        SendCanCmdStatus();
        if (!WaitTimeOut(30))
            SendWarnning("超时");
        TestStatus = "free";
    }
    if (s.value("TxCommand").toString() == "TestSave")
        SendTestSave();
}

void PagePwr::GoToWindow(QString w)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "JumpWindow");
    hash.insert("TxMessage", w);
    emit SendVariant(hash);
}

void PagePwr::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("功率异常:\n%1").arg(s));
    emit SendVariant(hash);
}

void PagePwr::SendTestItemsAllEmpty()
{
    PwrView.clear();
    DirView.clear();

    QString uid = QUuid::createUuid().toString();
    for (int i = 0; i < PWR_MAX; i++) {
        double I1 = m->item(i,1)->text().toDouble();
        double I2 = m->item(i,2)->text().toDouble();
        double P1 = m->item(i,3)->text().toDouble();
        double P2 = m->item(i,4)->text().toDouble();
        double C1 = m->item(i,5)->text().toDouble();
        double C2 = m->item(i,6)->text().toDouble();

        QVariantHash hash;
        hash.insert("TestEnable", m->item(i,0)->text());
        hash.insert("TestItem", tr("功率%1").arg(m->item(i,9)->text()));
        hash.insert("TestPara", tr("%1~%2A %3~%4W %5~%6V").arg(I1).arg(I2).arg(P1).arg(P2).arg(C1).arg(C2));
        hash.insert("TestResult", " ");
        hash.insert("TestJudge", " ");
        hash.insert("TestUid", uid);
        PwrView.append(hash);

        if (m->item(i,7)->text() != tr("不转"))
            hash.insert("TestEnable", "Y");
        else
            hash.insert("TestEnable", "N");
        hash.insert("TestItem", tr("转向%1").arg(m->item(i,9)->text()));
        hash.insert("TestPara", tr("%1").arg(m->item(i,7)->text()));
        hash.insert("TestResult", " ");
        hash.insert("TestJudge", " ");
        hash.insert("TestUid", uid);
        DirView.append(hash);
    }
    for (int i=0; i < PwrView.size(); i++) {
        QVariantHash hash = PwrView.at(i);
        if (hash.value("TestEnable") == "Y") {
            hash.insert("TxAddress", "WinTest");
            hash.insert("TxCommand", "ItemView");
            emit SendVariant(hash);
        }
        hash = DirView.at(i);
        if (hash.value("TestEnable") == "Y") {
            hash.insert("TxAddress", "WinTest");
            hash.insert("TxCommand", "ItemView");
            emit SendVariant(hash);
        }
    }
}

void PagePwr::SendTestItemsAllError()
{
    for (int i=0; i < PwrView.size(); i++) {
        QVariantHash hash = PwrView.at(i);
        if (hash.value("TestEnable") == "Y") {
            hash.insert("TxAddress", "WinTest");
            hash.insert("TxCommand", "ItemUpdate");
            hash.insert("TestResult", "---");
            hash.insert("TestJudge", "NG");
            emit SendVariant(hash);
        }
        hash = DirView.at(i);
        if (hash.value("TestEnable") == "Y") {
            hash.insert("TxAddress", "WinTest");
            hash.insert("TxCommand", "ItemUpdate");
            hash.insert("TestResult", "---");
            hash.insert("TestJudge", "NG");
            emit SendVariant(hash);
        }
    }
}

void PagePwr::SendPwrItems(int num)
{
    QVariantHash hash = PwrView.at(num);
    if (hash.value("TestEnable") == "Y") {
        hash.insert("TxAddress", "WinTest");
        hash.insert("TxCommand", "ItemUpdate");
        emit SendVariant(hash);
    }
}

void PagePwr::SendDirItems(int num)
{
    QVariantHash hash = DirView.at(num);
    if (hash.value("TestEnable") == "Y") {
        hash.insert("TxAddress", "WinTest");
        hash.insert("TxCommand", "ItemUpdate");
        emit SendVariant(hash);
    }
}

void PagePwr::SendPGItems(int num)
{
    QVariantHash hash = PGView.at(num);
    if (hash.value("TestEnable") == "Y") {
        hash.insert("TxAddress", "WinTest");
        hash.insert("TxCommand", "ItemUpdate");
        if (!hash.value("TestJudge").toString().isEmpty())
            hash.insert("WaveItem", hash.value("TestItem").toString());
        emit SendVariant(hash);
    }
}

void PagePwr::SendTestPause()
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestPause");
    hash.insert("TxMessage", "NG");
    emit SendVariant(hash);
}

void PagePwr::SendTestSave()
{
    for (int i=0; i < PwrView.size(); i++) {
        QVariantHash hash = PwrView.at(i);
        if (hash.value("TestEnable") == "Y") {
            QStringList s;
            s.append(hash.value("TestResult1").toString());
            s.append(hash.value("TestResult2").toString());
            s.append(hash.value("TestResult3").toString());
            s.append(hash.value("TestResult4").toString());
            hash.insert("TestResult", s.join(","));
            hash.insert("TxAddress", "WinHome");
            hash.insert("TxCommand", "TestSave");
            emit SendVariant(hash);
        }
    }
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestSave");
    hash.insert("ItemName", tr("功率"));
    hash.insert("TxMessage", PwrJudge);
    emit SendVariant(hash);
}

void PagePwr::TestThread(QVariantHash hash)
{
    PwrJudge = "OK";
    if (hash.value("Station").toString() == "left")
        stat = 0x13;
    if (hash.value("Station").toString() == "right")
        stat = 0x14;
    for (int row = 0; row < PWR_MAX; row++) {
        if (m->item(row,0)->text() == "Y") {
            TestStatus = "buzy";
            TestRow = row;
            PwrJudge = "OK";
            SendCanCmdStart(stat);
            if (!WaitTimeOut(100)) {
                PwrJudge = "NG";
                SendTestItemsAllError();
                break;
            } else {
                PwrView[TestRow].insert("TestJudge", PwrJudge);
                SendPwrItems(TestRow);
                QString n = m->item(row,7)->text();
                DirJudge = (Dir.last() == n) ? "OK":"NG";
                DirView[TestRow].insert("TestJudge", DirJudge);
                SendDirItems(TestRow);
                ClearResults();
            }
            Delay(100);
            if (TestStatus == "stop")
                break;
        }
    }
    if (PwrJudge == "NG" || DirJudge == "NG" || PGJudge == "NG")
        SendTestPause();
    TestStatus = "free";
}

void PagePwr::SafeThread(QVariantHash hash)
{
    if (hash.value("Station").toString() == "left")
        stat = 0x13;
    if (hash.value("Station").toString() == "right")
        stat = 0x14;
    TestStatus = "safe";
    TestRow = 0;
    SendCanCmdStartSafe();
    WaitTimeOut(100);
    ClearResults();
    TestStatus = "free";
}

/*********************************END OF FILE**********************************/

void PagePwr::on_btn1_clicked()
{
    SaveSet();
    GoToWindow(NULL);
}
