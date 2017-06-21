/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170417
 * author:      zhaonanlin
 * brief:       匝间测试模块
*******************************************************************************/
#include "PageImp.h"
#include "ui_PageImp.h"

PageImp::PageImp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageImp)
{
    ui->setupUi(this);
    InitWin();
    InitSet();
    TestStatus = "free";
    stat = 0x13;
    AvrCount = 0;
}

PageImp::~PageImp()
{
    delete ui;
}

void PageImp::InitWin()
{
    this->setFocus();
    m = new StandardItemModel(IMP_MAX, 12);

    QStringList headerList;
    headerList << tr("匝间") << tr("端一") << tr("端二") << tr("电压") << tr("次数")
               << tr("电晕") << tr("相位") << tr("面积") << tr("差积") <<tr("压缩")
               <<tr("波形") << tr("拉伸");
    m->setHorizontalHeaderLabels(headerList);
    ui->parameters->setModel(m);
    ui->parameters->setItemDelegateForColumn(0, new EnableDelegate(this));
    ui->parameters->setItemDelegateForColumn(1, new PortDelegate);
    ui->parameters->setItemDelegateForColumn(2, new PortDelegate);
    ui->parameters->setItemDelegateForColumn(3, new VoltDelegate);
    ui->parameters->setItemDelegateForColumn(4, new RateDelegate);
    ui->parameters->setItemDelegateForColumn(5, new RateDelegate);
    ui->parameters->setItemDelegateForColumn(6, new RateDelegate);
    ui->parameters->setItemDelegateForColumn(7, new RateDelegate);
    ui->parameters->setItemDelegateForColumn(8, new RateDelegate);
    ui->parameters->setItemDelegateForColumn(9, new ReadOnlyDelegate);
    ui->parameters->setItemDelegateForColumn(11, new ReadOnlyDelegate);
    for (int i=0; i < IMP_MAX; i++) {
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
        ui->parameters->setIndexWidget(m->index(i,10), Waves.at(i));
        FreqL.append(0);
        FreqR.append(0);
        VoltL.append(0);
        VoltR.append(0);
        WaveL.append(0);
        WaveR.append(0);
        Block0.append(5);
        Block1.append(395);
        m->setItem(i,9,new QStandardItem("<"));
        m->setItem(i,11,new QStandardItem(">"));
    }
    ui->parameters->setEditTriggers(QAbstractItemView::CurrentChanged);
    connect(ui->parameters,SIGNAL(clicked(QModelIndex)),this,SLOT(WaveChange(QModelIndex)));
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->parameters->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    ui->parameters->setColumnWidth(3, 50);
    ui->parameters->horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(5, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(6, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(7, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(8, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(9, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setResizeMode(11, QHeaderView::Stretch);
    ui->parameters->setColumnWidth(10, 400);
    ui->parameters->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->parameters->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->parameters->setColumnWidth(3, 50);
    ui->parameters->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);
    ui->parameters->horizontalHeader()->setSectionResizeMode(11, QHeaderView::Stretch);
    ui->parameters->setColumnWidth(10, 400);
    ui->parameters->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
}


void PageImp::InitSet()
{
    QSettings *g = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString n = g->value("/GLOBAL/FileInUse", "aip9918.ini").toString().remove(".ini");
    QString s = QString("./config/%1.ini").arg(n);
    ini = new QSettings(s, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("SetImp");
    QStringList temp;
    ui->BoxOffset->setChecked(ini->value("Offset", true).toBool());
    //可用
    temp = (QString(ini->value("Enable", "Y Y Y N N N N N").toByteArray())).split(" ");
    for (int i=0; i < qMin(temp.size(), IMP_MAX); i++)
        m->setItem(i,0,new QStandardItem(temp.at(i)));
    //端一
    temp = (ini->value("Terminal1", "1 2 1 4 5 6 7 8").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), IMP_MAX); i++)
        m->setItem(i,1,new QStandardItem(temp.at(i)));
    //端二
    temp = (ini->value("Terminal2", "2 3 3 5 6 7 8 1").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), IMP_MAX); i++)
        m->setItem(i,2,new QStandardItem(temp.at(i)));
    //电压
    temp = (ini->value("Volt", "500 500 500 500 500 500 500 500").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), IMP_MAX); i++)
        m->setItem(i,3,new QStandardItem(temp.at(i)));
    //次数
    temp = (ini->value("Time", "1 1 1 1 1 1 1 1").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), IMP_MAX); i++)
        m->setItem(i,4,new QStandardItem(temp.at(i)));
    //电晕
    temp = (ini->value("Flut", "0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), IMP_MAX); i++)
        m->setItem(i,5,new QStandardItem(temp.at(i)));
    //相位
    temp = (ini->value("Phase", "0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), IMP_MAX); i++)
        m->setItem(i,6,new QStandardItem(temp.at(i)));
    //面积
    temp = (ini->value("Area", "10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), IMP_MAX); i++)
        m->setItem(i,7,new QStandardItem(temp.at(i)));
    //差积
    temp = (ini->value("Diff", "10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int i=0; i < qMin(temp.size(), IMP_MAX); i++)
        m->setItem(i,8,new QStandardItem(temp.at(i)));
    //计算起点
    temp = (ini->value("Block0", "5 5 5 5 5 5 5 5").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), IMP_MAX); row++)
        Block0[row] = temp.at(row).toInt();
    //计算终点
    temp = (ini->value("Block1", "395 395 395 395 395 395 395 395").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), IMP_MAX); row++)
        Block1[row] = temp.at(row).toInt();
    //测试电压
    temp = (ini->value("VoltL", "500 500 500 500 500 500 500 500").toString().split(" "));
    for (int row=0; row < qMin(temp.size(), IMP_MAX); row++)
        VoltL[row] = temp.at(row).toInt();
    //频率
    temp = (ini->value("FreqL", "7 7 7 7 7 7 7 7").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), IMP_MAX); row++)
        FreqL[row] = temp.at(row).toInt();
    //测试电压
    temp = (ini->value("VoltR", "500 500 500 500 500 500 500 500").toString().split(" "));
    for (int row=0; row < qMin(temp.size(), IMP_MAX); row++)
        VoltR[row] = temp.at(row).toInt();
    //频率
    temp = (ini->value("FreqR", "7 7 7 7 7 7 7 7").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), IMP_MAX); row++)
        FreqR[row] = temp.at(row).toInt();
    //波形
    QVector<double> x(400), y(400);
    for (int i=0; i < IMP_MAX; i++) {
        QString ByteL = "WaveL"+QString::number(i);
        QString ByteR = "WaveR"+QString::number(i);
        WaveL[i] = ini->value(ByteL).toString().toUtf8();
        WaveR[i] = ini->value(ByteR).toString().toUtf8();
        if (ui->BoxStation->currentIndex() == 0)
            wave = QByteArray::fromBase64(WaveL[i].toUtf8());
        else
            wave = QByteArray::fromBase64(WaveR[i].toUtf8());
        if (wave.size() < 100)
            continue;
        for(int j=0; j < wave.size()/2; j++) {
            x[j] = j;
            y[j] = (quint16(wave.at(2*j)*256) + quint8(wave.at(2*j+1)))/4;
        }
        Waves.at(i)->graph(0)->setData(x,y);
    }
    wave.clear();
}

