#include "PageDcr.h"
#include "ui_pagedcr.h"

PageDcr::PageDcr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDcr)
{
    ui->setupUi(this);
    InitWin();
}

PageDcr::~PageDcr()
{
    delete ui;
}

void PageDcr::InitWin()
{
    this->setFocus();
    m = new StandardItemModel(DCR_MAX, 11);

    QStringList headerList;
    headerList << tr("电阻") << tr("端一") << tr("端二") << tr("线圈材料") << tr("单位")
               << tr("电阻下限") << tr("电阻上限") << tr("标准电阻") << tr("温度补偿")
               << tr("线路补偿1") << tr("线路补偿2");
    m->setHorizontalHeaderLabels(headerList);

    ui->parameters->setModel(m);
    ui->parameters->setItemDelegateForColumn(0, new EnableDelegate(this));
    ui->parameters->setItemDelegateForColumn(1, new PortDelegate);
    ui->parameters->setItemDelegateForColumn(2, new PortDelegate);
    ui->parameters->setItemDelegateForColumn(3, new MetalDelegate);
    ui->parameters->setItemDelegateForColumn(4, new UnitDelegate);
    ui->parameters->setItemDelegateForColumn(5, new DcrDelegate);
    ui->parameters->setItemDelegateForColumn(6, new DcrDelegate);
    ui->parameters->setItemDelegateForColumn(7, new DcrDelegate);
    ui->parameters->setItemDelegateForColumn(8, new OffsetDelegate);
    ui->parameters->setItemDelegateForColumn(9, new DcrDelegate);
    ui->parameters->setItemDelegateForColumn(10, new DcrDelegate);
    ui->parameters->setEditTriggers(QAbstractItemView::CurrentChanged);

#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->parameters->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(5, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(6, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(7, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(8, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(9, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(10, QHeaderView::Stretch);
    ui->parameters->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->parameters->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(10, QHeaderView::Stretch);
    ui->parameters->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
}

