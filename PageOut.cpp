#include "PageOut.h"
#include "ui_PageOut.h"

PageOut::PageOut(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageOut)
{
    ui->setupUi(this);
    Testing = false;
    isStop = true;

    Timer = new QTimer(this);
    connect(Timer,SIGNAL(timeout()),this,SLOT(TestStart()));
}

PageOut::~PageOut()
{
    delete ui;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.15
 * brief:       命令处理
*******************************************************************************/
void PageOut::ReadMessage(quint16 addr, quint16 cmd, QByteArray data)
{
    if (addr!=WIN_ID_OUT && addr!=CAN_ID_13OUT && addr!=CAN_ID_14OUT && addr!=CAN_ID_15OUT)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(addr,data);
        break;
    case CMD_CHECK:
        TestCheck(data.toInt());
        break;
    case CMD_INIT:
        TestConfig(data);
        break;
    default:
        break;
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       输出板信号处理
******************************************************************************/
void PageOut::ExcuteCanCmd(quint16 id,QByteArray msg)
{
    if (quint8(msg.at(0)) == 0x00) {
        Testing = false;
    }
    if (quint8(msg.at(0)) == 0x01 && quint8(msg.at(1) == 0x01)) {
        if (!isStop || Testing)
            return;
        if (id == CAN_ID_13OUT) {
            Pos = 0x13;
        }
        if (id == CAN_ID_14OUT) {
            Pos = 0x14;
        }
        isStop = false;
        Timer->start(10);
    }
    if (quint8(msg.at(0)) == 0x01 && quint8(msg.at(1) == 0x00)) {
        if (id == CAN_ID_13OUT && Pos == 0x13) {
            isStop = true;
            Testing = false;
            emit SendMessage(ADDR,CMD_STOP,NULL);
        }
        if (id == CAN_ID_14OUT && Pos == 0x14) {
            isStop = true;
            Testing = false;
            emit SendMessage(ADDR,CMD_STOP,NULL);
        }
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       查询输出板状态
******************************************************************************/
void PageOut::TestCheck(quint16 pos)
{
    if (Testing)
        return;
    qDebug()<<QTime::currentTime().toString()<<"查询输出状态";
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);

    Testing = true;
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
    emit SendMessage(ADDR,CMD_CAN,msg);
    if (!WaitTestOver(100)) {
        Testing = false;
        QMessageBox::warning(this,tr("警告"),w,QMessageBox::Ok);
    }
    qDebug()<<QTime::currentTime().toString()<<"查询输出状态OK";
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.17
 * brief:       启动测试
******************************************************************************/
void PageOut::TestStart()
{
    Timer->stop();
    emit SendMessage(ADDR,CMD_START,QString::number(Pos).toUtf8());
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       设置启动方式
******************************************************************************/
void PageOut::TestConfig(QByteArray data)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);

    quint8 mode = quint8(data.at(0));
    out<<quint16(0x13)<<quint8(0x02)<<quint8(0x03)<<quint8(mode);
    out<<quint16(0x14)<<quint8(0x02)<<quint8(0x03)<<quint8(mode);
    emit SendMessage(ADDR,CMD_CAN,msg);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      等待测试结束
 * date:       2017.02.15
 * brief:      去除超时处理
*******************************************************************************/
bool PageOut::WaitTestOver(quint16 t)
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
void PageOut::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}
