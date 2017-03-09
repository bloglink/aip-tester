#include "PageDcr.h"
#include "ui_PageDcr.h"

PageDcr::PageDcr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDcr)
{
    ui->setupUi(this);
    InitializesWindow();
    InitializesButton();
    InitializesSetting();
    Testing = false;
    isCheckOk = false;
    Offsetting = false;
}

PageDcr::~PageDcr()
{
    delete ui;
}

void PageDcr::InitializesWindow()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabParams->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(4,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(5,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(6,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(7,QHeaderView::Stretch);
    ui->TabParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabParams->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Stretch);
    ui->TabParams->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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

        Metal.append(new QComboBox(this));
        ui->TabParams->setCellWidget(row,3,Metal.at(row));
        QStringList t1;
        t1 <<tr("铜")<<tr("铝")<<tr("铜铝");
        Metal.at(row)->setView(new QListView(this));
        Metal.at(row)->addItems(t1);

        Unit.append(new QComboBox(this));
        ui->TabParams->setCellWidget(row,4,Unit.at(row));
        QStringList t2;
        t2 <<"mΩ"<<"Ω"<<"kΩ";
        Unit.at(row)->setView(new QListView(this));
        Unit.at(row)->addItems(t2);

        Min.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,5,Min.at(row));
        Min.at(row)->setMaximum(9999);
        Min.at(row)->setAlignment(Qt::AlignHCenter);
        Min.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Max.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,6,Max.at(row));
        Max.at(row)->setMaximum(9999);
        Max.at(row)->setAlignment(Qt::AlignHCenter);
        Max.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Std.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,7,Std.at(row));
        Std.at(row)->setMaximum(9999);
        Std.at(row)->setAlignment(Qt::AlignHCenter);
        Std.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Offset.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,8,Offset.at(row));
        Offset.at(row)->setMaximum(9999);
        Offset.at(row)->setAlignment(Qt::AlignHCenter);
        Offset.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    connect(Metal.at(0),SIGNAL(currentIndexChanged(int)),this,SLOT(AutoChangeMetal(int)));
    connect(Unit.at(0),SIGNAL(currentIndexChanged(int)),this,SLOT(AutoChangeUnit(int)));
    connect(Min.at(0),SIGNAL(valueChanged(double)),this,SLOT(AutoChangeMin(double)));
    connect(Max.at(0),SIGNAL(valueChanged(double)),this,SLOT(AutoChangeMax(double)));
    connect(Std.at(0),SIGNAL(valueChanged(double)),this,SLOT(AutoChangeStd(double)));
}

void PageDcr::InitializesButton()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSDLRAuto,Qt::Key_0);
    btnGroup->addButton(ui->BtnOffset,Qt::Key_1);
    btnGroup->addButton(ui->BtnSDLRExit,Qt::Key_2);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}

void PageDcr::BtnJudge(int id)
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

