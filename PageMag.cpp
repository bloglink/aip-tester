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
    InitWin();
    InitSet();
    TestStatus = "free";
}

PageMag::~PageMag()
{
    delete ui;
}

void PageMag::InitWin()
{
    this->setFocus();
    m = new StandardItemModel(MAG_MAX, 5);

    QStringList headerList;
    headerList << tr("反嵌") << tr("端一") << tr("端二") << tr("上限") << tr("波形");
    m->setHorizontalHeaderLabels(headerList);

    ui->parameters->setModel(m);
    ui->parameters->setItemDelegateForColumn(0, new EnableDelegate(this));
    ui->parameters->setItemDelegateForColumn(1, new PortDelegate);
    ui->parameters->setItemDelegateForColumn(2, new PortDelegate);
    ui->parameters->setItemDelegateForColumn(3, new RateDelegate);
    for (int i=0; i < MAG_MAX; i++) {
        Waves.append(new QCustomPlot(this));
        Waves.at(i)->setBackground(QBrush(QColor(25, 25, 25))); //设置背景色
        Waves.at(i)->xAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
        Waves.at(i)->yAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
        Waves.at(i)->xAxis->setTicks(false);
        Waves.at(i)->yAxis->setTicks(false);
        Waves.at(i)->xAxis->setTickLabels(false);
        Waves.at(i)->yAxis->setTickLabels(false);
        Waves.at(i)->axisRect()->setMinimumMargins(QMargins(0,0,0,0));
        Waves.at(i)->axisRect()->setupFullAxesBox();
        Waves.at(i)->rescaleAxes();
        Waves.at(i)->addGraph();
        Waves.at(i)->xAxis->setRange(0,400);
        Waves.at(i)->yAxis->setRange(0,255);
        Waves.at(i)->graph(0)->setPen(QPen(Qt::green));
        ui->parameters->setIndexWidget(m->index(i,4), Waves.at(i));
        AreaL.append(0);
        FreqL.append(0);
        AreaR.append(0);
        FreqR.append(0);
    }
    ui->parameters->setEditTriggers(QAbstractItemView::CurrentChanged);
    ui->BoxDir->setView(new QListView(this));
    ui->BoxStation->setView(new QListView(this));

#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->parameters->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
    ui->parameters->setColumnWidth(4, 400);
    ui->parameters->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->parameters->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->parameters->setColumnWidth(4, 400);
    ui->parameters->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
}

