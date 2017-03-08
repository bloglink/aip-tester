/**
  ******************************************************************************
  * @file    PageImp.cpp
  * @author  link
  * @version 2.1.0.5
  * @date    2017-03-06
  * @brief   匝间测试模块
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "PageImp.h"
#include "ui_PageImp.h"

PageImp::PageImp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageImp)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
    Testing = false;
    Sampling = false;
    isAvarage = false;
    AvrCount = 0;
}

PageImp::~PageImp()
{
    delete ui;
}
/**
  * @brief  Initializes the window
  */
void PageImp::WinInit()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabParams->horizontalHeader()->setResizeMode(0,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(4,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(5,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(6,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(7,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(8,QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(9,400);
    ui->TabParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabParams->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(9,400);
    ui->TabParams->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(ui->TabParams,SIGNAL(cellClicked(int,int)),this,SLOT(ItemClick(int,int)));
    input = new PageNum(this);
    QStringList t;
    t <<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12";
    input->BtnInit(t);
    connect(input,SIGNAL(ItemChange(QString)),this,SLOT(ItemChange(QString)));
    input->hide();
    ui->WindgetSetImp->setCurrentIndex(0);

    connect(ui->LabelWave,SIGNAL(BlockClick(int)),this,SLOT(BlockClick(int)));
    connect(ui->BoxStart,SIGNAL(valueChanged(int)),this,SLOT(BlockClick(int)));
    connect(ui->BoxEnd,SIGNAL(valueChanged(int)),this,SLOT(BlockClick(int)));

    ui->TabParams->setRowCount(MAX_ROW);
    for (int row=0; row<MAX_ROW; row++) {
        Enable.append(new QTableWidgetItem);
        ui->TabParams->setItem(row,0,Enable.at(row));
        Enable.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Enable.at(row)->setTextAlignment(Qt::AlignCenter);

        Terminal1.append(new QTableWidgetItem);
        ui->TabParams->setItem(row,1,Terminal1.at(row));
        Terminal1.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal1.at(row)->setTextAlignment(Qt::AlignCenter);

        Terminal2.append(new QTableWidgetItem);
        ui->TabParams->setItem(row,2,Terminal2.at(row));
        Terminal2.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal2.at(row)->setTextAlignment(Qt::AlignCenter);

        Volt.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,3,Volt.at(row));
        Volt.at(row)->setMaximum(3000);
        Volt.at(row)->setAlignment(Qt::AlignHCenter);
        Volt.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Time.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,4,Time.at(row));
        Time.at(row)->setMaximum(9999);
        Time.at(row)->setAlignment(Qt::AlignHCenter);
        Time.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Flut.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,5,Flut.at(row));
        Flut.at(row)->setMaximum(9999);
        Flut.at(row)->setAlignment(Qt::AlignHCenter);
        Flut.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Phase.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,6,Phase.at(row));
        Phase.at(row)->setMaximum(9999);
        Phase.at(row)->setAlignment(Qt::AlignHCenter);
        Phase.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Area.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,7,Area.at(row));
        Area.at(row)->setMaximum(9999);
        Area.at(row)->setAlignment(Qt::AlignHCenter);
        Area.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Diff.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,8,Diff.at(row));
        Diff.at(row)->setMaximum(9999);
        Diff.at(row)->setAlignment(Qt::AlignHCenter);
        Diff.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Freq.append(7);
        Block0.append(0);
        Block1.append(395);

        WaveImp.append(new Waveform(this));
        ui->TabParams->setCellWidget(row,9,WaveImp.at(row));
    }
    station = WIN_ID_OUT13;
}
/**
  * @brief  Initializes the buttons
  */
void PageImp::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSampleImp,Qt::Key_0);
    btnGroup->addButton(ui->BtnExitWave,Qt::Key_1);
    btnGroup->addButton(ui->BtnExitImp,Qt::Key_2);
    btnGroup->addButton(ui->BtnFrequcy1,Qt::Key_3);
    btnGroup->addButton(ui->BtnFrequcy2,Qt::Key_4);
    btnGroup->addButton(ui->BtnFrequcy3,Qt::Key_5);
    btnGroup->addButton(ui->BtnAvarage,Qt::Key_6);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/**
  * @brief  Button funcitons
  */
