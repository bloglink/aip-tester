/********************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       反嵌设置界面
********************************************************************************/
#include "CWinSetMag.h"
#include "ui_CWinSetMag.h"
/********************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       构造
********************************************************************************/
CWinSetMag::CWinSetMag(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CWinSetMag)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
    Testing = false;
    Sampling = false;
}
/********************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       析构
********************************************************************************/
CWinSetMag::~CWinSetMag()
{
    delete ui;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      初始化界面
*******************************************************************************/
void CWinSetMag::WinInit()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabSetMag->horizontalHeader()->setResizeMode(0,QHeaderView::Stretch);
    ui->TabSetMag->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
    ui->TabSetMag->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch);
    ui->TabSetMag->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);
    ui->TabSetMag->setColumnWidth(4,400);
    ui->TabSetMag->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabSetMag->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->TabSetMag->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->TabSetMag->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->TabSetMag->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->TabSetMag->setColumnWidth(4,400);
    ui->TabSetMag->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    ui->BoxDir->setView(new QListView(this));
    connect(ui->TabSetMag,SIGNAL(cellClicked(int,int)),this,SLOT(ItemClick(int,int)));
    input = new InputNum(this);
    QStringList t;
    t <<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12";
    input->BtnInit(t);
    connect(input,SIGNAL(ItemChange(QString)),this,SLOT(ItemChange(QString)));
    input->hide();
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      初始化按键
*******************************************************************************/
void CWinSetMag::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSampleMag,Qt::Key_0);
    btnGroup->addButton(ui->BtnExitMag,Qt::Key_2);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      按键功能
