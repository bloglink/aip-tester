#include "PageInd.h"
#include "ui_PageInd.h"

PageInd::PageInd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageInd)
{
    ui->setupUi(this);
    InitializesWindow();
    InitializesButton();
    InitializesSetting();
    Testing = false;
    isCheckOk = false;
    Offsetting = false;
}

PageInd::~PageInd()
{
    delete ui;
}

void PageInd::InitializesWindow()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabParams->horizontalHeader()->setResizeMode(4,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(5,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(6,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(7,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(8,QHeaderView::Stretch);
    ui->TabParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabParam->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->TabParam->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->TabParam->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Stretch);
    ui->TabParam->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Stretch);
    ui->TabParam->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Stretch);
    ui->TabParam->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(ui->TabParams,SIGNAL(cellClicked(int,int)),this,SLOT(ItemClick(int,int)));
    input = new PageNum(this);
    QStringList t;
    t <<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12";
    input->BtnInit(t);
    connect(input,SIGNAL(ItemChange(QString)),this,SLOT(ItemChange(QString)));
    input->hide();

    ui->TabParams->setRowCount(MAX_ROW);
    for (int row=0; row<MAX_ROW; row++) {
        Enable.append(new QTableWidgetItem);
        ui->TabParams->setItem(row,0,Enable.at(row));
        Enable.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Enable.at(row)->setTextAlignment(Qt::AlignCenter);

        Terminal1.append(new QTableWidgetItem);
        ui->TabParams->setItem(row,1,Terminal1.at(row));
        Terminal1.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal1.at(row)->setTextAlignment(Qt::AlignCenter);

        Terminal2.append(new QTableWidgetItem);
        ui->TabParams->setItem(row,2,Terminal2.at(row));
        Terminal2.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal2.at(row)->setTextAlignment(Qt::AlignCenter);

        Unit.append(new QComboBox(this));
        ui->TabParams->setCellWidget(row,3,Unit.at(row));
        QStringList t2;
        t2 <<"uH"<<"mH";
        Unit.at(row)->setView(new QListView(this));
        Unit.at(row)->addItems(t2);

        Min.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,4,Min.at(row));
        Min.at(row)->setMaximum(2000);
        Min.at(row)->setAlignment(Qt::AlignHCenter);
        Min.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Max.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,5,Max.at(row));
        Max.at(row)->setMaximum(2000);
        Max.at(row)->setAlignment(Qt::AlignHCenter);
        Max.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        QMin.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,6,QMin.at(row));
        QMin.at(row)->setMaximum(100);
        QMin.at(row)->setAlignment(Qt::AlignHCenter);
        QMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        QMax.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,7,QMax.at(row));
        QMax.at(row)->setMaximum(100);
        QMax.at(row)->setAlignment(Qt::AlignHCenter);
        QMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Std.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,8,Std.at(row));
        Std.at(row)->setMaximum(2000);
        Std.at(row)->setAlignment(Qt::AlignHCenter);
        Std.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Offset.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,9,Offset.at(row));
        Offset.at(row)->setMaximum(100);
        Offset.at(row)->setAlignment(Qt::AlignHCenter);
        Offset.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
}

void PageInd::InitializesButton()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSDLRAuto,Qt::Key_0);
    btnGroup->addButton(ui->BtnOffset,Qt::Key_1);
    btnGroup->addButton(ui->BtnSDLRExit,Qt::Key_2);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}

void PageInd::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        CalculateAuto();
        break;
    case Qt::Key_1:
        SendConfigCmd();
        Offsetting = true;
        SendStartCmd(WIN_ID_OUT13);
        if (!WaitOffset(100))
            qDebug()<<"Offset time out";
        break;
    case Qt::Key_2:
        emit SendMessage(ADDR,CMD_JUMP,NULL);
        break;
    default:
        break;
    }
}

