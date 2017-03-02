#include "PageInr.h"
#include "ui_PageInr.h"

PageInr::PageInr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageInr)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
    Testing = false;
    isCheckOk = false;
}

PageInr::~PageInr()
{
    delete ui;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       初始化界面
*******************************************************************************/
void PageInr::WinInit()
{
    ui->BoxVoltage->setView(new QListView(this));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       初始化按键
*******************************************************************************/
void PageInr::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExitIr,Qt::Key_2);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       按键功能
*******************************************************************************/
void PageInr::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        break;
    case Qt::Key_1:
        break;
    case Qt::Key_2:
        emit SendMessage(ADDR,CMD_JUMP,NULL);
        break;
    default:
        break;
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       数据初始化
*******************************************************************************/
void PageInr::DatInit()
{
    qDebug()<<QTime::currentTime().toString()<<"绝缘数据";
    QSettings *global = new QSettings(INI_PATH,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    FileInUse = global->value("FileInUse","default.ini").toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(FileInUse);
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetIr");

    QStringList temp = (set->value("Other","0 1 100 3 0").toString()).split(" ");
    if (temp.size() >= 5) {
        ui->BoxVoltage->setCurrentIndex(temp.at(0).toInt());
        ui->BoxMin->setValue(temp.at(1).toInt());
        ui->BoxMax->setValue(temp.at(2).toInt());
        ui->BoxTime->setValue(temp.at(3).toInt());
        ui->BoxOffset->setValue(temp.at(4).toInt());
    }
    Testing = false;
    qDebug()<<QTime::currentTime().toString()<<"绝缘数据OK";
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       数据保存
*******************************************************************************/
void PageInr::DatSave()
{
    qDebug()<<QTime::currentTime().toString()<<"绝缘保存";
    QStringList temp;
    temp.append(QString::number(ui->BoxVoltage->currentIndex()));
    temp.append(QString::number(ui->BoxMin->value()));
    temp.append(QString::number(ui->BoxMax->value()));
    temp.append(QString::number(ui->BoxTime->value()));
    temp.append(QString::number(ui->BoxOffset->value()));
    set->setValue("Other",(temp.join(" ").toUtf8()));
    qDebug()<<QTime::currentTime().toString()<<"绝缘保存OK";
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       命令处理
*******************************************************************************/

void PageInr::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_INR && addr != CAN_ID_INR)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(msg);
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
    case CMD_ALARM:
        TestAlarm(quint8(msg.at(0)));
        break;
    default:
        break;
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       CAN命令处理
*******************************************************************************/
void PageInr::ExcuteCanCmd(QByteArray msg)
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
 * date:       2016.12.19
 * brief:      更新显示
 * date:       2017.02.15
 * brief:      修改显示方式
*******************************************************************************/
void PageInr::TestInit()
{
    Items.clear();
    QStringList s;
    QString U1 = ui->BoxVoltage->currentText();
    QString M1 = ui->BoxMin->text();
    QString M2 = ui->BoxMax->text();
    s.append(QString(tr("绝缘")));
    s.append(QString("%1V,%2~%3MΩ").arg(U1).arg(M1).arg(M2));
    s.append(" ");
    s.append(" ");
    Items.append(s.join("@"));
    emit SendMessage(ADDR,CMD_INIT_ITEM,Items.join("\n").toUtf8());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       检测状态
*******************************************************************************/
void PageInr::TestCheck()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x01)<<quint8(0x00);
    emit SendMessage(ADDR,CMD_CAN,msg);
    Testing = true;
    if (!WaitTestOver(100)) {
        Testing = false;
        QMessageBox::warning(this,tr("警告"),tr("绝缘板异常"),QMessageBox::Ok);
        emit SendMessage(ADDR,CMD_DEBUG,"Check PageInr Error:Time out\n");
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       更新状态
 * date:        2017.01.13
 * brief:       清理测试结果
 * date:        2017.02.15
 * brief:       增加求平均
*******************************************************************************/
void PageInr::TestCheckOk(QByteArray )
{
    Testing = false;
    if (!isCheckOk) {
        emit SendMessage(ADDR,CMD_DEBUG,"Check PageInr OK\n");
        isCheckOk = true;
    }
    if (Volt.isEmpty() || Res.isEmpty())
        return;
    double vv = 0;
    double rr = 0;
    for (int i=0; i<Volt.size(); i++) {
        vv += Volt.at(i);
        rr += Res.at(i);
    }
    vv /= Volt.size();
    rr /= Res.size();
    if (abs(vv-ui->BoxVoltage->currentText().toInt()) <5)
        vv = ui->BoxVoltage->currentText().toInt();
    QString vvv = QString::number(vv,'f',0);
    QString rrr = QString::number(rr/10,'f',1);
    QString t = QString("%1V,%2MΩ").arg(vvv).arg(rrr);
    QString judge = "OK";

    if (rr/10>ui->BoxMax->value() || rr/10<ui->BoxMin->value()) {
        Judge = "NG";
        judge = "NG";
    }

    QStringList s = QString(Items.at(0)).split("@");
    if (s.at(2) == " ")
        s[2] = t;
    if (s.at(3) == " ")
        s[3] = judge;
    emit SendMessage(ADDR,CMD_ITEM,s.join("@").toUtf8());

    Volt.clear();
    Res.clear();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       开始测试
 * date:        2017.02.15
 * brief:       增加超时判断
*******************************************************************************/
void PageInr::TestStart(quint8 pos)
{
    if (Testing)
        return;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x05)<<quint8(0x01)<<quint8(0x04)<<quint8(0x00)
      <<quint8(pos)<<quint8(0x01);
    emit SendMessage(ADDR,CMD_CAN,msg);
    Testing = true;
    Judge = "OK";
    if(!WaitTestOver(100)) {
        Testing = false;
        Judge = "NG";
        emit SendMessage(ADDR,CMD_JUDGE,"NG");
        for (int i=0; i<Items.size(); i++) {
            QStringList s = QString(Items.at(i)).split("@");
            if (s.at(2) == " ")
                s[2] = "---";
            if (s.at(3) == " ")
                s[3] = "NG";
            emit SendMessage(ADDR,CMD_ITEM,s.join("@").toUtf8());
        }
    }
    QStringList s;
    s.append("绝缘");
    s.append(FileInUse);
    s.append(Judge);
    emit SendMessage(ADDR,CMD_JUDGE,s.join("@").toUtf8());
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       更新测试数据
 * date:        2017.01.13
 * brief:       求平均
 * date:        2017.02.15
 * brief:       移除求平均计算,移到TestCheckOk
*******************************************************************************/
void PageInr::TestResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double tt = quint16(msg.at(3)*256)+quint8(msg.at(4));
    Volt.append(v);
    Res.append(tt);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       停止测试
*******************************************************************************/
void PageInr::TestStop()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x01)<<quint8(0x02);
    emit SendMessage(ADDR,CMD_CAN,msg);
    Testing = false;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       配置
*******************************************************************************/
void PageInr::TestConfig()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    int volt = ui->BoxVoltage->currentText().toInt();
    int time = ui->BoxTime->value()*10;
    int min = ui->BoxMin->value()*10;
    int max = ui->BoxMax->value()*10;
    out<<quint16(0x23)<<quint8(0x08)<<quint8(0x03)<<quint8(0x01)<<quint8(0x04)
      <<quint8(0x00)<<quint8(0x00)<<quint8(0xff)<<quint8(0xff)<<quint8(0x00);
    out<<quint16(0x23)<<quint8(0x08)<<quint8(0x04)<<quint8(0x01)<<quint8(volt/256)
      <<quint8(volt%256)<<quint8(time/256)<<quint8(time%256)<<quint8(min/256)<<quint8(min%256);
    out<<quint16(0x23)<<quint8(0x07)<<quint8(0x05)<<quint8(0x01)
      <<quint8(max/256)<<quint8(max%256)<<quint8(0x00)<<quint8(0x03)<<quint8(0x0A);//上限
    emit SendMessage(ADDR,CMD_CAN,msg);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.17
 * brief:       报警输出
*******************************************************************************/
void PageInr::TestAlarm(quint8 port)
{

    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x02)<<quint8(0x09)<<quint8(port);
    emit SendMessage(ADDR,CMD_CAN,msg);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       等待测试结束
 * date:        2017.02.15
 * brief:       去除超时处理
*******************************************************************************/
bool PageInr::WaitTestOver(quint16 t)
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
 * date:        2016.12.19
 * brief:       延时
*******************************************************************************/
void PageInr::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       更新显示
*******************************************************************************/
void PageInr::showEvent(QShowEvent *)
{
    DatInit();
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       退出保存
*******************************************************************************/
void PageInr::hideEvent(QHideEvent *)
{
    DatSave();
}
/*******************************************************************************
 *                                  END
*******************************************************************************/
