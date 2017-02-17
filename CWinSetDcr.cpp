/********************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       电阻设置界面
 * date:        2017.01.04
 * brief:       增加不平衡度显示与判定
********************************************************************************/
#include "CWinSetDcr.h"
#include "ui_CWinSetDcr.h"
/********************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       构造
********************************************************************************/
CWinSetDcr::CWinSetDcr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CWinSetDcr)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
    Testing = false;
    isCheckOk = false;
    Timer = new QTimer(this);
    connect(Timer,SIGNAL(timeout()),this,SLOT(TestCheck()));
    Timer->start(5000);
}
/********************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       析构
********************************************************************************/
CWinSetDcr::~CWinSetDcr()
{
    delete ui;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      初始化界面
*******************************************************************************/
void CWinSetDcr::WinInit()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabSetDcr->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);
    ui->TabSetDcr->horizontalHeader()->setResizeMode(4,QHeaderView::Stretch);
    ui->TabSetDcr->horizontalHeader()->setResizeMode(5,QHeaderView::Stretch);
    ui->TabSetDcr->horizontalHeader()->setResizeMode(6,QHeaderView::Stretch);
    ui->TabSetDcr->horizontalHeader()->setResizeMode(7,QHeaderView::Stretch);
    ui->TabSetDcr->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabSetDcr->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->TabSetDcr->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->TabSetDcr->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->TabSetDcr->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Stretch);
    ui->TabSetDcr->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Stretch);
    ui->TabSetDcr->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(ui->TabSetDcr,SIGNAL(cellClicked(int,int)),this,SLOT(ItemClick(int,int)));
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
void CWinSetDcr::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSDLRAuto,Qt::Key_0);
    btnGroup->addButton(ui->BtnSDLRExit,Qt::Key_2);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      按键功能
*******************************************************************************/
void CWinSetDcr::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        DatAuto();
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
 * date:       2017.01.04
 * brief:      增加不平衡度
