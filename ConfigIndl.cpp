#include "ConfigIndl.h"
#include "ui_ConfigIndl.h"

ConfigIndl::ConfigIndl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigIndl)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
    Testing = false;
    isCheckOk = false;
}

ConfigIndl::~ConfigIndl()
{
    delete ui;
}
void ConfigIndl::WinInit()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabInductance->horizontalHeader()->setResizeMode(4,QHeaderView::Stretch);
    ui->TabInductance->horizontalHeader()->setResizeMode(5,QHeaderView::Stretch);
    ui->TabInductance->horizontalHeader()->setResizeMode(6,QHeaderView::Stretch);
    ui->TabInductance->horizontalHeader()->setResizeMode(7,QHeaderView::Stretch);
    ui->TabInductance->horizontalHeader()->setResizeMode(8,QHeaderView::Stretch);
    ui->TabInductance->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabInductance->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->TabInductance->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->TabInductance->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Stretch);
    ui->TabInductance->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Stretch);
    ui->TabInductance->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Stretch);
    ui->TabInductance->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(ui->TabInductance,SIGNAL(cellClicked(int,int)),this,SLOT(ItemClick(int,int)));
    input = new InputNum(this);
    QStringList t;
    t <<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12";
    input->BtnInit(t);
    connect(input,SIGNAL(ItemChange(QString)),this,SLOT(ItemChange(QString)));
    input->hide();
}

void ConfigIndl::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSDLRAuto,Qt::Key_0);
    btnGroup->addButton(ui->BtnSDLRExit,Qt::Key_2);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}

void ConfigIndl::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        DatAuto();
        break;
    case Qt::Key_1:
        break;
    case Qt::Key_2:
        emit TransformCmd(ADDR,WIN_CMD_SWITCH,NULL);
        break;
    default:
        break;
    }
}

