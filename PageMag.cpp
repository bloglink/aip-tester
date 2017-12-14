/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       反嵌模块
*******************************************************************************/
#include "PageMag.h"
#include "ui_PageMag.h"

PageMag::PageMag(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageMag)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
    TestStatus = "free";
}

PageMag::~PageMag()
{
    delete ui;
}

void PageMag::InitWindows()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->TabParams->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(4, 400);
    ui->TabParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabParams->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(4, 400);
    ui->TabParams->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    ui->BoxDir->setView(new QListView(this));
    ui->BoxStation->setView(new QListView(this));
    connect(ui->BoxStation, SIGNAL(currentIndexChanged(int)), this, SLOT(InitSettings()));
    connect(ui->TabParams, SIGNAL(cellClicked(int, int)), this, SLOT(ItemClick(int, int)));
    input = new PageNum(this);
    QStringList t;
    t  << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12";
    input->InitButtons(t);
    connect(input, SIGNAL(ItemChange(QString)), this, SLOT(ItemChange(QString)));
    input->hide();

    ui->TabParams->setRowCount(MAX_ROW);
    for (int row=0; row < MAX_ROW; row++) {
        Enable.append(new QTableWidgetItem);
        ui->TabParams->setItem(row, 0, Enable.at(row));
        Enable.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Enable.at(row)->setTextAlignment(Qt::AlignCenter);

        Terminal1.append(new QTableWidgetItem);
        ui->TabParams->setItem(row, 1, Terminal1.at(row));
        Terminal1.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal1.at(row)->setTextAlignment(Qt::AlignCenter);

        Terminal2.append(new QTableWidgetItem);
        ui->TabParams->setItem(row, 2, Terminal2.at(row));
        Terminal2.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal2.at(row)->setTextAlignment(Qt::AlignCenter);

        Max.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 3, Max.at(row));
        Max.at(row)->setDecimals(0);
        Max.at(row)->setMaximum(100);
        Max.at(row)->setAlignment(Qt::AlignHCenter);
        Max.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        WaveMag.append(new WaveBox(this));
        ui->TabParams->setCellWidget(row, 4, WaveMag.at(row));

        AreaL.append(0);
        FreqL.append(0);
        AreaR.append(0);
        FreqR.append(0);
    }
}

void PageMag::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSampleMag, Qt::Key_0);
    btnGroup->addButton(ui->BtnExitMag, Qt::Key_2);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void PageMag::ReadButtons(int id)
{
    QVariantHash hash;
    switch (id) {
    case Qt::Key_0:
        if (TestStatus == "sample")
            return;
        TestStatus = "sample";
        for (int i=0; i < WaveMag.size(); i++)
            WaveMag.at(i)->ShowWave(hash);
        if (ui->BoxStation->currentIndex() == 0)
            stat = WIN_ID_OUT13;
        if (ui->BoxStation->currentIndex() == 1)
            stat = WIN_ID_OUT14;
        SendCanCmdConfig(stat);
        SendCanCmdSample(stat);
        if (!WaitTimeOut(100)) {
            SendWarnning(tr("采样失败"));
        } else {
            TestStatus = "sample";
            CalculateDir();
            SaveSettings();
        }
        TestStatus = "free";
        break;
    case Qt::Key_2:
        SaveSettings();
        GoToWindow(NULL);
        break;
    default:
        break;
    }
}

