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
    InitWindows();
    InitButtons();
    InitSettings();
    TestStatus = "free";
}

PagePwr::~PagePwr()
{
    delete ui;
}

void PagePwr::InitWindows()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->TabParams->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setResizeMode(5, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setResizeMode(6, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setResizeMode(7, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setResizeMode(8, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setResizeMode(9, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setResizeMode(10, QHeaderView::Stretch);
    ui->PGParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabParams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabParams->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);
    ui->PGParams->horizontalHeader()->setSectionResizeMode(10, QHeaderView::Stretch);
    ui->PGParams->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(ui->TabParams, SIGNAL(cellClicked(int, int)), this, SLOT(ItemClick(int, int)));
    ui->TabParams->setRowCount(PWR_ROW);
    for (int row=0; row < PWR_ROW; row++) {
        Enable.append(new QTableWidgetItem);
        ui->TabParams->setItem(row, 0, Enable.at(row));
        Enable.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Enable.at(row)->setTextAlignment(Qt::AlignCenter);

        CurrMin.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 1, CurrMin.at(row));
        CurrMin.at(row)->setMaximum(20);
        CurrMin.at(row)->setAlignment(Qt::AlignHCenter);
        CurrMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        CurrMax.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 2, CurrMax.at(row));
        CurrMax.at(row)->setMaximum(20);
        CurrMax.at(row)->setAlignment(Qt::AlignHCenter);
        CurrMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        PowerMin.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 3, PowerMin.at(row));
        PowerMin.at(row)->setMaximum(9999);
        PowerMin.at(row)->setAlignment(Qt::AlignHCenter);
        PowerMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        PowerMax.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 4, PowerMax.at(row));
        PowerMax.at(row)->setMaximum(9999);
        PowerMax.at(row)->setAlignment(Qt::AlignHCenter);
        PowerMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        CapMin.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 5, CapMin.at(row));
        CapMin.at(row)->setMaximum(9999);
        CapMin.at(row)->setAlignment(Qt::AlignHCenter);
        CapMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        CapMax.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 6, CapMax.at(row));
        CapMax.at(row)->setMaximum(9999);
        CapMax.at(row)->setAlignment(Qt::AlignHCenter);
        CapMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        TestDir.append(new QComboBox(this));
        ui->TabParams->setCellWidget(row, 7, TestDir.at(row));
        QStringList t1;
        t1  << tr("不转") << tr("正转") << tr("反转");
        TestDir.at(row)->setView(new QListView(this));
        TestDir.at(row)->addItems(t1);

        TestTime.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 8, TestTime.at(row));
        TestTime.at(row)->setMinimum(0.6);
        TestTime.at(row)->setMaximum(9999);
        TestTime.at(row)->setAlignment(Qt::AlignHCenter);
        TestTime.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Grade.append(new QTableWidgetItem);
        ui->TabParams->setItem(row, 9, Grade.at(row));
        Grade.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Grade.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    connect(ui->PGParams, SIGNAL(cellClicked(int, int)), this, SLOT(ItemClickPG(int, int)));
    ui->PGParams->setRowCount(PG_ROW);
    for (int row=0; row < PG_ROW; row++) {
        PGEnable.append(new QTableWidgetItem);
        ui->PGParams->setItem(row, 0, PGEnable.at(row));
        PGEnable.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        PGEnable.at(row)->setTextAlignment(Qt::AlignCenter);

        PGUpperMin.append(new QDoubleSpinBox(this));
        ui->PGParams->setCellWidget(row, 1, PGUpperMin.at(row));
        PGUpperMin.at(row)->setMaximum(9999);
        PGUpperMin.at(row)->setAlignment(Qt::AlignHCenter);
        PGUpperMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        PGUpperMax.append(new QDoubleSpinBox(this));
        ui->PGParams->setCellWidget(row, 2, PGUpperMax.at(row));
        PGUpperMax.at(row)->setMaximum(9999);
        PGUpperMax.at(row)->setAlignment(Qt::AlignHCenter);
        PGUpperMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        PGLowerMin.append(new QDoubleSpinBox(this));
        ui->PGParams->setCellWidget(row, 3, PGLowerMin.at(row));
        PGLowerMin.at(row)->setMaximum(9999);
        PGLowerMin.at(row)->setAlignment(Qt::AlignHCenter);
        PGLowerMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        PGLowerMax.append(new QDoubleSpinBox(this));
        ui->PGParams->setCellWidget(row, 4, PGLowerMax.at(row));
        PGLowerMax.at(row)->setMaximum(9999);
        PGLowerMax.at(row)->setAlignment(Qt::AlignHCenter);
        PGLowerMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        PGDutyMin.append(new QDoubleSpinBox(this));
        ui->PGParams->setCellWidget(row, 5, PGDutyMin.at(row));
        PGDutyMin.at(row)->setMaximum(9999);
        PGDutyMin.at(row)->setAlignment(Qt::AlignHCenter);
        PGDutyMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        PGDutyMax.append(new QDoubleSpinBox(this));
        ui->PGParams->setCellWidget(row, 6, PGDutyMax.at(row));
        PGDutyMax.at(row)->setMaximum(9999);
        PGDutyMax.at(row)->setAlignment(Qt::AlignHCenter);
        PGDutyMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        PGFreqMin.append(new QDoubleSpinBox(this));
        ui->PGParams->setCellWidget(row, 7, PGFreqMin.at(row));
        PGFreqMin.at(row)->setMaximum(9999);
        PGFreqMin.at(row)->setAlignment(Qt::AlignHCenter);
        PGFreqMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        PGFreqMax.append(new QDoubleSpinBox(this));
        ui->PGParams->setCellWidget(row, 8, PGFreqMax.at(row));
        PGFreqMax.at(row)->setMaximum(9999);
        PGFreqMax.at(row)->setAlignment(Qt::AlignHCenter);
        PGFreqMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        PGCurrMin.append(new QDoubleSpinBox(this));
        ui->PGParams->setCellWidget(row, 9, PGCurrMin.at(row));
        PGCurrMin.at(row)->setMaximum(9999);
        PGCurrMin.at(row)->setAlignment(Qt::AlignHCenter);
        PGCurrMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        PGCurrMax.append(new QDoubleSpinBox(this));
        ui->PGParams->setCellWidget(row, 10, PGCurrMax.at(row));
        PGCurrMax.at(row)->setMaximum(9999);
        PGCurrMax.at(row)->setAlignment(Qt::AlignHCenter);
        PGCurrMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        PGGrade.append(new QTableWidgetItem);
        ui->PGParams->setItem(row, 11, PGGrade.at(row));
        PGGrade.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        PGGrade.at(row)->setTextAlignment(Qt::AlignCenter);
    }
}