void PageImp::SaveSet()
{
    ini->setValue("Offset", ui->BoxOffset->isChecked());
    QStringList temp;
    temp.clear();
    for (int i=0; i < IMP_MAX; i++)
        temp.append(m->item(i,0)->text());
    ini->setValue("Enable", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < IMP_MAX; i++)
        temp.append(m->item(i,1)->text());
    ini->setValue("Terminal1", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < IMP_MAX; i++)
        temp.append(m->item(i,2)->text());
    ini->setValue("Terminal2", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < IMP_MAX; i++)
        temp.append(m->item(i,3)->text());
    ini->setValue("Volt", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < IMP_MAX; i++)
        temp.append(m->item(i,4)->text());
    ini->setValue("Time", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < IMP_MAX; i++)
        temp.append(m->item(i,5)->text());
    ini->setValue("Flut", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < IMP_MAX; i++)
        temp.append(m->item(i,6)->text());
    ini->setValue("Phase", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < IMP_MAX; i++)
        temp.append(m->item(i,7)->text());
    ini->setValue("Area", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < IMP_MAX; i++)
        temp.append(m->item(i,8)->text());
    ini->setValue("Diff", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Block0.size(); i++)
        temp.append(QString::number(Block0.at(i)));
    ini->setValue("Block0", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Block1.size(); i++)
        temp.append(QString::number(Block1.at(i)));
    ini->setValue("Block1", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < VoltL.size(); i++)
        temp.append(QString::number(VoltL.at(i)));
    ini->setValue("VoltL", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < FreqL.size(); i++)
        temp.append(QString::number(FreqL.at(i)));
    ini->setValue("FreqL", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < VoltR.size(); i++)
        temp.append(QString::number(VoltR.at(i)));
    ini->setValue("VoltR", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < FreqR.size(); i++)
        temp.append(QString::number(FreqR.at(i)));
    ini->setValue("FreqR", (temp.join(" ").toUtf8()));

    for (int i=0; i < ItemView.size(); i++) {
        QString ByteL = "WaveL"+QString::number(i);
        QString ByteR = "WaveR"+QString::number(i);
        ini->setValue(ByteL, WaveL[i]);
        ini->setValue(ByteR, WaveR[i]);
    }
}