void PageDcr::InitSet()
{
    QSettings *g = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString n = g->value("/GLOBAL/FileInUse", "aip9918.ini").toString().remove(".ini");
    QString s = QString("./config/%1.ini").arg(n);
    ini = new QSettings(s, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("SetDcr");

    QStringList temp = (ini->value("Other", "20 0 0.2 10 10 0").toString()).split(" ");
    if (temp.size() >= 6) {
        ui->BoxStd->setValue(temp.at(0).toDouble());
        ui->BoxOffset->setValue(temp.at(1).toDouble());
        ui->BoxTime->setValue(temp.at(2).toDouble());
        ui->BoxMin->setValue(temp.at(3).toDouble());
        ui->BoxMax->setValue(temp.at(4).toDouble());
        ui->BoxUnbalance->setValue(temp.at(5).toDouble());
    }
    //可用
    temp = (QString(ini->value("Enable", "Y Y Y N N N N N").toByteArray())).split(" ");
    for (int i=0; i < qMin(temp.size(), DCR_MAX); i++)
        m->setItem(i,0,new QStandardItem(temp.at(i)));
    //端一
    temp = (ini->value("Terminal1", "1 2 1 4 5 6 7 8").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), DCR_MAX); i++)
        m->setItem(i,1,new QStandardItem(temp.at(i)));
    //端二
    temp = (ini->value("Terminal2", "2 3 3 5 6 7 8 1").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), DCR_MAX); i++)
        m->setItem(i,2,new QStandardItem(temp.at(i)));
    //材料
    temp = (QString(ini->value("Metal", "0 0 0 0 0 0 0 0").toString())).split(" ");
    QStringList ss;
    ss << tr("铜") <<  tr("铝") << tr("铜铝");
    for (int i=0; i < qMin(temp.size(), DCR_MAX); i++)
        m->setItem(i,3,new QStandardItem(ss.at(QString(temp.at(i)).toInt())));
    //单位
    temp = (QString(ini->value("Unit", "1 1 1 1 1 1 1 1").toString())).split(" ");
    ss.clear();
    ss << tr("mΩ") <<  tr("Ω") << tr("kΩ");
    for (int i=0; i < qMin(temp.size(), DCR_MAX); i++)
        m->setItem(i,4,new QStandardItem(ss.at(QString(temp.at(i)).toInt())));
    //最小值
    temp = (ini->value("Min", "0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), DCR_MAX); i++)
        m->setItem(i,5,new QStandardItem(temp.at(i)));
    //最大值
    temp = (ini->value("Max", "200 200 200 200 200 200 200 200").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), DCR_MAX); i++)
        m->setItem(i,6,new QStandardItem(temp.at(i)));
    //标准值
    temp = (ini->value("Std", "100 100 100 100 100 100 100 100").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), DCR_MAX); i++)
        m->setItem(i,7,new QStandardItem(temp.at(i)));
    //补偿
    ss.clear();
    ss << tr("取消补偿") <<  tr("温度补偿") << tr("热敏电阻");
    temp = (ini->value("OffsetT", "0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), DCR_MAX); i++)
        m->setItem(i,8,new QStandardItem(ss.at(QString(temp.at(i)).toInt())));
    //补偿
    temp = (ini->value("Offset", "0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), DCR_MAX); i++)
        m->setItem(i,9,new QStandardItem(temp.at(i)));
    //补偿
    temp = (ini->value("OffsetR", "0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), DCR_MAX); i++)
        m->setItem(i,10,new QStandardItem(temp.at(i)));
}

void PageDcr::SaveSet()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxStd->value()));
    temp.append(QString::number(ui->BoxOffset->value()));
    temp.append(QString::number(ui->BoxTime->value()));
    temp.append(QString::number(ui->BoxMin->value()));
    temp.append(QString::number(ui->BoxMax->value()));
    temp.append(QString::number(ui->BoxUnbalance->value()));
    ini->setValue("Other", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < DCR_MAX; i++)
        temp.append(m->item(i,0)->text());
    ini->setValue("Enable", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < DCR_MAX; i++)
        temp.append(m->item(i,1)->text());
    ini->setValue("Terminal1", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < DCR_MAX; i++)
        temp.append(m->item(i,2)->text());
    ini->setValue("Terminal2", (temp.join(" ").toUtf8()));
    temp.clear();
    QStringList ss;
    ss << tr("铜") <<  tr("铝") << tr("铜铝");
    for (int i=0; i < DCR_MAX; i++)
        temp.append(QString::number(ss.lastIndexOf(m->item(i,3)->text())));
    ini->setValue("Metal", (temp.join(" ").toUtf8()));
    temp.clear();
    ss.clear();
    ss << tr("mΩ") <<  tr("Ω") << tr("kΩ");
    for (int i=0; i < DCR_MAX; i++)
        temp.append(QString::number(ss.lastIndexOf(m->item(i,4)->text())));
    ini->setValue("Unit", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < DCR_MAX; i++)
        temp.append(m->item(i,5)->text());
    ini->setValue("Min", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < DCR_MAX; i++)
        temp.append(m->item(i,6)->text());
    ini->setValue("Max", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < DCR_MAX; i++)
        temp.append(m->item(i,7)->text());
    ini->setValue("Std", (temp.join(" ").toUtf8()));
    temp.clear();
    ss.clear();
    ss << tr("取消补偿") <<  tr("温度补偿") << tr("热敏电阻");
    for (int i=0; i < DCR_MAX; i++)
        temp.append(QString::number(ss.lastIndexOf(m->item(i,8)->text())));
    ini->setValue("OffsetT", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < DCR_MAX; i++)
        temp.append(m->item(i,9)->text());
    ini->setValue("Offset", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < DCR_MAX; i++)
        temp.append(m->item(i,10)->text());
    ini->setValue("OffsetR", (temp.join(" ").toUtf8()));
}

void PageDcr::InitItems()
{
    ItemView.clear();
    QString uid = QUuid::createUuid().toString();
    for (int i = 0; i < DCR_MAX; i++) {
        QString T1 = m->item(i,1)->text();
        QString T2 = m->item(i,2)->text();
        QString U1 = m->item(i,4)->text();
        QString M1 = m->item(i,5)->text();
        QString M2 = m->item(i,6)->text();

        QVariantHash hash;

        hash.insert("TestEnable", m->item(i,0)->text());
        hash.insert("TestItem", tr("电阻%1-%2").arg(T1).arg(T2));
        hash.insert("TestPara", tr("%3-%4%5").arg(M1).arg(M2).arg(U1));
        hash.insert("TestResult", " ");
        hash.insert("TestJudge", " ");
        hash.insert("TestUid", uid);
        hash.insert("ItemName", tr("电阻%1").arg(i+1));
        ItemView.append(hash);
    }
    if (ui->BoxUnbalance->value() != 0 && ItemView.size() >= 3) {
        QVariantHash hash;
        hash.insert("TestItem", tr("电阻平衡"));
        hash.insert("TestPara", tr("%1%").arg(ui->BoxUnbalance->value()));
        hash.insert("TestResult", " ");
        hash.insert("TestJudge", " ");
        hash.insert("TestEnable", "Y");
        hash.insert("TestUid", uid);
        ItemView.append(hash);
    }
}

void PageDcr::ExcuteCanCmd(int addr, QByteArray msg)
{
    quint8 cmd = (quint8)msg.at(0);
    if (TestStatus == "free" && cmd != 0x06)
        return;
    TimeOut = 0;

    switch (cmd) {
    case 0x00: //状态
        ReadCanCmdStatus(msg);
        break;
    case 0x01: //结果
        ReadCanCmdResult(msg);
        break;
    case 0x06: //调试参数
        ReadCanCmdBack(msg);
        break;
    case 0x07: //模块编码
        ui->code->setText(msg.toHex());
        ReadCanCmdBack(msg);
        break;
    case 0x08: //软件版本
        ui->version->setText(msg.toHex());
        ReadCanCmdBack(msg);
        break;
    default:
        qDebug() << addr << msg.toHex();
        break;
    }
}

void PageDcr::ReadCanCmdStatus(QByteArray msg)
{
    int s = quint8(msg.at(1));
    switch (s) {
    case 0x00:
        break;
    case 0x01:
        return;
    case 0x02:
        SendWarnning("UNIVALID");
        break;
    case 0x03:
        SendWarnning("FLASH_ERROR");
        break;
    case 0x04:
        SendWarnning("ZERO_ERROR");
        break;
    case 0x05:
        SendWarnning("AMP11_ERROR");
        break;
    case 0x06:
        SendWarnning("AMP121_ERROR");
        break;
    case 0x07:
        SendWarnning("REFER1_ERROR");
        break;
    case 0x08:
        SendWarnning("REFER2_ERROR");
        break;
    case 0x09:
        SendWarnning("CUR_ERROR");
        break;
    case 0x0A:
        SendWarnning("CHAN_ERROR");
        break;
    default:
        SendWarnning("UNKONW_ERROR");
        break;
    }
    SendTemperature(msg);
    TestStatus = "free";
}

void PageDcr::ReadCanCmdResult(QByteArray msg)
{
    quint8 num = quint8(msg.at(1));
    quint8 grade = quint8(msg.at(2));
    double temp = (quint16)(msg.at(3)*256)+(quint8)msg.at(4);
    double tt = (quint16)(msg.at(5)*256)+quint8(msg.at(6));
    if (m->item(num,8)->text() == tr("温度补偿")) {
        if (m->item(num,3)->text() == tr("铜"))
            temp *= (1.00+ResCu*(ui->BoxStd->value()-(tt/10-50)-ui->BoxOffset->value()));
        if (m->item(num,3)->text() == tr("铝"))
            temp *= (1.00+ResAl*(ui->BoxStd->value()-(tt/10-50)-ui->BoxOffset->value()));
        if (m->item(num,3)->text() == tr("铜铝"))
            temp *= (1.00+ResCu_Al*(ui->BoxStd->value()-(tt/10-50)-ui->BoxOffset->value()));
    }
    temp *= qPow(10, (grade-6));
    ItemView[num].insert("TestResult", temp);
}

void PageDcr::ReadCanCmdBack(QByteArray msg)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinBack");
    hash.insert("TxCommand", "DcrMsg");
    hash.insert("TxMessage", msg.toBase64());
    emit SendVariant(hash);
}

void PageDcr::SendCanCmdStatus()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x22) << quint8(0x01) << quint8(0x00);
    emit CanMsg(msg);
}