void PagePwr::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExit, Qt::Key_0);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void PagePwr::ReadButtons(int id)
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

void PagePwr::InitSettings()
{
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    set = new QSettings(t, QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("PagePwr");

    QStringList temp = (set->value("Other", "220 50 1 5").toString()).split(" ");
    if (temp.size() >= 3) {
        ui->BoxVolt->setValue(temp.at(0).toDouble());
        ui->BoxFreq->setValue(temp.at(1).toDouble());
        ui->BoxCap->setValue(temp.at(2).toDouble());
        ui->BoxPGVolt->setValue(temp.at(3).toDouble());
    }
    //可用
    temp = (QString(set->value("Enable", "Y Y Y N N N").toByteArray())).split(" ");
    for (int row=0; row < qMin(temp.size(), PWR_ROW); row++)
        Enable.at(row)->setText(temp.at(row));
    //最小电流
    temp = (set->value("CurrMin", "0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PWR_ROW); row++)
        CurrMin.at(row)->setValue(temp.at(row).toDouble());
    //最大电流
    temp = (set->value("CurrMax", "5 5 5 5 5 5").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PWR_ROW); row++)
        CurrMax.at(row)->setValue(temp.at(row).toDouble());
    //最小功率
    temp = (set->value("PowerMin", "0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PWR_ROW); row++)
        PowerMin.at(row)->setValue(temp.at(row).toDouble());
    //最大功率
    temp = (set->value("PowerMax", "500 500 500 500 500 500").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PWR_ROW); row++)
        PowerMax.at(row)->setValue(temp.at(row).toDouble());
    //最小容压
    temp = (set->value("CapMin", "0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PWR_ROW); row++)
        CapMin.at(row)->setValue(temp.at(row).toDouble());
    //最大容压
    temp = (set->value("CapMax", "500 500 500 500 500 500").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PWR_ROW); row++)
        CapMax.at(row)->setValue(temp.at(row).toDouble());
    //转向
    temp = (QString(set->value("TestDir", "0 0 0 0 0 0 0 0").toByteArray())).split(" ");
    for (int row=0; row < qMin(temp.size(), PWR_ROW); row++)
        TestDir.at(row)->setCurrentIndex(temp.at(row).toInt());
    //测试时间
    temp = (set->value("TestTime", "1 1 1 1 1 1").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PWR_ROW); row++)
        TestTime.at(row)->setValue(temp.at(row).toDouble());
    temp.clear();
    temp << "123" << "124" << "125" << "126" << "127" << "128";
    for (int row=0; row < qMin(temp.size(), PWR_ROW); row++)
        Grade.at(row)->setText(temp.at(row));

    //可用
    temp = (QString(set->value("PGEnable", "Y Y Y").toByteArray())).split(" ");
    for (int row=0; row < qMin(temp.size(), PG_ROW); row++)
        PGEnable.at(row)->setText(temp.at(row));

    temp = (set->value("PGUpperMin", "4 4 4").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PG_ROW); row++)
        PGUpperMin.at(row)->setValue(temp.at(row).toDouble());

    temp = (set->value("PGUpperMax", "6 6 6").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PG_ROW); row++)
        PGUpperMax.at(row)->setValue(temp.at(row).toDouble());

    temp = (set->value("PGLowerMin", "0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PG_ROW); row++)
        PGLowerMin.at(row)->setValue(temp.at(row).toDouble());

    temp = (set->value("PGLowerMax", "1 1 1").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PG_ROW); row++)
        PGLowerMax.at(row)->setValue(temp.at(row).toDouble());

    temp = (set->value("PGDutyMin", "40 40 40").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PG_ROW); row++)
        PGDutyMin.at(row)->setValue(temp.at(row).toDouble());

    temp = (set->value("PGDutyMax", "60 60 60").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PG_ROW); row++)
        PGDutyMax.at(row)->setValue(temp.at(row).toDouble());

    temp = (set->value("PGFreqMin", "40 40 40").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PG_ROW); row++)
        PGFreqMin.at(row)->setValue(temp.at(row).toDouble());
    temp = (set->value("PGFreqMax", "60 60 60").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PG_ROW); row++)
        PGFreqMax.at(row)->setValue(temp.at(row).toDouble());

    temp = (set->value("PGCurrMin", "0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PG_ROW); row++)
        PGCurrMin.at(row)->setValue(temp.at(row).toDouble());

    temp = (set->value("PGCurrMax", "1 1 1").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), PG_ROW); row++)
        PGCurrMax.at(row)->setValue(temp.at(row).toDouble());
    temp.clear();
    temp << "123" << "124" << "125";
    for (int row=0; row < qMin(temp.size(), PG_ROW); row++)
        PGGrade.at(row)->setText(temp.at(row));

    if (!IsPGEnable()) {
        ui->BoxPG->hide();
        for (int i=0; i < PG_ROW; i++)
            PGEnable.at(i)->setText("N");
    }
}

