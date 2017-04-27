/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       TCP处理模块
*******************************************************************************/
#include "TcpClient.h"

TcpClient::TcpClient(QObject *parent) : QTcpSocket(parent)
{
    count        = 0;
    LoadSize     = 4*1024;;
    BlockSize    = 0;
    BytesRead    = 0;
    BytesToRead  = 0;
    BytesToWrite = 0;
    BytesWritten = 0;

    QDir *dir = new QDir;
    if (!dir->exists(QString(TMP)))
        dir->mkdir(QString(TMP));
    if (!dir->exists(QString(NET)))
        dir->mkdir(QString(NET));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(KeepAlive()));
    connect(this, SIGNAL(connected()), this, SLOT(Connected()));
    connect(this, SIGNAL(readyRead()), this, SLOT(GetBlock()));
    connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(PutFileData(qint64)));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(Error(QAbstractSocket::SocketError)));
}

void TcpClient::TcpInit()
{
    count = 0;
    QSettings *global = new QSettings(INI_PATH, QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    QString h = global->value("HOST", "s.aipuo.com").toString();
    QString v = global->value("Version", "V-2.0.0.0").toString();
    QString u = global->value("Number", "168912000X").toString();

    InitString.clear();
    InitString.append(u);
    InitString.append(" ");
    InitString.append(getHardwareAddress());
    InitString.append(" ");
    InitString.append(v);

    this->connectToHost(h,  6000);
    timer->start(10000);
}

void TcpClient::TcpQuit()
{
    this->close();
}

void TcpClient::Connected()
{
    ExcuteCmd(quint16(ADDR), quint16(GUEST_LOGIN), NULL);
    emit SendCommand(ADDR, CMD_NET, NULL);
}

void TcpClient::KeepAlive()
{
    if (this->state() != QAbstractSocket::ConnectedState) {
        TcpInit();
    } else {
        PutBlock(ADDR, HEART_BEAT, "NULL");
        count++;
        if (count >6) {
            this->abort();
        }
    }
}

void TcpClient::GetBlock()
{
    quint16 addr;
    quint16 cmd;
    QByteArray msg;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_8);
    while (1) {
        if (BlockSize == 0) {
            if (this->bytesAvailable() < qint64(sizeof(qint64)))
                return;
            in >> BlockSize;
        }
        if (BlockSize > 8*1024) {
            qDebug()  <<  "block size overflow";
            this->abort();
            return;
        }
        if (this->bytesAvailable() < BlockSize)
            return;
        in >> addr >> cmd >> msg;
        ExcuteCmd(addr, cmd, msg);
        BlockSize = 0;
    }
}

void TcpClient::GetFileHead(quint16 addr, QByteArray msg)
{
    SendVerify = msg.mid(0, 16);
    QString temp = msg.remove(0, 17);
    QStringList detail = temp.split(" ");
    BytesToRead = detail.at(0).toInt();
    BytesRead = 0;
    file = new QFile(QString(TMP).append(detail.at(1)));
    if (!file->open(QFile::ReadWrite))
        PutBlock(addr, FILE_HEAD_ERROR, file->errorString().toUtf8());
}

void TcpClient::GetFileData(quint16 addr, QByteArray msg)
{
    int ret;
    if (file->isOpen())
        ret = file->write(msg);
    else
        return;
    BytesRead += ret;
    if (BytesRead == BytesToRead) {
        file->seek(0);
        RecvVerify = QCryptographicHash::hash(file->readAll(), QCryptographicHash::Md5);
        file->close();
        BytesRead = 0;
        BytesToRead = 0;
        if (SendVerify == RecvVerify) {
            QProcess::execute("sync");
            QString cmd = QString("mv %1 %2").arg(file->fileName()).arg(NET);
            QProcess::execute(cmd);
            PutBlock(addr, FILE_SUCCESS, "NULL");
            qDebug()  <<  "Put Data Success";
        } else {
            QString cmd = QString("rm %1").arg(file->fileName());
            QProcess::execute(cmd);
            PutBlock(addr, FILE_ERROR, "NULL");
            qDebug()  <<  "Put Data Error";
        }
        return;
    }
}

