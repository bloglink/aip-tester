#include "PageMag.h"
#include "ui_PageMag.h"

PageMag::PageMag(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageMag)
{
    ui->setupUi(this);
    InitializesWindow();
    InitializesButton();
    InitializesSetting();
    Testing = false;
    Sampling = false;
}

PageMag::~PageMag()
{
    delete ui;
}

void PageMag::InitializesWindow()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabParams->horizontalHeader()->setResizeMode(0,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(4,400);
    ui->TabParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabParams->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(4,400);
    ui->TabParams->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    ui->BoxDir->setView(new QListView(this));
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

        Max.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row,3,Max.at(row));
        Max.at(row)->setMaximum(9999);
        Max.at(row)->setAlignment(Qt::AlignHCenter);
        Max.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        WaveMag.append(new Waveform(this));
        ui->TabParams->setCellWidget(row,4,WaveMag.at(row));

        Area.append(1000);
        Freq.append(1000);
    }
}

void PageMag::InitializesButton()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSampleMag,Qt::Key_0);
    btnGroup->addButton(ui->BtnExitMag,Qt::Key_2);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}

void PageMag::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        SendConfigCmd();
        SendSampleCmd();
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

void PageMag::InitializesSetting()
{
    qDebug()<<QTime::currentTime().toString()<<"反嵌数据";
    QSettings *global = new QSettings(INI_PATH,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    FileInUse = global->value("FileInUse",INI_DEFAULT).toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(FileInUse);
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetMag");

    QStringList temp = (set->value("Other","0 1 2").toString()).split(" ");
    if (temp.size() >= 3) {
        ui->BoxDir->setCurrentIndex(temp.at(0).toInt());
        ui->BoxMain->setValue(temp.at(1).toInt());
        ui->BoxAuxiliary->setValue(temp.at(2).toInt());
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
    //最大值
    temp = (set->value("Max","200 200 200 200 200 200 200 200").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Max.at(row)->setValue(temp.at(row).toDouble());
    //频率
    temp = (set->value("Freq","0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Freq[row] = temp.at(row).toInt();
    temp = (set->value("Area","1000 1000 1000 1000 1000 1000 1000 1000").toString()).split(" ");
    for (int row=0; row<qMin(temp.size(),MAX_ROW); row++)
        Area[row] = temp.at(row).toInt();
    //波形
    QByteArray w;
    for (int row=0; row<qMin(WaveMag.size(),MAX_ROW); row++) {
        QString ByteL = "WaveMagL"+QString::number(row);
        w = set->value(ByteL,"1000000000000000").toByteArray();
        WaveMag.at(row)->WaveBytes[0] = w;

        QString ByteR = "WaveMagR"+QString::number(row);
        w = set->value(ByteR,"1000000000000000").toByteArray();
        WaveMag.at(row)->WaveBytes[1] = w;

        QString T1 = Terminal1.at(row)->text();
        QString T2 = Terminal2.at(row)->text();
        WaveMag.at(row)->WaveItem = QString(tr("反嵌%1-%2")).arg(T1).arg(T2).toUtf8();
    }
    qDebug()<<QTime::currentTime().toString()<<"反嵌数据OK";
}

void PageMag::SaveSetting()
{
    qDebug()<<QTime::currentTime().toString()<<"反嵌保存";
    QStringList temp;
    temp.append(QString::number(ui->BoxDir->currentIndex()));
    temp.append(QString::number(ui->BoxMain->value()));
    temp.append(QString::number(ui->BoxAuxiliary->value()));
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
    for (int i=0; i<Max.size(); i++)
        temp.append(QString::number(Max.at(i)->value()));
    set->setValue("Max",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Area.size(); i++)
        temp.append(QString::number(Area.at(i)));
    set->setValue("Area",(temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i<Freq.size(); i++)
        temp.append(QString::number(Freq.at(i)));
    set->setValue("Freq",(temp.join(" ").toUtf8()));

    for (int row=0; row<WaveMag.size(); row++) {
        QString ByteL = "WaveMagL"+QString::number(row);
        QString ByteR = "WaveMagR"+QString::number(row);
        set->setValue(ByteL,WaveMag.at(row)->WaveBytes.at(0));
        set->setValue(ByteR,WaveMag.at(row)->WaveBytes.at(1));
    }
    qDebug()<<QTime::currentTime().toString()<<"反嵌保存OK";
}

void PageMag::ItemClick(int r, int c)
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
        input->showNormal();
        break;
    }
}

void PageMag::ItemChange(QString msg)
{
    ui->TabParams->currentItem()->setText(msg);
}

void PageMag::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR &&
            addr != WIN_ID_MAG &&
            addr != CAN_ID_DCR &&
            addr != CAN_ID_DCR_WAVE)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(addr,msg);
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
        InitializesItems();
        SendConfigCmd();
        break;
    case CMD_WAVE:
        SendWave(msg);
        break;
    default:
        break;

    }
}

void PageMag::ExcuteCanCmd(int id, QByteArray msg)
{
    if (!Testing && !Sampling)
        return;
    TimeOut = 0;
    if (id == CAN_ID_DCR_WAVE) {
        ReadWave(msg);
        return;
    }
    if (msg.size()==4 && (quint8)msg.at(0)==0x00) {
        ReadStatus(msg);
    }
    if (msg.size()==5 && (quint8)msg.at(0)==0x02) {
        ReadResult(msg);
    }
    if (msg.size()==2 && (quint8)msg.at(0)==0x03 && (quint8)msg.at(1)!=0xff) {
        if (Sampling)
            WaveMag.at(CurrentWave)->WaveByte.clear();
        if (Testing)
            WaveMag.at(CurrentWave)->WaveTest.clear();
    }
    if (msg.size()==2 && (quint8)msg.at(0)==0x03 && (quint8)msg.at(1)==0xff) {
        ReadWaveOk(msg);
    }
}

void PageMag::InitializesItems()
{
    Items.clear();
    WaveNumber.clear();
    QStringList n;
    for (int row = 0; row<Enable.size(); row++) {
        QStringList s;
        QString T1 = Terminal1.at(qMin(row,Terminal1.size()))->text();
        QString T2 = Terminal2.at(qMin(row,Terminal2.size()))->text();
        QString M1 = Max.at(qMin(row,Max.size()))->text();
        s.append(QString(tr("反嵌%1-%2")).arg(T1).arg(T2));
        s.append(QString("%1%").arg(M1));
        s.append(" ");
        s.append(" ");
        Items.append(s.join("@"));
        WaveMag.at(row)->WaveTest.clear();
    }
    for (int row = 0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            n.append(Items.at(row));
            WaveNumber.append(row);
        }
    }
    if (ui->BoxDir->currentIndex() != 0) {
        QStringList s;
        s.append(QString(tr("磁旋")));
        s.append(ui->BoxDir->currentText());
        s.append(" ");
        s.append(" ");
        Items.append(s.join("@"));
        n.append(Items.last());
    }
    emit SendMessage(ADDR,CMD_INIT_ITEM,n.join("\n").toUtf8());
}

