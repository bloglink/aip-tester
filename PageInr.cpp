/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       绝缘模块
*******************************************************************************/
#include "PageInr.h"
#include "ui_PageInr.h"

PageInr::PageInr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageInr)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
    Mode = INR_FREE;
    TestStatus = "free";
}

PageInr::~PageInr()
{
    delete ui;
}

void PageInr::InitWindows()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->TabParams->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(5, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(6, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(7, QHeaderView::Stretch);
    ui->TabParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabParams->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->TabParams->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(ui->TabParams, SIGNAL(cellClicked(int, int)), this, SLOT(ItemClick(int, int)));
    ui->Input->hide();
    Check << ui->Box1 << ui->Box2 << ui->Box3 << ui->Box4
          << ui->Box5 << ui->Box6 << ui->Box7 << ui->Box8;
    ui->TabParams->setRowCount(INR_ROW);
    for (int row=0; row < INR_ROW; row++) {
        Enable.append(new QTableWidgetItem);
        ui->TabParams->setItem(row, 0, Enable.at(row));
        Enable.at(row)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        Enable.at(row)->setTextAlignment(Qt::AlignCenter);

        Terminal1.append(new QTableWidgetItem);
        ui->TabParams->setItem(row, 1, Terminal1.at(row));
        Terminal1.at(row)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        Terminal1.at(row)->setTextAlignment(Qt::AlignCenter);

        Terminal2.append(new QTableWidgetItem);
        ui->TabParams->setItem(row, 2, Terminal2.at(row));
        Terminal2.at(row)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        Terminal2.at(row)->setTextAlignment(Qt::AlignCenter);

        Vol.append(new QComboBox(this));
        ui->TabParams->setCellWidget(row, 3, Vol.at(row));
        QStringList t1;
        t1  << tr("500") << tr("1000");
        Vol.at(row)->setView(new QListView(this));
        Vol.at(row)->addItems(t1);

        Min.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 4, Min.at(row));
        Min.at(row)->setMaximum(500);
        Min.at(row)->setDecimals(0);
        Min.at(row)->setAlignment(Qt::AlignHCenter);
        Min.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Max.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 5, Max.at(row));
        Max.at(row)->setMaximum(500);
        Max.at(row)->setDecimals(0);
        Max.at(row)->setAlignment(Qt::AlignHCenter);
        Max.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Time.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 6, Time.at(row));
        Time.at(row)->setMaximum(100);
        Time.at(row)->setDecimals(1);
        Time.at(row)->setAlignment(Qt::AlignHCenter);
        Time.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Offset.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 7, Offset.at(row));
        Offset.at(row)->setMaximum(9999);
        Offset.at(row)->setAlignment(Qt::AlignHCenter);
        Offset.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
}

void PageInr::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExitIr, Qt::Key_0);
    btnGroup->addButton(ui->btnInput, Qt::Key_1);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void PageInr::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        SaveSettings();
        GoToWindow(NULL);
        break;
    case Qt::Key_1:
        EnsureInput();
        break;
    default:
        break;
    }
}

