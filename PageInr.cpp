#include "PageInr.h"
#include "ui_PageInr.h"

PageInr::PageInr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageInr)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
    Mode = INR_FREE;
}

PageInr::~PageInr()
{
    delete ui;
}

void PageInr::InitWindows()
{
    ui->BoxVoltage->setView(new QListView(this));
}

void PageInr::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExitIr,Qt::Key_0);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}

void PageInr::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        emit SendCommand(ADDR,CMD_JUMP,NULL);
        break;
    default:
        break;
    }
}

void PageInr::InitSettings()
{
    qDebug()<<QTime::currentTime().toString()<<"绝缘数据";
    QSettings *global = new QSettings(INI_PATH,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    FileInUse = global->value("FileInUse",INI_DEFAULT).toString();
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
    qDebug()<<QTime::currentTime().toString()<<"绝缘数据OK";
}

void PageInr::SaveSettings()
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

void PageInr::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_INR && addr != CAN_ID_INR)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(msg);
        break;
    case CMD_CHECK:
        Mode = INR_INIT;
        SendCanCmdStatus();
        if (!WaitTimeOut(10)) {
            QMessageBox::warning(this,tr("警告"),tr("绝缘板异常"),QMessageBox::Ok);
            emit SendCommand(ADDR,CMD_DEBUG,"Check PageInr Error:Time out\n");
        }
        Mode = INR_FREE;
        break;
    case CMD_START:
        Mode = INR_TEST;
        Judge = "OK";
        SendCanCmdStart(msg.toInt());
        if(!WaitTimeOut(100)) {
            Judge = "NG";
            SendTestItemsAllError();
        }
        SendTestJudge();
        Mode = INR_FREE;
        break;
    case CMD_STOP:
        SendCanCmdStop();
        Mode = INR_FREE;
        break;
    case CMD_INIT:
        InitSettings();
        InitTestItems();
        SendCanCmdConfig();
        break;
    case CMD_ALARM:
        SendCanCmdAlarm(quint8(msg.at(0)));
        break;
    default:
        break;
    }
}

void PageInr::ExcuteCanCmd(QByteArray msg)
{
    if (Mode == INR_FREE)
        return;
    TimeOut = 0;
    if (msg.size() == 4 && (quint8)msg.at(0) == 0x00)
        ReadCanCmdStatus(msg);
    if (msg.size() == 7 && (quint8)msg.at(0) == 0x01)
        ReadCanCmdResult(msg);
}

void PageInr::InitTestItems()
{
    Items.clear();
    QString s;
    QString U1 = ui->BoxVoltage->currentText();
    QString M1 = ui->BoxMin->text();
    QString M2 = ui->BoxMax->text();
    if (M2.toInt() == 0)
        s = QString(tr("绝缘@%1V,%2Mohm@ @ ")).arg(U1).arg(M1);
    else
        s = QString(tr("绝缘@%1V,%2~%3Mohm@ @ ")).arg(U1).arg(M1).arg(M2);
    Items.append(s);
    emit SendCommand(ADDR,CMD_INIT_ITEM,Items.join("\n").toUtf8());
}

void PageInr::SendTestItemsAllError()
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

void PageInr::SendCanCmdStatus()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x01)<<quint8(0x00);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void PageInr::SendCanCmdStart(quint8 pos)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x05)<<quint8(0x01)<<quint8(0x04)<<quint8(0x00)
      <<quint8(pos)<<quint8(0x01);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void PageInr::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x01)<<quint8(0x02);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void PageInr::SendCanCmdConfig()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    int volt = ui->BoxVoltage->currentText().toInt();
    int time = ui->BoxTime->value()*10;
    int min = ui->BoxMin->value();
    int max = ui->BoxMax->value();
    out<<quint16(0x23)<<quint8(0x08)<<quint8(0x03)<<quint8(0x01)<<quint8(0x04)
      <<quint8(0x00)<<quint8(0x00)<<quint8(0xff)<<quint8(0xff)<<quint8(0x00);
    out<<quint16(0x23)<<quint8(0x08)<<quint8(0x04)<<quint8(0x01)<<quint8(volt/256)
      <<quint8(volt%256)<<quint8(time/256)<<quint8(time%256)<<quint8(min/256)<<quint8(min%256);
    out<<quint16(0x23)<<quint8(0x07)<<quint8(0x05)<<quint8(0x01)
      <<quint8(max/256)<<quint8(max%256)<<quint8(0x00)<<quint8(0x03)<<quint8(0x0A);//上限
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void PageInr::SendItemTemp()
{
    if (Volt.size()<2 || Res.size()<2) {
        return;
    }
    QString rrr = QString::number(Res.last(),'f',1);
    if (Res.last()>500)
        rrr = ">500";
    QString t = QString("%1Mohm").arg(rrr);
    QStringList s = QString(Items.at(0)).split("@");
    if (s.at(2) == " ")
        s[2] = t;
    emit SendCommand(ADDR,CMD_ITEM_TEMP,s.join("@").toUtf8());
}

void PageInr::SendItemJudge()
{
    if (Volt.isEmpty() || Res.isEmpty()) {
        Judge = "NG";
        SendTestItemsAllError();
        return;
    }
    QString rrr = QString::number(Res.last(),'f',1);
    if (Res.last()>500)
        rrr = ">500";
    QString t = QString("%Mohm").arg(rrr);

    QStringList s = QString(Items.at(0)).split("@");
    if (s.at(2) == " ")
        s[2] = t;
    if (s.at(3) == " ")
        s[3] = Judge;
    emit SendCommand(ADDR,CMD_ITEM,s.join("@").toUtf8());
}

void PageInr::SendTestJudge()
{
    QString s = QString(tr("绝缘@%1@%2")).arg(FileInUse).arg(Judge);
    emit SendCommand(ADDR,CMD_JUDGE,s.toUtf8());
}

void PageInr::SendCanCmdAlarm(quint8 port)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x02)<<quint8(0x09)<<quint8(port);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void PageInr::ReadCanCmdStatus(QByteArray )
{
    if (Mode == INR_INIT)
        emit SendCommand(ADDR,CMD_DEBUG,"Check PageInr OK\n");
    if (Mode == INR_TEST) {
        SendItemJudge();
        ClearResults();
    }
    Mode = INR_FREE;
}

void PageInr::ReadCanCmdResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double tt = quint16(msg.at(3)*256)+quint8(msg.at(4));
    tt *= qPow(10,-quint8(msg.at(5)));
    Volt.append(v);
    Res.append(tt);
    if (quint8(msg.at(6)) != 0x00) {
        Judge = "NG";
        SendItemJudge();
        ClearResults();
    }
}

void PageInr::ClearResults()
{
    Volt.clear();
    Res.clear();
}

bool PageInr::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (Mode != INR_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageInr::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

void PageInr::showEvent(QShowEvent *)
{
    InitSettings();
}

void PageInr::hideEvent(QHideEvent *)
{
    SaveSettings();
}
/*********************************END OF FILE**********************************/