void PageDcr::SendCanCmdStart(quint8 pos)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int i=0; i < DCR_MAX; i++) {
        if (m->item(i,0)->text() == "Y")
            tt += 0x0001 << i;
    }
    out << quint16(0x22) << quint8(0x06) << quint8(0x01) << quint8(0x01) << quint8(0x00)
        << quint8(pos) << quint8(tt/256) << quint8(tt%256);
    emit CanMsg(msg);
}

void PageDcr::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x22) << quint8(0x01) << quint8(0x02);
    emit CanMsg(msg);
}

void PageDcr::SendCanCmdConfig()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int i=0; i < DCR_MAX; i++) {
        if (m->item(i,0)->text() == "Y") {
            out << quint16(0x22) << quint8(0x06) << quint8(0x03) << quint8(i)
                << quint8(m->item(i,1)->text().toInt())
                << quint8(m->item(i,2)->text().toInt())
                << quint8(CalculateGear(i))
                << quint8(ui->BoxTime->value()*10);
        }
    }
    emit CanMsg(msg);
}
int PageDcr::CalculateGear(int i)
{
    double r = m->item(i,6)->text().toDouble();
    if (m->item(i,4)->text() == tr("mΩ"))
        r /= 1000;
    if (m->item(i,4)->text() == tr("kΩ"))
        r *= 1000;
    if (r <= 0.2)
        return 1;
    if (r <= 2)
        return 2;
    if (r <= 20)
        return 3;
    if (r <= 200)
        return 4;
    if (r <= 2000)
        return 5;
    if (r <= 20000)
        return 6;
    else
        return 7;
}
void PageDcr::SendCanCmdDebug()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x22) << quint8(0x02) << quint8(0x06) << quint8(0xEE);
    emit CanMsg(msg);
}