void PageInr::InitSettings()
{
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("SetIr");
    QStringList temp;
    //可用
    temp = (QString(ini->value("Enable", "Y Y Y Y"). toByteArray())).split(" ");
    for (int row=0; row < qMin(temp.size(), INR_ROW); row++)
        Enable.at(row)->setText(temp.at(row));
    //端一
    temp = (ini->value("Terminal1", "PE 1 4 7").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), INR_ROW); row++)
        Terminal1.at(row)->setText(temp.at(row));
    //端二
    temp = (ini->value("Terminal2", "ALL 2 3 5").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), INR_ROW); row++)
        Terminal2.at(row)->setText(temp.at(row));
    //电压
    temp = (ini->value("Voltage", "0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), INR_ROW); row++)
        Vol.at(row)->setCurrentIndex(temp.at(row).toDouble());
    //电流下限
    temp = (ini->value("Min", "100 100 100 100").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), INR_ROW); row++)
        Min.at(row)->setValue(temp.at(row).toDouble());
    //电流上限
    temp = (ini->value("Max", "0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), INR_ROW); row++)
        Max.at(row)->setValue(temp.at(row).toDouble());
    //测试时间
    temp = (ini->value("Time", "1 1 1 1").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), INR_ROW); row++)
        Time.at(row)->setValue(temp.at(row).toDouble());
    //补偿
    temp = (ini->value("Offset", "0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), INR_ROW); row++)
        Offset.at(row)->setValue(temp.at(row).toDouble());
    if (!EnablePhase()) {
        Enable.at(0)->setText("Y");
        Enable.at(1)->setText("N");
        Enable.at(2)->setText("N");
        Enable.at(3)->setText("N");
        ui->TabParams->hideColumn(2);
        ui->TabParams->hideColumn(1);
        ui->TabParams->hideColumn(0);
        ui->TabParams->hideRow(3);
        ui->TabParams->hideRow(2);
        ui->TabParams->hideRow(1);
    } else {
        ui->TabParams->showColumn(0);
        ui->TabParams->showColumn(1);
        ui->TabParams->showColumn(2);
        ui->TabParams->showRow(1);
        ui->TabParams->showRow(2);
        ui->TabParams->showRow(3);
    }
    qDebug() << QTime::currentTime().toString() << "PageInr read OK";
}

void PageInr::SaveSettings()
{
    QStringList temp;
    temp.clear();
    for (int i=0; i < Enable.size(); i++)
        temp.append(Enable.at(i)->text());
    ini->setValue("Enable", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Terminal1.size(); i++)
        temp.append(Terminal1.at(i)->text());
    ini->setValue("Terminal1", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Terminal2.size(); i++)
        temp.append(Terminal2.at(i)->text());
    ini->setValue("Terminal2", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Vol.size(); i++)
        temp.append(QString::number(Vol.at(i)->currentIndex()));
    ini->setValue("Voltage", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Min.size(); i++)
        temp.append(QString::number(Min.at(i)->value()));
    ini->setValue("Min", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Max.size(); i++)
        temp.append(QString::number(Max.at(i)->value()));
    ini->setValue("Max", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Time.size(); i++)
        temp.append(QString::number(Time.at(i)->value()));
    ini->setValue("Time", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Offset.size(); i++)
        temp.append(Offset.at(i)->text());
    ini->setValue("Offset", (temp.join(" ").toUtf8()));
    qDebug() << QTime::currentTime().toString() << "PageInr save OK";
}

void PageInr::ItemClick(int r, int c)
{
    ui->Input->hide();
    switch (c) {
    case 0:
        if (Enable.at(r)->text() != "Y")
            Enable.at(r)->setText("Y");
        else
            Enable.at(r)->setText("N");
        break;
    case 1:
    case 2:
        if (r == 1 || r == 2 || r == 3)
            InitInput(r, c);
    default:
        break;
    }
}

void PageInr::ExcuteCanCmd(int addr, QByteArray msg)
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

void PageInr::ReadCanCmdStatus(QByteArray msg)
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
    default:
        SendWarnning("UNKONW_ERROR");
        break;
    }
    TestStatus = "free";
}

void PageInr::ReadCanCmdResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double tt = quint16(msg.at(3)*256)+quint8(msg.at(4));
    tt *= qPow(10, -quint8(msg.at(5)));
    if (tt < Offset.at(TestRow)->value())
        tt = tt*Offset.at(TestRow)->value()/(Offset.at(TestRow)->value()-tt);
    Volt.append(v);
    Res.append(tt);
    QString rrr = QString::number(Res.last(), 'f', 1);
    if (Res.last() > 500)
        rrr = ">500";
    QString t = QString("%1Mohm").arg(rrr);
    if (Res.last() < 1)
        t = "超量程";
    ItemView[TestRow].insert("TestResult", t);
    if (quint8(msg.at(6)) != 0x00) {
        Judge = "NG";
        switch (quint8(msg.at(6))) {
        case 2:
            ItemView[TestRow].insert("TestJudge", "STOP");
            break;
        case 3:
            ItemView[TestRow].insert("TestJudge", "ARC");
            break;
        default:
            ItemView[TestRow].insert("TestJudge", QString("PT+%1").arg(quint8(msg.at(6))));
            break;
        }
        ClearResults();
    }
    SendTestItems(TestRow);
}

void PageInr::ReadCanCmdBack(QByteArray msg)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinBack");
    hash.insert("TxCommand", "InrMsg");
    hash.insert("TxMessage", msg.toBase64());
    emit SendVariant(hash);
}

void PageInr::SendCanCmdStatus()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x23) << quint8(0x01) << quint8(0x00);
    emit CanMsg(msg);
}

