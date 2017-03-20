#include "WinBack.h"
#include "ui_WinBack.h"

WinBack::WinBack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinBack)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
}

WinBack::~WinBack()
{
    delete ui;
}

void WinBack::InitWindows()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabItems->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabItems->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabOutput->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabItems->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabOutput->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    for (int i=0; i<ui->TabItems->columnCount(); i++) {
        for (int row=0; row<ui->TabItems->rowCount(); row++) {
            if (i>=2) {
                ui->TabItems->setItem(row,i,new QTableWidgetItem);
                ui->TabItems->item(row,i)->setText("---");
            }
            ui->TabItems->item(row,i)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ui->TabItems->item(row,i)->setTextAlignment(Qt::AlignCenter);
        }
    }
    for (int i=0; i<ui->TabOutput->columnCount(); i++) {
        for (int row=0; row<ui->TabOutput->rowCount(); row++) {
            ui->TabOutput->item(row,i)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ui->TabOutput->item(row,i)->setTextAlignment(Qt::AlignCenter);
        }
    }
    connect(ui->TabItems,SIGNAL(cellClicked(int,int)),this,SLOT(ClickItem(int,int)));
    connect(ui->TabOutput,SIGNAL(cellClicked(int,int)),this,SLOT(ClickOutput(int,int)));
    BoxDcr.append(ui->BoxDcrK1);
    BoxDcr.append(ui->BoxDcrK2);
    BoxDcr.append(ui->BoxDcrK3);
    BoxDcr.append(ui->BoxDcrK4);
    BoxDcr.append(ui->BoxDcrK5);
    BoxDcr.append(ui->BoxDcrK6);
    BoxDcr.append(ui->BoxDcrK7);
    BoxDcr.append(ui->BoxDcrK8);

    BoxInr.append(ui->BoxInrK0);
    BoxInr.append(ui->BoxInrB0);
    BoxInr.append(ui->BoxInrK1);
    BoxInr.append(ui->BoxInrB1);
    BoxInr.append(ui->BoxInrK2);
    BoxInr.append(ui->BoxInrB2);
    BoxInr.append(ui->BoxInrK3);
    BoxInr.append(ui->BoxInrB3);
    BoxInr.append(ui->BoxInrK4);
    BoxInr.append(ui->BoxInrB4);
    BoxInr.append(ui->BoxInrK5);
    BoxInr.append(ui->BoxInrB5);
    BoxInr.append(ui->BoxInrK6);
    BoxInr.append(ui->BoxInrB6);
    BoxInr.append(ui->BoxInrK7);
    BoxInr.append(ui->BoxInrB7);
    BoxInr.append(ui->BoxInrK8);
    BoxInr.append(ui->BoxInrB8);
    BoxInr.append(ui->BoxInrK9);
    BoxInr.append(ui->BoxInrB9);
    BoxInr.append(ui->BoxInrKA);
    BoxInr.append(ui->BoxInrBA);
}

void WinBack::InitButtons()
{
    QButtonGroup *BtnGroup = new QButtonGroup;
    BtnGroup->addButton(ui->BtnExit,Qt::Key_0);
    BtnGroup->addButton(ui->BtnVersion,Qt::Key_1);
    connect(BtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));

    QButtonGroup *btnGroupDcr = new QButtonGroup;
    btnGroupDcr->addButton(ui->BtnDcrParamRead,Qt::Key_1);
    btnGroupDcr->addButton(ui->BtnDcrParamSend,Qt::Key_2);
    btnGroupDcr->addButton(ui->BtnDcrParamClear,Qt::Key_3);
    connect(btnGroupDcr,SIGNAL(buttonClicked(int)),this,SLOT(JudgeDcrBtn(int)));

    QButtonGroup *btnGroupInr = new QButtonGroup;
    btnGroupInr->addButton(ui->BtnInrParamRead,Qt::Key_1);
    btnGroupInr->addButton(ui->BtnInrParamSend,Qt::Key_2);
    btnGroupInr->addButton(ui->BtnInrParamClear,Qt::Key_3);
    connect(btnGroupInr,SIGNAL(buttonClicked(int)),this,SLOT(JudgeInrBtn(int)));
}