void PageDcr::SendCanCmdCode()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x22) << quint8(0x02) << quint8(0x07) << quint8(0x00);
    emit CanMsg(msg);
}

void PageDcr::SendCanCmdVersion()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x22) << quint8(0x01) << quint8(0x08);
    emit CanMsg(msg);
}

void PageDcr::SendAlarm(quint8 addr)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x22) << quint8(0x02) << quint8(0x09) << quint8(addr);
    emit CanMsg(msg);
}

void PageDcr::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "PageDcr" && s.value("TxAddress") != "WinHome")
        return;
    if (s.value("TxCommand") == "ItemInit") {
        InitSet();
        InitItems();
        SendCanCmdConfig();
        SendTestItemsAllEmpty();
    }
    if (s.value("TxCommand") == "StartTest")
        TestThread(s);
    if (s.value("TxCommand") == "StopTest")
        TestStatus = "stop";
    if (s.value("TxCommand") == "CheckStatus") {
        TestStatus = "init";
        SendCanCmdCode();
        SendCanCmdVersion();
        SendCanCmdStatus();
        if (!WaitTimeOut(30))
            SendWarnning("超时");
        TestStatus = "free";
    }
    if (s.value("TxCommand") == "TestAlarm") {
        quint8 t = 0x00;
        if (s.value("TxMessage").toString().contains("LEDY"))
            t += 0x04;
        if (s.value("TxMessage").toString().contains("LEDG"))
            t += 0x02;
        if (s.value("TxMessage").toString().contains("LEDR"))
            t += 0x01;
        if (s.value("TxMessage").toString().contains("BEEP"))
            t += 0x08;
        SendAlarm(t);
    }
    if (s.value("TxCommand").toString() == "TestSave")
        SendTestSave();
}

void PageDcr::GoToWindow(QString w)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "JumpWindow");
    hash.insert("TxMessage", w);
    emit SendVariant(hash);
}

void PageDcr::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("电阻异常:\n%1").arg(s));
    emit SendVariant(hash);
}

void PageDcr::SendTestItemsAllEmpty()
{
    for (int i=0; i < ItemView.size(); i++) {
        QVariantHash hash = ItemView.at(i);
        if (hash.value("TestEnable") == "Y") {
            hash.insert("TxAddress", "WinTest");
            hash.insert("TxCommand", "ItemView");
            emit SendVariant(hash);
        }
    }
}

void PageDcr::SendTestItemsAllError()
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
    qDebug() << "dcr test all error";
}

void PageDcr::SendTestItems(int num)
{
    QVariantHash hash = ItemView.at(num);
    if (hash.value("TestEnable") == "Y") {
        hash.insert("TxAddress", "WinTest");
        hash.insert("TxCommand", "ItemUpdate");
        emit SendVariant(hash);
    }
}