*******************************************************************************/
void CWinSetMag::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        TestConfig();
        TestSample();
        break;
    case Qt::Key_1:
        break;
    case Qt::Key_2:
        emit TransformCmd(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    default:
        break;
    }
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      数据初始化
*******************************************************************************/
void CWinSetMag::DatInit()
{
    QSettings *global = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    FileInUse = global->value("FileInUse","default.ini").toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(FileInUse);
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetMag");

    QStringList temp = (set->value("Other","0 1 2").toString()).split(" ");
    if (temp.size() >= 3) {
        ui->BoxDir->setCurrentIndex(temp.at(0).toInt());
        ui->BoxMain->setValue(temp.at(1).toInt());
        ui->BoxAuxiliary->setValue(temp.at(2).toInt());
    }
    temp = (QString(set->value("Enable","Y Y Y Y Y Y Y Y").toByteArray())).split(" ");
    if (temp.size() > 8)
        ui->TabSetMag->setRowCount(temp.size());
    else
        ui->TabSetMag->setRowCount(8);
    for (int row=0; row<qMin(temp.size(),ui->TabSetMag->rowCount()); row++) {
        if (Enable.size() > row)
            continue;
        Enable.append(new QTableWidgetItem);
        ui->TabSetMag->setItem(row,0,Enable.at(row));
        Enable.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Enable.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetMag->rowCount()); row++) {
        if (Enable.size() <= row)
            break;
        Enable.at(row)->setText(temp.at(row));
    }
    temp = (set->value("Terminal1","0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetMag->rowCount()); row++) {
        if (Terminal1.size() > row)
            continue;
        Terminal1.append(new QTableWidgetItem);
        ui->TabSetMag->setItem(row,1,Terminal1.at(row));
        Terminal1.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal1.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetMag->rowCount()); row++) {
        if (Terminal1.size() <= row)
            break;
        Terminal1.at(row)->setText(temp.at(row));
    }
    temp = (set->value("Terminal2","1 1 1 1 1 1 1 1").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetMag->rowCount()); row++) {
        if (Terminal2.size() > row)
            continue;
        Terminal2.append(new QTableWidgetItem);
        ui->TabSetMag->setItem(row,2,Terminal2.at(row));
        Terminal2.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal2.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetMag->rowCount()); row++) {
        if (Terminal2.size() <= row)
            break;
        Terminal2.at(row)->setText(temp.at(row));
    }
    temp = (set->value("Max","10.00 10.00 10.00 10.00 10.00 10.00 10.00 10.00").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetMag->rowCount()); row++) {
        if (Max.size() > row)
            continue;
        Max.append(new QDoubleSpinBox(this));
        ui->TabSetMag->setCellWidget(row,3,Max.at(row));
        Max.at(row)->setMaximum(9999);
        Max.at(row)->setAlignment(Qt::AlignHCenter);
        Max.at(row)->setValue(temp.at(row).toDouble());
        Max.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetMag->rowCount()); row++) {
        if (Max.size() <= row)
            break;
        Max.at(row)->setValue(temp.at(row).toDouble());
    }
    temp = (set->value("Area","1000 1000 1000 1000 1000 1000 1000 1000").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetMag->rowCount()); row++) {
        if (Area.size() > row)
            continue;
        Area.append(temp.at(row).toInt());
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetMag->rowCount()); row++) {
        if (Area.size() <= row)
            break;
        Area[row] = temp.at(row).toInt();
    }
    //频率
    temp = (set->value("Freq","0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetMag->rowCount()); row++) {
        if (Freq.size() > row)
            continue;
        Freq.append(temp.at(row).toInt());
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetMag->rowCount()); row++) {
        if (Freq.size() <= row)
            break;
        Freq[row] = temp.at(row).toInt();
    }
    //波形
    for (int row=0; row<ui->TabSetMag->rowCount(); row++) {
        if (WaveMag.size() > row)
            continue;
        WaveMag.append(new Waveform(this));
        ui->TabSetMag->setCellWidget(row,4,WaveMag.at(row));
    }
    if (WaveMag.size() != 8)
        return;
    QByteArray w;
    for (int row=0; row<qMin(WaveMag.size(),ui->TabSetMag->rowCount()); row++) {
        QString byte = "WaveMag"+QString::number(row);
        w = set->value(byte,"1000000000000000").toByteArray();
        WaveMag.at(row)->WaveByteShow(w);
        QString T1 = Terminal1.at(row)->text();
        QString T2 = Terminal2.at(row)->text();
        WaveMag.at(row)->WaveItem = QString(tr("反嵌%1-%2")).arg(T1).arg(T2).toUtf8();
    }
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      数据保存
*******************************************************************************/
void CWinSetMag::DatSave()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxDir->currentIndex()));
    temp.append(QString::number(ui->BoxMain->value()));
    temp.append(QString::number(ui->BoxAuxiliary->value()));
    set->setValue("Other",(temp.join(" ").toUtf8()));
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
    for (int i=0; i<Max.size(); i++)
        temp.append(QString::number(Max.at(i)->value()));
    set->setValue("Max",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Area.size(); i++)
        temp.append(QString::number(Area.at(i)));
    set->setValue("Area",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Freq.size(); i++)
        temp.append(QString::number(Freq.at(i)));
    set->setValue("Freq",(temp.join(" ").toUtf8()));

    for (int row=0; row<qMin(WaveMag.size(),ui->TabSetMag->rowCount()); row++) {
        QString byte = "WaveMag"+QString::number(row);
        set->setValue(byte,WaveMag.at(row)->WaveByte);
    }
}
///*******************************************************************************
// * version:    1.0
// * author:     link
// * date:       2016.12.19
// * brief:      更新显示
//*******************************************************************************/
//void CWinSetMag::DisplayInit()
//{
//    ListItem.clear();
//    ListPara.clear();
//    WaveNumber.clear();
//    ListResult.clear();
//    ListJudge.clear();
//    for (int row = 0; row<Enable.size(); row++) {
//        if (Enable.at(row)->text() == "Y") {
//            if (row >= Terminal1.size())
//                break;
//            if (row >= Terminal2.size())
//                break;
//            QString T1 = Terminal1.at(row)->text();
//            QString T2 = Terminal2.at(row)->text();
//            ListItem.append(QString(tr("反嵌%1-%2")).arg(T1).arg(T2));
//            if (row >= Max.size())
//                break;
//            QString M2 = Max.at(row)->text();
//            ListPara.append(QString("%1%").arg(M2));
//            WaveNumber.append(row);
//            ListResult.append(" ");
//            ListJudge.append(" ");
//            WaveMag.at(row)->WaveTest.clear();
//        }
//    }
//    if (ui->BoxDir->currentIndex() == 1) {
//        ListItem.append(QString(tr("磁旋")));
//        ListPara.append(QString(tr("正转")));
//        ListResult.append(" ");
//        ListJudge.append(" ");
//    }
//    if (ui->BoxDir->currentIndex() == 2) {
//        ListItem.append(QString(tr("磁旋")));
//        ListPara.append(QString(tr("反转")));
//        ListResult.append(" ");
//        ListJudge.append(" ");
//    }
//}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.23
 * brief:      点击