void PageImp::InitItems()
{
    ItemView.clear();
    QString uid = QUuid::createUuid().toString();
    for (int i = 0; i < IMP_MAX; i++) {
        QString T1 = m->item(i,1)->text();
        QString T2 = m->item(i,2)->text();
        QString V = m->item(i,3)->text();
        QString C = m->item(i,5)->text();
        QString P = m->item(i,6)->text();
        QString A = m->item(i,7)->text();
        QString D = m->item(i,8)->text();
        QString para = tr("%3V").arg(V);
        if (C.toInt() != 0)
            para += " " + C;
        if (P.toInt() != 0)
            para += " " + P;
        if (A.toInt() != 0)
            para += " " + A + "%";
        if (D.toInt() != 0)
            para += " " + D + "%";
        QVariantHash hash;
        hash.insert("TestEnable", m->item(i,0)->text());
        hash.insert("TestItem", tr("匝间%1-%2").arg(T1).arg(T2));
        hash.insert("TestPara", para);
        hash.insert("TestResult", " ");
        hash.insert("TestJudge", " ");
        hash.insert("TestUid", uid);
        hash.insert("ItemName", tr("匝间%1").arg(i+1));
        if (stat == 0x13)
            hash.insert("WaveByte", WaveL.at(i));
        if (stat == 0x14)
            hash.insert("WaveByte", WaveR.at(i));
        hash.insert("WaveTest", "");
        ItemView.append(hash);
    }
}

void PageImp::WaveChange(QModelIndex m)
{
    if (m.column() == 9) {
        TestStatus = "sample";
        TestRow = m.row();
        if (stat == 0x13 && FreqL[TestRow] != 0)
            FreqL[TestRow]--;
        if (stat == 0x14 && FreqR[TestRow] != 0)
            FreqR[TestRow]--;
        QVector<double> x(1),y(1);
        x[1] = 0;
        y[1] = 0;
        Waves.at(TestRow)->graph(0)->setData(x,y);
        Waves.at(TestRow)->replot();

        SendCanCmdConfig();
        SendCanCmdSample(TestRow);
        if (!WaitTimeOut(100))
            SendWarnning(tr("采样失败"));
        else
            SaveSet();
        TestStatus = "free";
        Waves.at(TestRow)->replot();
    }
    if (m.column() == 11) {
        TestStatus = "sample";
        TestRow = m.row();
        if (stat == 0x13 && FreqL[TestRow] != 14)
            FreqL[TestRow]++;
        if (stat == 0x14 && FreqR[TestRow] != 14)
            FreqR[TestRow]++;
        QVector<double> x(1),y(1);
        x[1] = 0;
        y[1] = 0;
        Waves.at(TestRow)->graph(0)->setData(x,y);
        Waves.at(TestRow)->replot();
        SendCanCmdConfig();
        SendCanCmdSample(TestRow);
        if (!WaitTimeOut(100))
            SendWarnning(tr("采样失败"));
        else
            SaveSet();
        TestStatus = "free";
        Waves.at(TestRow)->replot();
    }
}