void PagePwr::SaveSettings()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxVolt->value()));
    temp.append(QString::number(ui->BoxFreq->value()));
    temp.append(QString::number(ui->BoxCap->value()));
    temp.append(QString::number(ui->BoxPGVolt->value()));
    set->setValue("Other", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < Enable.size(); i++)
        temp.append(Enable.at(i)->text());
    set->setValue("Enable", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < CurrMax.size(); i++)
        temp.append(QString::number(CurrMax.at(i)->value()));
    set->setValue("CurrMax", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < CurrMin.size(); i++)
        temp.append(QString::number(CurrMin.at(i)->value()));
    set->setValue("CurrMin", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < PowerMax.size(); i++)
        temp.append(QString::number(PowerMax.at(i)->value()));
    set->setValue("PowerMax", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < PowerMin.size(); i++)
        temp.append(QString::number(PowerMin.at(i)->value()));
    set->setValue("PowerMin", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < CapMax.size(); i++)
        temp.append(QString::number(CapMax.at(i)->value()));
    set->setValue("CapMax", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < CapMin.size(); i++)
        temp.append(QString::number(CapMin.at(i)->value()));
    set->setValue("CapMin", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < TestDir.size(); i++)
        temp.append(QString::number(TestDir.at(i)->currentIndex()));
    set->setValue("TestDir", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < TestTime.size(); i++)
        temp.append(QString::number(TestTime.at(i)->value()));
    set->setValue("TestTime", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < Grade.size(); i++)
        temp.append(Grade.at(i)->text());
    set->setValue("Grade", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < PGEnable.size(); i++)
        temp.append(PGEnable.at(i)->text());
    set->setValue("PGEnable", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < PGUpperMin.size(); i++)
        temp.append(QString::number(PGUpperMin.at(i)->value()));
    set->setValue("PGUpperMin", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < PGUpperMax.size(); i++)
        temp.append(QString::number(PGUpperMax.at(i)->value()));
    set->setValue("PGUpperMax", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < PGLowerMin.size(); i++)
        temp.append(QString::number(PGLowerMin.at(i)->value()));
    set->setValue("PGLowerMin", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < PGLowerMax.size(); i++)
        temp.append(QString::number(PGLowerMax.at(i)->value()));
    set->setValue("PGLowerMax", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < PGDutyMin.size(); i++)
        temp.append(QString::number(PGDutyMin.at(i)->value()));
    set->setValue("PGDutyMin", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < PGDutyMax.size(); i++)
        temp.append(QString::number(PGDutyMax.at(i)->value()));
    set->setValue("PGDutyMax", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < PGFreqMin.size(); i++)
        temp.append(QString::number(PGFreqMin.at(i)->value()));
    set->setValue("PGFreqMin", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < PGFreqMax.size(); i++)
        temp.append(QString::number(PGFreqMax.at(i)->value()));
    set->setValue("PGFreqMax", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < PGCurrMin.size(); i++)
        temp.append(QString::number(PGCurrMin.at(i)->value()));
    set->setValue("PGCurrMin", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < PGCurrMax.size(); i++)
        temp.append(QString::number(PGCurrMax.at(i)->value()));
    set->setValue("PGCurrMax", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < PGGrade.size(); i++)
        temp.append(PGGrade.at(i)->text());
    set->setValue("PGGrade", (temp.join(" ").toUtf8()));
}