void PageDcr::InitializesSetting()
{
    qDebug()<<QTime::currentTime().toString()<<"读取电阻配置";
    QSettings *global = new QSettings(INI_PATH,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    FileInUse = global->value("FileInUse",INI_DEFAULT).toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(FileInUse);
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetDcr");

    QStringList temp = (set->value("Other","20 0 0.5 10 10 50").toString()).split(" ");
    if (temp.size() >= 6) {
        ui->BoxStd->setValue(temp.at(0).toDouble());
        ui->BoxOffset->setValue(temp.at(1).toDouble());
        ui->BoxTime->setValue(temp.at(2).toDouble());
        ui->BoxMin->setValue(temp.at(3).toDouble());
        ui->BoxMax->setValue(temp.at(4).toDouble());
        ui->BoxUnbalance->setValue(temp.at(5).toDouble());
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
    //材料
    temp = (QString(set->value("Metal","0 0 0 0 0 0 0 0").toByteArray())).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Metal.at(row)->setCurrentIndex(temp.at(row).toInt());
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
    //标准值
    temp = (set->value("Std","100 100 100 100 100 100 100 100").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Std.at(row)->setValue(temp.at(row).toDouble());
    //补偿
    temp = (set->value("Offset","0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Offset.at(row)->setValue(temp.at(row).toDouble());
    qDebug()<<QTime::currentTime().toString()<<"读取电阻配置OK";
}

void PageDcr::SaveSetting()
{
    qDebug()<<QTime::currentTime().toString()<<"电阻保存";
    QStringList temp;
    temp.append(QString::number(ui->BoxStd->value()));
    temp.append(QString::number(ui->BoxOffset->value()));
    temp.append(QString::number(ui->BoxTime->value()));
    temp.append(QString::number(ui->BoxMin->value()));
    temp.append(QString::number(ui->BoxMax->value()));
    temp.append(QString::number(ui->BoxUnbalance->value()));
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
    for (int i=0; i<Metal.size(); i++)
        temp.append(QString::number(Metal.at(i)->currentIndex()));
    set->setValue("Metal",(temp.join(" ").toUtf8()));
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
    for (int i=0; i<Std.size(); i++)
        temp.append(QString::number(Std.at(i)->value()));
    set->setValue("Std",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Offset.size(); i++)
        temp.append(Offset.at(i)->text());
    set->setValue("Offset",(temp.join(" ").toUtf8()));
    qDebug()<<QTime::currentTime().toString()<<"电阻保存OK";
}

void PageDcr::CalculateAuto()
{
    for (int i=0; i<ui->TabParams->rowCount(); i++) {
        double std = Std.at(i)->value();
        double min = std*(100-ui->BoxMin->value())/100;
        double max = std*(100+ui->BoxMax->value())/100;
        Min.at(i)->setValue(min);
        Max.at(i)->setValue(max);
    }
}

bool PageDcr::CheckSetting()
{
    for (int i=0; i<qMin(Terminal1.size(),Terminal2.size()); i++) {
        int t1 = Terminal1.at(i)->text().toInt();
        int t2 = Terminal2.at(i)->text().toInt();
        if (abs(t1-t2)%3 == 0) {
            QString warning = QString("行%1:\n端子差不能为3,保存失败").arg(i+1);
            QMessageBox::warning(this,"警告", warning,QMessageBox::Ok);
            return false;
        }
    }
    for (int i=0; i<qMin(Max.size(),Min.size()); i++) {
        if (Min.at(i)->value()>Max.at(i)->value()) {
            QString warning = QString("行%1:\n上限大于下限,保存失败").arg(i+1);
            QMessageBox::warning(this,"警告", warning,QMessageBox::Ok);
            return false;
        }
    }
    return true;
}

void PageDcr::ItemClick(int r, int c)
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

void PageDcr::ItemChange(QString msg)
{
    int t = ui->TabParams->currentColumn();
    if (t==1 || t==2)
        ui->TabParams->currentItem()->setText(msg);
}

void PageDcr::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_DCR && addr != CAN_ID_DCR)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(msg);
        break;
    case CMD_CHECK:
        SendStatusCmd();
        break;
    case CMD_START:
        WaitTestTimeOut(100);
        SendStartCmd(msg.toInt());
        WaitTestFinished();
        SendTestJudge();
        break;
    case CMD_STOP:
        SendStopCmd();
        break;
    case CMD_INIT:
        InitializesSetting();
        InitializesItems();
        SendConfigCmd();
        break;
    default:
        break;
    }
}

void PageDcr::ExcuteCanCmd(QByteArray msg)
{
    if (!Testing && !Offsetting)
        return;
    TimeOut = 0;
    if (msg.size()==4 && (quint8)msg.at(0)==0x00) {
        ReadStatus(msg);
    }
    if (msg.size()==7 && (quint8)msg.at(0)==0x01) {
        if (Offsetting)
            ReadOffset(msg);
        else
            ReadResult(msg);
    }
}

void PageDcr::InitializesItems()
{
    Items.clear();
    Results.clear();
    QStringList n;
    for (int row = 0; row<Enable.size(); row++) {
        QStringList s;
        QString T1 = Terminal1.at(qMin(row,Terminal1.size()))->text();
        QString T2 = Terminal2.at(qMin(row,Terminal2.size()))->text();
        QString U1 = Unit.at(qMin(row,Unit.size()))->currentText();
        QString M1 = Min.at(qMin(row,Min.size()))->text();
        QString M2 = Max.at(qMin(row,Max.size()))->text();
        s.append(QString(tr("电阻%1-%2")).arg(T1).arg(T2));
        s.append(QString("%1~%2%3").arg(M1).arg(M2).arg(U1));
        s.append(" ");
        s.append(" ");
        Items.append(s.join("@"));
    }
    for (int row = 0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            n.append(Items.at(row));
        }
    }
    if (ui->BoxUnbalance->value() != 0 && Items.size()>=3) {
        QStringList s;
        s.append("电阻平衡");
        s.append(QString("%1%").arg(ui->BoxUnbalance->value()));
        s.append(" ");
        s.append(" ");
        Items.append(s.join("@"));
        n.append(Items.last());
    }
    emit SendMessage(ADDR,CMD_INIT_ITEM,n.join("\n").toUtf8());
}

void PageDcr::SendStatusCmd()
{
    qDebug()<<QTime::currentTime().toString()<<"查询电阻状态";
    if (Testing)
        return;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x22)<<quint8(0x01)<<quint8(0x00);
    emit SendMessage(ADDR,CMD_CAN,msg);
    Testing = true;
    if (!WaitTestTimeOut(100)) {
        Testing = false;
        QMessageBox::warning(this,tr("警告"),tr("电阻板异常"),QMessageBox::Ok);
        emit SendMessage(ADDR,CMD_DEBUG,"Check PageDcr Error:Time out\n");
    }
    qDebug()<<QTime::currentTime().toString()<<"查询电阻状态OK";
}