void WinBack::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        emit SendCommand(ADDR,CMD_JUMP,NULL);
        break;
    case Qt::Key_1:
        SendCanCmdVersion(0x13);
        SendCanCmdVersion(0x14);
        SendCanCmdVersion(0x15);
        SendCanCmdVersion(0x16);
        SendCanCmdVersion(0x17);
        SendCanCmdVersion(0x22);
        SendCanCmdVersion(0x23);
        SendCanCmdVersion(0x24);
        SendCanCmdVersion(0x25);
        SendCanCmdVersion(0x26);
        SendCanCmdVersion(0x27);
        break;
    default:
        break;
    }
}

void WinBack::JudgeDcrBtn(int id)
{
    switch (id) {
    case Qt::Key_1:
        SendCanCmdParam(0x22);
        break;
    case Qt::Key_2:
        SendCanCmdParamDcr();
        break;
    case Qt::Key_3:
        ClearParamDcr();
        SendCanCmdParamDcr();
        break;
    default:
        break;
    }
}

void WinBack::JudgeInrBtn(int id)
{
    switch (id) {
    case Qt::Key_1:
        SendCanCmdParam(0x23);
        break;
    case Qt::Key_2:
        SendCanCmdParamInr();
        break;
    case Qt::Key_3:
        ClearParamInr();
        SendCanCmdParamInr();
        break;
    default:
        break;
    }
}

void WinBack::InitSettings()
{
    QSettings *ini = new QSettings(INI_PATH,QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("GLOBAL");
    QStringList temp = ini->value("ItemEnable","1 2 3 4 6").toString().split(" ");
    for (int i=0; i<temp.size(); i++) {
        int row = temp.at(i).toInt();
        if (row > 0)
            ui->TabItems->item(row-1,1)->setText("Y");
    }

    temp = ini->value("OutEnable","0 1").toString().split(" ");
    for (int i=0; i<temp.size(); i++) {
        ui->TabOutput->item(temp.at(i).toInt(),1)->setText("Y");
    }

    QString t = ini->value("Number","168912000X").toString();
    ui->EditNumber->setText(t);
    ui->OppositeDir->setChecked(ini->value("OppositeDir",false).toBool());
    ui->HideVoltage->setChecked(ini->value("HideVoltage",false).toBool());
    temp = ini->value("KDCR","16384").toString().split(" ");
    for (int i=0; i<qMin(temp.size(),BoxDcr.size()); i++)
        BoxDcr.at(i)->setValue(temp.at(i).toInt());
    temp = ini->value("KINR","1024").toString().split(" ");
    for (int i=0; i<qMin(temp.size(),BoxInr.size()); i++)
        BoxInr.at(i)->setValue(temp.at(i).toInt());
}

void WinBack::SaveSettings()
{
    QSettings *ini = new QSettings(INI_PATH,QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("GLOBAL");
    QStringList temp;
    temp.append("0");
    for (int i=0; i<ui->TabItems->rowCount(); i++) {
        if (ui->TabItems->item(i,1)->text() == "Y")
            temp.append(QString::number(i+1));
    }
    ini->setValue("ItemEnable",temp.join(" "));

    temp.clear();
    for (int i=0; i<ui->TabOutput->rowCount(); i++) {
        if (ui->TabOutput->item(i,1)->text() == "Y")
            temp.append(QString::number(i));
    }
    ini->setValue("OutEnable",temp.join(" "));
    ini->setValue("Number",ui->EditNumber->text());
    ini->setValue("OppositeDir",ui->OppositeDir->isChecked());
    ini->setValue("HideVoltage",ui->HideVoltage->isChecked());

    temp.clear();
    for (int i=0; i<BoxDcr.size(); i++)
        temp.append(QString::number(BoxDcr.at(i)->value()));
    ini->setValue("KDCR",temp.join(" "));

    temp.clear();
    for (int i=0; i<BoxInr.size(); i++)
        temp.append(QString::number(BoxInr.at(i)->value()));
    ini->setValue("KINR",temp.join(" "));
}

void WinBack::ClickItem(int r, int c)
{
    if (c != 1)
        return;
    if (ui->TabItems->item(r,c)->text() == "N")
        ui->TabItems->item(r,c)->setText("Y");
    else
        ui->TabItems->item(r,c)->setText("N");
}

void WinBack::ClickOutput(int r, int c)
{
    if (c != 1)
        return;
    if (ui->TabOutput->item(r,c)->text() == "N")
        ui->TabOutput->item(r,c)->setText("Y");
    else
        ui->TabOutput->item(r,c)->setText("N");
}

void WinBack::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    switch (addr) {
    case ADDR:
    case CAN_ID_13OUT:
    case CAN_ID_14OUT:
    case CAN_ID_15OUT:
    case CAN_ID_16OUT:
    case CAN_ID_17OUT:
    case CAN_ID_DCR:
    case CAN_ID_INR:
    case CAN_ID_IMP:
    case CAN_ID_IND:
    case CAN_ID_PWR:
        break;
    default:
        return;
        break;
    }
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(addr,msg);
        break;
    default:
        break;
    }
}

