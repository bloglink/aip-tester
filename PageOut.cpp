#include "PageOut.h"
#include "ui_PageOut.h"

PageOut::PageOut(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageOut)
{
    ui->setupUi(this);

    Timer = new QTimer(this);
    connect(Timer,SIGNAL(timeout()),this,SLOT(SendWinCmdStart()));

    Mode = OUT_FREE;
}

PageOut::~PageOut()
{
    delete ui;
}

void PageOut::ReadMessage(quint16 addr, quint16 cmd, QByteArray data)
{
    if (addr!=WIN_ID_OUT13 && addr!=CAN_ID_13OUT && addr!=CAN_ID_14OUT && addr!=CAN_ID_15OUT)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(addr,data);
        break;
    case CMD_CHECK:
        qDebug()<<QTime::currentTime().toString()<<"查询输出状态";
        Mode = OUT_INIT;
        SendCanCmdStatus(data.toInt());
        Mode = OUT_FREE;
        qDebug()<<QTime::currentTime().toString()<<"查询输出状态OK";
        break;
    case CMD_INIT:
        SendCanCmdConfig(data);
        break;
    default:
        break;
    }
}

void PageOut::ExcuteCanCmd(quint16 id,QByteArray msg)
{
    if (quint8(msg.at(0)) == 0x00)
        ReadCanCmdStatus(id,msg);

    if (quint8(msg.at(0)) == 0x01 && quint8(msg.at(1) == 0x01)) {
        ReadCanCmdStart(id);
    }
    if (quint8(msg.at(0)) == 0x01 && quint8(msg.at(1) == 0x00)) {
        ReadCanCmdStop(id);
    }
}

void PageOut::SendCanCmdStatus(quint16 pos)
{

    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    QString w;
    switch (pos) {
    case 0:
        out<<quint16(0x13)<<quint8(0x01)<<quint8(0x00);
        w = tr("输出板13异常");
        break;
    case 1:
        out<<quint16(0x14)<<quint8(0x01)<<quint8(0x00);
        w = tr("输出板14异常");
        break;
    case 2:
        out<<quint16(0x15)<<quint8(0x01)<<quint8(0x00);
        w = tr("输出板15异常");
        break;
    default:
        break;
    }
    emit SendCommand(ADDR,CMD_CAN,msg);
    if (!WaitTestOver(100)) {
        QMessageBox::warning(this,tr("警告"),w,QMessageBox::Ok);
        emit SendCommand(ADDR,CMD_DEBUG,"Check PageOut Error:Time out\n");
    }
}

void PageOut::SendWinCmdStart()
{
    Timer->stop();
    emit SendCommand(ADDR,CMD_START,QString::number(Pos).toUtf8());
}

void PageOut::SendCanCmdConfig(QByteArray data)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);

    quint8 mode = quint8(data.at(0));
    emit SendCommand(ADDR,CMD_DEBUG,data.toHex());
    out<<quint16(0x13)<<quint8(0x02)<<quint8(0x03)<<quint8(mode);
    out<<quint16(0x14)<<quint8(0x02)<<quint8(0x03)<<quint8(mode);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void PageOut::ReadCanCmdStatus(quint16 addr, QByteArray msg)
{
    if (quint8(msg.at(1)) == 0x00) {
        QByteArray n = QString("OUT%1 Check Ok\n").arg(addr,0,16).toUtf8();
        emit SendCommand(ADDR,CMD_DEBUG,n);
        Mode = OUT_FREE;
    }
}

void PageOut::ReadCanCmdStart(quint16 addr)
{
    if (Mode != OUT_FREE)
        return;
    if (addr == CAN_ID_13OUT) {
        Pos = WIN_ID_OUT13;
    }
    if (addr == CAN_ID_14OUT) {
        Pos = WIN_ID_OUT14;
    }
    Mode = OUT_FREE;
    Timer->start(10);
}

void PageOut::ReadCanCmdStop(quint16 addr)
{
    if (addr == CAN_ID_13OUT && Pos == 0x13) {
        Mode = OUT_FREE;
        emit SendCommand(ADDR,CMD_STOP,NULL);
    }
    if (addr == CAN_ID_14OUT && Pos == 0x14) {
        Mode = OUT_FREE;
        emit SendCommand(ADDR,CMD_STOP,NULL);
    }
}

bool PageOut::WaitTestOver(quint16 t)
{
    TimeOut = 0;
    while (Mode != OUT_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageOut::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}
/*********************************END OF FILE**********************************/