void PageInr::SendCanCmdStart(quint8 pos)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x23) << quint8(0x05) << quint8(0x01) << quint8(0x04) << quint8(0x00)
        << quint8(pos) << quint8(0x01);
    emit CanMsg(msg);
}

void PageInr::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x23) << quint8(0x01) << quint8(0x02);
    emit CanMsg(msg);
}

void PageInr::SendCanCmdConfig(int row)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    int volt = Vol.at(row)->currentText().toInt();
    int time = Time.at(row)->value()*10;
    quint16 h = GetTeminal(row, 1);
    quint16 l = GetTeminal(row, 2);
    out << quint16(0x23) << quint8(0x08) << quint8(0x03) << quint8(0x01) << quint8(0x04)
        << quint8(h/256) << quint8(h%256) << quint8(l/256) << quint8(l%256) << quint8(0x00);
    out << quint16(0x23) << quint8(0x08) << quint8(0x04) << quint8(0x01) << quint8(volt/256)
        << quint8(volt%256) << quint8(time/256) << quint8(time%256) << quint8(0x00) << quint8(1);
    out << quint16(0x23) << quint8(0x07) << quint8(0x05) << quint8(0x01)
        << quint8(0/256) << quint8(0%256) << quint8(0x00) << quint8(0x03) << quint8(0x0A); // 上限
    emit CanMsg(msg);
}

void PageInr::SendCanCmdDebug()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x23) << quint8(0x02) << quint8(0x06) << quint8(0xEE);
    emit CanMsg(msg);
}

void PageInr::SendCanCmdCode()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x23) << quint8(0x02) << quint8(0x07) << quint8(0x00);
    emit CanMsg(msg);
}

void PageInr::SendCanCmdVersion()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x23) << quint8(0x01) << quint8(0x08);
    emit CanMsg(msg);
}

void PageInr::ClearResults()
{
    Volt.clear();
    Res.clear();
}

bool PageInr::WaitTimeOut(quint16 t)
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

void PageInr::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

quint16 PageInr::GetTeminal(int r, int c)
{
    if (r == 0 && c == 1)
        return 0x8000;
    if (r == 0 && c == 2)
        return 0x7fff;
    quint16 t = 0;
    QString s;
    if (c == 1)
        s = Terminal1.at(r)->text();
    if (c == 2)
        s = Terminal2.at(r)->text();
    if (s.contains("1"))
        t += 0x01;
    if (s.contains("2"))
        t += 0x02;
    if (s.contains("3"))
        t += 0x04;
    if (s.contains("4"))
        t += 0x08;
    if (s.contains("5"))
        t += 0x10;
    if (s.contains("6"))
        t += 0x20;
    if (s.contains("7"))
        t += 0x40;
    if (s.contains("8"))
        t += 0x80;
    return t;
}

void PageInr::InitInput(int r, int c)
{
    QString s;
    QString x;
    QString s1;
    ui->Input->show();
    for (int i=0; i < Check.size(); i++) {
        Check.at(i)->setChecked(false);
        Check.at(i)->hide();
    }
    if (c == 1) {
        s = Terminal1.at(r)->text();
        x = "147";
        s1 = Terminal2.at(r)->text();
        for (int i=0; i < s1.size(); i++)
            x.remove(s1.mid(i, 1));
    }
    if (c == 2) {
        s = Terminal2.at(r)->text();
        x = "12345678";
        s1 = Terminal1.at(r)->text();
        for (int i=0; i < s1.size(); i++)
            x.remove(s1.mid(i, 1));
    }

    for (int i = 0; i < x.size(); i++) {
        Check.at(x.mid(i, 1).toInt() - 1)->show();
    }
    for (int i = 0; i < s.size(); i++) {
        Check.at(s.mid(i, 1).toInt()-1)->setChecked(true);
    }
}

void PageInr::EnsureInput()
{
    QString s;
    for (int i = 0; i < Check.size(); i++) {
        if (Check.at(i)->isChecked())
            s.append(Check.at(i)->text());
    }
    int r = ui->TabParams->currentRow();
    int c = ui->TabParams->currentColumn();
    if ( r != 0 && (c == 1 || c == 2))
        ui->TabParams->currentItem()->setText(s);
    ui->Input->hide();
}

QString PageInr::CurrentSettings()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", INI_DEFAULT).toString();
    return n.remove(".ini");
}