void WinBack::ExcuteCanCmd(quint16 addr, QByteArray msg)
{
    if (!Testing)
        return;
    switch (addr) {
    case CAN_ID_DCR:
        ReadCanCmdDcr(msg);
        break;
    case CAN_ID_INR:
        ReadCanCmdInr(msg);
        break;
    case CAN_ID_IMP:
        ReadCanCmdImp(msg);
        break;
    case CAN_ID_IND:
        ReadCanCmdInd(msg);
        break;
    case CAN_ID_PWR:
        ReadCanCmdPwr(msg);
        break;
    case CAN_ID_13OUT:
        ReadCanCmdOut13(msg);
        break;
    case CAN_ID_14OUT:
        ReadCanCmdOut14(msg);
        break;
    case CAN_ID_15OUT:
        break;
    case CAN_ID_16OUT:
        break;
    case CAN_ID_17OUT:
        break;
    default:
        break;
    }
}

void WinBack::SendCanCmdVersion(quint16 id)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(id)<<quint8(0x01)<<quint8(0x08);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void WinBack::SendCanCmdParam(quint16 id)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(id)<<quint8(0x02)<<quint8(0x06)<<quint8(0xEE);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void WinBack::SendCanCmdParamDcr()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int i=0; i<BoxDcr.size(); i++) {
        out <<quint16(0x22)<<quint8(0x04)<<quint8(0x06)<<quint8(i+1)
           <<quint8(int(BoxDcr.at(i)->value())/256)
          <<quint8(int(BoxDcr.at(i)->value())%256);
    }
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void WinBack::SendCanCmdParamInr()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int i=0; i<BoxInr.size()/2; i++) {
        out <<quint16(0x23)<<quint8(0x06)<<quint8(0x06)<<quint8(i)
           <<quint8(int(BoxInr.at(i*2+0)->value())/256)
          <<quint8(int(BoxInr.at(i*2+0)->value())%256)
         <<quint8(int(BoxInr.at(i*2+1)->value())/256)
        <<quint8(int(BoxInr.at(i*2+1)->value())%256);
    }
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void WinBack::SendCanCmdStartDcr(quint8 gear)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x22)<<quint8(0x06)<<quint8(0x03)<<quint8(0x00)
      <<quint8(0x01)<<quint8(0x02)
     <<quint8(gear)<<quint8(5);
    out<<quint16(0x22)<<quint8(0x06)<<quint8(0x01)<<quint8(0x01)<<quint8(0x00)
      <<quint8(0x13)<<quint8(0x00)<<quint8(0x01);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void WinBack::ReadCanCmdDcr(QByteArray msg)
{
    if (msg.size()==8 && quint8(msg.at(0))==0x08) {
        QString v;
        for (int i=1; i<msg.size(); i++)
            v.append(QString::number(msg.mid(i,1).toInt()));
        v.insert(1,".");
        ui->TabItems->item(0,2)->setText(v);
        ui->TabItems->item(1,2)->setText(v);
    }
    if (msg.size()==4 && quint8(msg.at(0))==0x06) {
        int c = quint8(msg.at(1));
        quint16 k = quint16(msg.at(2))*256+quint8(msg.at(3));
        if (c>0x00 && c<=0x08)
            BoxDcr.at(c-1)->setValue(k);
    }
    if (msg.size()==7 && (quint8)msg.at(0)==0x01) {
        quint8 grade = quint8(msg.at(2));
        double temp = (quint16)(msg.at(3)*256)+(quint8)msg.at(4);
        qDebug()<<temp;
        switch (grade) {
        case 1:
            break;
        case 2:
            break;
        default:
            break;
        }
    }
}