*******************************************************************************/
void CWinSetMag::ItemClick(int r, int c)
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
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.23
 * brief:      切换文字
*******************************************************************************/
void CWinSetMag::ItemChange(QString msg)
{
    ui->TabSetMag->currentItem()->setText(msg);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.15
 * brief:      命令处理
*******************************************************************************/
void CWinSetMag::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_MAG && addr != CAN_ID_DCR && addr != CAN_ID_DCR_WAVE)
        return;
    switch (cmd) {
    case CAN_DAT_GET:
        ExcuteCanCmd(addr,msg);
        break;
    case CAN_CMD_CHECK:
        TestCheck();
        break;
    case CAN_CMD_START:
        TestStart(msg.toInt());
        break;
    case CAN_CMD_STOP:
        TestStop();
        break;
    case CAN_CMD_INIT:
        DatInit();
        TestInit();
        TestConfig();
        break;
    case WIN_CMD_WAVE:
        TestWaveShow(msg);
        break;
    default:
        break;

    }
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      CAN命令处理
*******************************************************************************/
void CWinSetMag::ExcuteCanCmd(int id, QByteArray msg)
{
    if (!Testing && !Sampling)
        return;
    TimeOut = 0;
    if (id == CAN_ID_DCR_WAVE) {
        TestWave(msg);
        return;
    }
    if (msg.size()==4 && (quint8)msg.at(0)==0x00) {
        TestCheckOk(msg);
    }
    if (msg.size()==5 && (quint8)msg.at(0)==0x02) {
        TestResult(msg);
    }
    if (msg.size()==2 && (quint8)msg.at(0)==0x03 && (quint8)msg.at(1)!=0xff) {
        if (Sampling)
            WaveMag.at(CurrentWave)->WaveByte.clear();
        if (Testing)
            WaveMag.at(CurrentWave)->WaveTest.clear();
    }
    if (msg.size()==2 && (quint8)msg.at(0)==0x03 && (quint8)msg.at(1)==0xff) {
        QByteArray w;
        QByteArray i;
        if (Sampling) {
            w = WaveMag.at(CurrentWave)->WaveByte;
            WaveMag.at(CurrentWave)->WaveByteShow(w);
        }
        if (Testing) {
            w = WaveMag.at(CurrentWave)->WaveTest;
            i = WaveMag.at(CurrentWave)->WaveItem;
            emit TransformCmd(ADDR,WIN_WAVE_ITEM,i);
            emit TransformCmd(ADDR,WIN_WAVE_BYTE,w);
        }
    }
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      更新显示
 * date:       2017.02.14
 * brief:      修改显示方式
*******************************************************************************/
void CWinSetMag::TestInit()
{
    Items.clear();
    WaveNumber.clear();
    QStringList n;
    for (int row = 0; row<Enable.size(); row++) {
        QStringList s;
        QString T1 = Terminal1.at(qMin(row,Terminal1.size()))->text();
        QString T2 = Terminal2.at(qMin(row,Terminal2.size()))->text();
        QString M1 = Max.at(qMin(row,Max.size()))->text();
        s.append(QString(tr("反嵌%1-%2")).arg(T1).arg(T2));
        s.append(QString("%1%").arg(M1));
        s.append(" ");
        s.append(" ");
        Items.append(s.join("@"));
        WaveMag.at(row)->WaveTest.clear();
    }
    for (int row = 0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            n.append(Items.at(row));
            WaveNumber.append(row);
        }
    }
    if (ui->BoxDir->currentIndex() != 0) {
        QStringList s;
        s.append(QString(tr("磁旋")));
        s.append(ui->BoxDir->currentText());
        s.append(" ");
        s.append(" ");
        Items.append(s.join("@"));
        n.append(Items.last());
    }
    emit TransformCmd(ADDR,WIN_CMD_SHOW,n.join("\n").toUtf8());
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      检测状态
 * date:       2017.02.15
 * brief:      取消功能,移到电阻模块
*******************************************************************************/
void CWinSetMag::TestCheck()
{

}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.27
 * brief:      更新状态
 * date:       2017.02.15
 * brief:      修改磁旋判断方式
*******************************************************************************/
void CWinSetMag::TestCheckOk(QByteArray )
{
    if (Testing) {
        Testing = false;
        if (ui->BoxDir->currentIndex() != 0) {
            TestDir();
        }
    }
    if (Sampling) {
        Sampling = false;
        emit TransformCmd(ADDR,WIN_CMD_DEBUG,"MAG Sample ok\n");
    }
    if (!isCheckOk) {
        isCheckOk = true;
        emit TransformCmd(ADDR,WIN_CMD_DEBUG,"MAG check ok\n");
    }
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      开始采样
 * date:       2017.02.15
 * brief:      增加测试或采样中返回
*******************************************************************************/
void CWinSetMag::TestSample()
{
    if (Testing || Sampling)
        return;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001<<row;
    }
    out<<quint16(0x22)<<quint8(0x06)<<quint8(0x01)<<quint8(0x02)<<quint8(0x01)
      <<quint8(0x13)<<quint8(tt/256)<<quint8(tt%256);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Sampling = true;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      开始测试
 * date:       2017.02.15
 * brief:      增加测试或采样中返回,增加超时判断
*******************************************************************************/
void CWinSetMag::TestStart(quint8 pos)
{
    if (Testing || Sampling)
        return;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001<<row;
    }
    out<<quint16(0x22)<<quint8(0x06)<<quint8(0x01)<<quint8(0x02)<<quint8(0x00)
      <<quint8(pos)<<quint8(tt/256)<<quint8(tt%256);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
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
                emit TransformCmd(ADDR,WIN_CMD_ITEM,s.join("@").toUtf8());
            }
        }
        if (ui->BoxDir->currentIndex() != 0) {
            QStringList s = QString(Items.last()).split("@");
            if (s.at(2) == " ")
                s[2] = "---";
            if (s.at(3) == " ")
                s[3] = "NG";
            emit TransformCmd(ADDR,WIN_CMD_ITEM,s.join("@").toUtf8());
        }
    }
    QStringList s;
    s.append("反嵌");
    s.append(FileInUse);
    s.append(Judge);
    emit TransformCmd(ADDR,WIN_CMD_JUDGE,s.join("@").toUtf8());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.27
 * brief:       更新测试数据
 * date:        2017.02.15
 * brief:       修改显示方式
