#include "PagePwr.h"
#include "ui_PagePwr.h"

PagePwr::PagePwr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PagePwr)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
    Mode = PWR_FREE;
}

PagePwr::~PagePwr()
{
    delete ui;
}

void PagePwr::InitWindows()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabParams->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabParams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabParams->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(ui->TabParams,SIGNAL(cellClicked(int,int)),this,SLOT(ItemClick(int,int)));
    ui->TabParams->setRowCount(PWR_ROW);
    for (int row=0; row<PWR_ROW; row++) {
        Enable.append(new QTableWidgetItem);
        ui->TabParams->setItem(row,0,Enable.at(row));
        Enable.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Enable.at(row)->setTextAlignment(Qt::AlignCenter);

        CurrMin.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,1,CurrMin.at(row));
        CurrMin.at(row)->setMaximum(20);
        CurrMin.at(row)->setAlignment(Qt::AlignHCenter);
        CurrMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        CurrMax.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,2,CurrMax.at(row));
        CurrMax.at(row)->setMaximum(20);
        CurrMax.at(row)->setAlignment(Qt::AlignHCenter);
        CurrMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        PowerMin.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,3,PowerMin.at(row));
        PowerMin.at(row)->setMaximum(9999);
        PowerMin.at(row)->setAlignment(Qt::AlignHCenter);
        PowerMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        PowerMax.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,4,PowerMax.at(row));
        PowerMax.at(row)->setMaximum(9999);
        PowerMax.at(row)->setAlignment(Qt::AlignHCenter);
        PowerMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        CapMin.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,5,CapMin.at(row));
        CapMin.at(row)->setMaximum(9999);
        CapMin.at(row)->setAlignment(Qt::AlignHCenter);
        CapMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        CapMax.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,6,CapMax.at(row));
        CapMax.at(row)->setMaximum(9999);
        CapMax.at(row)->setAlignment(Qt::AlignHCenter);
        CapMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        TestDir.append(new QComboBox(this));
        ui->TabParams->setCellWidget(row,7,TestDir.at(row));
        QStringList t1;
        t1 <<tr("不转")<<tr("正转")<<tr("反转");
        TestDir.at(row)->setView(new QListView(this));
        TestDir.at(row)->addItems(t1);

        TestTime.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,8,TestTime.at(row));
        TestTime.at(row)->setMaximum(9999);
        TestTime.at(row)->setAlignment(Qt::AlignHCenter);
        TestTime.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Grade.append(new QTableWidgetItem);
        ui->TabParams->setItem(row,9,Grade.at(row));
        Grade.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Grade.at(row)->setTextAlignment(Qt::AlignCenter);
    }
}

void PagePwr::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExit,Qt::Key_0);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(ReadButtons(int)));
}

void PagePwr::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        emit SendCommand(ADDR,CMD_JUMP,NULL);
        break;
    default:
        break;
    }
}

