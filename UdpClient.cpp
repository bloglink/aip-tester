#include "UdpClient.h"

UdpClient::UdpClient(QObject *parent) : QUdpSocket(parent)
{

}

void UdpClient::UdpInit()
{
    this->bind(6000);
    connect(this,SIGNAL(readyRead()),this,SLOT(ReadAllFrame()));
}

void UdpClient::UdpQuit()
{
    this->close();
}

void UdpClient::DatInit()
{
    qDebug()<<QTime::currentTime().toString()<<"读取UDP网络配置";

    QDir dir("./config");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QStringList FileNames;
    QFileInfoList list = dir.entryInfoList();
    for (int i=0; i<list.size(); i++)
        FileNames.append(list.at(i).fileName().remove(".ini"));

    QSettings *g_settings = new QSettings(INI_PATH,QSettings::IniFormat);
    g_settings->setIniCodec("GB18030");
    g_settings->beginGroup("GLOBAL");
    //当前使用的测试项目
    FileInUse = g_settings->value("FileInUse",INI_DEFAULT).toString();
    FileInUse.remove(".ini");
    Types = FileNames.join(" ");
    Types.remove(FileInUse);
    Types.insert(0,FileInUse);

    Number = g_settings->value("UserNumber").toString();
    if (Number.isEmpty())
        Number = g_settings->value("Number","168912000X").toString();
    qDebug()<<QTime::currentTime().toString()<<"读取UDP网络配置OK";
}

void UdpClient::DatSave()
{
    QSettings *g_settings = new QSettings(INI_PATH,QSettings::IniFormat);
    g_settings->setIniCodec("GB18030");
    g_settings->beginGroup("GLOBAL");
    g_settings->setValue("UserNumber",Number);
    g_settings->setValue("FileInUse",FileInUse);
}

void UdpClient::GetCommand(QByteArray msg)
{
    QStringList n = QString(msg).split(" ");
    Command = quint16(n.at(0).toInt());
    n.removeFirst();
    Param = n.join(" ").toUtf8();
}

void UdpClient::ReadAllFrame()
{
    while (this->hasPendingDatagrams()) {
        QByteArray msg;
        msg.resize(this->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        this->readDatagram(msg.data(),msg.size(),&sender,&senderPort);
        GetCommand(msg);

        switch (Command) {
        case 3000: //查询在线主机
            DatInit();
            TxMsg = "3001 ";
            TxMsg.append(Number);
            this->writeDatagram(TxMsg.toUtf8(),sender,senderPort);
            break;
        case 3002: //修改主机编号
            if (!Param.isEmpty())
                Number = Param;
            DatSave();
            TxMsg = "3002 ";
            TxMsg.append(Number);
            this->writeDatagram(TxMsg.toUtf8(),sender,senderPort);
            break;
        case 3004: //获取测试型号
            DatInit();
            TxMsg = "3005 ";
            TxMsg.append(Types);
            this->writeDatagram(TxMsg.toUtf8(),sender,senderPort);
            break;
        case 3006: //设置测试型号
            if (!Param.isEmpty())
                FileInUse = Param;
            DatSave();
            TxMsg = "3007 ";
            TxMsg.append(Number);
            this->writeDatagram(TxMsg.toUtf8(),sender,senderPort);
            break;
        case 3008: //启动测试
            emit SendMessage(ADDR,CMD_START,QString::number(0x13).toUtf8());
            TxMsg = "3009 ";
            TxMsg.append(Number);
            this->writeDatagram(TxMsg.toUtf8(),sender,senderPort);
            break;
        case 3010: //停止测试
            emit SendMessage(ADDR,CMD_STOP,QString::number(0x13).toUtf8());
            TxMsg = "3011 ";
            TxMsg.append(Number);
            this->writeDatagram(TxMsg.toUtf8(),sender,senderPort);
            break;
        case 3012: //获取状态
            TxMsg = "3013 ";
            TxMsg.append("free");
            this->writeDatagram(TxMsg.toUtf8(),sender,senderPort);
            break;
        case 3014:
            TxMsg = "3014 ";
            TxMsg.append("测试@测试@测试@测试\n测试@测试@100@测试");
            this->writeDatagram(TxMsg.toUtf8(),sender,senderPort);
            break;
        default:
            this->writeDatagram("Error command",sender,senderPort);
            break;
        }
    }
}