*******************************************************************************/
void CWinSetMag::TestResult(QByteArray msg)
{
    CurrentWave = (quint8)msg.at(1);
    quint16 Area1 = Area[CurrentWave];
    quint16 Area2 = quint16(msg.at(2)*256)+quint8(msg.at(3));
    if (Sampling) {
        Area[CurrentWave] = Area2;
        Freq[CurrentWave] = quint8(msg.at(4));
        return;
    }
    quint16 area = abs((Area2-Area1)*100/Area1);
    QString n = QString("%1%").arg(area);
    QString judge = "OK";
    if (CurrentWave >= Max.size() || area > Max.at(CurrentWave)->value()) {
        Judge = "NG";
        judge = "NG";
    }
    QStringList s = QString(Items.at(CurrentWave)).split("@");
    if (s.at(2) == " ")
        s[2] = n;
    if (s.at(3) == " ")
        s[3] = judge;
    emit TransformCmd(ADDR,WIN_CMD_ITEM,s.join("@").toUtf8());
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.27
 * brief:      接收波形
*******************************************************************************/
void CWinSetMag::TestWave(QByteArray msg)
{
    if (Sampling)
        WaveMag.at(CurrentWave)->WaveByte.append(msg);
    else
        WaveMag.at(CurrentWave)->WaveTest.append(msg);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      显示波形
*******************************************************************************/
void CWinSetMag::TestWaveShow(QByteArray msg)
{
    int t = WaveNumber.size();
    for (int i=0; i<WaveNumber.size(); i++) {
        if (WaveMag.at(WaveNumber.at(i))->WaveItem == msg)
            t = i;
    }
    if (t == WaveNumber.size())
        return;
    for (int i=0; i<qMin(3,WaveNumber.size()-t); i++) {
        QByteArray w;
        w = WaveMag.at(WaveNumber.at(t+i))->WaveItem;
        emit TransformCmd(ADDR,WIN_WAVE_ITEM,w);
        w = WaveMag.at(WaveNumber.at(t+i))->WaveTest;
        emit TransformCmd(ADDR,WIN_WAVE_BYTE,w);
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.11.28
 * brief:       显示旋向
*******************************************************************************/
void CWinSetMag::TestDir()
{
    //计算主副相的面积，差积，和左右移动的差积
    qint32 	area1,area2,diff,diff1,diff2;
    area1=0;
    area2=0;
    diff=0;
    diff1=0;
    diff2=0;
    int b1 = ui->BoxMain->value()-1;
    int b2 = ui->BoxAuxiliary->value()-1;
    int s1 = WaveMag.at(b1)->WaveTest.size();
    int s2 = WaveMag.at(b2)->WaveTest.size();
    for (int i=10; i<qMin(s1,s2)-10; i++) {
        int P1 = quint8(WaveMag.at(b1)->WaveTest.at(i))-0x80;
        int P2 = quint8(WaveMag.at(b2)->WaveTest.at(i))-0x80;
        area1 += P1*P1;
        area2 += P2*P2;
        diff += (P1-P2)*(P1-P2);
        for (int j=1; j<11; j++) {
            int P3 = quint8(WaveMag.at(b2)->WaveTest.at(i+j))-0x80;
            int P4 = quint8(WaveMag.at(b2)->WaveTest.at(i-j))-0x80;
            diff1 += (P1-P3)*(P1-P3);
            diff2 += (P1-P4)*(P1-P4);
        }
    }
    diff1 /= 10;
    diff2 /= 10;
    QString n;
    QString judge = "OK";

    if(((diff*3<area1)&&(diff*3<area2))||(area1<(area2>>4))||(area2<(area1>>4)))
        n = tr("不转");
    else if(diff1<diff2) //副相在前
        n = tr("正转");
    else if(diff1>diff2)//主相在前
        n = tr("反转");
    else
        n = tr("不转");
    if (n != ui->BoxDir->currentText()) {
        Judge = "NG";
        judge = "NG";
    }
    QStringList s = QString(Items.last()).split("@");
    if (s.at(2) == " ")
        s[2] = n;
    if (s.at(3) == " ")
        s[3] = judge;
    emit TransformCmd(ADDR,WIN_CMD_ITEM,s.join("@").toUtf8());
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      停止测试
*******************************************************************************/
void CWinSetMag::TestStop()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x22)<<quint8(0x01)<<quint8(0x02);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Testing = false;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      配置
*******************************************************************************/
void CWinSetMag::TestConfig()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int row=0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            out<<quint16(0x22)<<quint8(0x05)<<quint8(0x04)<<quint8(row)
              <<quint8(Terminal1.at(row)->text().toInt())
             <<quint8(Terminal2.at(row)->text().toInt())
            <<quint8(Freq.at(row));
        }
    }
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      等待测试结束
*******************************************************************************/
bool CWinSetMag::WaitTestOver(quint16 t)
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
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      延时
*******************************************************************************/
void CWinSetMag::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      更新显示
*******************************************************************************/
void CWinSetMag::showEvent(QShowEvent *)
{
    DatInit();
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      退出保存
*******************************************************************************/
void CWinSetMag::hideEvent(QHideEvent *)
{
    DatSave();
}
/*******************************************************************************
 *                                  END
*******************************************************************************/