void PageImp::ExcuteCanCmd(int addr, QByteArray msg)
{
    quint8 cmd = (quint8)msg.at(0);
    if (TestStatus == "free" && cmd != 0x06)
        return;
    TimeOut = 0;
    if (addr == 0x481) {
        wave.append(msg);
        return;
    }
    switch (cmd) {
    case 0x00:
        ReadCanCmdStatus(msg);
        break;
    case 0x02:
        ReadCanCmdResult(msg);
        break;
    case 0x03:
        if ((quint8)msg.at(1) != 0xff)
            wave.clear();
        else
            ReadCanCmdWaveOk();
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
        break;
    }
}
void PageImp::ReadCanCmdStatus(QByteArray msg)
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
void PageImp::ReadCanCmdResult(QByteArray msg)
{
    TestRow = quint8(msg.at(1));
    if (TestStatus != "sample")
        return;
    if (stat == 0x13) {
        FreqL[TestRow] = quint8(msg.at(3));
        VoltL[TestRow] = quint16(msg.at(4)*256)+quint8(msg.at(5));
    }
    if (stat == 0x14) {
        FreqR[TestRow] = quint8(msg.at(3));
        VoltR[TestRow] = quint16(msg.at(4)*256)+quint8(msg.at(5));
    }
}

void PageImp::ReadCanCmdWaveOk()
{
    if (TestStatus == "sample") {
        ItemView[TestRow].insert("WaveByte", wave.toBase64());
        if (stat == 0x13)
            WaveL[TestRow] = wave.toBase64();
        if (stat == 0x14)
            WaveR[TestRow] = wave.toBase64();
        QVector<double> x(400),y(400);
        for(int i=0; i < wave.size()/2; i++) {
            x[i] = i;
            y[i] = (quint16(wave.at(2*i)*256) + quint8(wave.at(2*i+1)))/4;
        }
        Waves.at(TestRow)->graph(0)->setData(x,y);
        return;
    }
    //    if (TestStatus == "add") {
    //        QList<quint16> Byte;
    //        QList<quint16> Test;
    //        for (int i=0; i < wave.size()/2; i++)
    //            Test.append(quint16(wave.at(i*2+0)*256) + quint8(wave.at(i*2+1)));
    //        QByteArray s = QByteArray::fromBase64(ItemView[TestRow].value("WaveByte").toByteArray());
    //        for (int i=0; i < s.size()/2; i++)
    //            Byte.append(quint16(s.at(i*2+0)*256) + quint8(s.at(i*2+1)));

    //        for (int i=0; i < qMin(Byte.size(), Test.size()); i++) {
    //            Byte[i] -= (Byte[i] - Test[i]) / AvrCount;
    //        }

    //        QByteArray n;
    //        for (int i=0; i < Byte.size(); i++) {
    //            n.append(quint8(Byte.at(i)/256));
    //            n.append(quint8(Byte.at(i)%256));
    //        }
    //        ItemView[TestRow].insert("WaveByte", n.toBase64());
    //        WaveImp[TestRow]->ShowWave(ItemView[TestRow]);
    //        WaveView(ItemView[TestRow]);
    //        if (stat == 0x13)
    //            WaveL[TestRow] = n.toBase64();
    //        if (stat == 0x14)
    //            WaveR[TestRow] = n.toBase64();
    //        return;
    //    }
    ItemView[TestRow].insert("WaveTest", wave.toBase64());
    CalculateResult();
    SendTestItems(TestRow);
}