void PagePwr::InitSettings()
{
    QSettings *global = new QSettings(INI_PATH,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    PowerSupply = global->value("PowerSupply","0").toInt();
    FileInUse = global->value("FileInUse",INI_DEFAULT).toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(FileInUse);
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("PagePwr");

    QStringList temp = (set->value("Other","220 50 1").toString()).split(" ");
    if (temp.size() >= 3) {
        ui->BoxVolt->setValue(temp.at(0).toDouble());
        ui->BoxFreq->setValue(temp.at(1).toDouble());
        ui->BoxCap->setValue(temp.at(2).toDouble());
    }
    //可用
    temp = (QString(set->value("Enable","Y Y Y N N N").toByteArray())).split(" ");
    for (int row=0; row<qMin(temp.size(),PWR_ROW); row++)
        Enable.at(row)->setText(temp.at(row));
    //最小电流
    temp = (set->value("CurrMin","0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),PWR_ROW); row++)
        CurrMin.at(row)->setValue(temp.at(row).toDouble());
    //最大电流
    temp = (set->value("CurrMax","5 5 5 5 5 5").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),PWR_ROW); row++)
        CurrMax.at(row)->setValue(temp.at(row).toDouble());
    //最小功率
    temp = (set->value("PowerMin","0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),PWR_ROW); row++)
        PowerMin.at(row)->setValue(temp.at(row).toDouble());
    //最大功率
    temp = (set->value("PowerMax","500 500 500 500 500 500").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),PWR_ROW); row++)
        PowerMax.at(row)->setValue(temp.at(row).toDouble());
    //最小容压
    temp = (set->value("CapMin","0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),PWR_ROW); row++)
        CapMin.at(row)->setValue(temp.at(row).toDouble());
    //最大容压
    temp = (set->value("CapMax","500 500 500 500 500 500").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),PWR_ROW); row++)
        CapMax.at(row)->setValue(temp.at(row).toDouble());
    //转向
    temp = (QString(set->value("TestDir","0 0 0 0 0 0 0 0").toByteArray())).split(" ");
    for (int row=0; row<qMin(temp.size(),PWR_ROW); row++)
        TestDir.at(row)->setCurrentIndex(temp.at(row).toInt());
    //测试时间
    temp = (set->value("TestTime","1 1 1 1 1 1").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),PWR_ROW); row++)
        TestTime.at(row)->setValue(temp.at(row).toDouble());
    temp.clear();
    temp<<"123"<<"124"<<"125"<<"126"<<"127"<<"128";
    for (int row=0; row<qMin(temp.size(),PWR_ROW); row++)
        Grade.at(row)->setText(temp.at(row));
}

