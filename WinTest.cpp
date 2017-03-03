/**
  ******************************************************************************
  * @file    Test.cpp
  * @author  link
  * @version 2.0.4.0
  * @date    2017-03-01
  * @brief   Show test details
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "WinTest.h"
#include "ui_WinTest.h"
/**
  * @brief  Initializes
  * @param  *parent:parent widget
  * @retval None
  */
WinTest::WinTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinTest)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(showTime()));
    timer->start(30000);
}
/**
  * @brief  Destruct the window
  * @param  None
  * @retval None
  */
WinTest::~WinTest()
{
    delete ui;
}
/**
  * @brief  Initializes the window
  * @param  None
  * @retval None
  */
void WinTest::WinInit()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabTest->setColumnWidth(1,150);
    ui->TabTest->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch);
    ui->TabWave->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabWave->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->WidgetItem->hide();
    ui->TabTest->setColumnWidth(1,180);
    ui->TabTest->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->TabWave->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabWave->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    for (int i=0; i<3; i++) {
        wave.append(new Waveform(this));
        ui->TabWave->setCellWidget(0,i,wave.at(i));
    }
    ui->LabelState->setStyleSheet("color:green;font:Bold 45pt Ubuntu;");
    ui->TextPos->setStyleSheet("color:red;font:Bold 45pt Ubuntu;");
    connect(ui->TabTest,SIGNAL(cellClicked(int,int)),this,SLOT(ItemClick(int,int)));
}
/**
  * @brief  Initializes the buttons
  * @param  None
  * @retval None
  */
void WinTest::BtnInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnWinHome,Qt::Key_0);
    btnGroup->addButton(ui->BtnWinType,Qt::Key_1);
    btnGroup->addButton(ui->BtnCmdStart,Qt::Key_3);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/**
  * @brief  Button functions
  * @param  id:button id
  * @retval None
  */
void WinTest::BtnJudge(int win)
{
    switch (win) {
    case Qt::Key_0:
        emit SendMessage(ADDR,CMD_JUMP,NULL);
        break;
    case Qt::Key_1:
        emit SendMessage(ADDR,CMD_JUMP,"WinType");
        break;
    case Qt::Key_3:
        if (ui->BtnCmdStart->text() == "单次测试") {
            ui->BtnCmdStart->setText("中断测试");
            emit SendMessage(ADDR,CMD_START,QString::number(0x13).toUtf8());
        } else {
            ui->BtnCmdStart->setText("单次测试");
            emit SendMessage(ADDR,CMD_STOP,NULL);
        }
        break;
    default:
        break;
    }
}
/**
  * @brief  Initializes settings
  * @param  None
  * @retval None
  */
void WinTest::SetInit()
{
    qDebug()<<QTime::currentTime().toString()<<"读取测试配置";

    QSettings *g_settings = new QSettings(INI_PATH,QSettings::IniFormat);
    g_settings->setIniCodec("GB18030");
    g_settings->beginGroup("GLOBAL");

    int width = g_settings->value("Width","200").toInt();
    ui->TabTest->setColumnWidth(1,width);

    int user = g_settings->value("User","0").toInt();
    if (user == 0)
        ui->TextTestUser->setText(tr("用户:操作员"));
    if (user == 1)
        ui->TextTestUser->setText(tr("用户:管理员"));

    motor_type = g_settings->value("FileInUse",INI_DEFAULT).toString();
    motor_type.remove(".ini");
    ui->TextTestType->setText(QString("型号:%1").arg(motor_type));
    //当前使用的测试项目
    QString n = QString("./config/%1.ini").arg(motor_type);
    QSettings *c_settings = new QSettings(n,QSettings::IniFormat);
    c_settings->setIniCodec("GB18030");

    QStringList WireColor = c_settings->value("/GLOBAL/WireColor","").toString().split(" ");
    if (WireColor.size() == 8) {
        ui->LabelWireColor1->setStyleSheet(QString("background-color:%1").arg(WireColor.at(0)));
        ui->LabelWireColor2->setStyleSheet(QString("background-color:%1").arg(WireColor.at(1)));
        ui->LabelWireColor3->setStyleSheet(QString("background-color:%1").arg(WireColor.at(2)));
        ui->LabelWireColor4->setStyleSheet(QString("background-color:%1").arg(WireColor.at(3)));
        ui->LabelWireColor5->setStyleSheet(QString("background-color:%1").arg(WireColor.at(4)));
        ui->LabelWireColor6->setStyleSheet(QString("background-color:%1").arg(WireColor.at(5)));
        ui->LabelWireColor7->setStyleSheet(QString("background-color:%1").arg(WireColor.at(6)));
        ui->LabelWireColor8->setStyleSheet(QString("background-color:%1").arg(WireColor.at(7)));
    }

    qDebug()<<QTime::currentTime().toString()<<"读取测试配置OK";
    emit SendMessage(ADDR,CMD_INIT,NULL);
}
/**
  * @brief  Save settings
  * @param  None
  * @retval None
  */
