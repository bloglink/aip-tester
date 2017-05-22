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
    InitWindows();
    InitButtons();
    InitSettings();
    TestStatus = "free";
}

PageImp::~PageImp()
{
    delete ui;
}

void PageImp::InitWindows()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->TabParams->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(3, 50);
    ui->TabParams->horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(5, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(6, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(7, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(8, QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(9, 400);
    ui->TabParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabParams->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(3, 50);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(9, 400);
    ui->TabParams->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(ui->TabParams, SIGNAL(cellClicked(int, int)), this, SLOT(ItemClick(int, int)));
    input = new PageNum(this);
    QStringList t;
    t  << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12";
    input->InitButtons(t);
    connect(input, SIGNAL(ItemChange(QString)), this, SLOT(ItemChange(QString)));
    input->hide();
    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->WaveView, SIGNAL(SendVariant(QVariantHash)), this, SLOT(WaveClick(QVariantHash)));
    connect(ui->BoxStart, SIGNAL(valueChanged(int)), this, SLOT(BlockClick(int)));
    connect(ui->BoxEnd, SIGNAL(valueChanged(int)), this, SLOT(BlockClick(int)));

    ui->TabParams->setRowCount(MAX_ROW);
    for (int row = 0; row  <  MAX_ROW; row++) {
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

        Volt.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 3, Volt.at(row));
        Volt.at(row)->setMaximum(3000);
        Volt.at(row)->setAlignment(Qt::AlignHCenter);
        Volt.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Volt.at(row)->setDecimals(0);

        Time.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 4, Time.at(row));
        Time.at(row)->setMaximum(10);
        Time.at(row)->setAlignment(Qt::AlignHCenter);
        Time.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Time.at(row)->setDecimals(0);

        Flut.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 5, Flut.at(row));
        Flut.at(row)->setMaximum(9999);
        Flut.at(row)->setAlignment(Qt::AlignHCenter);
        Flut.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Flut.at(row)->setDecimals(0);

        Phase.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 6, Phase.at(row));
        Phase.at(row)->setMaximum(9999);
        Phase.at(row)->setAlignment(Qt::AlignHCenter);
        Phase.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Phase.at(row)->setDecimals(0);

        Area.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 7, Area.at(row));
        Area.at(row)->setMaximum(9999);
        Area.at(row)->setAlignment(Qt::AlignHCenter);
        Area.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Area.at(row)->setDecimals(0);

        Diff.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 8, Diff.at(row));
        Diff.at(row)->setMaximum(9999);
        Diff.at(row)->setAlignment(Qt::AlignHCenter);
        Diff.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Diff.at(row)->setDecimals(0);


        Block0.append(5);
        Block1.append(395);
        VoltL.append(0);
        FreqL.append(7);
        VoltR.append(0);
        FreqR.append(7);
        WaveLeft.append("0");
        WaveRight.append("0");

        WaveImp.append(new WaveBox(this));
        ui->TabParams->setCellWidget(row, 9, WaveImp.at(row));
    }
    stat = WIN_ID_OUT13;
    connect(Volt.at(0), SIGNAL(editingFinished()), this, SLOT(AutoChangeVolt()));
    AvrCount = 0;
}

