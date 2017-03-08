#include "PageInr.h"
#include "ui_PageInr.h"

PageInr::PageInr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageInr)
{
    ui->setupUi(this);
    InitializesWindow();
    InitializesButton();
    InitializesSetting();
    Testing = false;
    isCheckOk = false;
}

PageInr::~PageInr()
{
    delete ui;
}

void PageInr::InitializesWindow()
{
    ui->BoxVoltage->setView(new QListView(this));
}

void PageInr::InitializesButton()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExitIr,Qt::Key_0);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}

void PageInr::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        emit SendMessage(ADDR,CMD_JUMP,NULL);
        break;
    default:
        break;
    }
}

void PageInr::InitializesSetting()
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
    Testing = false;
    qDebug()<<QTime::currentTime().toString()<<"绝缘数据OK";
}

void PageInr::SaveSetting()
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
        Testing = true;
        SendStatusCmd();
        Testing = false;
        break;
    case CMD_START:
        Testing = true;
        SendStartCmd(msg.toInt());
        Testing = false;
        break;
    case CMD_STOP:
        SendStopCmd();
        Testing = false;
        break;
    case CMD_INIT:
        InitializesSetting();
        InitializesItem();
        SendConfigCmd();
        break;
    case CMD_ALARM:
        SendAlarmCmd(quint8(msg.at(0)));
        break;
    default:
        break;
    }
}

void PageInr::ExcuteCanCmd(QByteArray msg)
{
    if (!Testing)
        return;
    TimeOut = 0;
    if (msg.size() == 4 && (quint8)msg.at(0) == 0x00)
        ReadStatus(msg);
    if (msg.size() == 7 && (quint8)msg.at(0) == 0x01)
        ReadResult(msg);
}

void PageInr::InitializesItem()
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

void PageInr::SendStatusCmd()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x01)<<quint8(0x00);
    emit SendMessage(ADDR,CMD_CAN,msg);
    if (!WaitTestOver(100)) {
        QMessageBox::warning(this,tr("警告"),tr("绝缘板异常"),QMessageBox::Ok);
        emit SendMessage(ADDR,CMD_DEBUG,"Check PageInr Error:Time out\n");
    }
}

void PageInr::ReadStatus(QByteArray )
{
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

void PageInr::SendStartCmd(quint8 pos)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x05)<<quint8(0x01)<<quint8(0x04)<<quint8(0x00)
      <<quint8(pos)<<quint8(0x01);
    emit SendMessage(ADDR,CMD_CAN,msg);
    Judge = "OK";
    if(!WaitTestOver(100)) {
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

void PageInr::ReadResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double tt = quint16(msg.at(3)*256)+quint8(msg.at(4));
    Volt.append(v);
    Res.append(tt);
}

void PageInr::SendStopCmd()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x01)<<quint8(0x02);
    emit SendMessage(ADDR,CMD_CAN,msg);
}

void PageInr::SendConfigCmd()
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

void PageInr::SendAlarmCmd(quint8 port)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x23)<<quint8(0x02)<<quint8(0x09)<<quint8(port);
    emit SendMessage(ADDR,CMD_CAN,msg);
}

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

void PageInr::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

void PageInr::showEvent(QShowEvent *)
{
    InitializesSetting();
}

void PageInr::hideEvent(QHideEvent *)
{
    SaveSetting();
}
/*********************************END OF FILE**********************************/