void PageMag::InitSettings()
{
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    set = new QSettings(t, QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetMag");

    QStringList temp = (set->value("Other", "0 1 2").toString()).split(" ");
    if (temp.size() >= 3) {
        ui->BoxDir->setCurrentIndex(temp.at(0).toInt());
        ui->BoxMain->setValue(temp.at(1).toInt());
        ui->BoxAuxiliary->setValue(temp.at(2).toInt());
    }
    //可用
    temp = (QString(set->value("Enable", "Y Y Y N N N N N").toByteArray())).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Enable.at(row)->setText(temp.at(row));
    //端一
    temp = (set->value("Terminal1", "1 2 1 4 5 6 7 8").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Terminal1.at(row)->setText(temp.at(row));
    //端二
    temp = (set->value("Terminal2", "2 3 3 5 6 7 8 1").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Terminal2.at(row)->setText(temp.at(row));
    //最大值
    temp = (set->value("Max", "10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Max.at(row)->setValue(temp.at(row).toDouble());
    //频率
    temp = (set->value("FreqL", "0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        FreqL[row] = temp.at(row).toInt();
    temp = (set->value("AreaL", "1000 1000 1000 1000 1000 1000 1000 1000").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        AreaL[row] = temp.at(row).toInt();
    //频率
    temp = (set->value("FreqR", "0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        FreqR[row] = temp.at(row).toInt();
    temp = (set->value("AreaR", "1000 1000 1000 1000 1000 1000 1000 1000").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        AreaR[row] = temp.at(row).toInt();
    //波形
    QByteArray w;
    QVariantHash hash;
    for (int row=0; row < qMin(WaveMag.size(), MAX_ROW); row++) {
        QString ByteL = "WaveMagL"+QString::number(row);
        QString ByteR = "WaveMagR"+QString::number(row);
        if (ui->BoxStation->currentIndex() == 0)
            w = set->value(ByteL).toString().toUtf8();
        if (ui->BoxStation->currentIndex() == 1)
            w = set->value(ByteR).toString().toUtf8();
        hash.insert("WaveTest", w);
        WaveMag.at(row)->ShowWave(hash);
    }
}

void PageMag::SaveSettings()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxDir->currentIndex()));
    temp.append(QString::number(ui->BoxMain->value()));
    temp.append(QString::number(ui->BoxAuxiliary->value()));
    set->setValue("Other", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Enable.size(); i++)
        temp.append(Enable.at(i)->text());
    set->setValue("Enable", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Terminal1.size(); i++)
        temp.append(Terminal1.at(i)->text());
    set->setValue("Terminal1", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Terminal2.size(); i++)
        temp.append(Terminal2.at(i)->text());
    set->setValue("Terminal2", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Max.size(); i++)
        temp.append(QString::number(Max.at(i)->value()));
    set->setValue("Max", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < AreaL.size(); i++)
        temp.append(QString::number(AreaL.at(i)));
    set->setValue("AreaL", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < FreqL.size(); i++)
        temp.append(QString::number(FreqL.at(i)));
    set->setValue("FreqL", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < AreaR.size(); i++)
        temp.append(QString::number(AreaR.at(i)));
    set->setValue("AreaR", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < FreqR.size(); i++)
        temp.append(QString::number(FreqR.at(i)));
    set->setValue("FreqR", (temp.join(" ").toUtf8()));

    for (int i=0; i < ItemView.size(); i++) {
        QString ByteL = "WaveMagL"+QString::number(i);
        QString ByteR = "WaveMagR"+QString::number(i);
        if (ui->BoxStation->currentIndex() == 0)
            set->setValue(ByteL, ItemView[i].value("WaveTest").toString());
        if (ui->BoxStation->currentIndex() == 1)
            set->setValue(ByteR, ItemView[i].value("WaveTest").toString());
    }
}

void PageMag::ItemClick(int r,  int c)
{
    switch (c) {
    case 0:
        if (Enable.at(r)->text() != "Y")
            Enable.at(r)->setText("Y");
        else
            Enable.at(r)->setText("N");
        break;
    case 1:
    case 2:
        input->showNormal();
        break;
    }
}

void PageMag::ItemChange(QString msg)
{
    ui->TabParams->currentItem()->setText(msg);
}

void PageMag::ExcuteCanCmd(int addr, QByteArray msg)
{
    if (TestStatus == "free")
        return;
    TimeOut = 0;
    if (addr == CAN_ID_DCR_WAVE) {
        wave.append(msg);
        return;
    }
    quint8 cmd = (quint8)msg.at(0);
    switch (cmd) {
    case 0x00:
        ReadCanCmdStatus(msg);
        break;
    case 0x02:
        ReadCanCmdResult(msg);
        break;
    case 0x03:
        if ((quint8)msg.at(1) != 0xff) {
            wave.clear();
        } else {
            ItemView[TestRow].insert("WaveTest", wave.toBase64());
            if (TestStatus == "sample")
                WaveMag.at(TestRow)->ShowWave(ItemView[TestRow]);
            if (TestStatus == "buzy")
                SendTestItems(TestRow);
            qDebug() << TestRow;
        }
        break;
    }
}

void PageMag::ReadCanCmdStatus(QByteArray msg)
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
    TestStatus = "free";
}

void PageMag::ReadCanCmdResult(QByteArray msg)
{
    TestRow = (quint8)msg.at(1);
    if (TestStatus == "sample") {
        if (stat == WIN_ID_OUT13) {
            AreaL[TestRow] = quint16(msg.at(2)*256)+quint8(msg.at(3));
            FreqL[TestRow] = quint8(msg.at(4));
        }
        if (stat == WIN_ID_OUT14) {
            AreaR[TestRow] = quint16(msg.at(2)*256)+quint8(msg.at(3));
            FreqR[TestRow] = quint8(msg.at(4));
        }
        return;
    }
    quint16 Area1 = 1;
    quint16 Area2 = quint16(msg.at(2)*256)+quint8(msg.at(3));
    if (stat == WIN_ID_OUT13)
        Area1 = AreaL[TestRow];
    if (stat == WIN_ID_OUT14) {
        Area1 = AreaR[TestRow];
    }
    quint16 area = abs((Area2-Area1)*100/Area1);
    QString n = QString("%1%").arg(area);
    QString judge = "OK";
    if (area > Max.at(TestRow)->value()) {
        Judge = "NG";
        judge = "NG";
    }
    ItemView[TestRow].insert("TestResult", n);
    ItemView[TestRow].insert("TestJudge", judge);
}

void PageMag::SendCanCmdSample(quint8 s)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001 << row;
    }
    out << quint16(0x22) << quint8(0x06) << quint8(0x01) << quint8(0x02) << quint8(0x01)
        << quint8(s) << quint8(tt/256) << quint8(tt%256);
    emit CanMsg(msg);
}

void PageMag::SendCanCmdStart(quint8 s)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001 << row;
    }
    out << quint16(0x22) << quint8(0x06) << quint8(0x01) << quint8(0x02) << quint8(0x00)
        << quint8(s) << quint8(tt/256) << quint8(tt%256);
    emit CanMsg(msg);
}

void PageMag::SendCanCmdConfig(quint8 s)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int row=0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            quint8 freq = FreqL.at(row);
            if (s == WIN_ID_OUT13)
                freq = FreqL.at(row);
            if (s == WIN_ID_OUT14)
                freq = FreqR.at(row);
            out << quint16(0x22) << quint8(0x05) << quint8(0x04) << quint8(row)
                << quint8(Terminal1.at(row)->text().toInt())
                << quint8(Terminal2.at(row)->text().toInt())
                << quint8(freq);
        }
    }
    emit CanMsg(msg);
}

