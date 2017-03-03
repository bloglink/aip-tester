#include "PageInd.h"
#include "ui_PageInd.h"

PageInd::PageInd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageInd)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
    Testing = false;
    isCheckOk = false;
}

PageInd::~PageInd()
{
    delete ui;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      初始化界面
*******************************************************************************/
void PageInd::WinInit()
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
    input = new PageNum(this);
    QStringList t;
    t <<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12";
    input->BtnInit(t);
    connect(input,SIGNAL(ItemChange(QString)),this,SLOT(ItemChange(QString)));
    input->hide();
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      初始化按键
*******************************************************************************/
void PageInd::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSDLRAuto,Qt::Key_0);
    btnGroup->addButton(ui->BtnSDLRExit,Qt::Key_2);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      按键功能
*******************************************************************************/
void PageInd::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        DatAuto();
        break;
    case Qt::Key_1:
        break;
    case Qt::Key_2:
        emit SendMessage(ADDR,CMD_JUMP,NULL);
        break;
    default:
        break;
    }
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      数据初始化
*******************************************************************************/
void PageInd::DatInit()
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
    qDebug()<<QTime::currentTime().toString()<<"电感数据OK";
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      数据保存
*******************************************************************************/
void PageInd::DatSave()
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
    qDebug()<<QTime::currentTime().toString()<<"电感保存OK";
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.19
 * brief:      自动计算
*******************************************************************************/
void PageInd::DatAuto()
{
    for (int i=0; i<ui->TabInductance->rowCount(); i++) {
        double std = Std.at(i)->value();
        double min = std*(100-ui->BoxMin->value())/100;
        double max = std*(100+ui->BoxMax->value())/100;
        Min.at(i)->setValue(min);
        Max.at(i)->setValue(max);
    }
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.23
 * brief:      点击
*******************************************************************************/
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
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.23
 * brief:      切换文字
*******************************************************************************/
void PageInd::ItemChange(QString msg)
{
    int t = ui->TabInductance->currentColumn();
    if (t==1 || t==2)
        ui->TabInductance->currentItem()->setText(msg);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.15
 * brief:       命令处理
*******************************************************************************/
void PageInd::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_IND && addr != CAN_ID_IND)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(msg);
        break;
    case CMD_CHECK:
        TestCheck();
        break;
    case CMD_START:
        TestStart(msg.toInt());
        break;
    case CMD_STOP:
        TestStop();
        break;
    case CMD_INIT:
        DatInit();
        TestInit();
        TestConfig();
        break;
    default:
        break;
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.15
 * brief:       CAN命令处理
*******************************************************************************/
void PageInd::ExcuteCanCmd(QByteArray msg)
{
    if (!Testing)
        return;
    TimeOut = 0;
    if (msg.size()==8 && (quint8)msg.at(0)==0x00) {
        TestCheckOk(msg);
    }
    if (msg.size()==8 && (quint8)msg.at(0)==0x01) {
        TestResult(msg);
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.15
 * brief:       更新显示
*******************************************************************************/
void PageInd::TestInit()
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
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.15
 * brief:      检测状态
*******************************************************************************/
void PageInd::TestCheck()
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
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.15
 * brief:      更新状态
*******************************************************************************/
void PageInd::TestCheckOk(QByteArray )
{
    if (!isCheckOk) {
        isCheckOk = true;
        emit SendMessage(ADDR,CMD_DEBUG,"INDL check ok\n");
    }
    if (Testing)
        Testing = false;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.15
 * brief:      开始测试
*******************************************************************************/
void PageInd::TestStart(quint8 pos)
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
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.15
 * brief:       更新测试数据
*******************************************************************************/
void PageInd::TestResult(QByteArray msg)
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
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.15
 * brief:      停止测试
*******************************************************************************/
void PageInd::TestStop()
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
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.15
 * brief:      配置
*******************************************************************************/
void PageInd::TestConfig()
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
            <<quint8(TestGear(i))<<quint8(TestMode(i));
        }
    }
    emit SendMessage(ADDR,CMD_CAN,msg);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.15
 * brief:      计算测试档位
*******************************************************************************/
int PageInd::TestGear(int row)
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
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.15
 * brief:      计算测试方式
*******************************************************************************/
int PageInd::TestMode(int row)
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
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.15
 * brief:      等待测试结束
*******************************************************************************/
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
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.15
 * brief:      延时
*******************************************************************************/
void PageInd::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.15
 * brief:      更新显示
*******************************************************************************/
void PageInd::showEvent(QShowEvent *)
{
    DatInit();
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.15
 * brief:      判断保存
*******************************************************************************/
void PageInd::hideEvent(QHideEvent *)
{
    DatSave();
}