void PageMag::InitSet()
{
    QSettings *g = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString n = g->value("/GLOBAL/FileInUse", "aip9918.ini").toString().remove(".ini");
    QString s = QString("./config/%1.ini").arg(n);
    ini = new QSettings(s, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("SetMag");

    QStringList temp = (ini->value("Other", "0 1 2").toString()).split(" ");
    if (temp.size() >= 3) {
        ui->BoxDir->setCurrentIndex(temp.at(0).toInt());
        ui->BoxMain->setValue(temp.at(1).toInt());
        ui->BoxAuxiliary->setValue(temp.at(2).toInt());
    }
    //可用
    temp = (QString(ini->value("Enable", "Y Y Y N N N N N").toByteArray())).split(" ");
    for (int i=0; i < qMin(temp.size(), MAX_ROW); i++)
        m->setItem(i,0,new QStandardItem(temp.at(i)));
    //端一
    temp = (ini->value("Terminal1", "1 2 1 4 5 6 7 8").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), MAX_ROW); i++)
        m->setItem(i,1,new QStandardItem(temp.at(i)));
    //端二
    temp = (ini->value("Terminal2", "2 3 3 5 6 7 8 1").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), MAX_ROW); i++)
        m->setItem(i,2,new QStandardItem(temp.at(i)));
    //最大值
    temp = (ini->value("Max", "10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), MAX_ROW); i++)
        m->setItem(i,3,new QStandardItem(temp.at(i)));
    //频率
    temp = (ini->value("FreqL", "0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        FreqL[row] = temp.at(row).toInt();
    temp = (ini->value("AreaL", "1000 1000 1000 1000 1000 1000 1000 1000").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        AreaL[row] = temp.at(row).toInt();
    //频率
    temp = (ini->value("FreqR", "0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        FreqR[row] = temp.at(row).toInt();
    temp = (ini->value("AreaR", "1000 1000 1000 1000 1000 1000 1000 1000").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        AreaR[row] = temp.at(row).toInt();
    //波形
    QByteArray w;
    QVector<double> x(400), y(400);
    for (int row=0; row < qMin(Waves.size(), MAX_ROW); row++) {
        QString ByteL = "WaveMagL"+QString::number(row);
        QString ByteR = "WaveMagR"+QString::number(row);
        if (ui->BoxStation->currentIndex() == 0)
            w = ini->value(ByteL).toString().toUtf8();
        if (ui->BoxStation->currentIndex() == 1)
            w = ini->value(ByteR).toString().toUtf8();

        for (int i=0; i < qMin(400,w.size()); i++) {
            x[i] = i;
            y[i] = quint8(w.at(i));
        }
        Waves.at(row)->graph(0)->setData(x,y);
    }
}

void PageMag::SaveSet()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxDir->currentIndex()));
    temp.append(QString::number(ui->BoxMain->value()));
    temp.append(QString::number(ui->BoxAuxiliary->value()));
    ini->setValue("Other", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < MAG_MAX; i++)
        temp.append(m->item(i,0)->text());
    ini->setValue("Enable", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < MAG_MAX; i++)
        temp.append(m->item(i,1)->text());
    ini->setValue("Terminal1", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < MAG_MAX; i++)
        temp.append(m->item(i,2)->text());
    ini->setValue("Terminal2", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < MAG_MAX; i++)
        temp.append(m->item(i,3)->text());
    ini->setValue("Max", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < MAG_MAX; i++)
        temp.append(QString::number(AreaL.at(i)));
    ini->setValue("AreaL", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < MAG_MAX; i++)
        temp.append(QString::number(FreqL.at(i)));
    ini->setValue("FreqL", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < MAG_MAX; i++)
        temp.append(QString::number(AreaR.at(i)));
    ini->setValue("AreaR", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < MAG_MAX; i++)
        temp.append(QString::number(FreqR.at(i)));
    ini->setValue("FreqR", (temp.join(" ").toUtf8()));

    for (int i=0; i < ItemView.size(); i++) {
        QString ByteL = "WaveMagL"+QString::number(i);
        QString ByteR = "WaveMagR"+QString::number(i);
        if (ui->BoxStation->currentIndex() == 0)
            ini->setValue(ByteL, ItemView[i].value("WaveTest").toString());
        if (ui->BoxStation->currentIndex() == 1)
            ini->setValue(ByteR, ItemView[i].value("WaveTest").toString());
    }
}

void PageMag::InitItems()
{
    ItemView.clear();
    QString uid = QUuid::createUuid().toString();
    for (int i = 0; i < MAG_MAX; i++) {
        QString T1 = m->item(i,1)->text();
        QString T2 = m->item(i,2)->text();
        QString M2 = m->item(i,3)->text();
        QVariantHash hash;
        hash.insert("TestEnable", m->item(i,0)->text());
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
}

void PageMag::ExcuteCanCmd(int addr, QByteArray msg)
{
    if (TestStatus == "free")
        return;
    TimeOut = 0;
    if (addr == 0x441) {
        wave.append(msg);
        return;
    }
    quint8 cmd = (quint8)msg.at(0);
    switch (cmd) {
    case 0x00:
        ReadCanCmdStatus(msg);
        break;
    case 0x02:
        if (TestStatus == "sample")
            ReadCanCmdSample(msg);
        else
            ReadCanCmdResult(msg);
        break;
    case 0x03:
        if ((quint8)msg.at(1) != 0xff) {
            wave.clear();
        } else {
            ItemView[TestRow].insert("WaveTest", wave.toBase64());
            if (TestStatus == "sample")
                ReadCanCmdWave(wave);
            if (TestStatus == "buzy")
                SendTestItems(TestRow);
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
    quint16 Area1 = 0;
    quint16 Area2 = quint16(msg.at(2)*256)+quint8(msg.at(3));
    if (stat == 0x13)
        Area1 = AreaL[TestRow];
    if (stat == 0x14) {
        Area1 = AreaR[TestRow];
    }
    quint16 area = abs((Area2-Area1)*100/Area1);
    QString n = QString("%1%").arg(area);
    QString judge = "OK";
    if (area > m->item(TestRow,3)->text().toInt()) {
        Judge = "NG";
        judge = "NG";
    }
    ItemView[TestRow].insert("TestResult", n);
    ItemView[TestRow].insert("TestJudge", judge);
}

void PageMag::ReadCanCmdSample(QByteArray msg)
{
    TestRow = (quint8)msg.at(1);
    if (stat == 0x13) {
        AreaL[TestRow] = quint16(msg.at(2)*256)+quint8(msg.at(3));
        FreqL[TestRow] = quint8(msg.at(4));
    }
    if (stat == 0x14) {
        AreaR[TestRow] = quint16(msg.at(2)*256)+quint8(msg.at(3));
        FreqR[TestRow] = quint8(msg.at(4));
    }
    return;
}

void PageMag::ReadCanCmdWave(QByteArray msg)
{
    QVector<double> x(400), y(400);
    for (int i=0; i < qMin(400,msg.size()); i++) {
        x[i] = i;
        y[i] = quint8(msg.at(i));
    }
    Waves.at(TestRow)->graph(0)->setData(x,y);
}

void PageMag::SendCanCmdSample(quint8 s)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row < MAG_MAX; row++) {
        if (m->item(row,0)->text() == "Y")
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
    for (int row=0; row < MAG_MAX; row++) {
        if (m->item(row,0)->text() == "Y")
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
    for (int i=0; i < MAG_MAX; i++) {
        if (m->item(i,0)->text() == "Y") {
            quint8 freq = FreqL.at(i);
            if (s == 0x13)
                freq = FreqL.at(i);
            if (s == 0x14)
                freq = FreqR.at(i);
            out << quint16(0x22) << quint8(0x05) << quint8(0x04) << quint8(i)
                << quint8(m->item(i,1)->text().toInt())
                << quint8(m->item(i,1)->text().toInt())
                << quint8(freq);
        }
    }
    emit CanMsg(msg);
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

void PageMag::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "PageMag" && s.value("TxAddress") != "WinHome")
        return;
    if (s.value("TxCommand") == "ItemInit") {
        if (s.value("Station").toString() == "left")
            stat = 0x13;
        if (s.value("Station").toString() == "right")
            stat = 0x14;
        InitSet();
        InitItems();
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

void PageMag::SendTestItemsAll()
{
    for (int i=0; i < MAG_MAX; i++) {
        if (ItemView[i].value("TestEnable") == "Y") {
            SendTestItems(i);
        }
    }
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
        stat = 0x13;
    if (hash.value("Station").toString() == "right")
        stat = 0x14;
    SendTestWavesAllEmpty();
    SendCanCmdStart(stat);
    if (!WaitTimeOut(100)) {
        Judge = "NG";
        SendTestItemsAllError();
    } else {
        SendTestItemsAll();
    }
    if (Judge == "NG")
        SendTestPause();
    TestStatus = "free";
}

void PageMag::showEvent(QShowEvent *e)
{
    this->setFocus();
    InitSet();
    InitItems();
    e->accept();
}

void PageMag::on_btn1_clicked()
{
    if (TestStatus == "sample")
        return;
    TestStatus = "sample";
    if (ui->BoxStation->currentIndex() == 0)
        stat = 0x13;
    if (ui->BoxStation->currentIndex() == 1)
        stat = 0x14;
    SendCanCmdConfig(stat);
    SendCanCmdSample(stat);
    if (!WaitTimeOut(1000)) {
        SendWarnning(tr("采样失败"));
    } else {
        TestStatus = "sample";
        SaveSet();
    }
    TestStatus = "free";
    this->setFocus();
}

void PageMag::on_btn2_clicked()
{
    SaveSet();
    GoToWindow(NULL);
}
/*********************************END OF FILE**********************************/