void PageMag::CalculateDir()
{
    if (ui->BoxDir->currentIndex() != 0 && ItemView.size() >= 2) {
        //计算主副相的面积，差积，和左右移动的差积
        qint32 area1, area2, diff, diff1, diff2;
        area1 = 0;
        area2 = 0;
        diff = 0;
        diff1 = 0;
        diff2 = 0;
        int b1 = ui->BoxMain->value()-1;
        int b2 = ui->BoxAuxiliary->value()-1;
        QByteArray w1 = QByteArray::fromBase64(ItemView.at(b1).value("WaveTest").toString().toUtf8());
        QByteArray w2 = QByteArray::fromBase64(ItemView.at(b2).value("WaveTest").toString().toUtf8());

        for (int i=10; i < qMin(w1.size(), w2.size())-10; i++) {
            int P1 = quint8(w1.at(i))-0x80;
            int P2 = quint8(w2.at(i))-0x80;
            area1 += P1*P1;
            area2 += P2*P2;
            diff += (P1-P2)*(P1-P2);
            for (int j=1; j < 11; j++) {
                int P3 = quint8(w2.at(i+j))-0x80;
                int P4 = quint8(w2.at(i-j))-0x80;
                diff1 += (P1-P3)*(P1-P3);
                diff2 += (P1-P4)*(P1-P4);
            }
        }
        diff1 /= 10;
        diff2 /= 10;
        QString n;
        QString judge = "OK";

        if (((diff*3 < area1) &&(diff*3 < area2)) || (area1 < (area2 >> 4)) || (area2 < (area1 >> 4)))
            n = tr("不转"); // 偏移不超过1/3, 或面积差大于3/4, 判定为不转
        else if (diff1 < diff2) // 副相在前
            n = tr("正转");
        else if (diff1 > diff2) // 主相在前
            n = tr("反转");
        else
            n = tr("不转");
        if (n != ui->BoxDir->currentText()) {
            Judge = "NG";
            judge = "NG";
        }
        if (TestStatus == "sample") {
            if (n == tr("正转"))
                ui->BoxDir->setCurrentIndex(1);
            if (n == tr("反转"))
                ui->BoxDir->setCurrentIndex(2);
            return;
        }
        int number = ItemView.size()-1;
        ItemView[number].insert("TestResult", n);
        ItemView[number].insert("TestJudge", judge);
        SendTestItems(number);
    }
}

bool PageMag::WaitTimeOut(quint16 t)
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

void PageMag::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

QString PageMag::CurrentSettings()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", INI_DEFAULT).toString();
    return n.remove(".ini");
}