*******************************************************************************/
void CWinSetDcr::DatInit()
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
    set->beginGroup("SetDcr");


    QStringList temp = (set->value("Other","20 0 0.5 10 10 50").toString()).split(" ");
    if (temp.size() >= 6) {
        ui->BoxStd->setValue(temp.at(0).toDouble());
        ui->BoxOffset->setValue(temp.at(1).toDouble());
        ui->BoxTime->setValue(temp.at(2).toDouble());
        ui->BoxMin->setValue(temp.at(3).toDouble());
        ui->BoxMax->setValue(temp.at(4).toDouble());
        ui->BoxUnbalance->setValue(temp.at(5).toDouble());
    }
    //可用
    temp = (QString(set->value("Enable","Y Y Y Y Y Y Y Y").toByteArray())).split(" ");
    if (temp.size() > 8)
        ui->TabSetDcr->setRowCount(temp.size());
    else
        ui->TabSetDcr->setRowCount(8);
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Enable.size() > row)
            continue;
        Enable.append(new QTableWidgetItem);
        ui->TabSetDcr->setItem(row,0,Enable.at(row));
        Enable.at(row)->setText(temp.at(row));
        Enable.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Enable.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Enable.size() <= row)
            break;
        Enable.at(row)->setText(temp.at(row));
    }
    //端一
    temp = (set->value("Terminal1","1 2 3 4 5 6 7 8").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Terminal1.size() > row)
            continue;
        Terminal1.append(new QTableWidgetItem);
        ui->TabSetDcr->setItem(row,1,Terminal1.at(row));
        Terminal1.at(row)->setText(temp.at(row));
        Terminal1.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal1.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Terminal1.size() <= row)
            break;
        Terminal1.at(row)->setText(temp.at(row));
    }
    //端二
    temp = (set->value("Terminal2","2 3 4 5 6 7 8 1").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Terminal2.size() > row)
            continue;
        Terminal2.append(new QTableWidgetItem);
        ui->TabSetDcr->setItem(row,2,Terminal2.at(row));
        Terminal2.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal2.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Terminal2.size() <= row)
            break;
        Terminal2.at(row)->setText(temp.at(row));
    }
    //材料
    temp = (QString(set->value("Metal","0 0 0 0 0 0 0 0").toByteArray())).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Metal.size() > row)
            continue;
        Metal.append(new QComboBox(this));
        ui->TabSetDcr->setCellWidget(row,3,Metal.at(row));
        QStringList t;
        t <<tr("铜")<<tr("铝")<<tr("铜铝");
        Metal.at(row)->setView(new QListView(this));
        Metal.at(row)->addItems(t);

    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Metal.size() <= row)
            break;
        Metal.at(row)->setCurrentIndex(temp.at(row).toInt());
    }
    //单位
    temp = (QString(set->value("Unit","1 1 1 1 1 1 1 1").toByteArray())).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Unit.size() > row)
            continue;
        Unit.append(new QComboBox(this));
        ui->TabSetDcr->setCellWidget(row,4,Unit.at(row));
        QStringList t;
        t <<"mΩ"<<"Ω"<<"kΩ";
        Unit.at(row)->setView(new QListView(this));
        Unit.at(row)->addItems(t);
        Unit.at(row)->setCurrentIndex(temp.at(row).toInt());
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Unit.size() <= row)
            break;
        Unit.at(row)->setCurrentIndex(temp.at(row).toInt());
    }
    //最小值
    temp = (set->value("Min","0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Min.size() > row)
            continue;
        Min.append(new QDoubleSpinBox(this));
        ui->TabSetDcr->setCellWidget(row,5,Min.at(row));
        Min.at(row)->setMaximum(9999);
        Min.at(row)->setAlignment(Qt::AlignHCenter);
        Min.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Min.size() <= row)
            break;
        Min.at(row)->setValue(temp.at(row).toDouble());
    }
    //最大值
    temp = (set->value("Max","200.00 200.00 200.00 200.00 200.00 200.00 200.00 200.00").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Max.size() > row)
            continue;
        Max.append(new QDoubleSpinBox(this));
        ui->TabSetDcr->setCellWidget(row,6,Max.at(row));
        Max.at(row)->setMaximum(9999);
        Max.at(row)->setAlignment(Qt::AlignHCenter);
        Max.at(row)->setValue(temp.at(row).toDouble());
        Max.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Max.size() <= row)
            break;
        Max.at(row)->setValue(temp.at(row).toDouble());
    }
    //标准值
    temp = (set->value("Std","100.00 100.00 100.00 100.00 100.00 100.00 100.00 100.00").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabSetDcr->rowCount()); row++) {
        if (Std.size() > row)
            continue;
        Std.append(new QDoubleSpinBox(this));
        ui->TabSetDcr->setCellWidget(row,7,Std.at(row));
        Std.at(row)->setMaximum(9999);
        Std.at(row)->setAlignment(Qt::AlignHCenter);
        Std.at(row)->setValue(temp.at(row).toDouble());
        Std.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      数据保存
 * date:       2017.01.04
 * brief:      增加不平衡度
*******************************************************************************/
void CWinSetDcr::DatSave()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxStd->value()));
    temp.append(QString::number(ui->BoxOffset->value()));
    temp.append(QString::number(ui->BoxTime->value()));
    temp.append(QString::number(ui->BoxMin->value()));
    temp.append(QString::number(ui->BoxMax->value()));
    temp.append(QString::number(ui->BoxUnbalance->value()));
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
    for (int i=0; i<Metal.size(); i++)
        temp.append(QString::number(Metal.at(i)->currentIndex()));
    set->setValue("Metal",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Unit.size(); i++)
        temp.append(QString::number(Unit.at(i)->currentIndex()));
    set->setValue("Unit",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Min.size(); i++)
        temp.append(QString::number(Min.at(i)->value()));
    set->setValue("Min",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Max.size(); i++)
        temp.append(QString::number(Max.at(i)->value()));
    set->setValue("Max",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Std.size(); i++)
        temp.append(QString::number(Std.at(i)->value()));
    set->setValue("Std",(temp.join(" ").toUtf8()));
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      自动计算
*******************************************************************************/
void CWinSetDcr::DatAuto()
{
    for (int i=0; i<ui->TabSetDcr->rowCount(); i++) {
        double std = Std.at(i)->value();
        double min = std*(100-ui->BoxMin->value())/100;
        double max = std*(100+ui->BoxMax->value())/100;
        Min.at(i)->setValue(min);
        Max.at(i)->setValue(max);
    }
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      数据格式化
*******************************************************************************/
bool CWinSetDcr::DatStdd()
{
    for (int i=0; i<qMin(Terminal1.size(),Terminal2.size()); i++) {
        int t1 = Terminal1.at(i)->text().toInt();
        int t2 = Terminal2.at(i)->text().toInt();
        if (abs(t1-t2)%3 == 0) {
            QString warning = QString("行%1:\n端子差不能为3,保存失败").arg(i+1);
            QMessageBox::warning(this,"警告", warning,QMessageBox::Ok);
            return false;
        }
    }
    for (int i=0; i<qMin(Max.size(),Min.size()); i++) {
        if (Min.at(i)->value()>Max.at(i)->value()) {
            QString warning = QString("行%1:\n上限大于下限,保存失败").arg(i+1);
            QMessageBox::warning(this,"警告", warning,QMessageBox::Ok);
            return false;
        }
    }
    return true;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.23
 * brief:      点击
*******************************************************************************/
void CWinSetDcr::ItemClick(int r, int c)
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
        input->show();
        break;
    default:
        break;
    }
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.23
 * brief:      切换文字
*******************************************************************************/
void CWinSetDcr::ItemChange(QString msg)
{
    int t = ui->TabSetDcr->currentColumn();
    if (t==1 || t==2)
        ui->TabSetDcr->currentItem()->setText(msg);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.15
 * brief:       命令处理
*******************************************************************************/
void CWinSetDcr::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_DCR && addr != CAN_ID_DCR)
        return;
    switch (cmd) {
    case CAN_DAT_GET:
        ExcuteCanCmd(msg);
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
void CWinSetDcr::ExcuteCanCmd(QByteArray msg)
{
    if (!Testing)
        return;
    TimeOut = 0;
    if (msg.size()==4 && (quint8)msg.at(0)==0x00) {
        TestCheckOk(msg);
    }
    if (msg.size()==7 && (quint8)msg.at(0)==0x01) {
        TestResult(msg);
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       更新显示
 * date:        2017.01.04
 * brief:       增加不平衡度显示
 * date:        2017.02.14
 * brief:       修改显示方式
*******************************************************************************/
void CWinSetDcr::TestInit()
{
    Items.clear();
    Results.clear();
    QStringList n;
    for (int row = 0; row<Enable.size(); row++) {
        QStringList s;
        QString T1 = Terminal1.at(qMin(row,Terminal1.size()))->text();
        QString T2 = Terminal2.at(qMin(row,Terminal2.size()))->text();
        QString U1 = Unit.at(qMin(row,Unit.size()))->currentText();
        QString M1 = Min.at(qMin(row,Min.size()))->text();
        QString M2 = Max.at(qMin(row,Max.size()))->text();
        s.append(QString(tr("电阻%1-%2")).arg(T1).arg(T2));
        s.append(QString("%1~%2%3").arg(M1).arg(M2).arg(U1));
        s.append(" ");
        s.append(" ");
        Items.append(s.join("@"));
    }
    for (int row = 0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            n.append(Items.at(row));
        }
    }
    if (ui->BoxUnbalance->value() != 0 && Items.size()>=3) {
        QStringList s;
        s.append("电阻平衡");
        s.append(QString("%1%").arg(ui->BoxUnbalance->value()));
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
 * brief:      增加超时判断
*******************************************************************************/
void CWinSetDcr::TestCheck()
{
    if (Testing)
        return;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x22)<<quint8(0x01)<<quint8(0x00);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Testing = true;
    if (!WaitTestOver(100)) {
        Testing = false;
        QMessageBox::warning(this,tr("警告"),tr("电阻板异常"),QMessageBox::Ok);
        emit TransformCmd(ADDR,WIN_CMD_DEBUG,"DCR Error\n");
        Timer->stop();
    }
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      更新状态
*******************************************************************************/
void CWinSetDcr::TestCheckOk(QByteArray msg)
{
    if (!isCheckOk) {
        isCheckOk = true;
        emit TransformCmd(ADDR,WIN_CMD_DEBUG,"DCR check ok\n");
    }
    if (Testing)
        Testing = false;
    double offset = ui->BoxOffset->value();
    double temp = (quint16(msg.at(2)*256)+quint8(msg.at(3)))/10-50+offset;
    QString t = QString(tr("温度:%1°C")).arg(temp);
    emit TransformCmd(ADDR,WIN_CMD_TEMP,t.toUtf8());
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      开始测试
 * date:       2017.02.15
 * brief:      增加超时判断
*******************************************************************************/
void CWinSetDcr::TestStart(quint8 pos)
{
    if (Testing)
        return;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001<<row;
    }
    out<<quint16(0x22)<<quint8(0x06)<<quint8(0x01)<<quint8(0x01)<<quint8(0x00)
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
        if (ui->BoxUnbalance->value() != 0) {
            QStringList s = QString(Items.last()).split("@");
            if (s.at(2) == " ")
                s[2] = "---";
            if (s.at(3) == " ")
                s[3] = "NG";
            emit TransformCmd(ADDR,WIN_CMD_ITEM,s.join("@").toUtf8());
        }
    }
    QStringList s;
    s.append("电阻");
    s.append(FileInUse);
    s.append(Judge);
    emit TransformCmd(ADDR,WIN_CMD_JUDGE,s.join("@").toUtf8());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.27
 * brief:       更新测试数据
 * date:        2017.01.04
 * brief:       增加温度补偿与不平衡度判定
 * date:        2017.02.14
 * brief:       修改显示方式
*******************************************************************************/
void CWinSetDcr::TestResult(QByteArray msg)
{
    quint8 number = quint8(msg.at(1));
    quint8 grade = quint8(msg.at(2));
    double temp = (quint16)(msg.at(3)*256)+(quint8)msg.at(4);
    double tt = (quint16)(msg.at(5)*256)+quint8(msg.at(6));

    if (ui->BoxOffsetEnable->isChecked()) {
        double offset = TestOffset(tt,number);
        temp *= offset;
    }
    QString t;
    QString judge = "OK";
    switch (grade) {
    case 1:
        temp /= 100;
        t = QString("%1mΩ").arg(temp,0,'r',3);
        break;
    case 2:
        temp /= 10;
        t = QString("%1mΩ").arg(temp,0,'r',2);
        break;
    case 3:
        temp /= 1000;
        t = QString("%1Ω").arg(temp,0,'r',3);
        break;
    case 4:
        temp /= 100;
        t = QString("%1Ω").arg(temp,0,'r',2);
        break;
    case 5:
        temp /= 10;
        t = QString("%1Ω").arg(temp,0,'r',1);
        break;
    case 6:
        temp /= 1000;
        t = QString("%1kΩ").arg(temp,0,'r',3);
        break;
    case 7:
        temp /= 100;
        t = QString("%1kΩ").arg(temp,0,'r',2);
        break;
    default:
        qDebug()<<tt;
        break;
    }
    Results.append(temp);
    if (temp<Min.at(number)->value() || temp>Max.at(number)->value()) {
        Judge = "NG";
        judge = "NG";
    }

    QStringList s = QString(Items.at(number)).split("@");
    if (s.at(2) == " ")
        s[2] = t;
    if (s.at(3) == " ")
        s[3] = judge;
    emit TransformCmd(ADDR,WIN_CMD_ITEM,s.join("@").toUtf8());

    //计算不平衡度
    if ((ui->BoxUnbalance->value() != 0) && (Results.size() == 3)) {
        double sum = 0;
        double avr = 0;
        QString u;
        for (int i=0; i<Results.size(); i++) {
            sum += Results.at(i);
        }
        avr = sum/Results.size();
        for (int i=0; i<Results.size(); i++) {
            double un = fabs(Results.at(i)-avr)*100/avr;
            u.append(QString::number(un,'f',1));
            u.append("% ");
            if (un >= ui->BoxUnbalance->value()) {
                judge = "NG";
                Judge = "NG";
            }
        }
        QStringList s = QString(Items.last()).split("@");
        if (s.at(2) == " ")
            s[2] = u;
        if (s.at(3) == " ")
            s[3] = judge;
        emit TransformCmd(ADDR,WIN_CMD_ITEM,s.join("@").toUtf8());
    }
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      计算温度补偿
*******************************************************************************/
double CWinSetDcr::TestOffset(double t, quint8 num)
{
    double temp;
    double res = 0;
    switch (Metal.at(num)->currentIndex()) {
    case 0:
        res = ResCu;
        break;
    case 1:
        res = ResCu_Al;
        break;
    case 2:
        res = ResAl;
        break;
    default:
        break;
    }
    temp = 1+res*(ui->BoxStd->value()-(t/10-50)-ui->BoxOffset->value());
    return temp;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      停止测试
*******************************************************************************/
void CWinSetDcr::TestStop()
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
void CWinSetDcr::TestConfig()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int row=0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            out<<quint16(0x22)<<quint8(0x06)<<quint8(0x03)<<quint8(row)
              <<quint8(Terminal1.at(row)->text().toInt())
             <<quint8(Terminal2.at(row)->text().toInt())
            <<quint8(TestGear(row))
            <<quint8(ui->BoxTime->value()*10);
        }
    }
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      计算测试档位
*******************************************************************************/
int CWinSetDcr::TestGear(int row)
{
    if (row >= Max.size())
        return 7;
    if (row >= Unit.size())
        return 7;
    double r = Max.at(row)->value();
    if (Unit.at(row)->currentText() == "mΩ")
        r /= 1000;
    if (Unit.at(row)->currentText() == "kΩ")
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
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      检测版本
*******************************************************************************/
void CWinSetDcr::TestVersion()
{

}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      等待测试结束
 * date:       2017.02.15
 * brief:      去除超时处理
*******************************************************************************/
bool CWinSetDcr::WaitTestOver(quint16 t)
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
void CWinSetDcr::Delay(int ms)
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
void CWinSetDcr::showEvent(QShowEvent*)
{
    DatInit();
    DatStdd();
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      判断保存
*******************************************************************************/
void CWinSetDcr::hideEvent(QHideEvent *)
{
    if(DatStdd())
        DatSave();
}
/*******************************************************************************
 *                                  END
*******************************************************************************/
