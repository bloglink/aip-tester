/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       测试显示模块
*******************************************************************************/
#include "WinTest.h"
#include "ui_WinTest.h"

WinTest::WinTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinTest)
{
    ui->setupUi(this);
    initUI();
    InitButtons();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ShowTime()));
    timer->start(30000);
    codeTimer = new QTimer(this);
    connect(codeTimer, SIGNAL(timeout()), this, SLOT(ShowCode()));
}

WinTest::~WinTest()
{
    delete ui;
}

void WinTest::initItems(quint16 station)
{
    for (int i=0; i < mView->rowCount(); i++) {
        mView->item(i, 2)->setText("");
        mView->item(i, 3)->setText("");
    }
    qDebug() << station;
}

void WinTest::updateItems(QString dat)
{
    QDomDocument docs;
    docs.setContent(dat);
    if (!docs.elementsByTagName("Test_Data_Param").isEmpty()) {
        mView->setRowCount(0);
        QDomNodeList list = docs.elementsByTagName("Test_Data_Param").at(0).childNodes();
        for (int i=0; i < list.size(); i++) {
            QDomElement dom = list.at(i).toElement();
            QStringList temp = dom.text().split(",");
            if (dom.nodeName() == "Test_1") {
                for (int i=0; i < temp.size(); i++) {
                    mView->appendRow(new QStandardItem(temp.at(i)));
                    mView->setItem(i, 1, new QStandardItem);
                    mView->setItem(i, 2, new QStandardItem);
                    mView->setItem(i, 3, new QStandardItem);
                }
            }
            if (dom.nodeName() == "Test_2") {
                for (int i=0; i < qMin(temp.size(), mView->rowCount()); i++)
                    mView->setItem(i, 1, new QStandardItem(temp.at(i)));
            }
        }
    }
    if (!docs.elementsByTagName("Test_Data_Result").isEmpty()) {
        QDomNodeList list = docs.elementsByTagName("Test_Data_Result").at(0).childNodes();
        for (int i=0; i < list.size(); i++) {
            QDomElement dom = list.at(i).toElement();
            QString temp = dom.text();
            if (dom.nodeName() == "Test_3") {
                for (int i=0; i < mView->rowCount(); i++) {
                    if (mView->item(i, 3)->text().isEmpty()) {
                        mView->item(i, 2)->setText(temp);
                        break;
                    }
                }
            }
        }
    }
    if (!docs.elementsByTagName("Test_Data_Judge").isEmpty()) {
        QDomNodeList list = docs.elementsByTagName("Test_Data_Judge").at(0).childNodes();
        for (int i=0; i < list.size(); i++) {
            QDomElement dom = list.at(i).toElement();
            QString temp = dom.text();
            if (dom.nodeName() == "Test_3") {
                for (int i=0; i < mView->rowCount(); i++) {
                    if (mView->item(i, 3)->text().isEmpty()) {
                        mView->item(i, 3)->setText(temp);
                        if (temp == "OK")
                            mView->item(i, 3)->setForeground(QBrush(QColor(Qt::green)));
                        else
                            mView->item(i, 3)->setForeground(QBrush(QColor(Qt::red)));
                        break;
                    }
                }
            }
        }
    }
}

bool WinTest::updateResult()
{
//    for (int i=0; i < mView->rowCount(); i++) {
//        if (mView->item(i, 3)->text() != "OK") {
//            judge->setStyleSheet("font:55pt;color:red");
//            judge->setText("NG");
//            qDebug() << i;
//            return false;
//        }
//    }
//    judge->setStyleSheet("font:55pt;color:green");
//    judge->setText("OK");
    return true;
}

void WinTest::initUI()
{
    QStringList headers;
    headers << tr("测试项目") << tr("测试参数") << tr("测试结果") << tr("测试判定");
    mView = new StandardItem(0, headers.size());
    mView->setHorizontalHeaderLabels(headers);

    ui->view->setModel(mView);
    ui->view->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->view->setColumnWidth(0, 150);
    ui->view->setColumnWidth(1, 250);
    ui->view->setColumnWidth(3, 450);

    ui->wave->setBackground(QBrush(QColor(19, 19, 19))); //设置背景色
    ui->wave->xAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    ui->wave->yAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    ui->wave->xAxis->setTicks(false);
    ui->wave->yAxis->setTicks(false);
    ui->wave->xAxis->setTickLabels(false);
    ui->wave->yAxis->setTickLabels(false);
    ui->wave->axisRect()->setMinimumMargins(QMargins(0, 0, 0, 0));
    ui->wave->axisRect()->setupFullAxesBox();
    ui->wave->rescaleAxes();
    ui->wave->addGraph();
    ui->wave->xAxis->setRange(0, 400);
    ui->wave->yAxis->setRange(0, 250);
    ui->wave->graph(0)->setPen(QPen(Qt::green));
//    DrawWave();

    ui->LabelState->setStyleSheet("color:green;font:Bold 42pt Ubuntu;");
    ui->TextPos->setStyleSheet("color:white;font:Bold 42pt Ubuntu;");
}