void PageMag::showEvent(QShowEvent *e)
{
    InitSettings();
    SendTestItemsAllEmpty();
    e->accept();
}

void PageMag::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "PageMag" && s.value("TxAddress") != "WinHome")
        return;
    if (s.value("TxCommand") == "ItemInit") {
        if (s.value("Station").toString() == "left")
            stat = WIN_ID_OUT13;
        if (s.value("Station").toString() == "right")
            stat = WIN_ID_OUT14;
        InitSettings();
        SendCanCmdConfig(stat);
        SendTestItemsAllEmpty();
    }
    if (s.value("TxCommand") == "StartTest")
        TestThread(s);
    if (s.value("TxCommand").toString() == "TestSave")
        SendTestSave();
}

void PageMag::GoToWindow(QString w)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "JumpWindow");
    hash.insert("TxMessage", w);
    emit SendVariant(hash);
}

void PageMag::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("反嵌异常:\n%1").arg(s));
    emit SendVariant(hash);
}

void PageMag::SendTestItemsAllEmpty()
{
    ItemView.clear();
    QString uid = QUuid::createUuid().toString();
    for (int i = 0; i < Enable.size(); i++) {
        QString T1 = Terminal1.at(i)->text();
        QString T2 = Terminal2.at(i)->text();
        QString M2 = Max.at(i)->text();
        QVariantHash hash;
        hash.insert("TestEnable", Enable.at(i)->text());
        hash.insert("TestItem", tr("反嵌%1-%2").arg(T1).arg(T2));
        hash.insert("TestPara", tr("%3%").arg(M2));
        hash.insert("TestResult", " ");
        hash.insert("TestJudge", " ");
        hash.insert("TestUid", uid);
        hash.insert("ItemName", tr("反嵌%1").arg(i+1));
        ItemView.append(hash);
    }
    if (ui->BoxDir->currentIndex() != 0 && ItemView.size() >= 2) {
        QVariantHash hash;
        hash.insert("TestItem", tr("磁旋"));
        hash.insert("TestPara", tr("%1").arg(ui->BoxDir->currentText()));
        hash.insert("TestResult", " ");
        hash.insert("TestJudge", " ");
        hash.insert("TestEnable", "Y");
        hash.insert("TestUid", uid);
        hash.insert("ItemName", tr("磁旋"));
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

void PageMag::SendTestItemsAllError()
{
    QByteArray def;
    for (int i=0; i < 400; i++) {
        def.append(QByteArray(1, 0x80));
    }
    for (int i=0; i < ItemView.size(); i++) {
        QVariantHash hash = ItemView.at(i);
        if (hash.value("TestEnable") == "Y") {
            hash.insert("TxAddress", "WinTest");
            hash.insert("TxCommand", "ItemUpdate");
            hash.insert("TestResult", "---");
            hash.insert("TestJudge", "NG");
            hash.insert("WaveTest", def.toBase64());
            hash.insert("WaveItem", hash.value("TestItem").toString());
            emit SendVariant(hash);
        }
    }
    qDebug() << "mag test all error";
}

void PageMag::SendTestWavesAllEmpty()
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinTest");
    hash.insert("TxCommand", "WaveInit");
    emit SendVariant(hash);
}

void PageMag::SendTestItems(int num)
{
    QVariantHash hash = ItemView.at(num);
    if (hash.value("TestEnable") == "Y") {
        hash.insert("TxAddress", "WinTest");
        hash.insert("TxCommand", "ItemUpdate");
        if (!hash.value("WaveTest").toString().isEmpty())
            hash.insert("WaveItem", hash.value("TestItem").toString());
        emit SendVariant(hash);
    }
}

void PageMag::SendTestPause()
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestPause");
    hash.insert("TxMessage", Judge);
    emit SendVariant(hash);
}

void PageMag::SendTestSave()
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
    hash.insert("ItemName", tr("反嵌"));
    hash.insert("TxMessage", Judge);
    emit SendVariant(hash);
}

void PageMag::TestThread(QVariantHash hash)
{
    TestStatus = "buzy";
    Judge = "OK";
    if (hash.value("Station").toString() == "left")
        stat = WIN_ID_OUT13;
    if (hash.value("Station").toString() == "right")
        stat = WIN_ID_OUT14;
    SendTestWavesAllEmpty();
    SendCanCmdStart(stat);
    if (!WaitTimeOut(100)) {
        Judge = "NG";
        SendTestItemsAllError();
    } else {
        CalculateDir();
    }
    if (Judge == "NG")
        SendTestPause();
    TestStatus = "free";
}
/*********************************END OF FILE**********************************/