void ConfigIndl::DatInit()
{
    QSettings *global = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");

    //当前使用的测试项目
    QString t = QString("./config/%1").arg(global->value("FileInUse","default.ini").toString());
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetDcr");

    QStringList temp = (set->value("Other","20 0 0.5 10 10 50").toString()).split(" ");
    if (temp.size() >= 6) {
        ui->BoxTime->setValue(temp.at(0).toDouble());
        ui->BoxUnbalance->setValue(temp.at(1).toDouble());
        ui->BoxMin->setValue(temp.at(2).toDouble());
        ui->BoxMax->setValue(temp.at(3).toDouble());
        ui->BoxFreq->setCurrentIndex(temp.at(4).toInt());
        ui->BoxMode->setCurrentIndex(temp.at(5).toInt());
    }
    //可用
    temp = (QString(set->value("Enable","Y Y Y Y Y Y Y Y").toByteArray())).split(" ");
    if (temp.size() > 8)
        ui->TabInductance->setRowCount(temp.size());
    else
        ui->TabInductance->setRowCount(8);
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (Enable.size() > row)
            continue;
        Enable.append(new QTableWidgetItem);
        ui->TabInductance->setItem(row,0,Enable.at(row));
        Enable.at(row)->setText(temp.at(row));
        Enable.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Enable.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (Enable.size() <= row)
            break;
        Enable.at(row)->setText(temp.at(row));
    }
    //端一
    temp = (set->value("Terminal1","1 2 3 4 5 6 7 8").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (Terminal1.size() > row)
            continue;
        Terminal1.append(new QTableWidgetItem);
        ui->TabInductance->setItem(row,1,Terminal1.at(row));
        Terminal1.at(row)->setText(temp.at(row));
        Terminal1.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal1.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (Terminal1.size() <= row)
            break;
        Terminal1.at(row)->setText(temp.at(row));
    }
    //端二
    temp = (set->value("Terminal2","2 3 4 5 6 7 8 1").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (Terminal2.size() > row)
            continue;
        Terminal2.append(new QTableWidgetItem);
        ui->TabInductance->setItem(row,2,Terminal2.at(row));
        Terminal2.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal2.at(row)->setTextAlignment(Qt::AlignCenter);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (Terminal2.size() <= row)
            break;
        Terminal2.at(row)->setText(temp.at(row));
    }
    //单位
    temp = (QString(set->value("Unit","1 1 1 1 1 1 1 1").toByteArray())).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (Unit.size() > row)
            continue;
        Unit.append(new QComboBox(this));
        ui->TabInductance->setCellWidget(row,3,Unit.at(row));
        QStringList t;
        t <<"uH"<<"mH";
        Unit.at(row)->setView(new QListView(this));
        Unit.at(row)->addItems(t);
        Unit.at(row)->setCurrentIndex(temp.at(row).toInt());
    }
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (Unit.size() <= row)
            break;
        Unit.at(row)->setCurrentIndex(temp.at(row).toInt());
    }
    //最小值
    temp = (set->value("Min","100 100 100 100 100 100 100 100").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (Min.size() > row)
            continue;
        Min.append(new QDoubleSpinBox(this));
        ui->TabInductance->setCellWidget(row,4,Min.at(row));
        Min.at(row)->setMaximum(2000);
        Min.at(row)->setAlignment(Qt::AlignHCenter);
        Min.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (Min.size() <= row)
            break;
        Min.at(row)->setValue(temp.at(row).toDouble());
    }
    //最大值
    temp = (set->value("Max","300 300 300 300 300 300 300 300").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (Max.size() > row)
            continue;
        Max.append(new QDoubleSpinBox(this));
        ui->TabInductance->setCellWidget(row,5,Max.at(row));
        Max.at(row)->setMaximum(2000);
        Max.at(row)->setAlignment(Qt::AlignHCenter);
        Max.at(row)->setValue(temp.at(row).toDouble());
        Max.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (Max.size() <= row)
            break;
        Max.at(row)->setValue(temp.at(row).toDouble());
    }
    //最小值
    temp = (set->value("QMin","0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (QMin.size() > row)
            continue;
        QMin.append(new QDoubleSpinBox(this));
        ui->TabInductance->setCellWidget(row,6,QMin.at(row));
        QMin.at(row)->setMaximum(100);
        QMin.at(row)->setAlignment(Qt::AlignHCenter);
        QMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (QMin.size() <= row)
            break;
        QMin.at(row)->setValue(temp.at(row).toDouble());
    }
    //最大值
    temp = (set->value("QMax","200.00 200.00 200.00 200.00 200.00 200.00 200.00 200.00").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (QMax.size() > row)
            continue;
        QMax.append(new QDoubleSpinBox(this));
        ui->TabInductance->setCellWidget(row,7,QMax.at(row));
        QMax.at(row)->setMaximum(100);
        QMax.at(row)->setAlignment(Qt::AlignHCenter);
        QMax.at(row)->setValue(temp.at(row).toDouble());
        QMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (QMax.size() <= row)
            break;
        QMax.at(row)->setValue(temp.at(row).toDouble());
    }
    //标准值
    temp = (set->value("Std","200 200 200 200 200 200 200 200").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),ui->TabInductance->rowCount()); row++) {
        if (Std.size() > row)
            continue;
        Std.append(new QDoubleSpinBox(this));
        ui->TabInductance->setCellWidget(row,8,Std.at(row));
        Std.at(row)->setMaximum(2000);
        Std.at(row)->setAlignment(Qt::AlignHCenter);
        Std.at(row)->setValue(temp.at(row).toDouble());
        Std.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
}

