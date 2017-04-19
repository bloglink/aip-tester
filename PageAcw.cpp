/**
  ******************************************************************************
  * @author  link
  * @version 2.1.0.170402
  * @brief   交耐配置与测试流程
  ******************************************************************************
  */
#include "PageAcw.h"
#include "ui_PageAcw.h"

PageAcw::PageAcw(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageAcw)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
    Mode = ACW_FREE;
}

PageAcw::~PageAcw()
{
    delete ui;
}

void PageAcw::InitWindows()
{
    ui->BoxArc->setView(new QListView(this));
    ui->BoxFrequcy->setView(new QListView(this));
}

void PageAcw::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExitAcw,Qt::Key_0);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(ReadButtons(int)));
}

void PageAcw::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        SaveSettings();
        emit SendCommand(ADDR,CMD_JUMP,NULL);
        break;
    default:
        break;
    }
}

void PageAcw::InitSettings()
{
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t,QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("SetAcw");

    QStringList temp = (ini->value("Other","500 0 5.00 3 0 0 0").toString()).split(" ");
    if (temp.size() >= 7) {
        ui->BoxVoltage->setValue(temp.at(0).toDouble());
        ui->BoxMin->setValue(temp.at(1).toDouble());
        ui->BoxMax->setValue(temp.at(2).toDouble());
        ui->BoxTime->setValue(temp.at(3).toDouble());
        ui->BoxArc->setCurrentIndex(temp.at(4).toInt());
        ui->BoxFrequcy->setCurrentIndex(temp.at(5).toInt());
        ui->BoxOffset->setValue(temp.at(6).toInt());
    }
    qDebug()<<QTime::currentTime().toString()<<"PageAcw read OK";
}

void PageAcw::SaveSettings()
{
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t,QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("SetAcw");
    QStringList temp;
    temp.append(QString::number(ui->BoxVoltage->value()));
    temp.append(QString::number(ui->BoxMin->value()));
    temp.append(QString::number(ui->BoxMax->value()));
    temp.append(QString::number(ui->BoxTime->value()));
    temp.append(QString::number(ui->BoxArc->currentIndex()));
    temp.append(QString::number(ui->BoxFrequcy->currentIndex()));
    temp.append(QString::number(ui->BoxOffset->value()));
    ini->setValue("Other",(temp.join(" ").toUtf8()));
    
    qDebug()<<QTime::currentTime().toString()<<"PageAcw save OK";
}

void PageAcw::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr!=ADDR && addr!=WIN_ID_ACW && addr!=CAN_ID_INR)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(msg);
        break;
    case CMD_START:
        Mode = ACW_TEST;
        Judge = "OK";
        SendCanCmdStart(msg.toInt());
        if(!WaitTimeOut(100)) {
            Judge = "NG";
            SendTestItemsAllError();
        }
        SendTestJudge();
        Mode = ACW_FREE;
        break;
    case CMD_STOP:
        SendCanCmdStop();
        Mode = ACW_FREE;
        break;
    case CMD_INIT:
        InitSettings();
        SendTestItemsAllEmpty();
        SendCanCmdConfig();
        break;
    default:
        break;
    }
}

void PageAcw::ExcuteCanCmd(QByteArray msg)
{
    if (Mode == ACW_FREE)
        return;
    TimeOut = 0;
    if (msg.size() == 4 && (quint8)msg.at(0) == 0x00)
        ReadCanCmdStatus(msg);
    if (msg.size() == 7 && (quint8)msg.at(0) == 0x01)
        ReadCanCmdResult(msg);
}
/**
  * @brief   耐压状态,ID:0x61;长度:0x02;命令:0x00 status;
  */
void PageAcw::ReadCanCmdStatus(QByteArray msg)
{
    if (quint8(msg.at(1)) != 0) {
        emit SendCommand(ADDR,CMD_DEBUG,"ACW Error:");
        emit SendCommand(ADDR,CMD_DEBUG,msg.toHex());
        emit SendCommand(ADDR,CMD_DEBUG,"\n");
        Mode = ACW_FREE;
        return;
    }
    if (Mode == ACW_TEST) {
        SendTestItem();
        ClearResults();
    }
    Mode = ACW_FREE;
}
/**
  * @brief   耐压结果,ID:0x61;长度:0x07;命令:0x01 volt volt curr curr grade judge;
  * @brief   测试中不合格直接停止测试
  */
void PageAcw::ReadCanCmdResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double tt = quint16(msg.at(3)*256)+quint8(msg.at(4));
    tt *= qPow(10,-quint8(msg.at(5)));
    Volt.append(v);
    Curr.append(tt);
    SendTestItemTemp();
    if (quint8(msg.at(6)) != 0x00) {
        Judge = "NG";
        SendTestItem();
        ClearResults();
    }
}
void PageAcw::SendTestItemsAllEmpty()
{
    Items.clear();
    QString U1 = QString::number(ui->BoxVoltage->value());
    QString M1 = ui->BoxMin->text();
    QString M2 = ui->BoxMax->text();
    QString s = QString(tr("交耐@%1V,%2~%3mA@ @ ")).arg(U1).arg(M1).arg(M2);
    Items.append(s);
    emit SendCommand(ADDR,CMD_INIT_ITEM,Items.join("\n").toUtf8());
}