void PageMag::SendStatusCmd()
{

}

void PageMag::ReadStatus(QByteArray )
{
    if (Testing) {
        if (ui->BoxDir->currentIndex() != 0) {
            TestDir();
        }
        Testing = false;
    }
    if (Sampling) {
        Sampling = false;
        emit SendMessage(ADDR,CMD_DEBUG,"MAG Sample ok\n");
    }
    if (!isCheckOk) {
        isCheckOk = true;
        emit SendMessage(ADDR,CMD_DEBUG,"MAG check ok\n");
    }
}

void PageMag::SendSampleCmd()
{
    if (Testing || Sampling)
        return;
    if (ui->BoxStation->currentIndex() == 0)
        station = WIN_ID_OUT13;
    if (ui->BoxStation->currentIndex() == 1)
        station = WIN_ID_OUT14;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001<<row;
    }
    out<<quint16(0x22)<<quint8(0x06)<<quint8(0x01)<<quint8(0x02)<<quint8(0x01)
      <<quint8(station)<<quint8(tt/256)<<quint8(tt%256);
    emit SendMessage(ADDR,CMD_CAN,msg);
    Sampling = true;
}

void PageMag::SendStartCmd(quint8 pos)
{
    WaitTestTimeOut(100);
    if (Testing || Sampling)
        return;

    station = pos;

    emit SendMessage(ADDR,CMD_WAVE_HIDE,NULL);

    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001<<row;
    }
    out<<quint16(0x22)<<quint8(0x06)<<quint8(0x01)<<quint8(0x02)<<quint8(0x00)
      <<quint8(station)<<quint8(tt/256)<<quint8(tt%256);
    emit SendMessage(ADDR,CMD_CAN,msg);
    Testing = true;
    Judge = "OK";
    if(!WaitTestTimeOut(100)) {
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
        if (ui->BoxDir->currentIndex() != 0) {
            QStringList s = QString(Items.last()).split("@");
            if (s.at(2) == " ")
                s[2] = "---";
            if (s.at(3) == " ")
                s[3] = "NG";
            emit SendMessage(ADDR,CMD_ITEM,s.join("@").toUtf8());
        }
    }
    QStringList s;
    s.append("反嵌");
    s.append(FileInUse);
    s.append(Judge);
    emit SendMessage(ADDR,CMD_JUDGE,s.join("@").toUtf8());
}

