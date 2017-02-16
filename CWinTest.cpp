/********************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.11.23
 * brief:       测试界面
 * date:        2017.01.05
 * brief:       显示测试数量，默认工位改为左
********************************************************************************/
#include "CWinTest.h"
#include "ui_CWinTest.h"
/********************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.19
 * brief:       构造
********************************************************************************/
CWinTest::CWinTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CWinTest)
{
    ui->setupUi(this);
    WinInit();
    KeyInit();

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(ShowTime()));
    timer->start(1000);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.11.23
 * brief:       析构
*******************************************************************************/
CWinTest::~CWinTest()
{
    delete ui;
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.11.23
 * brief:       界面初始化
*******************************************************************************/
void CWinTest::WinInit()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabTest->setColumnWidth(1,150);
    ui->TabTest->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch);
    ui->TabWave->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabWave->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->WidgetItem->hide();
    ui->TabTest->setColumnWidth(1,150);
    ui->TabTest->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->TabWave->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabWave->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    for (int i=0; i<3; i++) {
        wave.append(new Waveform(this));
        ui->TabWave->setCellWidget(0,i,wave.at(i));
    }
    ui->LabelState->setStyleSheet("color:rgb(0,255,0);font:Bold 42pt Ubuntu;border:none;");
    ui->TextPos->setStyleSheet("color:rgb(255,0,0);border:none;font:Bold 40pt Ubuntu;");
    connect(ui->TabTest,SIGNAL(cellClicked(int,int)),this,SLOT(ItemClick(int,int)));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.11.23
 * brief:       按钮初始化
*******************************************************************************/
void CWinTest::KeyInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnWinHome,Qt::Key_0);
    btnGroup->addButton(ui->BtnWinType,Qt::Key_1);
    btnGroup->addButton(ui->BtnCmdStart,Qt::Key_3);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(KeyJudge(int)));
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.11.23
 * brief:       按钮功能
*******************************************************************************/
void CWinTest::KeyJudge(int win)
{
    switch (win) {
    case Qt::Key_0:
        emit TransformCmd(ADDR,WIN_CMD_SWITCH,"WinHome");
        break;
    case Qt::Key_1:
        emit TransformCmd(ADDR,WIN_CMD_SWITCH,"WinType");
        break;
    case Qt::Key_3:
        if (ui->BtnCmdStart->text() == "单次测试") {
            ui->BtnCmdStart->setText("中断测试");
            emit TransformCmd(ADDR,CAN_CMD_START,QString::number(0x13).toUtf8());
        } else {
            ui->BtnCmdStart->setText("单次测试");
            emit TransformCmd(ADDR,CAN_CMD_STOP,NULL);
        }
        break;
    default:
        break;
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.11.23
 * brief:       数据初始化
*******************************************************************************/
void CWinTest::DatInit()
{
    global = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");

    //当前使用的测试项目
    QString t = QString("./config/%1").arg(global->value("FileInUse","default.ini").toString());
    set = new QSettings(t,QSettings::IniFormat);
    set->setIniCodec("GB18030");

    QStringList WireColor = set->value("/GLOBAL/WireColor","").toString().split(" ");
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
    WaveClear();
    int width = set->value("/GLOBAL/Width","200").toInt();
    ui->TabTest->setColumnWidth(1,width);
    emit TransformCmd(ADDR,WIN_CMD_INIT,NULL);
}

void CWinTest::DatSave()
{
    int width = ui->TabTest->columnWidth(1);
    set->setValue("/GLOBAL/Width",width);
}
void CWinTest::ShowItems(QStringList item)
{
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
    ui->LabelState->setStyleSheet("color:rgb(0,255,0);font:Bold 42pt Ubuntu;border:none;");
    ui->LabelState->setText("--");
}

void CWinTest::ShowItem(QString item)
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

void CWinTest::ShowJudge(QString judge)
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
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.21
 * brief:       显示波形项目
*******************************************************************************/
void CWinTest::ShowWaveItem(QByteArray msg)
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
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.21
 * brief:       显示标准波形
*******************************************************************************/
void CWinTest::ShowWaveByte(QByteArray msg)
{
    for (int i=0; i<wave.size(); i++) {
        if (wave.at(i)->WaveByte.isEmpty()) {
            wave.at(i)->WaveByteShow(msg);
            break;
        }
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.21
 * brief:       显示测试波形
*******************************************************************************/
void CWinTest::ShowWaveTest(QByteArray msg)
{
    for (int i=0; i<wave.size(); i++) {
        if (wave.at(i)->WaveTest.isEmpty()) {
            wave.at(i)->WaveTestShow(msg);
            break;
        }
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.21
 * brief:       显示温度
*******************************************************************************/
void CWinTest::ShowTemp(QByteArray msg)
{
    ui->TextTestTemp->setText(msg);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.21
 * brief:       显示工位
*******************************************************************************/
void CWinTest::ShowPos(QByteArray msg)
{
    ui->TextPos->setText(msg);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.05
 * brief:       显示测试数量
*******************************************************************************/
void CWinTest::ShowAmount(QStringList amount)
{
    if (amount.size() >= 3) {
        int sum = ui->LabelSum->text().toInt()+amount.at(0).toInt();
        int qua = ui->LabelQualified->text().toInt()+amount.at(1).toInt();
        int unq = ui->LabelUnqualified->text().toInt()+amount.at(2).toInt();
        ui->LabelSum->setText(QString::number(sum));
        ui->LabelQualified->setText(QString::number(qua));
        ui->LabelUnqualified->setText(QString::number(unq));
    }
    ui->BtnCmdStart->setText("单次测试");
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.11.23
 * brief:       显示时间
*******************************************************************************/
void CWinTest::ShowTime()
{
    QString t = QTime::currentTime().toString();
    ui->TextTestTime->setText(t);
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.21
 * brief:       清空波形显示
*******************************************************************************/
void CWinTest::WaveClear()
{
    for (int i=0; i<wave.size(); i++) {
        wave.at(i)->WaveByteShow(NULL);
        wave.at(i)->WaveTestShow(NULL);
        wave.at(i)->WaveItemShow(NULL);
        QString t = QString::number(i+1);
        ui->TabWave->horizontalHeaderItem(i)->setText(t);
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.11.23
 * brief:       更新显示波形
*******************************************************************************/
void CWinTest::ItemClick(int r, int )
{
    QString t = ui->TabTest->item(r,0)->text();
    if (t.contains(tr("反嵌")) || t.contains(tr("匝间"))) {
        WaveClear();
        emit TransformCmd(ADDR,WIN_CMD_WAVE,t.toUtf8());
    }
}
/*******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.11.23
 * brief:       进入初始化
*******************************************************************************/
void CWinTest::showEvent(QShowEvent *)
{
    DatInit();
}

void CWinTest::hideEvent(QHideEvent *)
{
    DatSave();
}