void WinTest::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnWinHome, Qt::Key_0);
    btnGroup->addButton(ui->BtnWinType, Qt::Key_1);
    btnGroup->addButton(ui->BtnCmdStart, Qt::Key_3);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void WinTest::ReadButtons(int win)
{
    switch (win) {
    case Qt::Key_0:
        SaveSettings();
        emit SendCommand(ADDR, CMD_JUMP, NULL);
        break;
    case Qt::Key_1:
        SaveSettings();
        emit SendCommand(ADDR, CMD_JUMP, "WinType");
        break;
    case Qt::Key_3:
        if (ui->BtnCmdStart->text() == "单次测试") {
            ui->BtnCmdStart->setText("中断测试");
            emit SendCommand(ADDR, CMD_START, QString("%1 %2").arg(0x13).arg(0x00).toUtf8());
        } else {
            ui->BtnCmdStart->setText("单次测试");
            emit SendCommand(ADDR, CMD_START, QString("%1 %2").arg(0x13).arg(0x00).toUtf8());
        }
        break;
    default:
        break;
    }
}

void WinTest::InitSettings()
{
    QSettings *g_settings = new QSettings(INI_PATH, QSettings::IniFormat);
    g_settings->setIniCodec("GB18030");
    g_settings->beginGroup("GLOBAL");

//    int width = g_settings->value("Width", "200").toInt();
//    ui->TabTest->setColumnWidth(1, width);

    int user = g_settings->value("User", "0").toInt();
    if (user == 0)
        ui->TextTestUser->setText(tr("用户:操作员"));
    if (user == 1)
        ui->TextTestUser->setText(tr("用户:管理员"));

    motor_type = g_settings->value("FileInUse", INI_DEFAULT).toString();
    motor_type.remove(".ini");
    ui->TextTestType->setText(QString("型号:%1").arg(motor_type));
    //当前使用的测试项目
    QString n = QString("./config/%1.ini").arg(motor_type);
    QSettings *c_settings = new QSettings(n, QSettings::IniFormat);
    c_settings->setIniCodec("GB18030");

    QStringList WireColor = c_settings->value("/GLOBAL/WireColor", "").toString().split(" ");
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

    qDebug() << QTime::currentTime().toString() << "WinTest read OK";
}

void WinTest::SaveSettings()
{
//    QSettings *settings_g = new QSettings(INI_PATH, QSettings::IniFormat);
//    settings_g->setIniCodec("GB18030");
//    settings_g->beginGroup("GLOBAL");
//    int width = ui->TabTest->columnWidth(1);
//    settings_g->setValue("Width", width);
//    qDebug() << QTime::currentTime().toString() << "WinTest save OK";
}

void WinTest::ShowItem(QString item)
{
//    ui->TabTest->clearFocus();
//    QStringList s = item.split("@");
//    for (int i=0; i < ui->TabTest->rowCount(); i++) {
//        QString n = ui->TabTest->item(i, 0)->text();
//        QString t = ui->TabTest->item(i, 3)->text();
//        if (t != " ")
//            continue;
//        if (s.at(0) == n) {
//            int v = qMax(ui->TabTest->verticalScrollBar()->value(), i-10);
//            ui->TabTest->verticalScrollBar()->setValue(v);
//            ui->TabTest->item(i, 2)->setText(s.at(2));
//            ui->TabTest->item(i, 3)->setText(s.at(3));
//            if (s.at(3) == "NG")
//                ui->TabTest->item(i, 3)->setTextColor(QColor(Qt::red));
//            else
//                ui->TabTest->item(i, 3)->setTextColor(QColor(Qt::green));
//            break;
//        }
//    }
}

void WinTest::ReplaceItem(QString item)
{
//    QStringList t = item.split("\n");
//    for (int i=0; i < t.size(); i++) {
//        QStringList s = t.at(i).split("@");
//        for (int i=0; i < ui->TabTest->rowCount(); i++) {
//            QString n = ui->TabTest->item(i, 0)->text();
//            if (s.at(0) == n) {
//                ui->TabTest->item(i, 2)->setText(" ");
//                ui->TabTest->item(i, 3)->setText(" ");
//                break;
//            }
//        }
//    }
}