void ConfigIndl::DatSave()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxTime->value()));
    temp.append(QString::number(ui->BoxUnbalance->value()));
    temp.append(QString::number(ui->BoxMin->value()));
    temp.append(QString::number(ui->BoxMax->value()));
    temp.append(QString::number(ui->BoxFreq->currentIndex()));
    temp.append(QString::number(ui->BoxMode->currentIndex()));
    set->setValue("Other",(temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i<Enable.size(); i++)
        temp.append(Enable.at(i)->text());
    set->setValue("Enable",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Terminal1.size(); i++)
        temp.append(Terminal1.at(i)->text());
    set->setValue("Terminal1",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Terminal2.size(); i++)
        temp.append(Terminal2.at(i)->text());
    set->setValue("Terminal2",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Unit.size(); i++)
        temp.append(QString::number(Unit.at(i)->currentIndex()));
    set->setValue("Unit",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Min.size(); i++)
        temp.append(QString::number(Min.at(i)->value()));
    set->setValue("Min",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Max.size(); i++)
        temp.append(QString::number(Max.at(i)->value()));
    set->setValue("Max",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<QMin.size(); i++)
        temp.append(QString::number(QMin.at(i)->value()));
    set->setValue("QMin",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<QMax.size(); i++)
        temp.append(QString::number(QMax.at(i)->value()));
    set->setValue("QMax",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Std.size(); i++)
        temp.append(QString::number(Std.at(i)->value()));
    set->setValue("Std",(temp.join(" ").toUtf8()));
}

void ConfigIndl::DatAuto()
{
    for (int i=0; i<ui->TabInductance->rowCount(); i++) {
        double std = Std.at(i)->value();
        double min = std*(100-ui->BoxMin->value())/100;
        double max = std*(100+ui->BoxMax->value())/100;
        Min.at(i)->setValue(min);
        Max.at(i)->setValue(max);
    }
}
void ConfigIndl::DisplayInit()
{
    ListItem.clear();
    ListPara.clear();
    ListResult.clear();
    ListJudge.clear();
    for (int row = 0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            if (row >= Terminal1.size())
                break;
            if (row >= Terminal2.size())
                break;
            QString T1 = Terminal1.at(row)->text();
            QString T2 = Terminal2.at(row)->text();
            ListItem.append(QString(tr("电感%1-%2")).arg(T1).arg(T2));
            QString M1 = Min.at(row)->text();
            QString M2 = Max.at(row)->text();
            QString Q1 = QMin.at(row)->text();
            QString Q2 = QMax.at(row)->text();
            ListPara.append(QString("%1~%2,%3~%4").arg(M1).arg(M2).arg(Q1).arg(Q2));
            ListResult.append(" ");
            ListJudge.append(" ");
        }
    }
}
void ConfigIndl::ItemClick(int r, int c)
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
        input->show();
        break;
    default:
        break;
    }
}

void ConfigIndl::ItemChange(QString msg)
{
    int t = ui->TabInductance->currentColumn();
    if (t==1 || t==2)
        ui->TabInductance->currentItem()->setText(msg);
}

int ConfigIndl::Gear(int row)
{
    int t = 0;
    double s = Std.at(row)->text().toDouble();
    if (Unit.at(row)->currentText() == "mH") {
        switch (ui->BoxFreq->currentIndex()) {
        case 0:
            if (2*3.14*s*0.1/1000 < 1)
                t = 0x09;
            else if (2*3.14*s*0.1/1000 < 10)
                t = 0x29;
            else
                t = 0x39;
            break;
        case 1:
            if (2*3.14*s*0.12/1000 < 1)
                t = 0x0A;
            else if (2*3.14*s*0.12/1000 < 10)
                t = 0x2A;
            else
                t = 0x3A;
            break;
        case 2:
            if (2*3.14*s*1/1000 < 1)
                t = 0x0A;
            else if (2*3.14*s*1/1000 < 10)
                t = 0x2A;
            else
                t = 0x3A;
            break;
        case 3:
            if (2*3.14*s*01/1000 < 1)
                t = 0x0C;
            else if (2*3.14*s*10/1000 < 10)
                t = 0x2C;
            else
                t = 0x3C;
            break;
        default:
            break;
        }
    } else {
        switch (ui->BoxFreq->currentIndex()) {
        case 0:
            t = 0x09;
            break;
        case 1:
            t = 0x0A;
            break;
        case 2:
            t = 0x0B;
            break;
        case 3:
            t = 0x0C;
            break;
        default:
            break;
        }
    }
    return t;
}

