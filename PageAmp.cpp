#include "PageAmp.h"

PageAmp::PageAmp(QObject *parent) : QObject(parent)
{
    TestStatus = "free";
}

void PageAmp::ExcuteCanCmd(int addr, QByteArray msg)
{
    quint8 cmd = (quint8)msg.at(0);
    switch (cmd) {
    case 0x00:
        ReadCanCmdStatus(msg);
        break;
    default:
        qDebug() << addr << msg.toHex();
        break;
    }
}

void PageAmp::SendCanCmdStatus()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x2A) << quint8(0x01) << quint8(0x00);
    emit CanMsg(msg);
}

void PageAmp::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("功放异常:\n%1").arg(s));
    emit SendVariant(hash);
}

void PageAmp::SendError(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Error");
    hash.insert("TxMessage", tr("功放异常:\n%1").arg(s));
    emit SendVariant(hash);
}

void PageAmp::ReadCanCmdStatus(QByteArray msg)
{
    int s = quint8(msg.at(1));
    switch (s) {
    case 0x00:
        break;
    case 0x01:
        return;
    default:
        SendWarnning(tr("OTHER_ERROR %1").arg(s));
        break;
    }
    int e = quint8(msg.at(2));
    switch (e) {
    case 0x00:
        break;
    case 0x01:
        SendError(tr("未接地"));
        break;
    case 0x02:
        SendWarnning("L/N reversed");
        break;
    default:
        SendWarnning(tr("PWR_ERROR %1").arg(e));
        break;
    }
    TestStatus = "free";
}

bool PageAmp::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (TestStatus != "free") {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageAmp::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void PageAmp::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "PageOut" && s.value("TxAddress") != "WinHome")
        return;
    if (s.value("TxCommand") == "CheckStatus") {
        TestStatus = "init";
        SendCanCmdStatus();
        WaitTimeOut(30);
        TestStatus = "free";
    }
}