void PageMag::ReadResult(QByteArray msg)
{
    CurrentWave = (quint8)msg.at(1);
    quint16 Area1 = Area[CurrentWave];
    quint16 Area2 = quint16(msg.at(2)*256)+quint8(msg.at(3));
    if (Sampling) {
        Area[CurrentWave] = Area2;
        Freq[CurrentWave] = quint8(msg.at(4));
        return;
    }
    quint16 area = abs((Area2-Area1)*100/Area1);
    QString n = QString("%1%").arg(area);
    QString judge = "OK";
    if (CurrentWave >= Max.size() || area > Max.at(CurrentWave)->value()) {
        Judge = "NG";
        judge = "NG";
    }
    QStringList s = QString(Items.at(CurrentWave)).split("@");
    if (s.at(2) == " ")
        s[2] = n;
    if (s.at(3) == " ")
        s[3] = judge;
    emit SendMessage(ADDR,CMD_ITEM,s.join("@").toUtf8());
}

void PageMag::ReadWave(QByteArray msg)
{
    if (Sampling)
        WaveMag.at(CurrentWave)->WaveByte.append(msg);
    else
        WaveMag.at(CurrentWave)->WaveTest.append(msg);
}

void PageMag::ReadWaveOk(QByteArray )
{
    if (station == WIN_ID_OUT13) {
        WaveMag.at(CurrentWave)->InitWaveByte(0);
        WaveMag.at(CurrentWave)->InitWaveTest(0);
    }
    if (station == WIN_ID_OUT14) {
        WaveMag.at(CurrentWave)->InitWaveByte(1);
        WaveMag.at(CurrentWave)->InitWaveTest(1);
    }
    QByteArray w;
    QByteArray i;
    if (Sampling) {
        w = WaveMag.at(CurrentWave)->WaveByte;
        if (station == WIN_ID_OUT13) {
            WaveMag.at(CurrentWave)->WaveBytes[0] = w;
        }
        if (station == WIN_ID_OUT14) {
            WaveMag.at(CurrentWave)->WaveBytes[1] = w;
        }
        WaveMag.at(CurrentWave)->WaveByteShow(w);
    }
    if (Testing) {
        w = WaveMag.at(CurrentWave)->WaveTest;
        i = WaveMag.at(CurrentWave)->WaveItem;

        if (station == WIN_ID_OUT13) {
            WaveMag.at(CurrentWave)->WaveTests[0] = w;
        }
        if (station == WIN_ID_OUT14) {
            WaveMag.at(CurrentWave)->WaveTests[1] = w;
        }

        emit SendMessage(ADDR,CMD_WAVE_ITEM,i);
        emit SendMessage(ADDR,CMD_WAVE_BYTE,w);
    }
}