void WinTest::ShowJudge(QString judge)
{
    int sum = ui->LabelSum->text().toInt();
    int qua = ui->LabelQualified->text().toInt();
    int unq = ui->LabelUnqualified->text().toInt();
    sum++;
    if (judge == "NG") {
        ui->LabelState->setStyleSheet("color:rgb(255, 0, 0);font: Bold 42pt Ubuntu;border: none;");
        ui->LabelState->setText("NG");
        unq++;
    } else {
        ui->LabelState->setStyleSheet("color:rgb(0, 255, 0);font:Bold 42pt Ubuntu;border:none;");
        ui->LabelState->setText("OK");
        qua++;
    }
    ui->LabelSum->setText(QString::number(sum));
    ui->LabelQualified->setText(QString::number(qua));
    ui->LabelUnqualified->setText(QString::number(unq));
    ui->BtnCmdStart->setText("单次测试");
}

void WinTest::ShowWaveItem(QByteArray msg)
{
//    if (!ui->wave.last()->WaveItem.isEmpty()) {
//        ClearWave();
//    }
//    for (int i=0; i < ui->wave.size(); i++) {
//        if (ui->wave.at(i)->WaveItem.isEmpty()) {
//            ui->wave.at(i)->WaveItemShow(msg);
//            break;
//        }
//    }
}

void WinTest::ShowWaveByte(QByteArray msg)
{
//    for (int i=0; i < ui->wave.size(); i++) {
//        if (ui->wave.at(i)->WaveByte.isEmpty()) {
//            ui->wave.at(i)->WaveByteShow(msg);
//            break;
//        }
//    }
}

void WinTest::ShowWaveTest(QByteArray msg)
{
//    for (int i=0; i < ui->wave.size(); i++) {
//        if (ui->wave.at(i)->WaveTest.isEmpty()) {
//            ui->wave.at(i)->WaveTestShow(msg);
//            break;
//        }
//    }
}

void WinTest::ShowTemperature(QByteArray msg)
{
    ui->TextTestTemp->setText(msg);
}

void WinTest::ShowStation(QByteArray msg)
{
    if (msg.toInt() == 0x13)
        ui->TextPos->setText(tr("左"));
    if (msg.toInt() == 0x14)
        ui->TextPos->setText(tr("右"));
}

void WinTest::ShowTime()
{
    QString t = QTime::currentTime().toString("hh:mm");
    ui->TextTestTime->setText(t);
}

void WinTest::ShowCode()
{
    codeTimer->stop();
    code = code.mid(BarCode1().toInt(), BarCode2().toInt()-BarCode1().toInt());
    if (code.size() < 2)
        return;
    QString s = "code" + code;
    emit SendCommand(ADDR, CMD_CODE, s.toUtf8());
    ui->TextTestNumb->setText(QString("编码:%1").arg(code));
    code.clear();
}

void WinTest::ClearWave()
{
//    for (int i=0; i < wave.size(); i++) {
//        wave.at(i)->WaveByteShow(NULL);
//        wave.at(i)->WaveTestShow(NULL);
//        wave.at(i)->WaveItemShow(NULL);
//        QString t = QString::number(i+1);
//        ui->TabWave->horizontalHeaderItem(i)->setText(t);
//    }
}

void WinTest::ClickItem(int r,  int )
{
//    QString t = ui->TabTest->item(r, 0)->text();
//    if (t.contains(tr("反嵌")) || t.contains(tr("匝间")) || t.contains(tr("PG"))) {
//        ClearWave();
//        emit SendCommand(ADDR, CMD_WAVE, t.toUtf8());
//    }
}

void WinTest::ClickWave(QModelIndex)
{
    qDebug() << "wave";
    ui->stackedWidget->setCurrentIndex(1);
}

void WinTest::ClickWave(int x)
{
    if (x < 0)
        return;
    ui->stackedWidget->setCurrentIndex(0);
}

void WinTest::ClickWave0(int x)
{
//    if (x < 0)
//        return;
//    ui->stackedWidget->setCurrentIndex(1);
//    ui->LabelWave->WaveByteShow(ui->wave.at(0)->WaveByte);
//    ui->LabelWave->WaveTestShow(ui->wave.at(0)->WaveTest);
//    ui->LabelWave->WaveItemShow(ui->wave.at(0)->WaveItem);
}