int ConfigIndl::Mode(int row)
{
    int t = 0;
    int s = 0;
    if (ui->BoxMode->currentIndex() == 0)
        s = 0x00;
    else
        s = 0x40;

    if (Unit.at(row)->currentText()=="uH" && Max.at(row)->text().toInt()<500)
        t = s | 0x0F;
    else
        t = s | 0x09;
    return t;
}
void ConfigIndl::UpdateResult(QByteArray msg)
{
    for (int i=0; i<ListResult.size(); i++) {
        if (ListResult.at(i) == " ") {
            ListResult[i] = msg;
            break;
        }
    }
}

void ConfigIndl::UpdateJudge(QByteArray msg)
{
    for (int i=0; i<ListJudge.size(); i++) {
        if (ListJudge.at(i) == " ") {
            ListJudge[i] = msg;
            break;
        }
    }
}

void ConfigIndl::UpdateUnbalance()
{

}

void ConfigIndl::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

void ConfigIndl::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_INDL && addr != CAN_ID_INDL)
        return;
    switch (cmd) {
    case CAN_DAT_GET:
        ExcuteCmd(msg);
        break;
    case CAN_CMD_CHECK:
        CmdCheckState();
        break;
    case CAN_CMD_START:
        CmdStartTest(msg.toInt());
        break;
    case CAN_CMD_STOP:
        CmdStopTest();
        break;
    case CAN_CMD_INIT:
        ShowInit();
        CmdConfigure();
        break;
    default:
        break;
    }
}

void ConfigIndl::ShowInit()
{
    Items.clear();
    for (int row = 0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            QStringList s;
            QString T1 = Terminal1.at(qMin(row,Terminal1.size()))->text();
            QString T2 = Terminal2.at(qMin(row,Terminal2.size()))->text();
            QString M1 = Min.at(qMin(row,Min.size()))->text();
            QString M2 = Max.at(qMin(row,Max.size()))->text();
            QString Q1 = QMin.at(qMin(row,QMin.size()))->text();
            QString Q2 = QMax.at(qMin(row,QMax.size()))->text();
            s.append(QString(tr("电感%1-%2")).arg(T1).arg(T2));
            s.append(QString("%1~%2,%3~%4").arg(M1).arg(M2).arg(Q1).arg(Q2));
            s.append(" ");
            s.append(" ");
            Items.append(s.join("@"));
        }
    }
    if (ui->BoxUnbalance->value() != 0 && ListItem.size()>=3) {
        QStringList s;
        s.append("电感平衡");
        s.append(QString("%1%").arg(ui->BoxUnbalance->value()));
        s.append(" ");
        s.append(" ");
        Items.append(s.join("@"));
    }
    emit TransformCmd(ADDR,WIN_CMD_SHOW,Items.join("\n").toUtf8());
}
void ConfigIndl::ExcuteCmd(QByteArray msg)
{
    if (!Testing)
        return;
    TimeOut = 0;
    if (msg.size()==8 && (quint8)msg.at(0)==0x00) {
        UpdateState(msg);
    }
    if (msg.size()==8 && (quint8)msg.at(0)==0x01) {
        UpdateTestData(msg);
    }
}
void ConfigIndl::UpdateState(QByteArray )
{
    if (!isCheckOk) {
        isCheckOk = true;
        emit TransformCmd(ADDR,WIN_CMD_DEBUG,"INDL check ok\n");
    }
    if (Testing)
        Testing = false;
}