void PagePwr::SaveSettings()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxVolt->value()));
    temp.append(QString::number(ui->BoxFreq->value()));
    temp.append(QString::number(ui->BoxCap->value()));
    set->setValue("Other",(temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i<Enable.size(); i++)
        temp.append(Enable.at(i)->text());
    set->setValue("Enable",(temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i<CurrMax.size(); i++)
        temp.append(QString::number(CurrMax.at(i)->value()));
    set->setValue("CurrMax",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<CurrMin.size(); i++)
        temp.append(QString::number(CurrMin.at(i)->value()));
    set->setValue("CurrMin",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<PowerMax.size(); i++)
        temp.append(QString::number(PowerMax.at(i)->value()));
    set->setValue("PowerMax",(temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i<PowerMin.size(); i++)
        temp.append(QString::number(PowerMin.at(i)->value()));
    set->setValue("PowerMin",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<CapMax.size(); i++)
        temp.append(QString::number(CapMax.at(i)->value()));
    set->setValue("CapMax",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<CapMin.size(); i++)
        temp.append(QString::number(CapMin.at(i)->value()));
    set->setValue("CapMin",(temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i<TestDir.size(); i++)
        temp.append(QString::number(TestDir.at(i)->currentIndex()));
    set->setValue("TestDir",(temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i<TestTime.size(); i++)
        temp.append(QString::number(TestTime.at(i)->value()));
    set->setValue("TestTime",(temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i<Grade.size(); i++)
        temp.append(Grade.at(i)->text());
    set->setValue("Grade",(temp.join(" ").toUtf8()));

}

void PagePwr::ItemClick(int r, int c)
{
    switch (c) {
    case 0:
        if (Enable.at(r)->text() != "Y")
            Enable.at(r)->setText("Y");
        else
            Enable.at(r)->setText("N");
        break;
    case 1:
    case 2:
    default:
        break;
    }
}

void PagePwr::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_PWR && addr != CAN_ID_PWR)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(msg);
        break;
    case CMD_CHECK:
        Mode = PWR_INIT;
        SendCanCmdStatus();
        if (!WaitTimeOut(100)) {
            QMessageBox::warning(this,tr("警告"),tr("功率板异常"),QMessageBox::Ok);
            emit SendCommand(ADDR,CMD_DEBUG,"Check PagePwr Error:Time out\n");
        }
        Mode = PWR_FREE;
        break;
    case CMD_START:
        for (int row = 0; row<Enable.size(); row++) {
            if (Enable.at(row)->text() == "Y") {
                Mode = PWR_TEST;
                TestRow = row;
                Judge = "OK";
                SendCanCmdStart();
                if(!WaitTimeOut(100)) {
                    Judge = "NG";
                    SendTestItemsAllError();
                    break;
                }
                Mode = PWR_FREE;
            }
        }
        Mode = PWR_FREE;
        SendTestJudge();
        break;
    case CMD_STOP:
        SendCanCmdStop();
        Mode = PWR_FREE;
        break;
    case CMD_INIT:
        InitSettings();
        InitTestItems();
        SendTestItems();
        break;
    default:
        break;
    }
}

void PagePwr::ExcuteCanCmd(QByteArray msg)
{
    if (Mode == PWR_FREE)
        return;
    TimeOut = 0;
    if (msg.size() == 4 && (quint8)msg.at(0) == 0x00)
        ReadCanCmdStatus(msg);
    if (msg.size() == 8 && (quint8)msg.at(0) == 0x01)
        ReadCanCmdResult(msg);
    if (msg.size() == 8 && (quint8)msg.at(0) == 0x02)
        ReadCanCmdDir(msg);
}

void PagePwr::InitTestItems()
{
    Items.clear();
    for (int row = 0; row<Enable.size(); row++) {
        QStringList s;
        QString G1 = Grade.at(qMin(row,Grade.size()))->text();
        double I1 = CurrMin.at(qMin(row,CurrMin.size()))->value();
        double I2 = CurrMax.at(qMin(row,CurrMax.size()))->value();
        double P1 = PowerMin.at(qMin(row,PowerMin.size()))->value();
        double P2 = PowerMax.at(qMin(row,PowerMax.size()))->value();
        double C1 = CapMin.at(qMin(row,CapMin.size()))->value();
        double C2 = CapMax.at(qMin(row,CapMax.size()))->value();
        s.append(QString(tr("功率%1")).arg(G1));
        s.append(QString("%1~%2A,%3~%4W,%5~%6V").arg(I1).arg(I2).arg(P1).arg(P2).arg(C1).arg(C2));
        s.append(" ");
        s.append(" ");
        Items.append(s.join("@"));
    }

    if (TestDir.at(0)->currentIndex() != 0) {
        QString s = QString(tr("转向@%1@ @ ").arg(TestDir.at(0)->currentText()));
        Items.append(s);
    }
}

void PagePwr::SendTestItems()
{
    QStringList n;
    for (int row = 0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            n.append(Items.at(row));
        }
    }
    if (TestDir.at(0)->currentIndex() != 0) {
        QString s = QString(tr("转向@%1@ @ ").arg(TestDir.at(0)->currentText()));
        n.append(s);
    }
    emit SendCommand(ADDR,CMD_INIT_ITEM,n.join("\n").toUtf8());
}

void PagePwr::SendTestItemsAllError()
{
    for (int row = 0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            QStringList s = QString(Items.at(row)).split("@");
            if (s.at(2) == " ")
                s[2] = "---";
            if (s.at(3) == " ")
                s[3] = "NG";
            emit SendCommand(ADDR,CMD_ITEM,s.join("@").toUtf8());
        }
    }
    if (TestDir.at(0)->currentIndex() != 0) {
        QString s = QString(tr("转向@%1@---@NG").arg(TestDir.at(0)->currentText()));
        emit SendCommand(ADDR,CMD_ITEM,s.toUtf8());
    }
}