void WinTest::SetSave()
{
    qDebug()<<QTime::currentTime().toString()<<"保存测试配置";

    QSettings *settings_g = new QSettings(INI_PATH,QSettings::IniFormat);
    settings_g->setIniCodec("GB18030");
    settings_g->beginGroup("GLOBAL");

    int width = ui->TabTest->columnWidth(1);
    settings_g->setValue("Width",width);

    qDebug()<<QTime::currentTime().toString()<<"保存测试配置OK";
}
/**
  * @brief  Show test item
  * @param  item:test item
  * @retval None
  */
void WinTest::showItem(QString item)
{
    QStringList s = item.split("@");
    for (int i=0; i<ui->TabTest->rowCount(); i++) {
        QString n = ui->TabTest->item(i,0)->text();
        QString t = ui->TabTest->item(i,2)->text();
        if (t != " ")
            continue;
        if (s.at(0) == n) {
            ui->TabTest->item(i,2)->setText(s.at(2));
            ui->TabTest->item(i,3)->setText(s.at(3));
            if (s.at(3) == "NG")
                ui->TabTest->item(i,3)->setTextColor(QColor(Qt::red));
            else
                ui->TabTest->item(i,3)->setTextColor(QColor(Qt::green));
            break;
        }
    }
}
/**
  * @brief  Show test judge
  * @param  judge:test judge
  * @retval None
  */
void WinTest::showJudge(QString judge)
{
    int sum = ui->LabelSum->text().toInt();
    int qua = ui->LabelQualified->text().toInt();
    int unq = ui->LabelUnqualified->text().toInt();
    sum++;
    if (judge == "NG") {
        ui->LabelState->setStyleSheet("color:rgb(255,0,0);font: Bold  42pt Ubuntu;border: none;");
        ui->LabelState->setText("NG");
        unq++;
    } else {
        ui->LabelState->setStyleSheet("color:rgb(0,255,0);font:Bold 42pt Ubuntu;border:none;");
        ui->LabelState->setText("OK");
        qua++;
    }
    ui->LabelSum->setText(QString::number(sum));
    ui->LabelQualified->setText(QString::number(qua));
    ui->LabelUnqualified->setText(QString::number(unq));
    ui->BtnCmdStart->setText("单次测试");
}
/**
  * @brief  Show wave item
  * @param  msg:wave item
  * @retval None
  */
void WinTest::showWaveItem(QByteArray msg)
{
    if (!wave.last()->WaveItem.isEmpty()) {
        WaveClear();
    }
    for (int i=0; i<wave.size(); i++) {
        if (wave.at(i)->WaveItem.isEmpty()) {
            wave.at(i)->WaveItemShow(msg);
            break;
        }
    }
}
/**
  * @brief  Show wave standard
  * @param  msg:wave standard
  * @retval None
  */
void WinTest::showWaveByte(QByteArray msg)
{
    for (int i=0; i<wave.size(); i++) {
        if (wave.at(i)->WaveByte.isEmpty()) {
            wave.at(i)->WaveByteShow(msg);
            break;
        }
    }
}
/**
  * @brief  Show wave tested
  * @param  msg:wave tested
  * @retval None
  */
void WinTest::showWaveTest(QByteArray msg)
{
    for (int i=0; i<wave.size(); i++) {
        if (wave.at(i)->WaveTest.isEmpty()) {
            wave.at(i)->WaveTestShow(msg);
            break;
        }
    }
}
/**
  * @brief  Show temperature
  * @param  msg:temperature
  * @retval None
  */
void WinTest::showTemperature(QByteArray msg)
{
    ui->TextTestTemp->setText(msg);
}
/**
  * @brief  Show station
  * @param  msg:station
  * @retval None
  */