void PageDcr::ReadStatus(QByteArray msg)
{
    if (!isCheckOk) {
        isCheckOk = true;
        emit SendMessage(ADDR,CMD_DEBUG,"Check PageDcr OK\n");
    }
    if (Testing)
        Testing = false;
    if (Offsetting)
        Offsetting = false;
    double offset = ui->BoxOffset->value();
    double temp = (quint16(msg.at(2)*256)+quint8(msg.at(3)))/10-50+offset;
    QString t = QString(tr("温度:%1°C")).arg(temp);
    emit SendMessage(ADDR,CMD_TEMP,t.toUtf8());
}

void PageDcr::ReadOffset(QByteArray msg)
{
    quint8 number = quint8(msg.at(1));
    quint8 grade = quint8(msg.at(2));
    double temp = (quint16)(msg.at(3)*256)+(quint8)msg.at(4);
    qDebug()<<temp;

    if (Unit.at(number)->currentText() == "mΩ")
        temp *= qPow(10,(grade-3));
    if (Unit.at(number)->currentText() == "Ω")
        temp *= qPow(10,(grade-6));
    if (Unit.at(number)->currentText() == "kΩ")
        temp *= qPow(10,(grade-9));

    if (temp*20 > Max.at(number)->value())
        temp = 0;
    Offset.at(number)->setValue(temp);

}

void PageDcr::SendStartCmd(quint8 pos)
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
    out<<quint16(0x22)<<quint8(0x06)<<quint8(0x01)<<quint8(0x01)<<quint8(0x00)
      <<quint8(pos)<<quint8(tt/256)<<quint8(tt%256);
    emit SendMessage(ADDR,CMD_CAN,msg);
}
/**
  * @brief  Wait for test finish or time out
  * @param  None
  * @retval None
  */