void PagePwr::ItemClick(int r,  int c)
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
    default:
        break;
    }
}

void PagePwr::ItemClickPG(int r,  int c)
{
    switch (c) {
    case 0:
        if (PGEnable.at(r)->text() != "Y") {
            Enable.at(r)->setText("Y");
            PGEnable.at(r)->setText("Y");
        } else {
            PGEnable.at(r)->setText("N");
        }
        break;
    case 1:
    case 2:
    default:
        break;
    }
}

void PagePwr::ExcuteCanCmd(int addr,  QByteArray msg)
{
    if (addr != CAN_ID_PWR && addr != CAN_ID_PG_WAVE)
        return;
    if (TestStatus == "free")
        return;
    TimeOut = 0;
    if (addr == CAN_ID_PG_WAVE) {
        wave.append(msg);
        return;
    }
    quint8 cmd = (quint8)msg.at(0);
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
    case 0x0A:
        ReadCanCmdPGCurrs(msg);
        break;
    case 0x0B:
        ReadCanCmdPGFreqs(msg);
        break;
    case 0x0C:
        ReadCanCmdPGDutys(msg);
        break;
    case 0x0D:
        if ((quint8)msg.at(1) == 0x00)
            wave.clear();
        else
            ReadCanCmdPGWaveStop();
        break;
    default:
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

void PagePwr::ReadCanCmdPGCurrs(QByteArray msg)
{
    if (TestStatus == "safe")
        return;
    PGCurrs.append(quint16(msg.at(1)*256+quint8(msg.at(2))));
    PGLowers.append(quint16(msg.at(3)*256+quint8(msg.at(4))));
    PGUppers.append(quint16(msg.at(5)*256+quint8(msg.at(6))));
}

void PagePwr::ReadCanCmdPGFreqs(QByteArray msg)
{
    if (TestStatus == "safe")
        return;
    PGFreqAvr.append(quint16(msg.at(3)*256+quint8(msg.at(4))));
}

void PagePwr::ReadCanCmdPGDutys(QByteArray msg)
{
    if (TestStatus == "safe")
        return;
    PGDutyAvr.append(quint16(msg.at(3)*256+quint8(msg.at(4))));
}


void PagePwr::ReadCanCmdPGWaveStop()
{
    if (TestStatus == "safe")
        return;
    QString hhh = QString::number(PGUppers.last()/100, 'f', 2);
    QString lll = QString::number(PGLowers.last()/100, 'f', 2);
    QString ddd = QString::number(PGDutyAvr.last()/10, 'f', 2);
    QString fff = QString::number(PGFreqAvr.last()/10, 'f', 0);
    QString ccc = QString::number(PGCurrs.last()/100, 'f', 2);
    QString t = QString("H:%1V,L:%2V,D:%3,F:%4Hz,%5mA").
            arg(hhh).arg(lll).arg(ddd).arg(fff).arg(ccc);
    PGView[TestRow].insert("WaveTest", wave.toBase64());
    PGView[TestRow].insert("TestResult", t);
    SendPGItems(TestRow);

    if (PGUppers.last()/100 > PGUpperMax.at(TestRow)->value())
        PGJudge = "NG";
    if (PGUppers.last()/100 < PGUpperMin.at(TestRow)->value())
        PGJudge = "NG";
    if (PGLowers.last()/100 > PGLowerMax.at(TestRow)->value())
        PGJudge = "NG";
    if (PGLowers.last()/100 < PGLowerMin.at(TestRow)->value())
        PGJudge = "NG";
    if (PGDutyAvr.last()/10 > PGDutyMax.at(TestRow)->value())
        PGJudge = "NG";
    if (PGDutyAvr.last()/10 < PGDutyMin.at(TestRow)->value())
        PGJudge = "NG";
    if (PGFreqAvr.last()/10 > PGFreqMax.at(TestRow)->value())
        PGJudge = "NG";
    if (PGFreqAvr.last()/10 < PGFreqMin.at(TestRow)->value())
        PGJudge = "NG";
    if (PGCurrs.last()/100 > PGCurrMax.at(TestRow)->value())
        PGJudge = "NG";
    if (PGCurrs.last()/100 < PGCurrMin.at(TestRow)->value())
        PGJudge = "NG";
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
    quint16 t = TestTime.at(TestRow)->value()*10;
    quint8 p = CurrentPorwer().toInt() << 4;
    quint8 vv = 0;
    if (IsPGEnable() || PGEnable.at(TestRow)->text() == "Y")
        vv = ui->BoxPGVolt->value()*10;
    quint8 g = TestRow+1;
    if (ui->BoxFreq->value() == 60)
        p += 0x02;
    if (IsPGEnable() && PGEnable.at(TestRow)->text() == "Y")
        g += 0x08;
    if (s == WIN_ID_OUT14)
        g  <<= 4;
    quint8 dir = CurrentPowerDir();
    if (TestDir.at(TestRow)->currentIndex() == 0)
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
    if (stat == WIN_ID_OUT14)
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

    if (rr > CurrMax.at(TestRow)->value() ||
            rr < CurrMin.at(TestRow)->value() ||
            pp > PowerMax.at(TestRow)->value() ||
            pp < PowerMin.at(TestRow)->value() ||
            cc > CapMax.at(TestRow)->value() ||
            cc < CapMin.at(TestRow)->value()) {
        PwrJudge = "NG";
    }
}

void PagePwr::ClearResults()
{
    Volt.clear();
    Curr.clear();
    Power.clear();
    CVolt.clear();

    PGCurrs.clear();
    PGUppers.clear();
    PGLowers.clear();
    PGFreqAvr.clear();
    PGDutyAvr.clear();

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

QString PagePwr::CurrentSettings()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", INI_DEFAULT).toString();
    return n.remove(".ini");
}

QString PagePwr::CurrentPorwer()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/PowerSupply", "0").toString();
    return n;
}