void WinTest::ClickWave1(int x)
{
//    if (x < 0)
//        return;
//    ui->stackedWidget->setCurrentIndex(1);
//    ui->LabelWave->WaveByteShow(ui->wave.at(1)->WaveByte);
//    ui->LabelWave->WaveTestShow(ui->wave.at(1)->WaveTest);
//    ui->LabelWave->WaveItemShow(ui->wave.at(1)->WaveItem);
}

void WinTest::ClickWave2(int x)
{
//    if (x < 0)
//        return;
//    ui->stackedWidget->setCurrentIndex(1);
//    ui->LabelWave->WaveByteShow(ui->wave.at(2)->WaveByte);
//    ui->LabelWave->WaveTestShow(ui->wave.at(2)->WaveTest);
//    ui->LabelWave->WaveItemShow(ui->wave.at(2)->WaveItem);
}

QString WinTest::BarCode1()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    return ini->value("/GLOBAL/Barcode1", "0").toString();
}

QString WinTest::BarCode2()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    return ini->value("/GLOBAL/Barcode2", "9").toString();
}

void WinTest::ReadMessage(quint16 addr,  quint16 cmd,  QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_TEST)
        return;
    switch (cmd) {
    case CMD_INIT:
        ClearWave();
        InitItem(msg);
        InitSettings();
        break;
    case CMD_ITEM:
        ShowItem(msg);
        break;
    case CMD_TEMP:
        ShowTemperature(msg);
        break;
    case CMD_WAVE_BYTE:
        ShowWaveByte(msg);
        break;
    case CMD_WAVE_TEST:
        ShowWaveTest(msg);
        break;
    case CMD_WAVE_ITEM:
        ShowWaveItem(msg);
        break;
    case CMD_WAVE_HIDE:
        ClearWave();
        break;
    case CMD_START:
        ShowStation(msg);
        break;
    case CMD_JUDGE:
        ShowJudge(msg);
        break;
    case CMD_ITEM_REPLACE:
        ReplaceItem(msg);
        break;
    default:
        break;
    }
}

void WinTest::InitItem(QByteArray msg)
{
//    ui->TabTest->verticalScrollBar()->setValue(0);
//    ui->LabelState->setStyleSheet("color:rgb(0, 255, 0);font:Bold 42pt Ubuntu;border:none;");
//    ui->LabelState->setText("--");
//    if (msg.isEmpty()) {
//        ui->TabTest->setRowCount(0);
//        return;
//    }
//    QStringList item = QString(msg).split("\n");
//    ui->TabTest->setRowCount(item.size());
//    for (int i=0; i < item.size(); i++) {
//        QStringList s = QString(item.at(i)).split("@");
//        ui->TabTest->setItem(i, 0, new QTableWidgetItem);
//        ui->TabTest->item(i, 0)->setTextAlignment(Qt::AlignCenter);
//        ui->TabTest->item(i, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
//        ui->TabTest->item(i, 0)->setText(s.at(0));

//        ui->TabTest->setItem(i, 1, new QTableWidgetItem);
//        ui->TabTest->item(i, 1)->setTextAlignment(Qt::AlignCenter);
//        ui->TabTest->item(i, 1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
//        ui->TabTest->item(i, 1)->setText(s.at(1));

//        ui->TabTest->setItem(i, 2, new QTableWidgetItem);
//        ui->TabTest->item(i, 2)->setTextAlignment(Qt::AlignCenter);
//        ui->TabTest->item(i, 2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
//        ui->TabTest->item(i, 2)->setText(s.at(2));

//        ui->TabTest->setItem(i, 3, new QTableWidgetItem);
//        ui->TabTest->item(i, 3)->setTextAlignment(Qt::AlignCenter);
//        ui->TabTest->item(i, 3)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
//        ui->TabTest->item(i, 3)->setText(s.at(3));
//    }
}

void WinTest::showEvent(QShowEvent *e)
{
    this->setFocus();
    ShowTime();
    InitSettings();
    emit SendCommand(ADDR, CMD_INIT, NULL);
    e->accept();
}

void WinTest::keyPressEvent(QKeyEvent *e)
{
    codeTimer->stop();
    code.append(e->text());
    e->accept();
}

void WinTest::keyReleaseEvent(QKeyEvent *e)
{
    codeTimer->start(10);
    e->accept();
}
void WinTest::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    emit sendNetMsg("6008");
    emit sendNetMsg("6004 Sys");
}
/*********************************END OF FILE**********************************/