void ConfigIndl::UpdateTestData(QByteArray msg)
{
    quint8 number = quint8(msg.at(1));
    if (quint8(msg.at(3) == 0x00)) {
        Result1.dat[0] = msg.at(4);
        Result1.dat[1] = msg.at(5);
        Result1.dat[2] = msg.at(6);
        Result1.dat[3] = msg.at(7);
    }
    if (quint8(msg.at(3) == 0x01)) {
        Result2.dat[0] = msg.at(4);
        Result2.dat[1] = msg.at(5);
        Result2.dat[2] = msg.at(6);
        Result2.dat[3] = msg.at(7);
        QString t = "---";
        if (Result1.Result <= 1000)
            t = QString::number(Result1.Result,'f',1) + "uH";
        else if (Result1.Result <= 10000)
            t = QString::number(Result1.Result/1000,'f',3) + "mH";
        else if (Result1.Result <= 100000)
            t = QString::number(Result1.Result/1000,'f',2) + "mH";
        else if (Result1.Result <= 1000000)
            t = QString::number(Result1.Result/1000,'f',1) + "mH";
        else if (Result1.Result <= 10000000)
            t = QString::number(Result1.Result/1000,'f',0) + "mH";

        t +="," + QString::number(Result2.Result,'f',2);
        UpdateResult(t.toUtf8());
        double max = Max.at(number)->value();
        double min = Min.at(number)->value();
        double qmax = QMax.at(number)->value();
        double qmin = QMin.at(number)->value();
        if (Unit.at(number)->currentText() == "mH") {
            max *= 1000;
            min *= 1000;
        }
        if (Result1.Result>=min && Result1.Result<= max && Result2.Result>=qmin && Result2.Result<=qmax)
            UpdateJudge("OK");
        else
            UpdateJudge("NG");
        emit TransformCmd(ADDR,WIN_CMD_RESULT,NULL);
    }

}

void ConfigIndl::CmdCheckState()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x26)<<quint8(0x01)<<quint8(0x00);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Testing = true;
}

void ConfigIndl::CmdStartTest(quint8 pos)
{
    WaitTestOver();
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001<<row;
    }
    out<<quint16(0x26)<<quint8(0x06)<<quint8(0x01)<<quint8(0x00)<<quint8(0x00)
      <<quint8(pos)<<quint8(tt/256)<<quint8(tt%256);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Testing = true;
}

void ConfigIndl::CmdStopTest()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x26)<<quint8(0x01)<<quint8(0x02);
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
    Testing = false;
}

void ConfigIndl::CmdConfigure()
{
    WaitTestOver();
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int i=0; i<Enable.size(); i++) {
        if (Enable.at(i)->text() == "Y") {
            out<<quint16(0x26)<<quint8(0x07)<<quint8(0x03)<<quint8(i)
              <<quint8(Terminal1.at(i)->text().toInt())
             <<quint8(Terminal2.at(i)->text().toInt())
            <<quint8(ui->BoxTime->value())
            <<quint8(Gear(i))<<quint8(Mode(i));
        }
    }
    emit TransformCmd(ADDR,CAN_DAT_PUT,msg);
}

bool ConfigIndl::WaitTestOver()
{
    TimeOut = 0;
    while (Testing) {
        Delay(10);
        TimeOut++;
        if (TimeOut > 100) {
            for (int i=0; i<ListResult.size(); i++) {
                if (ListResult.at(i) == " ") {
                    ListResult[i] = "---";
                }
            }
            for (int i=0; i<ListJudge.size(); i++) {
                if (ListJudge.at(i) == " ") {
                    ListJudge[i] = "NG";
                }
            }
            Testing = false;
            qDebug()<<"error";
            emit TransformCmd(ADDR,WIN_CMD_RESULT,NULL);
            return false;
        }
    }
    return true;
}
void ConfigIndl::showEvent(QShowEvent *)
{
    DatInit();
}

void ConfigIndl::hideEvent(QHideEvent *)
{
    DatSave();
}