void WinBack::ReadCanCmdInr(QByteArray msg)
{
    emit SendCommand(ADDR,CMD_DEBUG,msg.toHex());
    if (msg.size()==0x08 && quint8(msg.at(0))==0x08) {
        QString v;
        for (int i=1; i<msg.size(); i++)
            v.append(QString::number(msg.mid(i,1).toInt()));
        v.insert(1,".");
        ui->TabItems->item(2,2)->setText(v);
        ui->TabItems->item(3,2)->setText(v);
        ui->TabItems->item(4,2)->setText(v);
    }
    if (msg.size()==6 && quint8(msg.at(0))==0x06) {
        int c = quint8(msg.at(1));
        quint16 k = quint16(msg.at(2))*256+quint8(msg.at(3));
        quint16 b = quint16(msg.at(4))*256+quint8(msg.at(5));
        if (c>=0x00 && c<=0x0A) {
            BoxInr.at(c*2+0)->setValue(k);
            BoxInr.at(c*2+1)->setValue(b);
        }
    }
}

void WinBack::ReadCanCmdImp(QByteArray msg)
{
    if (msg.size()==0x08) {
        QString v;
        for (int i=0; i<msg.size(); i++)
            v.append(QString::number(msg.mid(i,1).toInt()));
        ui->TabItems->item(5,2)->setText(v);
    }
}

void WinBack::ReadCanCmdInd(QByteArray msg)
{
    if (msg.size()==0x08) {
        QString v;
        for (int i=0; i<msg.size(); i++)
            v.append(QString::number(msg.mid(i,1).toInt()));
        ui->TabItems->item(6,2)->setText(v);
    }
}

void WinBack::ReadCanCmdPwr(QByteArray msg)
{
    if (msg.size()==0x08) {
        QString v;
        for (int i=0; i<msg.size(); i++)
            v.append(QString::number(msg.mid(i,1).toInt()));
        ui->TabItems->item(7,2)->setText(v);
        ui->TabItems->item(8,2)->setText(v);
        ui->TabItems->item(9,2)->setText(v);
    }
}

void WinBack::ReadCanCmdOut13(QByteArray msg)
{
    if (msg.size()==0x08) {
        QString v;
        for (int i=0; i<msg.size(); i++)
            v.append(QString::number(msg.mid(i,1).toInt()));
        ui->TabOutput->item(0,2)->setText(v);
    }
}

void WinBack::ReadCanCmdOut14(QByteArray msg)
{
    if (msg.size()==0x08) {
        QString v;
        for (int i=0; i<msg.size(); i++)
            v.append(QString::number(msg.mid(i,1).toInt()));
        ui->TabOutput->item(1,2)->setText(v);
    }
}

void WinBack::ClearParamDcr()
{
    for (int i=0; i<BoxDcr.size(); i++)
        BoxDcr.at(i)->setValue(0x4000);
}

void WinBack::ClearParamInr()
{
    for (int i=0; i<BoxInr.size()/2; i++) {
        BoxInr.at(i*2+0)->setValue(1024);
        BoxInr.at(i*2+1)->setValue(500);
    }
}

void WinBack::showEvent(QShowEvent *)
{
    InitSettings();
    Testing = true;
}

void WinBack::hideEvent(QHideEvent *)
{
    SaveSettings();
    Testing = false;
}