void PageImp::ReadCanCmdBack(QByteArray msg)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinBack");
    hash.insert("TxCommand", "ImpMsg");
    hash.insert("TxMessage", msg.toBase64());
    emit SendVariant(hash);
}

void PageImp::SendCanCmdStatus()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x24) << quint8(0x01) << quint8(0x00);
    emit CanMsg(msg);
}

void PageImp::SendCanCmdSampleAuto()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row < IMP_MAX; row++) {
        if (m->item(row,0)->text()  ==  "Y")
            tt += 0x0001 << row;
    }
    quint8 mode = 0x01;
    if (ui->BoxOffset->isChecked())
        mode = 0x04;
    out << quint16(0x24) << quint8(0x05) << quint8(0x01) << quint8(mode) << quint8(stat)
        << quint8(tt/256) << quint8(tt%256);
    emit CanMsg(msg);
}

void PageImp::SendCanCmdSample(quint16 t)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0x0001 << t;
    out << quint16(0x24) << quint8(0x05) << quint8(0x01) << quint8(0x02) << quint8(stat)
        << quint8(tt/256) << quint8(tt%256);
    emit CanMsg(msg);
}

void PageImp::SendCanCmdStart(quint8 pos)
{
    stat = pos;
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row < IMP_MAX; row++) {
        if (m->item(row,0)->text()  ==  "Y")
            tt += 0x0001 << row;
    }
    out << quint16(0x24) << quint8(0x05) << quint8(0x01) << quint8(0x00) << quint8(stat)
        << quint8(tt/256) << quint8(tt%256);
    emit CanMsg(msg);
}

void PageImp::SendCanCmdConfig()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int i=0; i < IMP_MAX; i++) {
        int v = 0;
        quint8 f = FreqL.at(i);
        if (stat == 0x13)
            f = FreqL.at(i);
        if (stat == 0x14)
            f = FreqR.at(i);
        if (TestStatus != "sample") {
            if (stat == 0x13)
                v = VoltL.at(i);
            if (stat == 0x14)
                v = VoltR.at(i);
        } else {
            v = m->item(i,3)->text().toInt();
        }
        out << quint16(0x24) << quint8(0x08) << quint8(0x03) << quint8(i)
            << quint8(m->item(i,1)->text().toInt())
            << quint8(m->item(i,2)->text().toInt())
            << quint8(v/256) << quint8(v%256)
            << quint8(CalculateGear(i)) << quint8(f);
    }
    emit CanMsg(msg);
}

void PageImp::SendCanCmdDebug()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x24) << quint8(0x02) << quint8(0x06) << quint8(0xEE);
    emit CanMsg(msg);
}

void PageImp::SendCanCmdCode()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x24) << quint8(0x02) << quint8(0x07) << quint8(0x00);
    emit CanMsg(msg);
}

void PageImp::SendCanCmdVersion()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x24) << quint8(0x01) << quint8(0x08);
    emit CanMsg(msg);
}

