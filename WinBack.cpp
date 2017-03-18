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
}

void WinBack::InitButtons()
{
    QButtonGroup *BtnGroup = new QButtonGroup;
    BtnGroup->addButton(ui->BtnExit,Qt::Key_0);
    BtnGroup->addButton(ui->BtnVersion,Qt::Key_1);
    BtnGroup->addButton(ui->BtnDcrParam,Qt::Key_2);
    connect(BtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
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
        SendCanCmdVersion(0x22);
        SendCanCmdVersion(0x23);
        SendCanCmdVersion(0x24);
        SendCanCmdVersion(0x25);
        SendCanCmdVersion(0x26);
        SendCanCmdVersion(0x27);
        break;
    case Qt::Key_2:
        SendCanCmdParam(0x22);
        break;
    default:
        break;
    }
}

void WinBack::InitSettings()
{
    QSettings *g_ini = new QSettings(INI_PATH,QSettings::IniFormat);
    g_ini->setIniCodec("GB18030");
    g_ini->beginGroup("GLOBAL");
    QStringList temp = g_ini->value("ItemEnable","1 2 3 4 6").toString().split(" ");
    for (int i=0; i<temp.size(); i++) {
        int row = temp.at(i).toInt();
        if (row > 0)
            ui->TabItems->item(row-1,1)->setText("Y");
    }

    temp = g_ini->value("OutEnable","0 1").toString().split(" ");
    for (int i=0; i<temp.size(); i++) {
        ui->TabOutput->item(temp.at(i).toInt(),1)->setText("Y");
    }

    QString t = g_ini->value("Number","168912000X").toString();
    ui->EditNumber->setText(t);
    ui->OppositeDir->setChecked(g_ini->value("OppositeDir",false).toBool());
    ui->HideVoltage->setChecked(g_ini->value("HideVoltage",false).toBool());
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
        quint8 c = quint8(msg.at(1));
        quint16 k = quint16(msg.at(2))*256+quint8(msg.at(3));
        switch (c) {
        case 1:
            ui->EditDcrK1->setText(QString::number(k));
            break;
        case 2:
            ui->EditDcrK2->setText(QString::number(k));
            break;
        case 3:
            ui->EditDcrK3->setText(QString::number(k));
            break;
        case 4:
            ui->EditDcrK4->setText(QString::number(k));
            break;
        case 5:
            ui->EditDcrK5->setText(QString::number(k));
            break;
        case 6:
            ui->EditDcrK6->setText(QString::number(k));
            break;
        case 7:
            ui->EditDcrK7->setText(QString::number(k));
            break;
        case 8:
            ui->EditDcrK8->setText(QString::number(k));
            break;
        }
    }
}

void WinBack::ReadCanCmdInr(QByteArray msg)
{
    emit SendCommand(ADDR,CMD_DEBUG,msg.toHex());
    if (msg.size()==0x08) {
        QString v;
        for (int i=0; i<msg.size(); i++)
            v.append(QString::number(msg.mid(i,1).toInt()));
        ui->TabItems->item(2,2)->setText(v);
        ui->TabItems->item(3,2)->setText(v);
        ui->TabItems->item(4,2)->setText(v);
    }
    if (msg.size()==6 && quint8(msg.at(0))==0x06) {
        quint8 c = quint8(msg.at(1));
        quint16 k = quint16(msg.at(2))*256+quint8(msg.at(3));
        quint16 b = quint16(msg.at(4))*256+quint8(msg.at(5));
        if (c<6) {
            ui->TabItems->item(2,c*2+3)->setText(QString::number(k));
            ui->TabItems->item(2,c*2+4)->setText(QString::number(b));
        } else {
            ui->TabItems->item(3,(c-6)*2+3)->setText(QString::number(k));
            ui->TabItems->item(3,(c-6)*2+4)->setText(QString::number(b));
            ui->TabItems->item(4,(c-6)*2+3)->setText(QString::number(k));
            ui->TabItems->item(4,(c-6)*2+4)->setText(QString::number(b));
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