void PageImp::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSampleImp, Qt::Key_0);
    btnGroup->addButton(ui->BtnExitWave, Qt::Key_1);
    btnGroup->addButton(ui->BtnExitImp, Qt::Key_2);
    btnGroup->addButton(ui->BtnFrequcy1, Qt::Key_3);
    btnGroup->addButton(ui->BtnFrequcy2, Qt::Key_4);
    btnGroup->addButton(ui->BtnFrequcy3, Qt::Key_5);
    btnGroup->addButton(ui->BtnAvarage, Qt::Key_6);
    btnGroup->addButton(ui->BtnExit, Qt::Key_7);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void PageImp::ReadButtons(int id)
{
    if (TestStatus != "free")
        return;
    QVariantHash hash;
    switch (id) {
    case Qt::Key_0: //自动采样
        TestStatus = "sample";
        InitStation();
        for (int i=0; i < WaveImp.size(); i++) {
            WaveImp.at(i)->ShowWave(hash);
        }
        SendCanCmdConfig();
        SendCanCmdSampleAuto();
        if (!WaitTimeOut(100))
            SendWarnning(tr("采样失败"));
        else
            SaveSettings();
        TestStatus = "free";
        AvrCount = 0;
        break;
    case Qt::Key_1:
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case Qt::Key_2: //退出
        SaveSettings();
        GoToWindow(NULL);
        break;
    case Qt::Key_3: //减频采样
        TestStatus = "sample";
        InitStation();
        TestRow = ui->TabParams->currentRow();
        if (FreqL[ui->TabParams->currentRow()] != 14)
            FreqL[TestRow]--;
        WaveImp.at(TestRow)->ShowWave(hash);
        WaveView(hash);
        SendCanCmdConfig();
        SendCanCmdSample(TestRow);
        if (!WaitTimeOut(100))
            SendWarnning(tr("采样失败"));
        else
            SaveSettings();
        TestStatus = "free";
        AvrCount = 0;
        break;
    case Qt::Key_4: //定频采样
        TestStatus = "sample";
        InitStation();
        TestRow = ui->TabParams->currentRow();
        WaveImp.at(TestRow)->ShowWave(hash);
        WaveView(hash);
        SendCanCmdConfig();
        SendCanCmdSample(TestRow);
        if (!WaitTimeOut(100))
            SendWarnning(tr("采样失败"));
        else
            SaveSettings();
        TestStatus = "free";
        AvrCount = 0;
        break;
    case Qt::Key_5: //加频采样
        TestStatus = "sample";
        InitStation();
        TestRow = ui->TabParams->currentRow();
        if (FreqL[ui->TabParams->currentRow()] != 14)
            FreqL[TestRow]++;
        WaveImp.at(TestRow)->ShowWave(hash);
        WaveView(hash);
        SendCanCmdConfig();
        SendCanCmdSample(TestRow);
        if (!WaitTimeOut(100))
            SendWarnning(tr("采样失败"));
        else
            SaveSettings();
        TestStatus = "free";
        AvrCount = 0;
        break;
    case Qt::Key_6: //添加样品
        TestStatus = "sample";
        InitStation();
        SendCanCmdConfig();
        SendCanCmdStart(stat);
        if (!WaitTimeOut(100))
            SendWarnning(tr("采样失败"));
        else
            SaveSettings();
        TestStatus = "free";
        AvrCount = 0;
        //        CalculateAvarageWave();
        break;
    case Qt::Key_7:
        GoToWindow(NULL);
        break;
    default:
        break;
    }
}

void PageImp::InitStation()
{
    if (ui->BoxStation->currentIndex()  ==  0)
        stat = WIN_ID_OUT13;
    if (ui->BoxStation->currentIndex()  ==  1)
        stat = WIN_ID_OUT14;
}

