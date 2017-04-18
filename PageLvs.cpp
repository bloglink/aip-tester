#include "PageLvs.h"
#include "ui_PageLvs.h"

PageLvs::PageLvs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageLvs)
{
    ui->setupUi(this);
    InitWindows();
    InitButton();
    InitSettings();
    Mode = LVS_FREE;
}

PageLvs::~PageLvs()
{
    delete ui;
}

void PageLvs::InitWindows()
{

}

void PageLvs::InitButton()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExit,Qt::Key_0);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}

void PageLvs::BtnJudge(int id)
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

void PageLvs::InitSettings()
{
    QSettings *global = new QSettings(INI_PATH,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    PowerSupply = global->value("PowerSupply","0").toInt();
    FileInUse = global->value("FileInUse",INI_DEFAULT).toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1").arg(global->value("FileInUse",INI_DEFAULT).toString());
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetLvs");

    QStringList temp = (set->value("Other","20 0 0.5 10 10 50").toString()).split(" ");
    if (temp.size() >= 6) {
        ui->BoxVolt->setValue(temp.at(0).toDouble());
        ui->BoxTime->setValue(temp.at(1).toDouble());
        ui->BoxCurrMin->setValue(temp.at(2).toDouble());
        ui->BoxCurrMax->setValue(temp.at(3).toDouble());
        ui->BoxPowerMin->setValue(temp.at(4).toDouble());
        ui->BoxPowerMax->setValue(temp.at(5).toDouble());
    }
    qDebug()<<QTime::currentTime().toString()<<"PageLvs read OK";
}

void PageLvs::SaveSettings()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxVolt->value()));
    temp.append(QString::number(ui->BoxTime->value()));
    temp.append(QString::number(ui->BoxCurrMin->value()));
    temp.append(QString::number(ui->BoxCurrMax->value()));
    temp.append(QString::number(ui->BoxPowerMin->value()));
    temp.append(QString::number(ui->BoxPowerMax->value()));
    set->setValue("Other",(temp.join(" ").toUtf8()));
    
    qDebug()<<QTime::currentTime().toString()<<"PageLvs save OK";
}

void PageLvs::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_LVS && addr != CAN_ID_PWR)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(msg);
        break;
    case CMD_START:
        Mode = LVS_TEST;
        Judge = "OK";
        SendCanCmdStart();
        if(!WaitTimeOut(100)) {
            Judge = "NG";
            SendTestItemsAllError();
            break;
        }
        SendTestJudge();
        Mode = LVS_FREE;
        break;
    case CMD_STOP:
        SendCanCmdStop();
        Mode = LVS_FREE;
        break;
    case CMD_INIT:
        InitSettings();
        InitTestItems();
        break;
    default:
        break;
    }
}

void PageLvs::ExcuteCanCmd(QByteArray msg)
{
    if (Mode == LVS_FREE)
        return;
    TimeOut = 0;
    if (msg.size() == 4 && (quint8)msg.at(0) == 0x00)
        ReadCanCmdStatus(msg);
    if (msg.size() == 8 && (quint8)msg.at(0) == 0x01)
        ReadCanCmdResult(msg);
}

void PageLvs::InitTestItems()
{
    Items.clear();
    QStringList s;
    s.append(QString(tr("低启")));

    double M1 = ui->BoxCurrMin->value();
    double M2 = ui->BoxCurrMax->value();
    double Q1 = ui->BoxPowerMin->value();
    double Q2 = ui->BoxPowerMax->value();
    s.append(QString("%1~%2A,%3~%4W").arg(M1).arg(M2).arg(Q1).arg(Q2));
    s.append(" ");
    s.append(" ");
    Items.append(s.join("@"));
    emit SendCommand(ADDR,CMD_INIT_ITEM,Items.join("\n").toUtf8());
}

void PageLvs::SendTestItemsAllError()
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

void PageLvs::SendCanCmdStart()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 t = ui->BoxTime->value()*10;
    quint16 v = ui->BoxVolt->value();
    quint8 p = PowerSupply<<4;
    if (ui->BoxFreq->value() == 60)
        p += 0x02;
    out<<quint16(0x27)<<quint8(0x07)<<quint8(0x01)<<quint8(0x01)
      <<quint8(t/256)<<quint8(t%256)<<quint8(p+v/256)<<quint8(v%256)
     <<quint8(0x00)<<quint8(0x00);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void PageLvs::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x27)<<quint8(0x01)<<quint8(0x02);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void PageLvs::SendItemTemp()
{
    if (Volt.size()<2 || Curr.size()<2 || Power.size()<2) {
        return;
    }
    QString vvv = QString::number(Volt.last()/10,'f',1);
    QString rrr = QString::number(Curr.last()/1000,'f',3);
    QString ppp = QString::number(Power.last()/10,'f',1);
    QString t = QString("%1A,%2W,%3V").arg(rrr).arg(ppp).arg(vvv);

    QStringList s = QString(Items.at(0)).split("@");
    if (s.at(2) == " ")
        s[2] = t;
    emit SendCommand(ADDR,CMD_ITEM_TEMP,s.join("@").toUtf8());
}

void PageLvs::SendItemJudge()
{
    if (Curr.isEmpty()) {
        SendTestItemsAllError();
        return;
    }
    QString rrr = QString::number(Curr.last()/1000,'f',3);
    QString ppp = QString::number(Power.last()/10,'f',1);
    QString t = QString("%1A,%2W").arg(rrr).arg(ppp);

    QStringList s = QString(Items.at(0)).split("@");
    if (s.at(2) == " ")
        s[2] = t;
    if (s.at(3) == " ")
        s[3] = Judge;
    emit SendCommand(ADDR,CMD_ITEM,s.join("@").toUtf8());
}

void PageLvs::SendTestJudge()
{
    QStringList s;
    s.append("低启");
    s.append(FileInUse);
    s.append(Judge);
    emit SendCommand(ADDR,CMD_JUDGE,s.join("@").toUtf8());
}

void PageLvs::ReadCanCmdStatus(QByteArray msg)
{
    if (quint8(msg.at(1)) != 0x00)
        return;

    if (Mode == LVS_TEST) {
        SendItemJudge();
        ClearResults();
    }
    Mode = LVS_FREE;
}

void PageLvs::ReadCanCmdResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double c = quint16(msg.at(3)*256)+quint8(msg.at(4));
    double p = quint16(msg.at(5)*256)+quint8(msg.at(6));
    Volt.append(v);
    Curr.append(c);
    Power.append(p);
    SendItemTemp();
    CalculateResult();
    if (Judge == "NG") {
        SendCanCmdStop();
        SendItemJudge();
        ClearResults();
        Mode = LVS_FREE;
    }
}

void PageLvs::CalculateResult()
{
    if (Volt.size()<5 || Curr.size()<5 || Power.size()<5)
        return;
    double rr = Curr.last()/1000;
    double pp = Power.last()/10;

    if (rr>ui->BoxCurrMax->value() || rr<ui->BoxCurrMin->value() ||
            pp>ui->BoxPowerMax->value() || pp<ui->BoxPowerMin->value())
        Judge = "NG";
}

void PageLvs::ClearResults()
{
    Volt.clear();
    Curr.clear();
    Power.clear();
}

bool PageLvs::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (Mode != LVS_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageLvs::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

void PageLvs::showEvent(QShowEvent *)
{
    InitSettings();
}

/*********************************END OF FILE**********************************/