bool PageInr::EnablePhase()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    return ini->value("/GLOBAL/EnablePhase", false).toBool();
}

void PageInr::showEvent(QShowEvent *e)
{
    ui->BtnExitIr->setFocus();
    InitSettings();
    e->accept();
}

void PageInr::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "PageInr" && s.value("TxAddress") != "WinHome")
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
    if (s.value("TxCommand") == "CheckStatus") {
        TestStatus = "init";
        SendCanCmdCode();
        SendCanCmdVersion();
        SendCanCmdStatus();
        if (!WaitTimeOut(30))
            SendWarnning("超时");
        TestStatus = "free";
    }
    if (s.value("TxCommand").toString() == "TestSave")
        SendTestSave();
}

void PageInr::GoToWindow(QString w)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "JumpWindow");
    hash.insert("TxMessage", w);
    emit SendVariant(hash);
}

void PageInr::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("绝缘异常:\n%1").arg(s));
    emit SendVariant(hash);
}

void PageInr::SendTestItemsAllEmpty()
{
    ItemView.clear();
    QString uid = QUuid::createUuid().toString();
    for (int i = 0; i < Enable.size(); i++) {
        QString T1 = Terminal1.at(i)->text();
        QString M1 = Min.at(i)->text();
        QString M2 = Max.at(i)->text();
        QString U1 = Vol.at(i)->currentText();
        QVariantHash hash;
        hash.insert("TestEnable", Enable.at(i)->text());
        if (EnablePhase()) {
            hash.insert("TestItem", tr("绝缘%1").arg(i+1));
            if (M2.toInt() != 0)
                hash.insert("TestPara", tr("高端:%1 %2V %3-%4M").arg(T1).arg(U1).arg(M1).arg(M2));
            else
                hash.insert("TestPara", tr("高端:%1 %2V %3M").arg(T1).arg(U1).arg(M1));

        } else {
            hash.insert("TestItem", tr("绝缘"));
            if (M2.toInt() != 0)
                hash.insert("TestPara", tr("%1V %2-%3M").arg(U1).arg(M1).arg(M2));
            else
                hash.insert("TestPara", tr("%1V %2M").arg(U1).arg(M1));
        }
        hash.insert("TestResult", " ");
        hash.insert("TestJudge", " ");
        hash.insert("TestUid", uid);
        hash.insert("ItemName", tr("绝缘%1").arg(i+1));
        ItemView.append(hash);
    }
    for (int i=0; i < ItemView.size(); i++) {
        QVariantHash hash = ItemView.at(i);
        if (hash.value("TestEnable") == "Y") {
            hash.insert("TxAddress", "WinTest");
            hash.insert("TxCommand", "ItemView");
            emit SendVariant(hash);
        }
    }
}

void PageInr::SendTestItemsAllError()
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
    qDebug() << "inr test all error";
}

void PageInr::SendTestItems(int num)
{
    QVariantHash hash = ItemView.at(num);
    if (hash.value("TestEnable") == "Y") {
        hash.insert("TxAddress", "WinTest");
        hash.insert("TxCommand", "ItemUpdate");
        emit SendVariant(hash);
    }
}

void PageInr::SendTestPause()
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestPause");
    hash.insert("TxMessage", Judge);
    emit SendVariant(hash);
}

void PageInr::SendTestSave()
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
    hash.insert("ItemName", tr("绝缘"));
    hash.insert("TxMessage", Judge);
    emit SendVariant(hash);
}

void PageInr::TestThread(QVariantHash hash)
{
    Judge = "OK";
    if (hash.value("Station").toString() == "left")
        stat = WIN_ID_OUT13;
    if (hash.value("Station").toString() == "right")
        stat = WIN_ID_OUT14;
    for (int row = 0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            TestStatus = "buzy";
            TestRow = row;
            Judge = "OK";
            SendCanCmdConfig(row);
            Delay(5);
            SendCanCmdStart(stat);
            if (!WaitTimeOut(100)) {
                Judge = "NG";
                SendTestItemsAllError();
                break;
            } else {
                ItemView[TestRow].insert("TestJudge", Judge);
                SendTestItems(TestRow);
                ClearResults();
            }
            Delay(100);
            if (TestStatus == "stop")
                break;
        }
    }
    if (Judge == "NG")
        SendTestPause();
    TestStatus = "free";
}


/*********************************END OF FILE**********************************/