void PageAcw::SendTestItemsAllError()
{
    for (int i=0; i<Items.size(); i++) {
        QStringList s = QString(Items.at(i)).split("@");
        if (s.at(2) == " ")
            s[2] = "---";
        if (s.at(3) == " ")
            s[3] = "NG";
        emit SendCommand(ADDR,CMD_ITEM,s.join("@").toUtf8());
    }
}
void PageAcw::SendTestItemTemp()
{
    if (Volt.size()<2 || Curr.size()<2) {
        return;
    }
    QString rrr = QString::number(Curr.at(Curr.size()-2),'f',2);
    QString t = QString("%1mA").arg(rrr);
    QStringList s = QString(Items.at(0)).split("@");
    if (s.at(2) == " ")
        s[2] = t;
    emit SendCommand(ADDR,CMD_ITEM_TEMP,s.join("@").toUtf8());
}
/**
  * @brief   耐压判定,下位机BUG,需移除最后一帧数据
  */
void PageAcw::SendTestItem()
{
    if (Volt.isEmpty() || Curr.isEmpty()) {
        Judge = "NG";
        SendTestItemsAllError();
        return;
    }
    if (Volt.size()>1 || Curr.size()>1) {
        Volt.removeLast();
        Curr.removeLast();
    }
    QStringList s = QString(Items.at(0)).split("@");
    if (s.at(2) == " ")
        s[2] = QString("%1mA").arg(Curr.last(),0,'g',2);
    if (s.at(3) == " ")
        s[3] = Judge;
    emit SendCommand(ADDR,CMD_ITEM,s.join("@").toUtf8());
}

void PageAcw::SendTestJudge()
{
    QString s = QString(tr("交耐@%1@%2")).arg(CurrentSettings()).arg(Judge);
    emit SendCommand(ADDR,CMD_JUDGE,s.toUtf8());
}
/**
  * @brief   耐压启动,ID:0x23;长度:0x05;命令:0x01 0x05 0x00 工位 0x00
  */
void PageAcw::SendCanCmdStart(quint8 pos)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x05)<<quint8(0x01)<<quint8(0x05)<<quint8(0x00)
      <<quint8(pos)<<quint8(0x00);
    emit SendCommand(ADDR,CMD_CAN,msg);
}
/**
  * @brief   耐压停止,ID:0x23;长度:0x01;命令:0x02
  */
void PageAcw::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x01)<<quint8(0x02);
    emit SendCommand(ADDR,CMD_CAN,msg);
}
/**
  * @brief   耐压配置1,ID:0x23;长度:0x08;命令:0x03 0x00 0x05 0x00 0x00 0xff 0xff freq
  * @brief   耐压配置2,ID:0x23;长度:0x08;命令:0x04 0x00 volt volt time time min  min
  * @brief   耐压配置3,ID:0x23;长度:0x07;命令:0x05 0x00 max  max  arc  0x03 0x0A
  */
void PageAcw::SendCanCmdConfig()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    int freq = ui->BoxFrequcy->currentText().toInt()%256;
    int volt = ui->BoxVoltage->value();
    int time = ui->BoxTime->value()*10;
    int min = ui->BoxMin->value()*100;
    int max = ui->BoxMax->value()*100;
    int arc = ui->BoxArc->currentIndex();
    out<<quint16(0x23)<<quint8(0x08)<<quint8(0x03)<<quint8(0x00)<<quint8(0x05)
      <<quint8(0x80)<<quint8(0x00)<<quint8(0x7f)<<quint8(0xff)<<quint8(freq);
    out<<quint16(0x23)<<quint8(0x08)<<quint8(0x04)<<quint8(0x00)<<quint8(volt/256)
      <<quint8(volt%256)<<quint8(time/256)<<quint8(time%256)<<quint8(min/256)<<quint8(min%256);
    out<<quint16(0x23)<<quint8(0x07)<<quint8(0x05)<<quint8(0x00)
      <<quint8(max/256)<<quint8(max%256)<<quint8(arc)<<quint8(0x03)<<quint8(0x0A);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void PageAcw::ClearResults()
{
    Volt.clear();
    Curr.clear();
}

bool PageAcw::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (Mode != ACW_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageAcw::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

QString PageAcw::CurrentSettings()
{
    QSettings *ini = new QSettings(INI_PATH,QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse",INI_DEFAULT).toString();
    return n.remove(".ini");
}

void PageAcw::showEvent(QShowEvent *)
{
    InitSettings();
}
/*********************************END OF FILE**********************************/