void WinTest::showStation(QByteArray msg)
{
    if (msg.toInt() == 0x13)
        ui->TextPos->setText(tr("左"));
    if (msg.toInt() == 0x14)
        ui->TextPos->setText(tr("右"));
}
/**
  * @brief  Show current time
  * @param  None
  * @retval None
  */
void WinTest::showTime()
{
    QString t = QTime::currentTime().toString("hh:mm");
    ui->TextTestTime->setText(t);
}
/**
  * @brief  Clear waves
  * @param  None
  * @retval None
  */
void WinTest::WaveClear()
{
    for (int i=0; i<wave.size(); i++) {
        wave.at(i)->WaveByteShow(NULL);
        wave.at(i)->WaveTestShow(NULL);
        wave.at(i)->WaveItemShow(NULL);
        QString t = QString::number(i+1);
        ui->TabWave->horizontalHeaderItem(i)->setText(t);
    }
}
/**
  * @brief  Update wave when click table items
  * @param  r:row of the test item
  * @retval None
  */
void WinTest::ItemClick(int r, int )
{
    QString t = ui->TabTest->item(r,0)->text();
    if (t.contains(tr("反嵌")) || t.contains(tr("匝间"))) {
        WaveClear();
        emit SendMessage(ADDR,CMD_WAVE,t.toUtf8());
    }
}
/**
  * @brief  Excute command
  * @param  addr:target address;cmd:command to excute;msg:command param
  * @retval None
  */
void WinTest::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr!=ADDR && addr!=WIN_ID_TEST)
        return;

    switch (cmd) {
    case CMD_INIT:
        WaveClear();
        ItemInit(msg);
        break;
    case CMD_ITEM:
        showItem(msg);
        break;
    case CMD_TEMP:
        showTemperature(msg);
        break;
    case CMD_WAVE_BYTE:
        showWaveByte(msg);
        break;
    case CMD_WAVE_TEST:
        showWaveTest(msg);
        break;
    case CMD_WAVE_ITEM:
        showWaveItem(msg);
        break;
    case CMD_WAVE_HIDE:
        WaveClear();
        break;
    case CMD_START:
        showStation(msg);
        break;
    case CMD_JUDGE:
        showJudge(msg);
        break;
    default:
        break;
    }
}
/**
  * @brief  Initializes before test
  * @param  None
  * @retval None
  */
void WinTest::ItemInit(QByteArray msg)
{
    ui->LabelState->setStyleSheet("color:rgb(0,255,0);font:Bold 42pt Ubuntu;border:none;");
    ui->LabelState->setText("--");
    if (msg.isEmpty()) {
        return;
    }
    QStringList item = QString(msg).split("\n");
    ui->TabTest->setRowCount(item.size());
    for (int i=0; i<item.size(); i++) {
        QStringList s = QString(item.at(i)).split("@");
        ui->TabTest->setItem(i,0,new QTableWidgetItem);
        ui->TabTest->item(i,0)->setTextAlignment(Qt::AlignCenter);
        ui->TabTest->item(i,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->TabTest->item(i,0)->setText(s.at(0));

        ui->TabTest->setItem(i,1,new QTableWidgetItem);
        ui->TabTest->item(i,1)->setTextAlignment(Qt::AlignCenter);
        ui->TabTest->item(i,1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->TabTest->item(i,1)->setText(s.at(1));

        ui->TabTest->setItem(i,2,new QTableWidgetItem);
        ui->TabTest->item(i,2)->setTextAlignment(Qt::AlignCenter);
        ui->TabTest->item(i,2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->TabTest->item(i,2)->setText(s.at(2));

        ui->TabTest->setItem(i,3,new QTableWidgetItem);
        ui->TabTest->item(i,3)->setTextAlignment(Qt::AlignCenter);
        ui->TabTest->item(i,3)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->TabTest->item(i,3)->setText(s.at(3));
    }
}
/**
  * @brief  Initializes data when show
  * @param  None
  * @retval None
  */
void WinTest::showEvent(QShowEvent *)
{
    SetInit();
}
/**
  * @brief  Save data when hide
  * @param  None
  * @retval None
  */
void WinTest::hideEvent(QHideEvent *)
{
    SetSave();
}
/*********************************END OF FILE**********************************/