void PageImp::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        TestConfig();
        TestSampleAuto();
        break;
    case Qt::Key_1:
        ui->WindgetSetImp->setCurrentIndex(0);
        break;
    case Qt::Key_2:
        emit SendMessage(ADDR,CMD_JUMP,NULL);
        break;
    case Qt::Key_3:
        if (Freq[ui->TabParams->currentRow()] != 0)
            Freq[ui->TabParams->currentRow()]--;
        TestConfig();
        TestSample(ui->TabParams->currentRow());
        break;
    case Qt::Key_4:
        TestConfig();
        TestSample(ui->TabParams->currentRow());
        break;
    case Qt::Key_5:
        if (Freq[ui->TabParams->currentRow()] != 14)
            Freq[ui->TabParams->currentRow()]++;
        TestConfig();
        TestSample(ui->TabParams->currentRow());
        break;
    case Qt::Key_6:
        TestConfig();
        TestSampleAuto();
        isAvarage = true;
    default:
        break;
    }
}
/**
  * @brief  Initializes settings
  */
void PageImp::DatInit()
{
    qDebug()<<QTime::currentTime().toString()<<"匝间数据";
    QSettings *global = new QSettings(INI_PATH,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    FileInUse = global->value("FileInUse",INI_DEFAULT).toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(FileInUse);
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetImp");
    QStringList temp;
    //可用
    temp = (QString(set->value("Enable","Y Y Y N N N N N").toByteArray())).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Enable.at(row)->setText(temp.at(row));
    //端一
    temp = (set->value("Terminal1","1 2 1 4 5 6 7 8").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Terminal1.at(row)->setText(temp.at(row));
    //端二
    temp = (set->value("Terminal2","2 3 3 5 6 7 8 1").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Terminal2.at(row)->setText(temp.at(row));
    //电压
    temp = (set->value("Volt","500 500 500 500 500 500 500 500").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Volt.at(row)->setValue(temp.at(row).toDouble());
    //次数
    temp = (set->value("Time","1 1 1 1 1 1 1 1").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Time.at(row)->setValue(temp.at(row).toDouble());
    //电晕
    temp = (set->value("Flut","0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Flut.at(row)->setValue(temp.at(row).toDouble());
    //相位
    temp = (set->value("Phase","10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Phase.at(row)->setValue(temp.at(row).toDouble());
    //面积
    temp = (set->value("Area","10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Area.at(row)->setValue(temp.at(row).toDouble());
    //差积
    temp = (set->value("Diff","10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Diff.at(row)->setValue(temp.at(row).toDouble());
    //频率
    temp = (set->value("Freq","7 7 7 7 7 7 7 7").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Freq[row] = temp.at(row).toInt();
    //计算起点
    temp = (set->value("Block0","5 5 5 5 5 5 5 5").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Block0[row] = temp.at(row).toInt();
    //计算终点
    temp = (set->value("Block1","395 395 395 395 395 395 395 395").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Block1[row] = temp.at(row).toInt();
    //波形
    QByteArray w;
    for (int row=0; row<qMin(WaveImp.size(),MAX_ROW); row++) {
        QString ByteL = "WaveImpL"+QString::number(row);
        w = set->value(ByteL,"1000000000000000").toByteArray();
        WaveImp.at(row)->WaveBytes[0] = w;

        QString ByteR = "WaveImpR"+QString::number(row);
        w = set->value(ByteR,"1000000000000000").toByteArray();
        WaveImp.at(row)->WaveBytes[1] = w;

        QString T1 = Terminal1.at(row)->text();
        QString T2 = Terminal2.at(row)->text();
        WaveImp.at(row)->WaveItem = QString(tr("匝间%1-%2")).arg(T1).arg(T2).toUtf8();
    }
    qDebug()<<QTime::currentTime().toString()<<"匝间数据OK";
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       数据保存
*******************************************************************************/
void PageImp::DatSave()
{
    qDebug()<<QTime::currentTime().toString()<<"匝间保存";
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

    for (int row=0; row<qMin(WaveImp.size(),ui->TabParams->rowCount()); row++) {

        QString ByteL = "WaveImpL"+QString::number(row);
        QString ByteR = "WaveImpR"+QString::number(row);
        set->setValue(ByteL,WaveImp.at(row)->WaveBytes.at(0));
        set->setValue(ByteR,WaveImp.at(row)->WaveBytes.at(1));
    }
    qDebug()<<QTime::currentTime().toString()<<"匝间保存OK";
}

void PageImp::VoltEdit()
{
    for (int i=0; i<Volt.size(); i++)
        Volt.at(i)->setValue(Volt.at(0)->value());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       点击
*******************************************************************************/
void PageImp::ItemClick(int r, int c)
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
void PageImp::ItemChange(QString msg)
{
    ui->TabParams->currentItem()->setText(msg);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       切换文字
*******************************************************************************/
void PageImp::BlockClick(int x)
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
 * version:    1.0
 * author:     link
 * date:       2017.02.15
 * brief:      命令处理
*******************************************************************************/
void PageImp::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_IMP && addr != CAN_ID_IMP && addr != CAN_ID_IMP_WAVE)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(addr,msg);
        break;
    case CMD_CHECK:
        TestCheck();
        break;
    case CMD_START:
        TestStart(msg.toInt());
        break;
    case CMD_STOP:
        TestStop();
        break;
    case CMD_INIT:
        DatInit();
        TestInit();
        TestConfig();
        break;
    case CMD_WAVE:
        TestWaveShow(msg);
        break;
    default:
        break;
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       命令处理
*******************************************************************************/
void PageImp::ExcuteCanCmd(int id,QByteArray msg)
{
    if (!Testing && !Sampling)
        return;
    TimeOut = 0;
    if (id == CAN_ID_IMP_WAVE) {
        TestWave(msg);
        return;
    }
    if (msg.size()==4 && (quint8)msg.at(0)==0x00) {
        TestCheckOk(msg);
    }
    if (msg.size()==7 && (quint8)msg.at(0)==0x02) {
        quint8 num = quint8(msg.at(1));
        Freq[num] = quint8(msg.at(3));
        int v = quint16(msg.at(4)*256)+quint8(msg.at(5));
        Volt.at(num)->setValue(v);
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
            emit SendMessage(ADDR,CMD_WAVE_ITEM,i);
            emit SendMessage(ADDR,CMD_WAVE_BYTE,w);
        }
    }
    if (msg.size()==2 && (quint8)msg.at(0)==0x03 && (quint8)msg.at(1)==0xff) {
        TestWaveOk(msg);
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       更新显示
 * date:        2017.02.15
 * brief:       修改显示方式
*******************************************************************************/
void PageImp::TestInit()
{
    Items.clear();
    WaveNumber.clear();
    QStringList n;
    for (int row = 0; row<Enable.size(); row++) {
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
    for (int row = 0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            n.append(Items.at(row));
            WaveNumber.append(row);
        }
    }
    emit SendMessage(ADDR,CMD_INIT_ITEM,n.join("\n").toUtf8());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       检测状态
*******************************************************************************/
void PageImp::TestCheck()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x24)<<quint8(0x01)<<quint8(0x00);
    emit SendMessage(ADDR,CMD_CAN,msg);
    Testing = true;
    if (!WaitTestOver(100)) {
        Testing = false;
        QMessageBox::warning(this,tr("警告"),tr("匝间板异常"),QMessageBox::Ok);
        emit SendMessage(ADDR,CMD_DEBUG,"Check PageImp Error:Time out\n");
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       更新状态
*******************************************************************************/
void PageImp::TestCheckOk(QByteArray)
{
    if (Testing) {
        Testing = false;
    }
    if (Sampling) {
        Sampling = false;
        emit SendMessage(ADDR,CMD_DEBUG,"IMP Sample ok\n");
    }
    if (!isCheckOk) {
        isCheckOk = true;
        emit SendMessage(ADDR,CMD_DEBUG,"IMP check ok\n");
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       自动采样
*******************************************************************************/
void PageImp::TestSampleAuto()
{
    if (ui->BoxStation->currentIndex() == 0)
        station = WIN_ID_OUT13;
    if (ui->BoxStation->currentIndex() == 1)
        station = WIN_ID_OUT14;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001<<row;
    }
    out<<quint16(0x24)<<quint8(0x05)<<quint8(0x01)<<quint8(0x01)<<quint8(station)
      <<quint8(tt/256)<<quint8(tt%256);
    emit SendMessage(ADDR,CMD_CAN,msg);
    Sampling = true;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       定频采集一个波形
*******************************************************************************/
void PageImp::TestSample(quint16 t)
{
    if (ui->BoxStation->currentIndex() == 0)
        station = WIN_ID_OUT13;
    if (ui->BoxStation->currentIndex() == 1)
        station = WIN_ID_OUT14;

    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0x0001<<t;
    out<<quint16(0x24)<<quint8(0x05)<<quint8(0x01)<<quint8(0x02)<<quint8(station)
      <<quint8(tt/256)<<quint8(tt%256);
    emit SendMessage(ADDR,CMD_CAN,msg);
    Sampling = true;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       开始测试
 * date:        2017.02.15
 * brief:       增加测试或采样中返回,增加超时判断
*******************************************************************************/
void PageImp::TestStart(quint8 pos)
{
    if (Testing || Sampling)
        return;

    station = pos;

    emit SendMessage(ADDR,CMD_WAVE_HIDE,NULL);

    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001<<row;
    }
    out<<quint16(0x24)<<quint8(0x05)<<quint8(0x01)<<quint8(0x00)<<quint8(station)
      <<quint8(tt/256)<<quint8(tt%256);
    emit SendMessage(ADDR,CMD_CAN,msg);
    Testing = true;
    Judge = "OK";
    if(!WaitTestOver(100)) {
        Testing = false;
        Judge = "NG";
        for (int row = 0; row<Enable.size(); row++) {
            if (Enable.at(row)->text() == "Y") {
                QStringList s = QString(Items.at(row)).split("@");
                if (s.at(2) == " ")
                    s[2] = "---";
                if (s.at(3) == " ")
                    s[3] = "NG";
                emit SendMessage(ADDR,CMD_ITEM,s.join("@").toUtf8());
            }
        }
    }
    QStringList s;
    s.append("匝间");
    s.append(FileInUse);
    s.append(Judge);
    emit SendMessage(ADDR,CMD_JUDGE,s.join("@").toUtf8());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       更新测试数据
*******************************************************************************/
void PageImp::TestResult(QByteArray )
{
    if (Sampling)
        return;
    quint8 num = station - WIN_ID_OUT13;
    WaveImp.at(CurrentWave)->InitWaveByte(num);
    WaveImp.at(CurrentWave)->InitWaveTest(num);

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

    QString n;
    QString judge = "OK";
    int number = CurrentWave;
    if (Flut.at(qMin(number,Flut.size()-1))->value()!=0)
        n = QString("面积:%1%,差积:%2%,电晕:%3,相位:%4%").arg(A).arg(D).arg(F).arg(P);
    else
        n = QString("面积:%1%,差积:%2%,相位:%3").arg(A).arg(D).arg(P);

    if (abs(A) >= Area.at(qMin(number,Area.size()-1))->value())
        judge = "NG";
    else if (abs(D) >= Diff.at(qMin(number,Diff.size()-1))->value())
        judge = "NG";
    else if (abs(F) >= Flut.at(qMin(number,Flut.size()-1))->value() && Flut.at(qMin(number,Flut.size()-1))->value()!=0)
        judge = "NG";
    else if (abs(P) >= Phase.at(qMin(number,Phase.size()-1))->value())
        judge = "NG";
    if (judge == "NG")
        Judge = "NG";
    QStringList t = QString(Items.at(CurrentWave)).split("@");
    if (t.at(2) == " ")
        t[2] = n;
    if (t.at(3) == " ")
        t[3] = judge;
    emit SendMessage(ADDR,CMD_ITEM,t.join("@").toUtf8());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       接收波形
*******************************************************************************/
void PageImp::TestWave(QByteArray msg)
{
    if (Sampling)
        WaveImp.at(CurrentWave)->WaveByte.append(msg);
    else
        WaveImp.at(CurrentWave)->WaveTest.append(msg);
}

void PageImp::TestWaveOk(QByteArray msg)
{
    QByteArray w;
    quint8 num = station - WIN_ID_OUT13;
    if (Sampling) {
        if (isAvarage) {
            AvrCount++;
            QByteArray byte = WaveImp.at(CurrentWave)->WaveByte;
            QByteArray test = WaveImp.at(CurrentWave)->WaveBytes[num];
            for (int i=0; i<qMin(byte.size(),test.size()); i++)
                w.append(test.at(i)+(test.at(i)-byte.at(i))/AvrCount);
        } else {
            w = WaveImp.at(CurrentWave)->WaveByte;
        }
        WaveImp.at(CurrentWave)->WaveBytes[num] = w;
        WaveImp.at(CurrentWave)->WaveByteShow(w);
        ui->LabelWave->WaveByteShow(w);

    }
    if (Testing) {
        w = WaveImp.at(CurrentWave)->WaveTest;
        WaveImp.at(CurrentWave)->WaveTests[num] = w;
        emit SendMessage(ADDR,CMD_WAVE_TEST,w);
        TestResult(msg);
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       更新显示波形
*******************************************************************************/
void PageImp::TestWaveShow(QByteArray msg)
{
    int t = WaveNumber.size();
    for (int i=0; i<WaveNumber.size(); i++) {
        if (WaveImp.at(WaveNumber.at(i))->WaveItem == msg)
            t = i;
    }
    if (t == WaveNumber.size())
        return;
    for (int i=0; i<qMin(3,WaveNumber.size()-t); i++) {
        QByteArray w;
        w = WaveImp.at(WaveNumber.at(t+i))->WaveItem;
        emit SendMessage(ADDR,CMD_WAVE_ITEM,w);
        w = WaveImp.at(WaveNumber.at(t+i))->WaveByte;
        emit SendMessage(ADDR,CMD_WAVE_BYTE,w);
        w = WaveImp.at(WaveNumber.at(t+i))->WaveTest;
        emit SendMessage(ADDR,CMD_WAVE_TEST,w);
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       停止测试
*******************************************************************************/
void PageImp::TestStop()
{
    if (!Testing)
        return;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x24)<<quint8(0x01)<<quint8(0x02);
    emit SendMessage(ADDR,CMD_CAN,msg);
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
void PageImp::TestConfig()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int row=0; row<Enable.size(); row++) {
        out<<quint16(0x24)<<quint8(0x08)<<quint8(0x03)<<quint8(row)
          <<quint8(Terminal1.at(row)->text().toInt())
         <<quint8(Terminal2.at(row)->text().toInt())
        <<quint8(int(Volt.at(row)->value())/256)<<quint8(int(Volt.at(row)->value())%256)
        <<quint8(TestGear(row))<<quint8(Freq.at(row));
    }
    emit SendMessage(ADDR,CMD_CAN,msg);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.04
 * brief:       档位
 * date:        2017.01.07
 * brief:       修正档位数据
*******************************************************************************/
int PageImp::TestGear(int row)
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
 * brief:       等待测试结束
*******************************************************************************/
bool PageImp::WaitTestOver(quint16 t)
{
    TimeOut = 0;
    while (Testing) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       延时
*******************************************************************************/
void PageImp::Delay(int ms)
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
 * brief:       更新显示
*******************************************************************************/
void PageImp::showEvent(QShowEvent *)
{
    DatInit();
    connect(Volt.at(0),SIGNAL(editingFinished()),this,SLOT(VoltEdit()));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.28
 * brief:       退出保存
*******************************************************************************/
void PageImp::hideEvent(QHideEvent *)
{
    DatSave();
}
/*******************************************************************************
 *                                        END
*******************************************************************************/