void PagePwr::SendCanCmdStatus()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x27)<<quint8(0x01)<<quint8(0x00);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void PagePwr::SendCanCmdStart()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 v = ui->BoxVolt->value();
    quint16 t = TestTime.at(TestRow)->value()*10;
    quint8 p = PowerSupply<<4;
    if (ui->BoxFreq->value() == 60)
        p += 0x02;
    out<<quint16(0x27)<<quint8(0x08)<<quint8(0x01)<<quint8(TestRow+1)
      <<quint8(t/256)<<quint8(t%256)<<quint8(p+v/256)<<quint8(v%256)
     <<quint8(0x00)<<quint8(0x00);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void PagePwr::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x27)<<quint8(0x01)<<quint8(0x02);
    emit SendCommand(ADDR,CMD_CAN,msg);
}

void PagePwr::SendTestJudge()
{
    QString s = QString(tr("功率@%1@%2")).arg(FileInUse).arg(Judge);
    emit SendCommand(ADDR,CMD_JUDGE,s.toUtf8());
}

void PagePwr::SendItemJudge()
{
    QString vvv = QString::number(Volt.last()/10,'f',1);
    QString rrr = QString::number(Curr.last()/1000,'f',3);
    QString ppp = QString::number(Power.last()/10,'f',1);
    QString t = QString("%1A,%2W,%3V").arg(rrr).arg(ppp).arg(vvv);

    QStringList s = QString(Items.at(TestRow)).split("@");
    if (s.at(2) == " ")
        s[2] = t;
    if (s.at(3) == " ")
        s[3] = Judge;
    emit SendCommand(ADDR,CMD_ITEM,s.join("@").toUtf8());
    if (TestDir.at(0)->currentIndex() != 0) {
        QString n = TestDir.at(0)->currentText();
        QString a = (dir==n)?"OK":"NG";
        QString s = QString(tr("转向@%1@%2@%3").arg(n).arg(dir).arg(a));
        emit SendCommand(ADDR,CMD_ITEM,s.toUtf8());
    }
}

void PagePwr::ReadCanCmdStatus(QByteArray msg)
{
    if (quint8(msg.at(1)) != 0x00)
        return;

    if (Mode == PWR_INIT)
        emit SendCommand(ADDR,CMD_DEBUG,"Power check ok\n");
    if (Mode == PWR_TEST) {
        SendItemJudge();
        ClearResults();
    }
    Mode = PWR_FREE;
}

void PagePwr::ReadCanCmdResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double c = quint16(msg.at(3)*256)+quint8(msg.at(4));
    double p = quint16(msg.at(5)*256)+quint8(msg.at(6));
    Volt.append(v);
    Curr.append(c);
    Power.append(p);
    CalculateResult();
    if (Judge == "NG") {
        SendCanCmdStop();
        SendItemJudge();
        ClearResults();
        Mode = PWR_FREE;
    }
}

void PagePwr::ReadCanCmdDir(QByteArray msg)
{
    if (quint8(msg.at(5)) == 0x00)
        dir = tr("不转");
    if (quint8(msg.at(5)) == 0x01)
        dir = tr("正转");
    if (quint8(msg.at(5)) == 0x02)
        dir = tr("反转");
}

void PagePwr::CalculateResult()
{
    if (Volt.size()<5 || Curr.size()<5 || Power.size()<5)
        return;
    double vv = Volt.last()/10;
    double rr = Curr.last()/1000;
    double pp = Power.last()/10;

    if (rr>CurrMax.at(TestRow)->value() ||
            rr<CurrMin.at(TestRow)->value() ||
            pp>PowerMax.at(TestRow)->value() ||
            pp<PowerMin.at(TestRow)->value() ||
            vv>CapMax.at(TestRow)->value() ||
            vv<CapMin.at(TestRow)->value()) {
        Judge = "NG";
    }
}

void PagePwr::ClearResults()
{
    Volt.clear();
    Curr.clear();
    Power.clear();
}

bool PagePwr::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (Mode != PWR_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PagePwr::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

void PagePwr::showEvent(QShowEvent *)
{
    InitSettings();
}

void PagePwr::hideEvent(QHideEvent *)
{
    SaveSettings();
}
/*********************************END OF FILE**********************************/