int PagePwr::CurrentPowerDir()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    return ini->value("/GLOBAL/PowerDir", "0").toInt();
}

bool PagePwr::IsPGEnable()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    return ini->value("/GLOBAL/EnablePG", false).toBool();
}

void PagePwr::showEvent(QShowEvent *e)
{
    InitSettings();
    e->accept();
}

void PagePwr::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "PagePwr" && s.value("TxAddress") != "WinHome")
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
    PGView.clear();
    QString uid = QUuid::createUuid().toString();
    for (int i = 0; i < Enable.size(); i++) {
        QString G1 = Grade.at(i)->text();
        double I1 = CurrMin.at(i)->value();
        double I2 = CurrMax.at(i)->value();
        double P1 = PowerMin.at(i)->value();
        double P2 = PowerMax.at(i)->value();
        double C1 = CapMin.at(i)->value();
        double C2 = CapMax.at(i)->value();

        QVariantHash hash;
        hash.insert("TestEnable", Enable.at(i)->text());
        hash.insert("TestItem", tr("功率%1").arg(G1));
        hash.insert("TestPara", tr("%1~%2A %3~%4W %5~%6V").arg(I1).arg(I2).arg(P1).arg(P2).arg(C1).arg(C2));
        hash.insert("TestResult", " ");
        hash.insert("TestJudge", " ");
        hash.insert("TestUid", uid);
        PwrView.append(hash);

        if (TestDir.at(i)->currentIndex() != 0)
            hash.insert("TestEnable", "Y");
        else
            hash.insert("TestEnable", "N");
        hash.insert("TestItem", tr("转向%1").arg(G1));
        hash.insert("TestPara", tr("%1").arg(TestDir.at(i)->currentText()));
        hash.insert("TestResult", " ");
        hash.insert("TestJudge", " ");
        hash.insert("TestUid", uid);
        DirView.append(hash);
    }
    for (int i = 0; i < PGEnable.size(); i++) {
        QString G1 = PGGrade.at(i)->text();
        double H = PGUpperMax.at(i)->value();
        double L = PGLowerMax.at(i)->value();
        double F = PGFreqMax.at(i)->value();
        double C = PGCurrMax.at(i)->value();
        double D = PGDutyMax.at(i)->value();

        QVariantHash hash;
        hash.insert("TestEnable", PGEnable.at(i)->text());
        hash.insert("TestItem", tr("PG%1").arg(G1));
        hash.insert("TestPara", tr("H:%1, L:%2, D:%3F:%4, %5mA").arg(H).arg(L).arg(D).arg(F).arg(C));
        hash.insert("TestResult", " ");
        hash.insert("TestJudge", " ");
        hash.insert("TestUid", uid);
        PGView.append(hash);
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
        if (i >= PGView.size())
            continue;
        hash = PGView.at(i);
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
        if (i >= PGView.size())
            continue;
        hash = PGView.at(i);
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
        stat = WIN_ID_OUT13;
    if (hash.value("Station").toString() == "right")
        stat = WIN_ID_OUT14;
    for (int row = 0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
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
                QString n = TestDir.at(TestRow)->currentText();
                DirJudge = (Dir.last() == n) ? "OK":"NG";
                DirView[TestRow].insert("TestJudge", DirJudge);
                SendDirItems(TestRow);
                PGView[TestRow].insert("TestJudge", PGJudge);
                SendPGItems(TestRow);
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
        stat = WIN_ID_OUT13;
    if (hash.value("Station").toString() == "right")
        stat = WIN_ID_OUT14;
    TestStatus = "safe";
    TestRow = 0;
    SendCanCmdStartSafe();
    WaitTimeOut(100);
    ClearResults();
    TestStatus = "free";
}

/*********************************END OF FILE**********************************/