void PageImp::CalculateResult()
{
    QByteArray byte;
    QByteArray test;
    if (stat == 0x13)
        byte = QByteArray::fromBase64(WaveL[TestRow].toUtf8());
    if (stat == 0x14)
        byte = QByteArray::fromBase64(WaveR[TestRow].toUtf8());
    test = QByteArray::fromBase64(ItemView[TestRow].value("WaveTest").toString().toUtf8());

    QList<int> ByteY;
    QList<int> TestY;
    for (int i=0; i < qMin(byte.size()/2, test.size()/2); i++) {
        ByteY.append(quint16(byte.at(i*2)*256) + quint8(byte.at(i*2+1)));
        TestY.append(quint16(test.at(i*2)*256) + quint8(test.at(i*2+1)));
    }
    qint32 Area1 = 0;
    qint32 Area2 = 0;
    qint32 Area3 = 0;
    qint32 Phase1 = 0;
    qint32 Phase2 = 0;
    qint32 Flut1 = 0;
    qint32 Flut2 = 0;
    qint32 F = 0;
    qint32 A = 0;
    qint32 D = 0;
    qint32 P = 0;
    quint16 s = Block0.at(TestRow);
    quint16 e = Block1.at(TestRow);
    if (s  <  1)
        s = 1;
    if (e > 398)
        e = 398;
    for (int i=s; i < e; i++) {
        int a1 = ByteY.at(i);
        int a2 = TestY.at(i);
        Area1 += abs(a1-0x200);
        Area2 += abs(a2-0x200);
        int b1 = ByteY.at(i-1);
        int b2 = ByteY.at(i);
        int b3 = ByteY.at(i+1);
        int c1 = TestY.at(i-1);
        int c2 = TestY.at(i);
        int c3 = TestY.at(i+1);
        Area3 += abs((b1+b2*2+b3)-(c1+c2*2+c3));

        Flut1 += abs(b2-b3);
        Flut2 += abs(c2-c3);
        Phase1 += (a1-0x200)*(a1-0x200);
        Phase2 += (a1-0x200)*(a2-0x200);
    }
    F = qMin(99, abs((Flut2-Flut1)*100/Flut1));
    P = qMin(99, abs((Phase1-Phase2)*100/Phase1));
    A = qMin(99, abs((Area2-Area1)*100/Area1));
    D = qMin(99, abs(qMin(Area2, Area3/4)*100/Area1));

    QString n;
    QString judge = "OK";

    if (m->item(TestRow,5)->text().toInt() != 0) {
        n.append(tr("电晕:%1 ").arg(F));
        if (m->item(TestRow,5)->text().toInt() < abs(F))
            judge = "NG";
    }
    if (m->item(TestRow,6)->text().toInt() != 0) {
        n.append(tr("相位:%1 ").arg(P));
        if (m->item(TestRow,6)->text().toInt() < abs(P))
            judge = "NG";
    }
    if (m->item(TestRow,7)->text().toInt() != 0) {
        n.append(tr("面积:%1 ").arg(A));
        if (m->item(TestRow,7)->text().toInt() < abs(A))
            judge = "NG";
    }
    if (m->item(TestRow,8)->text().toInt() != 0) {
        n.append(tr("差积:%1 ").arg(D));
        if (m->item(TestRow,8)->text().toInt() < abs(D))
            judge = "NG";
    }
    if (judge == "NG")
        Judge = "NG";

    ItemView[TestRow].insert("TestResult", n);
    ItemView[TestRow].insert("TestResult1", tr("电晕:%1").arg(F));
    ItemView[TestRow].insert("TestResult2", tr("相位:%1").arg(P));
    ItemView[TestRow].insert("TestResult3", tr("面积:%1").arg(A));
    ItemView[TestRow].insert("TestResult4", tr("差积:%1").arg(D));
    ItemView[TestRow].insert("TestJudge", judge);
}

int PageImp::CalculateGear(int row)
{
    int gear = 0;
    if (m->item(row,3)->text().toDouble() <= 1000)
        gear = 1;
    else if (m->item(row,3)->text().toDouble() <= 2000)
        gear = 2;
    else if (m->item(row,3)->text().toDouble() <= 4000)
        gear = 3;
    else if (m->item(row,3)->text().toDouble() <= 5000)
        gear = 4;
    gear <<=  4;
    gear += m->item(row,4)->text().toInt()+1;
    return gear;
}

bool PageImp::WaitTimeOut(quint16 t)
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

void PageImp::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void PageImp::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "PageImp" && s.value("TxAddress") != "WinHome")
        return;
    if (s.value("TxCommand") == "ItemInit") {
        if (s.value("Station").toString() == "left")
            stat = 0x13;
        if (s.value("Station").toString() == "right")
            stat = 0x14;
        InitSet();
        InitItems();
        SendCanCmdConfig();
        SendTestItemsAllEmpty();
    }
    if (s.value("TxCommand") == "StartTest")
        TestThread(s);
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