void PageDcr::SendTestItemsAll()
{
    QList<double> Results;
    for (int i=0; i < DCR_MAX; i++) {
        if (ItemView[i].value("TestEnable") == "Y") {
            double temp = ItemView[i].value("TestResult").toDouble();
            Results.append(temp);
            QString t;
            QString judge = "OK";
            double offset = 0;
            double max = m->item(i,6)->text().toDouble();
            double min = m->item(i,5)->text().toDouble();
            if (stat == 0x13)
                offset = m->item(i,0x09)->text().toDouble();
            if (stat == 0x14)
                offset = m->item(i,0x0A)->text().toDouble();
            if (m->item(i,0x04)->text() == tr("mΩ")) {
                offset /= 1000;
                max /= 1000;
                min /= 1000;
            }
            if (m->item(i,0x04)->text() == tr("kΩ")) {
                offset *= 1000;
                max *= 1000;
                min *= 1000;
            }
            temp -= qMin(temp, offset);
            int grade = CalculateGear(i);
            if (grade == 1 || grade == 2)
                t = QString("%1mΩ").arg(temp*1000, 0, 'r', (3-grade%3));
            if (grade == 3 || grade == 4 || grade == 5)
                t = QString("%1Ω").arg(temp, 0, 'r', (3-grade%3));
            if (grade == 6 || grade == 7)
                t = QString("%1kΩ").arg(temp/1000, 0, 'r', (3-grade%3));

            double gg = 2 * qPow(10, (grade-2)) * 1.1;
            if (temp > gg)
                t = QString(">%1Ω").arg(gg);


            if (temp < min || temp > max) {
                Judge = "NG";
                judge = "NG";
            }
            ItemView[i].insert("TestResult", t);
            ItemView[i].insert("TestJudge", judge);
            SendTestItems(i);
        }
    }
    //计算不平衡度
    if ((ui->BoxUnbalance->value() != 0) && (Results.size() >= 3)) {
        QString judge = "OK";
        double sum = 0;
        double avr = 0;
        QString u;
        for (int i=0; i < 3; i++) {
            sum += Results.at(i);
        }
        avr = sum/3;
        for (int i=0; i < 3; i++) {
            double un;
            if (avr == 0)
                un = 0;
            else
                un = fabs(Results.at(i)-avr)*100/avr;
            u.append(QString::number(un, 'f', 1));
            u.append("% ");
            if (un >= ui->BoxUnbalance->value()) {
                judge = "NG";
                Judge = "NG";
            }
        }
        int number = ItemView.size()-1;
        ItemView[number].insert("TestResult", u);
        ItemView[number].insert("TestJudge", judge);
        SendTestItems(number);
        Results.clear();
    }
}

void PageDcr::SendTestPause()
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestPause");
    hash.insert("TxMessage", Judge);
    emit SendVariant(hash);
}

void PageDcr::SendTestSave()
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
    hash.insert("ItemName", tr("电阻"));
    hash.insert("TxMessage", Judge);
    emit SendVariant(hash);
}

void PageDcr::SendTemperature(QByteArray msg)
{
    double offset = ui->BoxOffset->value();
    double temp = double((quint16(msg.at(2)*256)+quint8(msg.at(3))))/10-50+offset;
    QVariantHash hash;
    hash.insert("TxAddress", "WinTest");
    hash.insert("TxCommand", "Temperature");
    hash.insert("TxMessage", tr("温度:%1°C").arg(temp));
    emit SendVariant(hash);
}

void PageDcr::SendSafeCheck(QVariantHash s)
{
    QVariantHash hash = s;
    hash.insert("TxAddress", "PagePwr");
    hash.insert("TxCommand", "SafeCheck");
    emit SendVariant(hash);
}

void PageDcr::TestThread(QVariantHash hash)
{
    TestStatus = "buzy";
    Judge = "OK";
    if (hash.value("Station").toString() == "left")
        stat = 0x13;
    if (hash.value("Station").toString() == "right")
        stat = 0x14;
    SendCanCmdStart(stat);
    if (!WaitTimeOut(ui->BoxTime->value()*100+100)) {
        Judge = "NG";
        SendTestItemsAllError();
    } else {
        SendTestItemsAll();
    }
    if (Judge == "NG")
        SendTestPause();
    TestStatus = "free";
}
bool PageDcr::WaitTimeOut(quint16 t)
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

void PageDcr::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void PageDcr::showEvent(QShowEvent *e)
{
    this->setFocus();
    InitSet();
    e->accept();
}

void PageDcr::on_btn1_clicked()
{
    for (int i=0; i < DCR_MAX; i++) {
        double std = m->item(i,7)->text().toDouble();
        double min = std*(100-ui->BoxMin->value())/100;
        double max = std*(100+ui->BoxMax->value())/100;
        m->item(i,5)->setText(QString::number(min, 'r', 2));
        m->item(i,6)->setText(QString::number(max, 'r', 2));
    }
}

void PageDcr::on_btn2_clicked()
{
    stat = 0x13;
    SendCanCmdConfig();
    TestStatus = "offset";
    SendCanCmdStart(stat);
    if (!WaitTimeOut(100))
        SendWarnning(tr("补偿失败"));
    TestStatus = "free";
}

void PageDcr::on_btn3_clicked()
{
    stat = 0x14;
    SendCanCmdConfig();
    TestStatus = "offset";
    SendCanCmdStart(stat);
    if (!WaitTimeOut(100))
        SendWarnning(tr("补偿失败"));
    TestStatus = "free";
}

void PageDcr::on_btn4_clicked()
{
    SaveSet();
    GoToWindow(NULL);
}
/*********************************END OF FILE**********************************/