void PageInd::InitializesSetting()
{
    qDebug()<<QTime::currentTime().toString()<<"电感数据";
    QSettings *global = new QSettings(INI_PATH,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    FileInUse = global->value("FileInUse",INI_DEFAULT).toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(FileInUse);
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("PageInd");

    QStringList temp = (set->value("Other","1 0 20 20 0 0").toString()).split(" ");
    if (temp.size() >= 6) {
        ui->BoxTime->setValue(temp.at(0).toDouble());
        ui->BoxUnbalance->setValue(temp.at(1).toDouble());
        ui->BoxMin->setValue(temp.at(2).toDouble());
        ui->BoxMax->setValue(temp.at(3).toDouble());
        ui->BoxFreq->setCurrentIndex(temp.at(4).toInt());
        ui->BoxMode->setCurrentIndex(temp.at(5).toInt());
    }
    //可用
    temp = (QString(set->value("Enable","Y Y Y N N N N N").toByteArray())).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Enable.at(row)->setText(temp.at(row));
    //端一
    temp = (set->value("Terminal1","1 2 1 4 5 6 7 8").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Terminal1.at(row)->setText(temp.at(row));
    //端二
    temp = (set->value("Terminal2","2 3 3 5 6 7 8 1").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Terminal2.at(row)->setText(temp.at(row));
    //单位
    temp = (QString(set->value("Unit","1 1 1 1 1 1 1 1").toByteArray())).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Unit.at(row)->setCurrentIndex(temp.at(row).toInt());
    //最小值
    temp = (set->value("Min","0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Min.at(row)->setValue(temp.at(row).toDouble());
    //最大值
    temp = (set->value("Max","200 200 200 200 200 200 200 200").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Max.at(row)->setValue(temp.at(row).toDouble());
    //最小值
    temp = (set->value("QMin","0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        QMin.at(row)->setValue(temp.at(row).toDouble());
    //最大值
    temp = (set->value("QMax","200 200 200 200 200 200 200 200").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        QMax.at(row)->setValue(temp.at(row).toDouble());
    //标准值
    temp = (set->value("Std","100 100 100 100 100 100 100 100").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Std.at(row)->setValue(temp.at(row).toDouble());
    //补偿
    temp = (set->value("Offset","0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Offset.at(row)->setValue(temp.at(row).toDouble());
    qDebug()<<QTime::currentTime().toString()<<"电感数据OK";
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      数据保存
*******************************************************************************/
void PageInd::SaveSetting()
{
    qDebug()<<QTime::currentTime().toString()<<"电感保存";
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
    temp.clear();
    for (int i=0; i<Offset.size(); i++)
        temp.append(Offset.at(i)->text());
    set->setValue("Offset",(temp.join(" ").toUtf8()));
    qDebug()<<QTime::currentTime().toString()<<"电感保存OK";
}

void PageInd::CalculateAuto()
{
    for (int i=0; i<ui->TabParams->rowCount(); i++) {
        double std = Std.at(i)->value();
        double min = std*(100-ui->BoxMin->value())/100;
        double max = std*(100+ui->BoxMax->value())/100;
        Min.at(i)->setValue(min);
        Max.at(i)->setValue(max);
    }
}

void PageInd::ItemClick(int r, int c)
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

void PageInd::ItemChange(QString msg)
{
    int t = ui->TabParams->currentColumn();
    if (t==1 || t==2)
        ui->TabParams->currentItem()->setText(msg);
}

void PageInd::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_IND && addr != CAN_ID_IND)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(msg);
        break;
    case CMD_CHECK:
        SendStatusCmd();
        break;
    case CMD_START:
        SendStartCmd(msg.toInt());
        break;
    case CMD_STOP:
        SendStopCmd();
        break;
    case CMD_INIT:
        InitializesSetting();
        InitializesItem();
        SendConfigCmd();
        break;
    default:
        break;
    }
}

void PageInd::ExcuteCanCmd(QByteArray msg)
{
    if (!Testing && !Offsetting)
        return;
    TimeOut = 0;
    if (msg.size()==8 && (quint8)msg.at(0)==0x00) {
        ReadStatus(msg);
    }
    if (msg.size()==8 && (quint8)msg.at(0)==0x01) {
        if (Offsetting)
            ReadOffset(msg);
        else
            ReadResult(msg);
    }
}

void PageInd::InitializesItem()
{
    Items.clear();
    Results.clear();
    QStringList n;
    for (int row = 0; row<Enable.size(); row++) {
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
    for (int row = 0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            n.append(Items.at(row));
        }
    }
    if (ui->BoxUnbalance->value() != 0 && n.size()>=3) {
        QStringList s;
        s.append("电感平衡");
        s.append(QString("%1%").arg(ui->BoxUnbalance->value()));
        s.append(" ");
        s.append(" ");
        Items.append(s.join("@"));
        n.append(Items.last());
    }
    emit SendMessage(ADDR,CMD_INIT_ITEM,n.join("\n").toUtf8());
}

void PageInd::SendStatusCmd()
{
    if (Testing)
        return;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x26)<<quint8(0x01)<<quint8(0x00);
    emit SendMessage(ADDR,CMD_CAN,msg);
    Testing = true;
    if (!WaitTestOver(100)) {
        Testing = false;
        QMessageBox::warning(this,tr("警告"),tr("电感板异常"),QMessageBox::Ok);
        emit SendMessage(ADDR,CMD_DEBUG,"Check PageInd Error:Time out\n");
    }
}

void PageInd::ReadStatus(QByteArray )
{
    if (!isCheckOk) {
        isCheckOk = true;
        emit SendMessage(ADDR,CMD_DEBUG,"INDL check ok\n");
    }
    if (Testing)
        Testing = false;
}

void PageInd::SendStartCmd(quint8 pos)
{
    if (Testing)
        return;
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
    emit SendMessage(ADDR,CMD_CAN,msg);
    Testing = true;
    Judge = "OK";
    if(!WaitTestOver(100)) {
        Testing = false;
        Judge = "NG";
        for (int row = 0; row<Enable.size(); row++) {
            if (Enable.at(row)->text() == "Y") {
                QStringList s = QString(Items.at(row)).split("@");
                if (s.at(2) == " ")
                    s[2] = "---";
                if (s.at(3) == " ")
                    s[3] = "NG";
                emit SendMessage(ADDR,CMD_ITEM,s.join("@").toUtf8());
            }
        }
        if (ui->BoxUnbalance->value() != 0) {
            QStringList s = QString(Items.last()).split("@");
            if (s.at(2) == " ")
                s[2] = "---";
            if (s.at(3) == " ")
                s[3] = "NG";
            emit SendMessage(ADDR,CMD_ITEM,s.join("@").toUtf8());
        }
    }
    QStringList s;
    s.append("电感");
    s.append(FileInUse);
    s.append(Judge);
    emit SendMessage(ADDR,CMD_JUDGE,s.join("@").toUtf8());
}

void PageInd::ReadResult(QByteArray msg)
{
    quint8 number = quint8(msg.at(1));
    if (number >0 )
        number--;
    if (quint8(msg.at(3) == 0x00)) {
        Result1.dat[0] = quint8(msg.at(4));
        Result1.dat[1] = quint8(msg.at(5));
        Result1.dat[2] = quint8(msg.at(6));
        Result1.dat[3] = quint8(msg.at(7));
    }
    if (quint8(msg.at(3) == 0x01)) {
        Result2.dat[0] = quint8(msg.at(4));
        Result2.dat[1] = quint8(msg.at(5));
        Result2.dat[2] = quint8(msg.at(6));
        Result2.dat[3] = quint8(msg.at(7));
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

        Results.append(t.toDouble());

        t +="," + QString::number(Result2.Result,'f',2);
        double max = Max.at(number)->value();
        double min = Min.at(number)->value();
        double qmax = QMax.at(number)->value();
        double qmin = QMin.at(number)->value();
        if (Unit.at(number)->currentText() == "mH") {
            max *= 1000;
            min *= 1000;
        }
        QString judge = "OK";
        if (Result1.Result<min || Result1.Result>max || Result2.Result<qmin || Result2.Result>qmax){
            Judge = "NG";
            judge = "NG";
        }
        QStringList s = QString(Items.at(number)).split("@");
        if (s.at(2) == " ")
            s[2] = t;
        if (s.at(3) == " ")
            s[3] = judge;
        emit SendMessage(ADDR,CMD_ITEM,s.join("@").toUtf8());

        if ((ui->BoxUnbalance->value() != 0) && (Results.size() == 3)) {
            double sum = 0;
            double avr = 0;
            QString u;
            for (int i=0; i<Results.size(); i++) {
                sum += Results.at(i);
            }
            avr = sum/Results.size();
            judge = "OK";
            for (int i=0; i<Results.size(); i++) {
                double un = fabs(Results.at(i)-avr)*100/avr;
                u.append(QString::number(un,'f',1));
                u.append("% ");
                if (un >= ui->BoxUnbalance->value()) {
                    Judge = "NG";
                    judge = "NG";
                }
            }
            QStringList s = QString(Items.last()).split("@");
            if (s.at(2) == " ")
                s[2] = u;
            if (s.at(3) == " ")
                s[3] = judge;
            emit SendMessage(ADDR,CMD_ITEM,s.join("@").toUtf8());
        }
    }
}

void PageInd::ReadOffset(QByteArray msg)
{
    quint8 number = quint8(msg.at(1));
    if (number >0 )
        number--;
    Resultunion  Result;
    double v;
    if (quint8(msg.at(3) == 0x00)) {
        Result.dat[0] = quint8(msg.at(4));
        Result.dat[1] = quint8(msg.at(5));
        Result.dat[2] = quint8(msg.at(6));
        Result.dat[3] = quint8(msg.at(7));
        if (Unit.at(number)->currentText() == "mH")
            v = Result.Result/1000;
        else
            v = Result.Result;
        Offset.at(number)->setValue(v);
    }
}

void PageInd::SendStopCmd()
{
    if (!Testing)
        return;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x26)<<quint8(0x01)<<quint8(0x02);
    emit SendMessage(ADDR,CMD_CAN,msg);
    Testing = false;
}

void PageInd::SendConfigCmd()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int i=0; i<Enable.size(); i++) {
        if (Enable.at(i)->text() == "Y") {
            out<<quint16(0x26)<<quint8(0x07)<<quint8(0x03)<<quint8(i)
              <<quint8(Terminal1.at(i)->text().toInt())
             <<quint8(Terminal2.at(i)->text().toInt())
            <<quint8(ui->BoxTime->value())
            <<quint8(CalculateGear(i))<<quint8(CalculateMode(i));
        }
    }
    emit SendMessage(ADDR,CMD_CAN,msg);
}

int PageInd::CalculateGear(int row)
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
            if (2*3.14*s*10/1000 < 1)
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

int PageInd::CalculateMode(int row)
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

bool PageInd::WaitTestOver(quint16 t)
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
bool PageInd::WaitOffset(quint16 t)
{
    TimeOut = 0;
    while (Offsetting) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageInd::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

void PageInd::showEvent(QShowEvent *)
{
    InitializesSetting();
}

void PageInd::hideEvent(QHideEvent *)
{
    SaveSetting();
}