void PageImp::GoToWindow(QString w)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "JumpWindow");
    hash.insert("TxMessage", w);
    emit SendVariant(hash);
}

void PageImp::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("匝间异常:\n%1").arg(s));
    emit SendVariant(hash);
}

void PageImp::SendTestItemsAllEmpty()
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

void PageImp::SendTestItemsAllError()
{
    QByteArray def;
    for (int i=0; i < 400; i++) {
        def.append(QByteArray(1, 0x02));
        def.append(QByteArray(1, 0x04));
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
    qDebug() << "imp test all error";
}

void PageImp::SendTestWavesAllEmpty()
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinTest");
    hash.insert("TxCommand", "WaveInit");
    emit SendVariant(hash);
}

void PageImp::SendTestItems(int num)
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

void PageImp::SendTestPause()
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "TestPause");
    hash.insert("TxMessage", Judge);
    emit SendVariant(hash);
}

void PageImp::SendTestSave()
{
    for (int i=0; i < ItemView.size(); i++) {
        QVariantHash hash = ItemView.at(i);
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
    hash.insert("ItemName", tr("匝间"));
    hash.insert("TxMessage", Judge);
    emit SendVariant(hash);
}

void PageImp::TestThread(QVariantHash hash)
{
    TestStatus = "buzy";
    Judge = "OK";
    if (hash.value("Station").toString() == "left")
        stat = 0x13;
    if (hash.value("Station").toString() == "right")
        stat = 0x14;
    SendTestWavesAllEmpty();
    SendCanCmdStart(stat);
    if (!WaitTimeOut(500)) {
        Judge = "NG";
        SendTestItemsAllError();
    }
    if (Judge == "NG")
        SendTestPause();
    TestStatus = "free";
}

void PageImp::showEvent(QShowEvent *e)
{
    this->setFocus();
    InitSet();
    InitItems();
    for (int i=0; i < IMP_MAX; i++)
        Waves.at(i)->replot();
    e->accept();
}

void PageImp::on_btn1_clicked()
{
    //    case Qt::Key_6: //添加样品
    //        AvrCount++;
    //        if (AvrCount == 1) {
    //            TestStatus = "sample";
    //            InitStation();
    //            for (int i=0; i < WaveImp.size(); i++) {
    //                WaveImp.at(i)->ShowWave(hash);
    //            }
    //            SendCanCmdConfig();
    //            SendCanCmdSampleAuto();
    //            if (!WaitTimeOut(100))
    //                SendWarnning(tr("采样失败"));
    //            else
    //                SaveSet();
    //            TestStatus = "free";
    //            break;
    //        }
    //        TestStatus = "add";
    //        InitStation();
    //        SendCanCmdConfig();
    //        SendCanCmdStart(stat);
    //        if (!WaitTimeOut(100))
    //            SendWarnning(tr("采样失败"));
    //        else
    //            SaveSet();
    //        TestStatus = "free";


    if (TestStatus != "free")
        return;
    QVector<double> x(1),y(1);
    x[1] = 0;
    y[1] = 0;
    for (int i=0; i < IMP_MAX; i++) {
        Waves.at(i)->graph(0)->setData(x,y);
        Waves.at(i)->replot();
    }
    TestStatus = "sample";
    SendCanCmdConfig();
    SendCanCmdSampleAuto();
    if (!WaitTimeOut(100))
        SendWarnning(tr("采样失败"));
    else
        SaveSet();
    TestStatus = "free";
    this->setFocus();
    for (int i=0; i < IMP_MAX; i++)
        Waves.at(i)->replot();
}

void PageImp::on_btn2_clicked()
{
    if (TestStatus != "free")
        return;
}

void PageImp::on_btn3_clicked()
{
    if (TestStatus != "free")
        return;
    SaveSet();
    GoToWindow(NULL);
}
/*********************************END OF FILE**********************************/