void TcpClient::PutBlock(quint16 addr,  quint16 cmd,  QByteArray data)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << (qint64)0  <<  quint16(addr) << quint16(cmd) << data;
    out.device()->seek(0);
    out << (qint64)(msg.size()-sizeof(qint64));
    this->write(msg);
}

void TcpClient::PutFileHead(QByteArray data)
{
    QString name = data;
    file = new QFile(name);
    if (!file->open(QFile::ReadOnly)) {
        PutBlock(ADDR, FILE_HEAD_ERROR, file->errorString().toUtf8());
        qDebug()  <<  "open file error!"  <<  file->errorString();
        return;
    }
    QByteArray msg;
    msg.append(QCryptographicHash::hash(file->readAll(), QCryptographicHash::Md5));
    file->seek(0);
    msg.append(QString(" %1 ").arg(file->size()));
    msg.append(name.right(name.size()- name.lastIndexOf('/')-1));
    PutBlock(ADDR, FILE_HEAD, msg);
    BytesToWrite = file->size();
    BytesWritten = 0;
}

void TcpClient::PutFileData(qint64 numBytes)
{
    if (BytesToWrite == 0)
        return;
    BytesWritten += numBytes;
    PutBlock(quint16(ADDR), quint16(FILE_DATA), file->read(LoadSize));
    BytesToWrite -= qMin(BytesToWrite, LoadSize);
    if (BytesToWrite == 0)
        file->close();
    count = 0;
}

void TcpClient::ExcuteCmd(quint16 addr,  quint16 cmd,  QByteArray data)
{
    timer->stop();
    count = 0;
    switch (cmd) {
    case GUEST_LOGIN:
        PutBlock(ADDR, GUEST_LOGIN, InitString.toUtf8());
        break;
    case GUEST_LOGIN_ERROR:
        Display(data);
        break;
    case ADMIN_LOGIN:
        PutBlock(ADDR, ADMIN_LOGIN, InitString.toUtf8());
        break;
    case ADMIN_LOGIN_ERROR:
        emit SendCommand(addr, cmd, data);
        break;
    case ADMIN_LOGIN_SUCCESS:
        emit SendCommand(addr, cmd, data);
        break;
    case GUEST_PUT_HEAD:
        PutFileHead(data);
        break;
    case GUEST_GET_HEAD:
        this->PutBlock(ADDR, GUEST_GET_HEAD, data);
        break;
    case FILE_HEAD:
        GetFileHead(addr, data);
        break;
    case FILE_HEAD_ERROR:
        Display(data);
        break;
    case FILE_DATA:
        GetFileData(addr, data);
        break;
    case FILE_ERROR:
        Display(data);
        break;
    case SHELL_CMD:
        ShellCmd(addr, data);
        break;
    case SHELL_DAT:
        Display(data);
        break;
    case ONLINE_DEVICES:
        emit SendCommand(addr, cmd, data);
        break;
    case GUEST_DISPLAY:
        emit SendCommand(addr, cmd, data);
        break;
    case HEART_BEAT:
        break;
    default:
        qDebug() << addr << cmd << data;
        break;
    }
    timer->start(10000);
}

void TcpClient::ShellCmd(quint16 addr, QByteArray msg)
{
    QProcess *proc = new QProcess(this);
    proc->start(msg);
    proc->waitForFinished();
    PutBlock(quint16(addr), SHELL_DAT, proc->readAll());
}

QString TcpClient::getHardwareAddress()
{
    int i;
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    for (i=0; i < list.size(); i++) {
        if (list[i].hardwareAddress().size() == 17 &&
                list[i].hardwareAddress() != "00:00:00:00:00:00")
            return list[i].hardwareAddress();
    }
    return list[0].hardwareAddress();
}

void TcpClient::Error(QAbstractSocket::SocketError socketError)
{
    Display(this->errorString().toUtf8());
    if (socketError == QAbstractSocket::RemoteHostClosedError)
        return;
    qDebug() << "error:" << this->errorString(); //输出错误信息
    this->close();
}

void TcpClient::Display(QByteArray msg)
{
    if (TramsmitAddr != this->peerPort())
        emit SendCommand(TramsmitAddr, GUEST_DISPLAY, msg);
    else
        emit SendCommand(ADDR, GUEST_DISPLAY, msg);
}
/*********************************END OF FILE**********************************/
