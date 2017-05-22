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
    InitWindows();
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

void WinTest::InitWindows()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->TabTest->setColumnWidth(1, 150);
    ui->TabTest->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    ui->TabWave->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabWave->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->BtnCmdStart->hide();
#else
    ui->WidgetItem->hide();
    ui->TabTest->setColumnWidth(1, 180);
    ui->TabTest->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->TabWave->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabWave->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    for (int i=0; i < 3; i++) {
        wave.append(new WaveBox(this));
        ui->TabWave->setCellWidget(0, i, wave.at(i));
        connect(wave.at(i), SIGNAL(SendVariant(QVariantHash)), this, SLOT(WaveView(QVariantHash)));
    }
    connect(ui->WaveView, SIGNAL(SendVariant(QVariantHash)), this, SLOT(WaveView(QVariantHash)));
    ui->LabelState->setStyleSheet("color:green;font:Bold 42pt Ubuntu;");
    ui->TextPos->setStyleSheet("color:white;font:Bold 42pt Ubuntu;");
    connect(ui->TabTest, SIGNAL(cellClicked(int, int)), this, SLOT(ClickItem(int, int)));
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
        GoToWindow(NULL);
        emit SendCommand(ADDR, CMD_JUMP, NULL);
        break;
    case Qt::Key_1:
        SaveSettings();
        GoToWindow("WinType");
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

    int width = g_settings->value("Width", "200").toInt();
    ui->TabTest->setColumnWidth(1, width);

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
    QSettings *settings_g = new QSettings(INI_PATH, QSettings::IniFormat);
    settings_g->setIniCodec("GB18030");
    settings_g->beginGroup("GLOBAL");
    int width = ui->TabTest->columnWidth(1);
    settings_g->setValue("Width", width);
    qDebug() << QTime::currentTime().toString() << "WinTest save OK";
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
    ui->TextTestNumb->setText(QString("编码:%1").arg(code));
    code.clear();
}

void WinTest::ClearWave()
{
    QVariantHash s;
    for (int i=0; i < wave.size(); i++) {
        wave.at(i)->ShowWave(s);
    }
}

void WinTest::ClickItem(int r,  int )
{
    if (ItemView.at(r).value("WaveItem").toString().isEmpty())
        return;
    ClearWave();
    QString uid = ItemView.at(r).value("TestUid").toString();
    for (int i=r; i < qMin(3+r, ItemView.size()); i++) {
        if (ItemView.at(i).value("TestUid") == uid)
            wave.at(i-r)->ShowWave(ItemView.at(i));
    }
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

void WinTest::showEvent(QShowEvent *e)
{
    this->setFocus();
    ShowTime();
    InitSettings();
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "InitTest");
    hash.insert("Station", "left");
    emit SendVariant(hash);
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

void WinTest::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "WinTest" && s.value("TxAddress") != "WinHome")
        return;
    if (s.value("TxCommand") == "ItemView")
        ViewItem(s);
    if (s.value("TxCommand") == "TestInit")
        InitTest(s);
    if (s.value("TxCommand") == "WaveInit")
        ClearWave();
    if (s.value("TxCommand") == "ItemShow")
        ItemShow();
    if (s.value("TxCommand") == "ItemUpdate")
        UpdateItem(s);
    if (s.value("TxCommand") == "Temperature")
        ui->TextTestTemp->setText(s.value("TxMessage").toString());
    if (s.value("TxCommand") == "TestStatus")
        TestStatus = s.value("TxMessage").toString();
    if (s.value("TxCommand") == "TestJudge")
        UpdateJudge(s.value("TxMessage").toString());
}

void WinTest::GoToWindow(QString w)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "JumpWindow");
    hash.insert("TxMessage", w);
    emit SendVariant(hash);
}

void WinTest::InitTest(QVariantHash hash)
{
    ClearWave();
    ItemView.clear();
    ui->LabelState->setText("--");
    if (hash.value("Station").toString() == "left")
        ui->TextPos->setText(tr("左"));
    if (hash.value("Station").toString() == "right")
        ui->TextPos->setText(tr("右"));
}

void WinTest::ItemShow()
{
    ui->TabTest->setRowCount(ItemView.size());
    for (int i=0; i < ItemView.size(); i++) {
        ui->TabTest->setItem(i, 0, new QTableWidgetItem);
        ui->TabTest->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        ui->TabTest->item(i, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->TabTest->item(i, 0)->setText(ItemView[i].value("TestItem").toString());

        ui->TabTest->setItem(i, 1, new QTableWidgetItem);
        ui->TabTest->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        ui->TabTest->item(i, 1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->TabTest->item(i, 1)->setText(ItemView[i].value("TestPara").toString());

        ui->TabTest->setItem(i, 2, new QTableWidgetItem);
        ui->TabTest->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        ui->TabTest->item(i, 2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->TabTest->item(i, 2)->setText(ItemView[i].value("TestResult").toString());

        ui->TabTest->setItem(i, 3, new QTableWidgetItem);
        ui->TabTest->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        ui->TabTest->item(i, 3)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->TabTest->item(i, 3)->setText(ItemView[i].value("TestJudge").toString());
    }
}

void WinTest::UpdateItem(QVariantHash hash)
{
    for (int i=0; i < ItemView.size(); i++) {
        if (ItemView[i].value("TestItem").toString() == hash.value("TestItem").toString()) {
            if (ui->TabTest->item(i, 3)->text() != " ")
                continue;
            ItemView[i] = hash;
            ui->TabTest->item(i, 2)->setText(hash.value("TestResult").toString());
            ui->TabTest->item(i, 3)->setText(hash.value("TestJudge").toString());
            if (ui->TabTest->item(i, 3)->text() == "OK")
                ui->TabTest->item(i, 3)->setTextColor(QColor(Qt::green));
            else
                ui->TabTest->item(i, 3)->setTextColor(QColor(Qt::red));
            if (!hash.value("WaveItem").toString().isEmpty())
                UpdateWave(hash);
            break;
        }
    }
}

void WinTest::ViewItem(QVariantHash hash)
{
    if (TestStatus != "pause") {
        ItemView.append(hash);
        return;
    }
    for (int i=ItemView.size()-1; i >= 0 ; i--) {
        if (ItemView[i].value("TestItem") == hash.value("TestItem")) {
            if (ui->TabTest->item(i, 3)->text() == " ")
                continue;
            ItemView[i] = hash;
            ui->TabTest->item(i, 2)->setText(hash.value("TestResult").toString());
            ui->TabTest->item(i, 3)->setText(hash.value("TestJudge").toString());
            break;
        }
    }
}

void WinTest::UpdateWave(QVariantHash hash)
{
    if (!wave.last()->WaveItem.isEmpty()) {
        ClearWave();
    }
    for (int i=0; i < wave.size(); i++) {
        if (wave.at(i)->WaveItem.isEmpty()) {
            wave.at(i)->ShowWave(hash);
            break;
        }
    }
}

void WinTest::UpdateJudge(QString judge)
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

void WinTest::WaveView(QVariantHash s)
{
    if (ui->stackedWidget->currentIndex() == 0) {
        ui->stackedWidget->setCurrentIndex(1);
        ui->WaveView->ShowWave(s);
        return;
    }
    ui->stackedWidget->setCurrentIndex(0);
}

/*********************************END OF FILE**********************************/
