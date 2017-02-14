/********************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       匝间设置界面
 * date:        2017.01.04
 * brief:       增加档位
 * date:        2017.01.07
 * brief:       修正档位数据
********************************************************************************/
#include "CWinSetImp.h"
#include "ui_CWinSetImp.h"
/********************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       构造
********************************************************************************/
CWinSetImp::CWinSetImp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CWinSetImp)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
    DisplayInit();
}
/********************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       析构
********************************************************************************/
CWinSetImp::~CWinSetImp()
{
    delete ui;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       初始化界面
*******************************************************************************/
void CWinSetImp::WinInit()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabSetImp->horizontalHeader()->setResizeMode(0,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setResizeMode(4,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setResizeMode(5,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setResizeMode(6,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setResizeMode(7,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setResizeMode(8,QHeaderView::Stretch);
    ui->TabSetImp->setColumnWidth(9,400);
    ui->TabSetImp->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabSetImp->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Stretch);
    ui->TabSetImp->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Stretch);
    ui->TabSetImp->setColumnWidth(9,400);
    ui->TabSetImp->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(ui->TabSetImp,SIGNAL(cellClicked(int,int)),this,SLOT(ItemClick(int,int)));
    input = new InputNum(this);
    QStringList t;
    t <<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12";
    input->BtnInit(t);
    connect(input,SIGNAL(ItemChange(QString)),this,SLOT(ItemChange(QString)));
    input->hide();
    ui->WindgetSetImp->setCurrentIndex(0);

    connect(ui->LabelWave,SIGNAL(BlockClick(int)),this,SLOT(BlockClick(int)));
    connect(ui->BoxStart,SIGNAL(valueChanged(int)),this,SLOT(BlockClick(int)));
    connect(ui->BoxEnd,SIGNAL(valueChanged(int)),this,SLOT(BlockClick(int)));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       初始化按键
*******************************************************************************/
void CWinSetImp::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSampleImp,Qt::Key_0);
    btnGroup->addButton(ui->BtnExitWave,Qt::Key_1);
    btnGroup->addButton(ui->BtnExitImp,Qt::Key_2);
    btnGroup->addButton(ui->BtnFrequcy1,Qt::Key_3);
    btnGroup->addButton(ui->BtnFrequcy2,Qt::Key_4);
    btnGroup->addButton(ui->BtnFrequcy3,Qt::Key_5);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       按键功能
 * date:        2017.01.04
 * brief:       增加档位限制
*******************************************************************************/
void CWinSetImp::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        CmdConfigure();
        CmdStartSample();
        break;
    case Qt::Key_1:
        ui->WindgetSetImp->setCurrentIndex(0);
        break;
    case Qt::Key_2:
        emit TransformCmd(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    case Qt::Key_3:
        if (Freq[ui->TabSetImp->currentRow()] != 0)
            Freq[ui->TabSetImp->currentRow()]--;
        CmdConfigure();
        CmdSample(ui->TabSetImp->currentRow());
        break;
    case Qt::Key_4:
        CmdConfigure();
        CmdSample(ui->TabSetImp->currentRow());
        break;
    case Qt::Key_5:
        if (Freq[ui->TabSetImp->currentRow()] != 14)
            Freq[ui->TabSetImp->currentRow()]++;
        CmdConfigure();
        CmdSample(ui->TabSetImp->currentRow());
        break;
    default:
        break;
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       数据初始化
*******************************************************************************/
void CWinSetImp::DatInit()
{
    QSettings *global = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");

    //当前使用的测试项目
    QString t = QString("./config/%1").arg(global->value("FileInUse","default.ini").toString());
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetImp");
    //可用
    QStringList temp = (QString(set->value("Enable","0 0 0 0 0 0 0 0").toByteArray())).split(" ");
    if (temp.size() > 8)
        ui->TabSetImp->setRowCount(temp.size());
    else
        ui->TabSetImp->setRowCount(8);
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Enable.size() > row)
            continue;
        Enable.append(new QTableWidgetItem);
        ui->TabSetImp->setItem(row,0,Enable.at(row));
        Enable.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Enable.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Enable.size() <= row)
            break;
        Enable.at(row)->setText(temp.at(row));
    }
    //端一
    temp = (set->value("Terminal1","0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Terminal1.size() > row)
            continue;
        Terminal1.append(new QTableWidgetItem);
        ui->TabSetImp->setItem(row,1,Terminal1.at(row));
        Terminal1.at(row)->setText(temp.at(row));
        Terminal1.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal1.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Terminal1.size() <= row)
            break;
        Terminal1.at(row)->setText(temp.at(row));
    }
    //端二
    temp = (set->value("Terminal2","1 1 1 1 1 1 1 1").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Terminal2.size() > row)
            continue;
        Terminal2.append(new QTableWidgetItem);
        ui->TabSetImp->setItem(row,2,Terminal2.at(row));
        Terminal2.at(row)->setText(temp.at(row));
        Terminal2.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal2.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Terminal2.size() <= row)
            break;
        Terminal2.at(row)->setText(temp.at(row));
    }
    //电压
    temp = (set->value("Volt","500 500 500 500 500 500 500 500").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Volt.size() > row)
            continue;
        Volt.append(new QDoubleSpinBox(this));
        ui->TabSetImp->setCellWidget(row,3,Volt.at(row));
        Volt.at(row)->setDecimals(0);
        Volt.at(row)->setMaximum(3000);
        Volt.at(row)->setAlignment(Qt::AlignHCenter);

        Volt.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Volt.size() <= row)
            break;
        Volt.at(row)->setValue(temp.at(row).toDouble());
    }
    connect(Volt.at(0),SIGNAL(editingFinished()),this,SLOT(VoltEdit()));
    //次数
    temp = (set->value("Time","1 1 1 1 1 1 1 1").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Time.size() > row)
            continue;
        Time.append(new QDoubleSpinBox(this));
        ui->TabSetImp->setCellWidget(row,4,Time.at(row));
        Time.at(row)->setDecimals(0);
        Time.at(row)->setMaximum(99);
        Time.at(row)->setAlignment(Qt::AlignHCenter);
        Time.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Time.size() <= row)
            break;
        Time.at(row)->setValue(temp.at(row).toDouble());
    }
    //电晕
    temp = (set->value("Flut","10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Flut.size() > row)
            continue;
        Flut.append(new QDoubleSpinBox(this));
        ui->TabSetImp->setCellWidget(row,5,Flut.at(row));
        Flut.at(row)->setDecimals(0);
        Flut.at(row)->setMaximum(9999);
        Flut.at(row)->setAlignment(Qt::AlignHCenter);
        Flut.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Flut.size() <= row)
            break;
        Flut.at(row)->setValue(temp.at(row).toDouble());
    }
    //相位
    temp = (set->value("Phase","10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Phase.size() > row)
            continue;
        Phase.append(new QDoubleSpinBox(this));
        ui->TabSetImp->setCellWidget(row,6,Phase.at(row));
        Phase.at(row)->setDecimals(0);
        Phase.at(row)->setMaximum(9999);
        Phase.at(row)->setAlignment(Qt::AlignHCenter);
        Phase.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Phase.size() <= row)
            break;
        Phase.at(row)->setValue(temp.at(row).toDouble());
    }
    //面积
    temp = (set->value("Area","10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Area.size() > row)
            continue;
        Area.append(new QDoubleSpinBox(this));
        ui->TabSetImp->setCellWidget(row,7,Area.at(row));
        Area.at(row)->setDecimals(0);
        Area.at(row)->setMaximum(9999);
        Area.at(row)->setAlignment(Qt::AlignHCenter);
        Area.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Area.size() <= row)
            break;
        Area.at(row)->setValue(temp.at(row).toDouble());
    }
    //差积
    temp = (set->value("Diff","10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Diff.size() > row)
            continue;
        Diff.append(new QDoubleSpinBox(this));
        ui->TabSetImp->setCellWidget(row,8,Diff.at(row));
        Diff.at(row)->setDecimals(0);
        Diff.at(row)->setMaximum(9999);
        Diff.at(row)->setAlignment(Qt::AlignHCenter);
        Diff.at(row)->setValue(temp.at(row).toDouble());
        Diff.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Diff.size() <= row)
            break;
        Diff.at(row)->setValue(temp.at(row).toDouble());
    }
    //频率
    temp = (set->value("Freq","7 7 7 7 7 7 7 7").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Freq.size() > row)
            continue;
        Freq.append(temp.at(row).toInt());
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Freq.size() <= row)
            break;
        Freq[row] = temp.at(row).toInt();
    }
    //计算起点
    temp = (set->value("Block0","5 5 5 5 5 5 5 5").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Block0.size() > row)
            continue;
        Block0.append(temp.at(row).toInt());
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Block0.size() <= row)
            break;
        Block0[row] = temp.at(row).toInt();
    }
    //计算终点
    temp = (set->value("Block1","395 395 395 395 395 395 395 395").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Block1.size() > row)
            continue;
        Block1.append(temp.at(row).toInt());
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetImp->rowCount()); row++) {
        if (Block1.size() <= row)
            break;
        Block1[row] = temp.at(row).toInt();
    }
    //波形
    for (int row=0; row<ui->TabSetImp->rowCount(); row++) {
        if (WaveImp.size() > row)
            continue;
        WaveImp.append(new Waveform(this));
        ui->TabSetImp->setCellWidget(row,9,WaveImp.at(row));
    }
    if (WaveImp.size() != 8)
        return;
    QByteArray w;
    for (int row=0; row<qMin(WaveImp.size(),ui->TabSetImp->rowCount()); row++) {
        QString byte = "WaveImp"+QString::number(row);
        w = set->value(byte,"1000000000000000").toByteArray();
        WaveImp.at(row)->WaveByteShow(w);
        QString T1 = Terminal1.at(row)->text();
        QString T2 = Terminal2.at(row)->text();
        WaveImp.at(row)->WaveItem = QString(tr("匝间%1-%2")).arg(T1).arg(T2).toUtf8();
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       数据保存
*******************************************************************************/
void CWinSetImp::DatSave()
{
    QStringList temp;
    temp.clear();
    for (int i=0; i<Enable.size(); i++)
        temp.append(Enable.at(i)->text());
    set->setValue("Enable",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Terminal1.size(); i++)
        temp.append(Terminal1.at(i)->text());
    set->setValue("Terminal1",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Terminal2.size(); i++)
        temp.append(Terminal2.at(i)->text());
    set->setValue("Terminal2",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Volt.size(); i++)
        temp.append(Volt.at(i)->text());
    set->setValue("Volt",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Time.size(); i++)
        temp.append(Time.at(i)->text());
    set->setValue("Time",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Flut.size(); i++)
        temp.append(Flut.at(i)->text());
    set->setValue("Flut",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Phase.size(); i++)
        temp.append(Phase.at(i)->text());
    set->setValue("Phase",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Area.size(); i++)
        temp.append(Area.at(i)->text());
    set->setValue("Area",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Diff.size(); i++)
        temp.append(Diff.at(i)->text());
    set->setValue("Diff",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Freq.size(); i++)
        temp.append(QString::number(Freq.at(i)));
    set->setValue("Freq",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Block0.size(); i++)
        temp.append(QString::number(Block0.at(i)));
    set->setValue("Block0",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Block1.size(); i++)
        temp.append(QString::number(Block1.at(i)));
    set->setValue("Block1",(temp.join(" ").toUtf8()));

    for (int row=0; row<qMin(WaveImp.size(),ui->TabSetImp->rowCount()); row++) {
        QString byte = "WaveImp"+QString::number(row);
        set->setValue(byte,WaveImp.at(row)->WaveByte);
    }
}

void CWinSetImp::VoltEdit()
{
    for (int i=0; i<Volt.size(); i++)
        Volt.at(i)->setValue(Volt.at(0)->value());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       更新显示
*******************************************************************************/
void CWinSetImp::DisplayInit()
{
    ListItem.clear();
    ListPara.clear();
    WaveNumber.clear();
    ListResult.clear();
    ListJudge.clear();
    for (int row = 0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            if (row >= Terminal1.size())
                break;
            if (row >= Terminal2.size())
                break;
            QString T1 = Terminal1.at(row)->text();
            QString T2 = Terminal2.at(row)->text();
            ListItem.append(QString(tr("匝间%1-%2")).arg(T1).arg(T2));
            if (row >= Volt.size())
                break;
            if (row >= Time.size())
                break;
            if (row >= Flut.size())
                break;
            if (row >= Phase.size())
                break;
            if (row >= Area.size())
                break;
            if (row >= Diff.size())
                break;
            QString V = Volt.at(row)->text();
            QString C = Flut.at(row)->text();
            QString P = Phase.at(row)->text();
            QString A = Area.at(row)->text();
            QString D = Diff.at(row)->text();
            ListPara.append(QString("%1V,%2,%3,%4,%5").arg(V).arg(C).arg(P).arg(A).arg(D));
            WaveNumber.append(row);
            ListResult.append(" ");
            ListJudge.append(" ");
            WaveImp.at(row)->WaveTest.clear();
        }
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       点击
*******************************************************************************/
void CWinSetImp::ItemClick(int r, int c)
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
        ui->WindgetSetImp->setCurrentIndex(1);
        ui->LabelWave->WaveByteShow(WaveImp.at(r)->WaveByte);
        ui->LabelWave->WaveItemShow(WaveImp.at(r)->WaveItem);
        ui->LabelWave->WaveBlock(Block0.at(r),Block1.at(r));
        ui->BoxStart->setValue(Block0.at(r));
        ui->BoxEnd->setValue(Block1.at(r));
        CurrentWave = r;
        break;
    default:
        break;
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       切换文字
*******************************************************************************/
void CWinSetImp::ItemChange(QString msg)
{
    ui->TabSetImp->currentItem()->setText(msg);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       切换文字
*******************************************************************************/
void CWinSetImp::BlockClick(int x)
{
    if (x >=200) {
        ui->LabelWave->WaveBlock(ui->LabelWave->Block0,x);
        ui->BoxEnd->setValue(x);
        Block1[CurrentWave] = x;
    } else {
        ui->LabelWave->WaveBlock(x,ui->LabelWave->Block1);
        ui->BoxStart->setValue(x);
        Block0[CurrentWave] = x;
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       等待测试结束
*******************************************************************************/
bool CWinSetImp::WaitTestOver()
{
    TimeOut = 0;
    while (Testing) {
        Delay(10);
        TimeOut++;
        if (TimeOut > 50) {
            for (int i=0; i<ListResult.size(); i++) {
                if (ListResult.at(i) == " ") {
                    ListResult[i] = "---";
                }
            }
            for (int i=0; i<ListJudge.size(); i++) {
                if (ListJudge.at(i) == " ") {
                    ListJudge[i] = "NG";
                }
            }
            Testing = false;
            emit TransformCmd(ADDR,WIN_CMD_RESULT,NULL);
            return false;
        }
    }
    return true;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       延时
*******************************************************************************/
void CWinSetImp::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       命令处理
*******************************************************************************/
void CWinSetImp::ExcuteCmd(int id,QByteArray msg)
{
    if (!Testing && !Sampling)
        return;
    TimeOut = 0;
    if (id == CAN_ID_IMP_WAVE) {
        UpdateWave(msg);
        return;
    }
    if (msg.size()==4 && (quint8)msg.at(0)==0x00) {
        UpdateState(msg);
    }
    if (msg.size()==7 && (quint8)msg.at(0)==0x02) {
        UpdateWaveFreq(msg);
    }
    if (msg.size()==5 && (quint8)msg.at(0)==0x03) {
        CurrentWave = (quint8)msg.at(1);
        if (Sampling) {
            WaveImp.at(CurrentWave)->WaveByte.clear();
        }
        if (Testing) {
            WaveImp.at(CurrentWave)->WaveTest.clear();
            QByteArray w = WaveImp.at(CurrentWave)->WaveByte;
            QByteArray i = WaveImp.at(CurrentWave)->WaveItem;
            emit TransformCmd(ADDR,WIN_WAVE_ITEM,i);
            emit TransformCmd(ADDR,WIN_WAVE_BYTE,w);
        }
    }
    if (msg.size()==2 && (quint8)msg.at(0)==0x03 && (quint8)msg.at(1)==0xff) {
        QByteArray w;
        if (Sampling) {
            w = WaveImp.at(CurrentWave)->WaveByte;
            WaveImp.at(CurrentWave)->WaveByteShow(w);
            ui->LabelWave->WaveByteShow(w);
        }
        if (Testing) {
            w = WaveImp.at(CurrentWave)->WaveTest;
            emit TransformCmd(ADDR,WIN_WAVE_TEST,w);
            UpdateTestData(msg);
        }
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       接收波形
*******************************************************************************/
void CWinSetImp::UpdateWave(QByteArray msg)
{
    if (Sampling)
        WaveImp.at(CurrentWave)->WaveByte.append(msg);
    else
        WaveImp.at(CurrentWave)->WaveTest.append(msg);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       更新波形频率
*******************************************************************************/
void CWinSetImp::UpdateWaveFreq(QByteArray msg)
{
    quint8 num = quint8(msg.at(1));
    Freq[num] = quint8(msg.at(3));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       更新状态
*******************************************************************************/
void CWinSetImp::UpdateState(QByteArray)
{
    if (Testing) {
        Testing = false;
    }
    if (Sampling) {
        Sampling = false;
        emit TransformCmd(ADDR,WIN_CMD_DEBUG,"IMP Sample ok\n");
    }
    if (!isCheckOk) {
        isCheckOk = true;
        emit TransformCmd(ADDR,WIN_CMD_DEBUG,"IMP check ok\n");
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       更新测试数据
*******************************************************************************/
void CWinSetImp::UpdateTestData(QByteArray )
{
    if (Sampling)
        return;
    WaveImp.at(CurrentWave)->InitWaveByte();
    WaveImp.at(CurrentWave)->InitWaveTest();
    qint32 Area1=0;
    qint32 Area2=0;
    qint32 Area3=0;
    qint32 Phase1 = 0;
    qint32 Phase2 = 0;
    qint32 F = 0;
    qint32 A = 0;
    qint32 D = 0;
    qint32 P = 0;
    quint16 s = Block0.at(CurrentWave);
    quint16 e = Block1.at(CurrentWave);
    if (s < 1)
        s = 1;
    if (e > 398)
        e = 398;
    for (int i=s; i<e; i++) {
        int a1 = WaveImp.at(CurrentWave)->WaveByteH.at(i);
        int a2 = WaveImp.at(CurrentWave)->WaveTestH.at(i);
        Area1 += abs(a1-0x200);
        Area2 += abs(a2-0x200);
        int b1 = WaveImp.at(CurrentWave)->WaveByteH.at(i-1);
        int b2 = WaveImp.at(CurrentWave)->WaveByteH.at(i);
        int b3 = WaveImp.at(CurrentWave)->WaveByteH.at(i+1);
        int c1 = WaveImp.at(CurrentWave)->WaveTestH.at(i-1);
        int c2 = WaveImp.at(CurrentWave)->WaveTestH.at(i);
        int c3 = WaveImp.at(CurrentWave)->WaveTestH.at(i+1);
        Area3 += abs((b1+b2*2+b3)-(c1+c2*2+c3));

        F +=abs(c2-c3);
        Phase1 += (a1-0x200)*(a1-0x200);
        Phase2 += (a1-0x200)*(a2-0x200);
    }
    A = (Area2-Area1)*100/Area1;
    D = qMin(Area2,Area3/4)*100/Area1;
    P = (Phase1-Phase2)*100/Phase1;
    UpdateResult(QString("A:%1%,D:%2%,F:%3,P:%4%").arg(A).arg(D).arg(F).arg(P).toUtf8());
    quint8 number = quint8(CurrentWave);
    if (number >= Area.size() || abs(A) >= Area.at(number)->value())
        UpdateJudge("NG");
    else if (number >= Diff.size() || abs(D) >= Diff.at(number)->value())
        UpdateJudge("NG");
    else if (number >= Flut.size() || abs(F) >= Flut.at(number)->value())
        UpdateJudge("NG");
    else if (number >= Phase.size() || abs(P) >= Phase.at(number)->value())
        UpdateJudge("NG");
    else
        UpdateJudge("OK");
    emit TransformCmd(ADDR,WIN_CMD_RESULT,NULL);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       添加测试数据
*******************************************************************************/
void CWinSetImp::UpdateResult(QByteArray msg)
{
    for (int i=0; i<ListResult.size(); i++) {
        if (ListResult.at(i) == " ") {
            ListResult[i] = msg;
            break;
        }
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       添加测试结果
*******************************************************************************/
void CWinSetImp::UpdateJudge(QByteArray msg)
{
    for (int i=0; i<ListJudge.size(); i++) {
        if (ListJudge.at(i) == " ") {
            ListJudge[i] = msg;
            break;
        }
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.04
 * brief:       档位
 * date:        2017.01.07
 * brief:       修正档位数据
*******************************************************************************/
int CWinSetImp::Gear(int row)
{
    int gear = 0;
    if (Volt.at(row)->value() <= 1000)
        gear = 1;
    else if (Volt.at(row)->value()<=2000)
        gear = 2;
    else if (Volt.at(row)->value()<=4000)
        gear = 3;
    else if (Volt.at(row)->value()<=5000)
        gear = 4;
    gear <<= 4;
    gear += Time.at(row)->value()+1;
    return gear;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       检测状态
*******************************************************************************/
void CWinSetImp::CmdCheckState()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x24)<<quint8(0x01)<<quint8(0x00);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Testing = true;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       开始测试
*******************************************************************************/
void CWinSetImp::CmdStartTest(quint8 pos)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001<<row;
    }
    out<<quint16(0x24)<<quint8(0x05)<<quint8(0x01)<<quint8(0x00)<<quint8(pos)
      <<quint8(tt/256)<<quint8(tt%256);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Testing = true;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       定频采集一个波形
*******************************************************************************/
void CWinSetImp::CmdSample(quint16 t)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0x0001<<t;
    out<<quint16(0x24)<<quint8(0x05)<<quint8(0x01)<<quint8(0x02)<<quint8(0x13)
      <<quint8(tt/256)<<quint8(tt%256);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Sampling = true;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       自动采样
*******************************************************************************/
void CWinSetImp::CmdStartSample()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001<<row;
    }
    out<<quint16(0x24)<<quint8(0x05)<<quint8(0x01)<<quint8(0x01)<<quint8(0x13)
      <<quint8(tt/256)<<quint8(tt%256);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Sampling = true;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       更新显示波形
*******************************************************************************/
void CWinSetImp::DisplayWave(QByteArray msg)
{
    int t = 0;
    for (int i=0; i<WaveNumber.size(); i++) {
        if (WaveImp.at(WaveNumber.at(i))->WaveItem == msg)
            t = i;
    }
    for (int i=0; i<qMin(3,WaveNumber.size()-t); i++) {
        QByteArray w;
        w = WaveImp.at(WaveNumber.at(t+i))->WaveItem;
        emit TransformCmd(ADDR,WIN_WAVE_ITEM,w);
        w = WaveImp.at(WaveNumber.at(t+i))->WaveByte;
        emit TransformCmd(ADDR,WIN_WAVE_BYTE,w);
        w = WaveImp.at(WaveNumber.at(t+i))->WaveTest;
        emit TransformCmd(ADDR,WIN_WAVE_TEST,w);
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       停止测试
*******************************************************************************/
void CWinSetImp::CmdStopTest()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x24)<<quint8(0x01)<<quint8(0x02);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Testing = false;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       配置
 * date:        2017.01.04
 * brief:       增加档位
*******************************************************************************/
void CWinSetImp::CmdConfigure()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int row=0; row<Enable.size(); row++) {
        out<<quint16(0x24)<<quint8(0x08)<<quint8(0x03)<<quint8(row)
          <<quint8(Terminal1.at(row)->text().toInt())
         <<quint8(Terminal2.at(row)->text().toInt())
        <<quint8(int(Volt.at(row)->value())/256)<<quint8(int(Volt.at(row)->value())%256)
        <<quint8(Gear(row))<<quint8(Freq.at(row));
    }
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       更新显示
*******************************************************************************/
void CWinSetImp::showEvent(QShowEvent *)
{
    DatInit();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       退出保存
*******************************************************************************/
void CWinSetImp::hideEvent(QHideEvent *)
{
    DatSave();
}

void CWinSetImp::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_IMP && addr != CAN_ID_IMP && addr != CAN_ID_IMP_WAVE)
        return;
    switch (cmd) {
    case CAN_DAT_GET:
        ExcuteCmd(addr,msg);
        break;
    case CAN_CMD_CHECK:
        CmdCheckState();
        break;
    case CAN_CMD_START:
        CmdStartTest(msg.toInt());
        break;
    case CAN_CMD_STOP:
        CmdStopTest();
        break;
    case CAN_CMD_INIT:
        ShowInit();
        CmdConfigure();
        break;
    default:
        break;

    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       更新显示
 * date:        2017.02.14
 * brief:       修改显示方式
*******************************************************************************/
void CWinSetImp::ShowInit()
{
    Items.clear();
    for (int row = 0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            QStringList s;
            QString T1 = Terminal1.at(qMin(row,Terminal1.size()))->text();
            QString T2 = Terminal2.at(qMin(row,Terminal2.size()))->text();
            QString V = Volt.at(qMin(row,Volt.size()))->text();
            QString C = Flut.at(qMin(row,Flut.size()))->text();
            QString P = Phase.at(qMin(row,Phase.size()))->text();
            QString A = Area.at(qMin(row,Area.size()))->text();
            QString D = Diff.at(qMin(row,Diff.size()))->text();

            s.append(QString(tr("匝间%1-%2")).arg(T1).arg(T2));
            if (C.toInt() == 0)
                s.append(QString("%1V,%2,%3,%4").arg(V).arg(P).arg(A).arg(D));
            else
                s.append(QString("%1V,%2,%3,%4,%5").arg(V).arg(C).arg(P).arg(A).arg(D));
            s.append(" ");
            s.append(" ");
            Items.append(s.join("@"));
            WaveImp.at(row)->WaveTest.clear();
        }
    }
    emit TransformCmd(ADDR,WIN_CMD_SHOW,Items.join("\n").toUtf8());
}
/*******************************************************************************
 *                                        END
*******************************************************************************/