void PageImp::InitSettings()
{
    QSettings *global = new QSettings(INI_PATH, QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    FileInUse = global->value("FileInUse", INI_DEFAULT).toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(FileInUse);
    set = new QSettings(t, QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetImp");
    QStringList temp;
    ui->BoxOffset->setChecked(set->value("Offset", true).toBool());
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
    //电压
    temp = (set->value("Volt", "500 500 500 500 500 500 500 500").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Volt.at(row)->setValue(temp.at(row).toDouble());
    //次数
    temp = (set->value("Time", "1 1 1 1 1 1 1 1").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Time.at(row)->setValue(temp.at(row).toDouble());
    //电晕
    temp = (set->value("Flut", "0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Flut.at(row)->setValue(temp.at(row).toDouble());
    //相位
    temp = (set->value("Phase", "10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Phase.at(row)->setValue(temp.at(row).toDouble());
    //面积
    temp = (set->value("Area", "10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Area.at(row)->setValue(temp.at(row).toDouble());
    //差积
    temp = (set->value("Diff", "10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Diff.at(row)->setValue(temp.at(row).toDouble());
    //计算起点
    temp = (set->value("Block0", "5 5 5 5 5 5 5 5").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Block0[row] = temp.at(row).toInt();
    //计算终点
    temp = (set->value("Block1", "395 395 395 395 395 395 395 395").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Block1[row] = temp.at(row).toInt();
    //测试电压
    temp = (set->value("VoltL", "500 500 500 500 500 500 500 500").toString().split(" "));
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        VoltL[row] = temp.at(row).toInt();
    //频率
    temp = (set->value("FreqL", "7 7 7 7 7 7 7 7").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        FreqL[row] = temp.at(row).toInt();
    //测试电压
    temp = (set->value("VoltR", "500 500 500 500 500 500 500 500").toString().split(" "));
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        VoltR[row] = temp.at(row).toInt();
    //频率
    temp = (set->value("FreqL", "7 7 7 7 7 7 7 7").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        FreqR[row] = temp.at(row).toInt();
    //波形
    QVariantHash hash;
    for (int i=0; i < qMin(WaveImp.size(), MAX_ROW); i++) {
        QString ByteL = "WaveImpL"+QString::number(i);
        QString ByteR = "WaveImpR"+QString::number(i);
        WaveLeft[i] = set->value(ByteL).toString();
        WaveRight[i] = set->value(ByteR).toString();
        if (ui->BoxStation->currentIndex() == 0)
            hash.insert("WaveByte", WaveLeft[i]);
        if (ui->BoxStation->currentIndex() == 1)
            hash.insert("WaveByte", WaveRight[i]);
        WaveImp.at(i)->ShowWave(hash);
    }
    qDebug() << QTime::currentTime().toString() << "PageImp read OK";
}

void PageImp::SaveSettings()
{
    set->setValue("Offset", ui->BoxOffset->isChecked());
    QStringList temp;
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
    for (int i=0; i < Volt.size(); i++)
        temp.append(Volt.at(i)->text());
    set->setValue("Volt", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Time.size(); i++)
        temp.append(Time.at(i)->text());
    set->setValue("Time", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Flut.size(); i++)
        temp.append(Flut.at(i)->text());
    set->setValue("Flut", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Phase.size(); i++)
        temp.append(Phase.at(i)->text());
    set->setValue("Phase", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Area.size(); i++)
        temp.append(Area.at(i)->text());
    set->setValue("Area", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Diff.size(); i++)
        temp.append(Diff.at(i)->text());
    set->setValue("Diff", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Block0.size(); i++)
        temp.append(QString::number(Block0.at(i)));
    set->setValue("Block0", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Block1.size(); i++)
        temp.append(QString::number(Block1.at(i)));
    set->setValue("Block1", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < VoltL.size(); i++)
        temp.append(QString::number(VoltL.at(i)));
    set->setValue("VoltL", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < FreqL.size(); i++)
        temp.append(QString::number(FreqL.at(i)));
    set->setValue("FreqL", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < VoltR.size(); i++)
        temp.append(QString::number(VoltL.at(i)));
    set->setValue("VoltR", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < FreqR.size(); i++)
        temp.append(QString::number(FreqL.at(i)));
    set->setValue("FreqR", (temp.join(" ").toUtf8()));

    for (int i=0; i < ItemView.size(); i++) {
        QString ByteL = "WaveImpL"+QString::number(i);
        QString ByteR = "WaveImpR"+QString::number(i);
        set->setValue(ByteL, WaveLeft[i]);
        set->setValue(ByteR, WaveRight[i]);
    }
    qDebug() << QTime::currentTime().toString() << "PageImp save OK";
}

void PageImp::ItemClick(int r,  int c)
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
    case 9:
        TestRow = r;
        WaveView(ItemView[r]);
        ui->stackedWidget->setCurrentIndex(1);
        break;
    default:
        break;
    }
}

void PageImp::ItemChange(QString msg)
{
    ui->TabParams->currentItem()->setText(msg);
}

void PageImp::BlockClick(int x)
{
    if (x < 200)
        Block0[TestRow] = x;
    else
        Block1[TestRow] = x;
    QVariantHash hash = ItemView[TestRow];
    hash.insert("Block0", Block0[TestRow]);
    hash.insert("Block1", Block1[TestRow]);
    hash.insert("WaveItem", hash.value("TestItem").toString());
    ui->WaveView->ShowWave(hash);
}

void PageImp::ExcuteCanCmd(int addr, QByteArray msg)
{
    if (addr != CAN_ID_IMP && addr != CAN_ID_IMP_WAVE)
        return;
    if (TestStatus == "free")
        return;
    TimeOut = 0;
    if (addr  ==  CAN_ID_IMP_WAVE) {
        wave.append(msg);
        return;
    }
    qDebug() << "imp" << msg.toHex();
    if (msg.size() >= 4 && (quint8)msg.at(0) == 0x00)
        ReadCanCmdStatus(msg);
    if (msg.size() >= 7 && (quint8)msg.at(0) == 0x02)
        ReadCanCmdResult(msg);
    if (msg.size() >= 5 && (quint8)msg.at(0) == 0x03 && (quint8)msg.at(1) != 0xff)
        wave.clear();
    if (msg.size() >= 2 && (quint8)msg.at(0) == 0x03 && (quint8)msg.at(1) == 0xff)
        ReadCanCmdWaveOk();
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
    for (int row=0; row < Enable.size(); row++) {
        if (Enable.at(row)->text()  ==  "Y")
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
    for (int row=0; row < Enable.size(); row++) {
        if (Enable.at(row)->text()  ==  "Y")
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
    for (int row=0; row < Enable.size(); row++) {
        int v = 0;
        quint8 f = FreqL.at(row);
//        if (stat == WIN_ID_OUT13)
//            f = FreqL.at(row);
//        if (stat == WIN_ID_OUT14)
//            f = FreqR.at(row);
//        qDebug() << f << FreqL.at(row) << FreqR.at(row);
        if (TestStatus != "sample") {
            if (stat == WIN_ID_OUT13)
                v = VoltL.at(row);
            if (stat == WIN_ID_OUT14)
                v = VoltR.at(row);
        } else {
            v = Volt.at(row)->value();
        }
        out << quint16(0x24) << quint8(0x08) << quint8(0x03) << quint8(row)
            << quint8(Terminal1.at(row)->text().toInt())
            << quint8(Terminal2.at(row)->text().toInt())
            << quint8(v/256) << quint8(v%256)
            << quint8(CalculateGear(row)) << quint8(f);
    }
    emit CanMsg(msg);
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

void PageImp::CalculateResult()
{
    QByteArray byte;
    QByteArray test;
    if (stat == WIN_ID_OUT13)
        byte = QByteArray::fromBase64(WaveLeft[TestRow].toUtf8());
    if (stat == WIN_ID_OUT14)
        byte = QByteArray::fromBase64(WaveRight[TestRow].toUtf8());
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

    if (Flut[TestRow]->value() != 0) {
        n.append(tr("电晕:%1 ").arg(F));
        if (Flut[TestRow]->value() < abs(F))
            judge = "NG";
    }
    if (Phase[TestRow]->value() != 0) {
        n.append(tr("相位:%1 ").arg(P));
        if (Phase[TestRow]->value() < abs(P))
            judge = "NG";
    }
    if (Area[TestRow]->value() != 0) {
        n.append(tr("面积:%1 ").arg(A));
        if (Area[TestRow]->value() < abs(A))
            judge = "NG";
    }
    if (Diff[TestRow]->value() != 0) {
        n.append(tr("差积:%1 ").arg(D));
        if (Diff[TestRow]->value() < abs(D))
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

void PageImp::CalculateAvarageWave()
{
    //    QByteArray w;
    //    quint8 num = stat - WIN_ID_OUT13;
    //    QByteArray byte = WaveImp.at(TestRow)->WaveByte;
    //    QByteArray test = WaveImp.at(TestRow)->WaveBytes[num];
    //    AvrCount++;
    //    for (int i=0; i < qMin(byte.size(), test.size()); i++)
    //        w.append(quint8(test.at(i))+(quint8(test.at(i))-quint8(byte.at(i)))/AvrCount);
    //    if (AvrCount  ==  1)
    //        w = WaveImp.at(TestRow)->WaveByte;
    //    WaveImp.at(TestRow)->WaveBytes[num] = w;
    //    WaveImp.at(TestRow)->WaveByteShow(w);
    //    ui->LabelWave->WaveByteShow(w);
}

void PageImp::ReadCanCmdResult(QByteArray msg)
{
    TestRow = quint8(msg.at(1));
    if (TestStatus != "sample")
        return;
    FreqL[TestRow] = quint8(msg.at(3));
    VoltL[TestRow] = quint16(msg.at(4)*256)+quint8(msg.at(5));
//    if (stat == WIN_ID_OUT13) {
//        FreqL[TestRow] = quint8(msg.at(3));
//        VoltL[TestRow] = quint16(msg.at(4)*256)+quint8(msg.at(5));
//    }
//    if (stat == WIN_ID_OUT14) {
//        FreqR[TestRow] = quint8(msg.at(3));
//        VoltR[TestRow] = quint16(msg.at(4)*256)+quint8(msg.at(5));
//    }
}

void PageImp::ReadCanCmdWaveOk()
{
    if (TestStatus == "sample") {
        ItemView[TestRow].insert("WaveByte", wave.toBase64());
        WaveImp[TestRow]->ShowWave(ItemView[TestRow]);
        WaveView(ItemView[TestRow]);
        if (stat == WIN_ID_OUT13)
            WaveLeft[TestRow] = wave.toBase64();
        if (stat == WIN_ID_OUT14)
            WaveRight[TestRow] = wave.toBase64();
        return;
    }
    ItemView[TestRow].insert("WaveTest", wave.toBase64());
    CalculateResult();
    SendTestItems(TestRow);
}

int PageImp::CalculateGear(int row)
{
    int gear = 0;
    if (Volt.at(row)->value() <= 1000)
        gear = 1;
    else if (Volt.at(row)->value() <= 2000)
        gear = 2;
    else if (Volt.at(row)->value() <= 4000)
        gear = 3;
    else if (Volt.at(row)->value() <= 5000)
        gear = 4;
    gear <<=  4;
    gear += Time.at(row)->value()+1;
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

void PageImp::AutoChangeVolt()
{
    for (int i=1; i < Volt.size(); i++)
        Volt.at(i)->setValue(Volt.at(0)->value());
}

void PageImp::showEvent(QShowEvent *e)
{
    InitSettings();
    SendTestItemsAllEmpty();
    e->accept();
}

void PageImp::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "PageImp" && s.value("TxAddress") != "WinHome")
        return;
    if (s.value("TxCommand") == "ItemInit") {
        if (s.value("Station").toString() == "left")
            stat = WIN_ID_OUT13;
        if (s.value("Station").toString() == "right")
            stat = WIN_ID_OUT14;
        InitSettings();
        SendCanCmdConfig();
        SendTestItemsAllEmpty();
    }
    if (s.value("TxCommand") == "StartTest")
        TestThread(s);
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
    ItemView.clear();
    QString uid = QUuid::createUuid();
    for (int i = 0; i < Enable.size(); i++) {
        QString T1 = Terminal1.at(i)->text();
        QString T2 = Terminal2.at(i)->text();
        QString V = Volt.at(i)->text();
        QString C = Flut.at(i)->text();
        QString P = Phase.at(i)->text();
        QString A = Area.at(i)->text();
        QString D = Diff.at(i)->text();
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
        hash.insert("TestEnable", Enable.at(i)->text());
        hash.insert("TestItem", tr("匝间%1-%2").arg(T1).arg(T2));
        hash.insert("TestPara", para);
        hash.insert("TestResult", " ");
        hash.insert("TestJudge", " ");
        hash.insert("TestUid", uid);
        hash.insert("ItemName", tr("匝间%1").arg(i+1));
        if (stat == WIN_ID_OUT13)
            hash.insert("WaveByte", WaveLeft.at(i));
        if (stat == WIN_ID_OUT14)
            hash.insert("WaveByte", WaveRight.at(i));
        hash.insert("WaveTest", "");
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

void PageImp::WaveView(QVariantHash s)
{
    QVariantHash hash = s;
    hash.insert("Block0", Block0[TestRow]);
    hash.insert("Block1", Block1[TestRow]);
    hash.insert("WaveItem", hash.value("TestItem").toString());
    ui->WaveView->ShowWave(hash);
    ui->BoxStart->setValue(Block0[TestRow]);
    ui->BoxEnd->setValue(Block1[TestRow]);
}

void PageImp::WaveClick(QVariantHash s)
{
    ui->BoxStart->setValue(s.value("Block0").toInt());
    ui->BoxEnd->setValue(s.value("Block1").toInt());
    Block0[TestRow] = s.value("Block0").toInt();
    Block1[TestRow] = s.value("Block1").toInt();
}

void PageImp::TestThread(QVariantHash hash)
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
    }
    if (Judge == "NG")
        SendTestPause();
    TestStatus = "free";
}
/*********************************END OF FILE**********************************/