void PageMag::SendWave(QByteArray msg)
{
    int t = WaveNumber.size();
    for (int i=0; i<WaveNumber.size(); i++) {
        if (WaveMag.at(WaveNumber.at(i))->WaveItem == msg)
            t = i;
    }
    if (t == WaveNumber.size())
        return;
    for (int i=0; i<qMin(3,WaveNumber.size()-t); i++) {
        QByteArray w;
        w = WaveMag.at(WaveNumber.at(t+i))->WaveItem;
        emit SendMessage(ADDR,CMD_WAVE_ITEM,w);
        w = WaveMag.at(WaveNumber.at(t+i))->WaveTest;
        emit SendMessage(ADDR,CMD_WAVE_BYTE,w);
    }
}

void PageMag::TestDir()
{
    //计算主副相的面积，差积，和左右移动的差积
    qint32 	area1,area2,diff,diff1,diff2;
    area1=0;
    area2=0;
    diff=0;
    diff1=0;
    diff2=0;
    int b1 = ui->BoxMain->value()-1;
    int b2 = ui->BoxAuxiliary->value()-1;
    int s1 = WaveMag.at(b1)->WaveTest.size();
    int s2 = WaveMag.at(b2)->WaveTest.size();
    for (int i=10; i<qMin(s1,s2)-10; i++) {
        int P1 = quint8(WaveMag.at(b1)->WaveTest.at(i))-0x80;
        int P2 = quint8(WaveMag.at(b2)->WaveTest.at(i))-0x80;
        area1 += P1*P1;
        area2 += P2*P2;
        diff += (P1-P2)*(P1-P2);
        for (int j=1; j<11; j++) {
            int P3 = quint8(WaveMag.at(b2)->WaveTest.at(i+j))-0x80;
            int P4 = quint8(WaveMag.at(b2)->WaveTest.at(i-j))-0x80;
            diff1 += (P1-P3)*(P1-P3);
            diff2 += (P1-P4)*(P1-P4);
        }
    }
    diff1 /= 10;
    diff2 /= 10;
    QString n;
    QString judge = "OK";

    if(((diff*3<area1)&&(diff*3<area2))||(area1<(area2>>4))||(area2<(area1>>4)))
        n = tr("不转");
    else if(diff1<diff2) //副相在前
        n = tr("正转");
    else if(diff1>diff2)//主相在前
        n = tr("反转");
    else
        n = tr("不转");
    if (n != ui->BoxDir->currentText()) {
        Judge = "NG";
        judge = "NG";
    }
    QStringList s = QString(Items.last()).split("@");
    if (s.at(2) == " ")
        s[2] = n;
    if (s.at(3) == " ")
        s[3] = judge;
    emit SendMessage(ADDR,CMD_ITEM,s.join("@").toUtf8());
}

void PageMag::SendStopCmd()
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

void PageMag::SendConfigCmd()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int row=0; row<Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            out<<quint16(0x22)<<quint8(0x05)<<quint8(0x04)<<quint8(row)
              <<quint8(Terminal1.at(row)->text().toInt())
             <<quint8(Terminal2.at(row)->text().toInt())
            <<quint8(Freq.at(row));
        }
    }
    emit SendMessage(ADDR,CMD_CAN,msg);
}

bool PageMag::WaitTestTimeOut(quint16 t)
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

void PageMag::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

void PageMag::showEvent(QShowEvent *)
{
    InitializesSetting();
}

void PageMag::hideEvent(QHideEvent *)
{
    SaveSetting();
}
/*********************************END OF FILE**********************************/