void PageDcr::WaitTestFinished()
{
    Testing = true;
    JudgeAll = "OK";
    if(!WaitTestTimeOut(100)) {
        Testing = false;
        JudgeAll = "NG";
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
}

void PageDcr::SendTestJudge()
{
    QStringList s;
    s.append("电阻");
    s.append(FileInUse);
    s.append(JudgeAll);
    emit SendMessage(ADDR,CMD_JUDGE,s.join("@").toUtf8());

}

void PageDcr::ReadResult(QByteArray msg)
{
    quint8 number = quint8(msg.at(1));
    quint8 grade = quint8(msg.at(2));
    double temp = (quint16)(msg.at(3)*256)+(quint8)msg.at(4);
    double tt = (quint16)(msg.at(5)*256)+quint8(msg.at(6));

    if (ui->BoxOffsetEnable->isChecked()) {
        double offset = CalculateOffset(tt,number);
        temp *= offset;
    }
    QString t;
    QString JudgeItem = "OK";
    double offset = Offset.at(number)->value();
    if (Unit.at(number)->currentText() == "mΩ")
        offset *= 1000;
    if (Unit.at(number)->currentText() == "kΩ")
        offset /= 1000;

    temp *= qPow(10,(grade-6));
    temp -= qMin(temp,offset);
    if (grade==1 || grade==2)
        t = QString("%1mΩ").arg(temp*1000,0,'r',(3-grade%3));
    if (grade==3 || grade==4 || grade==5)
        t = QString("%1Ω").arg(temp,0,'r',(3-grade%3));
    if (grade==6 || grade==7)
        t = QString("%1kΩ").arg(temp/1000,0,'r',(3-grade%3));

    Results.append(temp);
    if (Unit.at(number)->currentText() == "mΩ")
        temp *= 1000;
    if (Unit.at(number)->currentText() == "kΩ")
        temp /= 1000;
    if (temp<Min.at(number)->value() || temp>Max.at(number)->value()) {
        JudgeAll = "NG";
        JudgeItem = "NG";
    }

    QStringList s = QString(Items.at(number)).split("@");
    if (s.at(2) == " ")
        s[2] = t;
    if (s.at(3) == " ")
        s[3] = JudgeItem;
    emit SendMessage(ADDR,CMD_ITEM,s.join("@").toUtf8());

    CalculateBalance();
}

double PageDcr::CalculateOffset(double t, quint8 num)
{
    double temp;
    double res = 0;
    switch (Metal.at(num)->currentIndex()) {
    case 0:
        res = ResCu;
        break;
    case 1:
        res = ResCu_Al;
        break;
    case 2:
        res = ResAl;
        break;
    default:
        break;
    }
    temp = 1+res*(ui->BoxStd->value()-(t/10-50)-ui->BoxOffset->value());
    return temp;
}

void PageDcr::CalculateBalance()
{
    //计算不平衡度
    if ((ui->BoxUnbalance->value() != 0) && (Results.size() == 3)) {
        QString JudgeItem = "OK";
        double sum = 0;
        double avr = 0;
        QString u;
        for (int i=0; i<Results.size(); i++) {
            sum += Results.at(i);
        }
        avr = sum/Results.size();
        for (int i=0; i<Results.size(); i++) {
            double un;
            if (avr == 0)
                un = 0;
            else
                un = fabs(Results.at(i)-avr)*100/avr;
            u.append(QString::number(un,'f',1));
            u.append("% ");
            if (un >= ui->BoxUnbalance->value()) {
                JudgeItem = "NG";
                JudgeAll = "NG";
            }
        }
        QStringList s = QString(Items.last()).split("@");
        if (s.at(2) == " ")
            s[2] = u;
        if (s.at(3) == " ")
            s[3] = JudgeItem;
        emit SendMessage(ADDR,CMD_ITEM,s.join("@").toUtf8());
    }
}

void PageDcr::SendStopCmd()
{
    if (!Testing)
        return;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<quint16(0x22)<<quint8(0x01)<<quint8(0x02);
    emit SendMessage(ADDR,CMD_CAN,msg);
    Testing = false;
}

void PageDcr::SendConfigCmd()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int row=0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            out<<quint16(0x22)<<quint8(0x06)<<quint8(0x03)<<quint8(row)
              <<quint8(Terminal1.at(row)->text().toInt())
             <<quint8(Terminal2.at(row)->text().toInt())
            <<quint8(CalculateGear(row))
            <<quint8(ui->BoxTime->value()*10);
        }
    }
    emit SendMessage(ADDR,CMD_CAN,msg);
}

int PageDcr::CalculateGear(int row)
{
    if (row >= Max.size())
        return 7;
    if (row >= Unit.size())
        return 7;
    double r = Max.at(row)->value();
    if (Unit.at(row)->currentText() == "mΩ")
        r /= 1000;
    if (Unit.at(row)->currentText() == "kΩ")
        r *= 1000;
    if (r <= 0.2)
        return 1;
    if (r <= 2)
        return 2;
    if (r <= 20)
        return 3;
    if (r <= 200)
        return 4;
    if (r <= 2000)
        return 5;
    if (r <= 20000)
        return 6;
    else
        return 7;
}

void PageDcr::SendVersionCmd()
{
}

bool PageDcr::WaitTestTimeOut(quint16 t)
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

bool PageDcr::WaitOffset(quint16 t)
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
void PageDcr::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

void PageDcr::AutoChangeMetal(int index)
{
    for (int i=1; i<Metal.size(); i++)
        Metal.at(i)->setCurrentIndex(index);
}

void PageDcr::AutoChangeUnit(int index)
{
    for (int i=1; i<Unit.size(); i++)
        Unit.at(i)->setCurrentIndex(index);
}

void PageDcr::AutoChangeMin(double x)
{
    for (int i=1; i<Min.size(); i++)
        Min.at(i)->setValue(x);
}

void PageDcr::AutoChangeMax(double x)
{
    for (int i=1; i<Max.size(); i++)
        Max.at(i)->setValue(x);
}

void PageDcr::AutoChangeStd(double x)
{
    for (int i=1; i<Std.size(); i++)
        Std.at(i)->setValue(x);
}

void PageDcr::showEvent(QShowEvent*)
{
    InitializesSetting();
    CheckSetting();
}

void PageDcr::hideEvent(QHideEvent *)
{
    if(CheckSetting())
        SaveSetting();
}
/*********************************END OF FILE**********************************/
