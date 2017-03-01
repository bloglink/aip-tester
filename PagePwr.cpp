#include "PagePwr.h"
#include "ui_PagePwr.h"

PagePwr::PagePwr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PagePwr)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
    Testing = false;
    isCheckOk = false;
}

PagePwr::~PagePwr()
{
    delete ui;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       初始化界面
*******************************************************************************/
void PagePwr::WinInit()
{
//    ui->BoxPG->hide();
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabPower->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabPower->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabPG->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabPG->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabPower->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabPower->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabPG->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabPG->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(ui->TabPower,SIGNAL(cellClicked(int,int)),this,SLOT(ItemClick(int,int)));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       初始化按键
*******************************************************************************/
void PagePwr::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExit,Qt::Key_0);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       按键功能
*******************************************************************************/
void PagePwr::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        emit SendMessage(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    default:
        break;
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       数据初始化
*******************************************************************************/
void PagePwr::DatInit()
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
    set->beginGroup("SetPower");

    QStringList temp = (set->value("Other","20 0 0.5 10 10 50").toString()).split(" ");
    if (temp.size() >= 6) {
        ui->BoxFreq->setValue(temp.at(0).toDouble());
        ui->BoxVolt->setValue(temp.at(1).toDouble());
        ui->BoxCap->setValue(temp.at(2).toDouble());
        ui->BoxVoltMax->setValue(temp.at(3).toDouble());
        ui->BoxVoltMin->setValue(temp.at(4).toDouble());
        ui->BoxVoltPG->setValue(temp.at(5).toDouble());
    }

    //可用
    temp = (QString(set->value("Enable","Y Y Y Y Y Y").toByteArray())).split(" ");
    if (temp.size() > 6)
        ui->TabPower->setRowCount(temp.size());
    else
        ui->TabPower->setRowCount(6);
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (Enable.size() > row)
            continue;
        Enable.append(new QTableWidgetItem);
        ui->TabPower->setItem(row,0,Enable.at(row));
        Enable.at(row)->setText(temp.at(row));
        Enable.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Enable.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (Enable.size() <= row)
            break;
        Enable.at(row)->setText(temp.at(row));
    }

    //最大电流
    temp = (set->value("CurrMin","0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (CurrMin.size() > row)
            continue;
        CurrMin.append(new QDoubleSpinBox(this));
        ui->TabPower->setCellWidget(row,1,CurrMin.at(row));
        CurrMin.at(row)->setMaximum(20);
        CurrMin.at(row)->setAlignment(Qt::AlignHCenter);
        CurrMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (CurrMin.size() <= row)
            break;
        CurrMin.at(row)->setValue(temp.at(row).toDouble());
    }
    //最大电流
    temp = (set->value("CurrMax","5 5 5 5 5 5").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (CurrMax.size() > row)
            continue;
        CurrMax.append(new QDoubleSpinBox(this));
        ui->TabPower->setCellWidget(row,2,CurrMax.at(row));
        CurrMax.at(row)->setMaximum(20);
        CurrMax.at(row)->setAlignment(Qt::AlignHCenter);
        CurrMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (CurrMax.size() <= row)
            break;
        CurrMax.at(row)->setValue(temp.at(row).toDouble());
    }
    //最小功率
    temp = (set->value("PowerMin","1 1 1 1 1 1").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (PowerMin.size() > row)
            continue;
        PowerMin.append(new QDoubleSpinBox(this));
        ui->TabPower->setCellWidget(row,3,PowerMin.at(row));
        PowerMin.at(row)->setMaximum(20);
        PowerMin.at(row)->setAlignment(Qt::AlignHCenter);
        PowerMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (PowerMin.size() <= row)
            break;
        PowerMin.at(row)->setValue(temp.at(row).toDouble());
    }
    //最大功率
    temp = (set->value("PowerMax","500 500 500 500 500 500").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (PowerMax.size() > row)
            continue;
        PowerMax.append(new QDoubleSpinBox(this));
        ui->TabPower->setCellWidget(row,4,PowerMax.at(row));
        PowerMax.at(row)->setMaximum(20);
        PowerMax.at(row)->setAlignment(Qt::AlignHCenter);
        PowerMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (PowerMax.size() <= row)
            break;
        PowerMax.at(row)->setValue(temp.at(row).toDouble());
    }
    //最小容压
    temp = (set->value("CapMin","1 1 1 1 1 1").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (CapMin.size() > row)
            continue;
        CapMin.append(new QDoubleSpinBox(this));
        ui->TabPower->setCellWidget(row,5,CapMin.at(row));
        CapMin.at(row)->setMaximum(20);
        CapMin.at(row)->setAlignment(Qt::AlignHCenter);
        CapMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (CapMin.size() <= row)
            break;
        CapMin.at(row)->setValue(temp.at(row).toDouble());
    }
    //最大容压
    temp = (set->value("CapMax","500 500 500 500 500 500").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (CapMax.size() > row)
            continue;
        CapMax.append(new QDoubleSpinBox(this));
        ui->TabPower->setCellWidget(row,6,CapMax.at(row));
        CapMax.at(row)->setMaximum(20);
        CapMax.at(row)->setAlignment(Qt::AlignHCenter);
        CapMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (CapMax.size() <= row)
            break;
        CapMax.at(row)->setValue(temp.at(row).toDouble());
    }
    //转向
    temp = (QString(set->value("TestDir","0 0 0 0 0 0 0 0").toByteArray())).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (TestDir.size() > row)
            continue;
        TestDir.append(new QComboBox(this));
        ui->TabPower->setCellWidget(row,7,TestDir.at(row));
        QStringList t;
        t <<tr("不转")<<tr("正转")<<tr("反转");
        TestDir.at(row)->setView(new QListView(this));
        TestDir.at(row)->addItems(t);

    }
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (TestDir.size() <= row)
            break;
        TestDir.at(row)->setCurrentIndex(temp.at(row).toInt());
    }
    //测试时间
    temp = (set->value("TestTime","1 1 1 1 1 1").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (TestTime.size() > row)
            continue;
        TestTime.append(new QDoubleSpinBox(this));
        ui->TabPower->setCellWidget(row,8,TestTime.at(row));
        TestTime.at(row)->setMaximum(20);
        TestTime.at(row)->setAlignment(Qt::AlignHCenter);
        TestTime.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    temp.clear();
    temp<<"123"<<"124"<<"125"<<"126"<<"127"<<"128";
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (Grade.size() > row)
            continue;
        Grade.append(new QTableWidgetItem);
        ui->TabPower->setItem(row,9,Grade.at(row));
        Grade.at(row)->setText(temp.at(row));
        Grade.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Grade.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabPower->rowCount()); row++) {
        if (Grade.size() <= row)
            break;
        Grade.at(row)->setText(temp.at(row));
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       数据保存
*******************************************************************************/
void PagePwr::DatSave()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxFreq->value()));
    temp.append(QString::number(ui->BoxVolt->value()));
    temp.append(QString::number(ui->BoxCap->value()));
    temp.append(QString::number(ui->BoxVoltMax->value()));
    temp.append(QString::number(ui->BoxVoltMin->value()));
    temp.append(QString::number(ui->BoxVoltPG->value()));
    set->setValue("Other",(temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i<Enable.size(); i++)
        temp.append(Enable.at(i)->text());
    set->setValue("Enable",(temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i<CurrMax.size(); i++)
        temp.append(QString::number(CurrMax.at(i)->value()));
    set->setValue("CurrMax",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<CurrMin.size(); i++)
        temp.append(QString::number(CurrMin.at(i)->value()));
    set->setValue("CurrMin",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<PowerMax.size(); i++)
        temp.append(QString::number(PowerMax.at(i)->value()));
    set->setValue("PowerMax",(temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i<PowerMin.size(); i++)
        temp.append(QString::number(PowerMin.at(i)->value()));
    set->setValue("PowerMin",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<CapMax.size(); i++)
        temp.append(QString::number(CapMax.at(i)->value()));
    set->setValue("CapMax",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<CapMin.size(); i++)
        temp.append(QString::number(CapMin.at(i)->value()));
    set->setValue("CapMin",(temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i<TestDir.size(); i++)
        temp.append(QString::number(TestDir.at(i)->currentIndex()));
    set->setValue("TestDir",(temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i<TestTime.size(); i++)
        temp.append(QString::number(TestTime.at(i)->value()));
    set->setValue("TestTime",(temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i<Grade.size(); i++)
        temp.append(Grade.at(i)->text());
    set->setValue("Grade",(temp.join(" ").toUtf8()));

}

void PagePwr::ItemClick(int r, int c)
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
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       命令处理
*******************************************************************************/
void PagePwr::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_PWR && addr != CAN_ID_PWR)
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
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       CAN命令处理
*******************************************************************************/
void PagePwr::ExcuteCanCmd(QByteArray msg)
{
    if (!Testing)
        return;
    TimeOut = 0;
    if (msg.size() == 4 && (quint8)msg.at(0) == 0x00)
        TestCheckOk(msg);
    if (msg.size() == 7 && (quint8)msg.at(0) == 0x01)
        TestResult(msg);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.16
 * brief:      更新显示
*******************************************************************************/
void PagePwr::TestInit()
{
    Items.clear();
    QStringList n;
    for (int row = 0; row<Enable.size(); row++) {
        QStringList s;
        QString G1 = Grade.at(qMin(row,Grade.size()))->text();
        double I1 = CurrMin.at(qMin(row,CurrMin.size()))->value();
        double I2 = CurrMax.at(qMin(row,CurrMax.size()))->value();
        double P1 = PowerMin.at(qMin(row,PowerMin.size()))->value();
        double P2 = PowerMax.at(qMin(row,PowerMax.size()))->value();
        double C1 = CapMin.at(qMin(row,CapMin.size()))->value();
        double C2 = CapMax.at(qMin(row,CapMax.size()))->value();
        s.append(QString(tr("功率%1")).arg(G1));
        s.append(QString("%1~%2A,%3~%4W,%5~%6V").arg(I1).arg(I2).arg(P1).arg(P2).arg(C1).arg(C2));
        s.append(" ");
        s.append(" ");
        Items.append(s.join("@"));
    }
    for (int row = 0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            n.append(Items.at(row));
        }
    }

    emit SendMessage(ADDR,WIN_CMD_SHOW,n.join("\n").toUtf8());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       检测状态
*******************************************************************************/
void PagePwr::TestCheck()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x27)<<quint8(0x01)<<quint8(0x00);
    emit SendMessage(ADDR,CAN_DAT_PUT,msg);
    Testing = true;
    if (!WaitTestOver(100)) {
        Testing = false;
        QMessageBox::warning(this,tr("警告"),tr("功率板异常"),QMessageBox::Ok);
        emit SendMessage(ADDR,WIN_CMD_DEBUG,"PWR Error\n");
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       更新状态,求平均并清理测试结果
*******************************************************************************/
void PagePwr::TestCheckOk(QByteArray)
{
    Testing = false;
    if (!isCheckOk) {
        emit SendMessage(ADDR,WIN_CMD_DEBUG,"Power check ok\n");
        isCheckOk = true;
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       开始测试
*******************************************************************************/
void PagePwr::TestStart(quint8 pos)
{
    if (Testing)
        return;
    qDebug()<<pos;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 v = ui->BoxVolt->value()*10;
    out<<quint16(0x27)<<quint8(0x07)<<quint8(0x01)<<quint8(0x01)
      <<quint8(Time/256)<<quint8(Time%256)<<quint8(v/256)<<quint8(v%256)
     <<quint8(0x00)<<quint8(0x00);
    emit SendMessage(ADDR,CAN_DAT_PUT,msg);
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
                emit SendMessage(ADDR,WIN_CMD_ITEM,s.join("@").toUtf8());
            }
        }
    }
    QStringList s;
    s.append("电感");
    s.append(FileInUse);
    s.append(Judge);
    emit SendMessage(ADDR,WIN_CMD_JUDGE,s.join("@").toUtf8());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       更新测试数据
*******************************************************************************/
void PagePwr::TestResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double c = quint16(msg.at(3)*256)+quint8(msg.at(4));
    double p = quint16(msg.at(5)*256)+quint8(msg.at(6));
    Volt.append(v);
    Curr.append(c);
    Power.append(p);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       停止测试
*******************************************************************************/
void PagePwr::TestStop()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x27)<<quint8(0x01)<<quint8(0x02);
    emit SendMessage(ADDR,CAN_DAT_PUT,msg);
    Testing = false;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       配置
*******************************************************************************/
void PagePwr::TestConfig()
{

}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.16
 * brief:      等待测试结束
*******************************************************************************/
bool PagePwr::WaitTestOver(quint16 t)
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
 * date:        2017.02.16
 * brief:       延时
*******************************************************************************/
void PagePwr::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       更新显示
*******************************************************************************/
void PagePwr::showEvent(QShowEvent *)
{
    DatInit();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       退出保存
*******************************************************************************/
void PagePwr::hideEvent(QHideEvent *)
{
    DatSave();
}
