/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       测试界面
*******************************************************************************/
#include "tester.h"

Tester::Tester(QWidget *parent) : QWidget(parent)
{
    initUI();
    countAll = 0;
    countOk = 0;
}

void Tester::setMasterName(QString user)
{
    textUser->setText(QString("操作员:%1").arg(user));
}

void Tester::initItems(quint16 s)
{
    for (int i=0; i < mView->rowCount(); i++) {
        mView->item(i, 2)->setText("");
        mView->item(i, 3)->setText("");
    }
    if (s == 0x13)
        station->setText("左");
    else
        station->setText("右");
    judge->setText("ON");
    judge->setStyleSheet("font:55pt;color:yellow");
    clearWave();
}

void Tester::updateItems(QString dat)
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

void Tester::updateItem(QString item)
{
    int t = 0;
    for (int i=0; i < mView->rowCount(); i++) {
        if (!mView->item(i, 2)->text().isEmpty()) {
            t = i;
        }
    }
    mView->item(t, 2)->setText(item);
}

bool Tester::updateJudge(QString item)
{
    int t = 0;
    for (int i=0; i < mView->rowCount(); i++) {
        if (!mView->item(i, 3)->text().isEmpty()) {
            t = i;
        }
    }
    mView->item(t, 3)->setText(item);
    if (item == "OK")
        mView->item(t, 3)->setForeground(QBrush(QColor(Qt::green)));
    else
        mView->item(t, 3)->setForeground(QBrush(QColor(Qt::red)));
    return true;
}

bool Tester::updateResult(quint16 s)
{
    countAll++;
    if (s != 0) {
        judge->setStyleSheet("font:55pt;color:red");
        judge->setText("NG");
        return false;
    }
    for (int i=0; i < mView->rowCount(); i++) {
        if (mView->item(i, 3)->text() != "OK") {
            judge->setStyleSheet("font:55pt;color:red");
            judge->setText("NG");
            return false;
        }
    }
    countOk++;
    judge->setStyleSheet("font:55pt;color:green");
    judge->setText("OK");
    DrawHistogram();
    return true;
}

void Tester::updateTemp(QString s)
{
    textTemp->setText(s);
}

QString Tester::readResult()
{
    QString temp;
    for (int i=0; i < mView->rowCount(); i++) {
        temp.append(mView->item(i, 0)->text());
        temp.append("@");
        temp.append(mView->item(i, 1)->text());
        temp.append("@");
        temp.append(mView->item(i, 2)->text());
        temp.append("@");
        temp.append(mView->item(i, 3)->text());
        temp.append("\n");
    }
    textNumb->setText(QString("编码:__"));
    return temp;
}

QString Tester::readNumb()
{
    QString s = textNumb->text().remove("编码:");
    return s.remove("@");
}

void Tester::drawMAG(QString w)
{
    mag_count = mag_count%3;
    QStringList ws = w.split(" ");
    ws.removeFirst();
    QVector<double> ux(ws.size()), uy(ws.size());
    for (int i=0; i < ws.size(); i++) {
        ux[i] = i+mag_count*400;
        uy[i] = ws.at(i).toDouble()*100/256;
    }

    switch (mag_count%3) {
    case 0:
        graphs.at(3)->setData(ux, uy);
        break;
    case 1:
        graphs.at(4)->setData(ux, uy);
        break;
    case 2:
        graphs.at(5)->setData(ux, uy);
        break;
    default:
        break;
    }
    mag_count++;
    wave->replot();
}

void Tester::saveMAG(QString w)
{
    if (mag_count%3 == 0) {
        clearWave();
    }
    int row = 0;
    for (int i=0; i < mView->rowCount(); i++) {
        if (!mView->item(i, 2)->text().isEmpty()) {
            row = i+1;
        }
    }
    waveMAG[row] = w;

    labels[mag_count%3] = mView->item(row, 0)->text();

    QVector<double> ticks;
    ticks << 200 << 600 << 1000;
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    wave->xAxis->setTicker(textTicker);
}

void Tester::drawIMP(QString w)
{
    imp_count = imp_count%3;

    QStringList ws = w.split(" ");
    ws.removeFirst();
    QVector<double> ux(ws.size()), uy(ws.size());
    for (int i=0; i < ws.size(); i++) {
        ux[i] = i+imp_count*400;
        uy[i] = ws.at(i).toDouble()*100/1024;
    }

    switch (imp_count%3) {
    case 0:
        graphs.at(3)->setData(ux, uy);
        break;
    case 1:
        graphs.at(4)->setData(ux, uy);
        break;
    case 2:
        graphs.at(5)->setData(ux, uy);
        break;
    default:
        break;
    }
//    imp_count++;
    wave->replot();
}

void Tester::saveIMP(QString w)
{
    if (imp_count%3 == 0) {
        clearWave();
    }
    int row = 0;
    for (int i=0; i < mView->rowCount(); i++) {
        if (!mView->item(i, 2)->text().isEmpty()) {
            row = i+1;
        }
    }
    waveIMP[row] = w;
    labels[imp_count%3] = mView->item(row, 0)->text();

    QVector<double> ticks;
    ticks << 200 << 600 << 1000;
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    wave->xAxis->setTicker(textTicker);
}

void Tester::drawStd(QString w)
{
    imp_count = imp_count%3;
    QStringList ws = w.split(" ");
    ws.removeFirst();
    QVector<double> ux(ws.size()), uy(ws.size());
    for (int i=0; i < ws.size(); i++) {
        ux[i] = i+imp_count*400;
        uy[i] = ws.at(i).toDouble()*100/1024;
    }

    switch (imp_count%3) {
    case 0:
        graphs.at(0)->setData(ux, uy);
        break;
    case 1:
        graphs.at(1)->setData(ux, uy);
        break;
    case 2:
        graphs.at(2)->setData(ux, uy);
        break;
    default:
        break;
    }
    imp_count++;
    wave->replot();
}

void Tester::saveStd(QString w)
{
    int t = 0;
    for (int i=0; i < mView->rowCount(); i++) {
        if (!mView->item(i, 2)->text().isEmpty()) {
            t = i;
        }
    }
    waveStd[t+1] = w;
}

void Tester::initUI()
{
    initView();
    initWave();
    initStatusBar();
    initButtonBar();
    initHistogram();
    initWireColor();
    initStationBar();
    initLayout();
}

void Tester::initView()
{
    QStringList headers;
    headers << tr("测试项目") << tr("测试参数") << tr("测试结果") << tr("测试判定");
    mView = new LQStandardItem();
    mView->setHorizontalHeaderLabels(headers);

    view = new QTableView(this);
    view->setModel(mView);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    view->setColumnWidth(0, 120);
    view->setColumnWidth(1, 400);
    view->setColumnWidth(3, 120);
    view->setFocusPolicy(Qt::NoFocus);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(clickView()));
}

void Tester::initWave()
{
    wave = new QCustomPlot(this);
    wave->setBackground(QBrush(QColor(25, 25, 25))); //设置背景色
    wave->xAxis->grid()->setPen(Qt::NoPen);
    wave->yAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    wave->yAxis->setTicks(false);
    wave->yAxis->setTickLabels(false);
    wave->axisRect()->setMinimumMargins(QMargins(0, 0, 0, 0));
    wave->axisRect()->setMargins(QMargins(0, 0, 0, 0));
    wave->xAxis->setBasePen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    wave->yAxis->setBasePen(Qt::NoPen);
    wave->xAxis2->setBasePen(Qt::NoPen);
    wave->yAxis2->setBasePen(Qt::NoPen);
    wave->xAxis->setRange(0, 1200);
    wave->yAxis->setRange(0, 101);

    wave->xAxis->setTickLabelColor(Qt::white);
    wave->xAxis->setLabelColor(Qt::white);
    wave->xAxis->setTickLabelColor(Qt::white);

    QVector<double> ticks;
    ticks << 200 << 600 << 1000;
    labels << tr("项目1") << tr("项目2") << tr("项目3");
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    wave->xAxis->setTicker(textTicker);

    for (int i=0; i < 3; i++) {
        graphs.append(wave->addGraph());
        graphs.at(i)->setPen(QPen(Qt::white, 2));
    }
    for (int i=3; i < 6; i++) {
        graphs.append(wave->addGraph());
        graphs.at(i)->setPen(QPen(Qt::green, 2));
    }
    for (int i=6; i < 8; i++) {
        graphs.append(wave->addGraph());
        graphs.at(i)->setPen(QPen(QColor("#447684"), 2));
    }

    QVector<double> xx1(100), xx2(100), yy(100);
    for (int i=0; i < 100; i++) {
        xx1[i] = 400;
        xx2[i] = 800;
        yy[i] = i*2-50;
    }
    graphs.at(6)->setData(xx1, yy);
    graphs.at(7)->setData(xx2, yy);
}

void Tester::initStatusBar()
{
    textType = new QLabel("型号:__", this);
    textNumb = new QLabel("编码:__", this);
    textUser = new QLabel("操作员:__", this);
    textTemp = new QLabel("温度:__", this);

    scanner = new QTimer(this);
    connect(scanner, SIGNAL(timeout()), this, SLOT(showBarcode()));
}

void Tester::initButtonBar()
{
    btnLayout = new QVBoxLayout;
    btnLayout->setMargin(0);

    QPushButton *btnHome = new QPushButton("开机主页", this);
    btnLayout->addWidget(btnHome);
    btnHome->setObjectName("author");
    btnHome->setMinimumSize(97, 44);
    connect(btnHome, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    QPushButton *btnConf = new QPushButton("型号管理", this);
    btnLayout->addWidget(btnConf);
    btnConf->setObjectName("config");
    btnConf->setMinimumSize(97, 44);
    connect(btnConf, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    QPushButton *btnTestL = new QPushButton("开始测试L", this);
    btnLayout->addWidget(btnTestL);
    btnTestL->setMinimumSize(97, 44);
    connect(btnTestL, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    QPushButton *btnTestR = new QPushButton("开始测试R", this);
    btnLayout->addWidget(btnTestR);
    btnTestR->setMinimumSize(97, 44);
    connect(btnTestR, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    QPushButton *btnStop = new QPushButton("停止测试", this);
    btnLayout->addWidget(btnStop);
    btnStop->setMinimumSize(97, 44);
    connect(btnStop, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    QLabel *btnLogo = new QLabel(this);
    btnLayout->addWidget(btnLogo);
    btnLogo->setPixmap(QPixmap("./image/logo.png"));
    btnLogo->setScaledContents(true);
    btnLogo->setFixedHeight(70);
}

void Tester::initHistogram()
{
    histogram = new QCustomPlot(this);
    histogram->setFixedHeight(150);
    QStringList colors;
    colors << "blue" << "green" << "red";
    for (int i=0; i < 3; i++) {
        bars.append(new QCPBars(histogram->xAxis, histogram->yAxis));
        bars.at(i)->setWidth(0.9);
        bars.at(i)->setPen(Qt::NoPen);
        bars.at(i)->setBrush(QColor(colors.at(i)));
    }

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    histogram->setBackground(plotGradient);

    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3;
    labels << "ALL" << "OK" << "NG";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    histogram->xAxis->setTicker(textTicker);
    histogram->xAxis->setTickLength(0, 1);  // x轴标尺步长

    histogram->xAxis->setBasePen(Qt::NoPen);
    histogram->yAxis->setBasePen(Qt::NoPen);
    histogram->xAxis2->setBasePen(Qt::NoPen);
    histogram->yAxis2->setBasePen(Qt::NoPen);
    histogram->xAxis->grid()->setVisible(false);
    histogram->yAxis->grid()->setVisible(false);
    histogram->xAxis->setTickLabelColor(Qt::white);
    histogram->yAxis->setTicks(false);
    histogram->xAxis->setLabelColor(Qt::white);
    histogram->setBackground(QBrush(QColor(25, 25, 25))); //设置背景色
    histogram->axisRect()->setMinimumMargins(QMargins(0, 0, 0, 0));
    histogram->rescaleAxes();
    histogram->xAxis->setRange(0.5, 3.5);  // x轴范围
}

void Tester::initWireColor()
{
    colorLayout = new QGridLayout;
    for (int i=0; i < 8; i++) {
        QLabel *color = new QLabel(QString::number(i+1));
        colors.append(color);
        colorLayout->addWidget(color, i/2, i%2);
        color->setAlignment(Qt::AlignCenter);
        color->setStyleSheet("background-color:#191919");
    }
}

void Tester::initStationBar()
{
    station = new QLabel(this);
    station->setStyleSheet("font:50pt;color:yellow");
    station->setText("左");
    station->setAlignment(Qt::AlignCenter);

    judge = new QLabel(this);
    judge->setStyleSheet("font:55pt;color:green");
    judge->setText("OK");
    judge->setAlignment(Qt::AlignCenter);
}

void Tester::initLayout()
{
    QHBoxLayout *statusLayout = new QHBoxLayout;
    statusLayout->addWidget(textType);
    statusLayout->addWidget(textNumb);
    statusLayout->addWidget(textUser);
    statusLayout->addWidget(textTemp);

    QVBoxLayout *waveLayout = new QVBoxLayout;
    waveLayout->addWidget(wave);
    waveLayout->setMargin(0);

    QGroupBox *waveGroup = new QGroupBox(this);
    waveGroup->setLayout(waveLayout);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(view);
    leftLayout->addWidget(waveGroup);
    leftLayout->addLayout(statusLayout);
    leftLayout->setStretch(0, 7);
    leftLayout->setStretch(1, 3);

    QGroupBox *leftGroup = new QGroupBox(this);
    leftGroup->setLayout(leftLayout);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addLayout(btnLayout);
    rightLayout->addStretch();
    rightLayout->addWidget(histogram);
    rightLayout->addLayout(colorLayout);
    rightLayout->addWidget(station);
    rightLayout->addWidget(judge);

    QGroupBox *rightGroup = new QGroupBox(this);
    rightGroup->setLayout(rightLayout);
    rightGroup->setFixedWidth(150);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(leftGroup);
    layout->addWidget(rightGroup);
    this->setLayout(layout);
}

void Tester::initSettings()
{
    //当前使用的测试项目
    QString n = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(n, QSettings::IniFormat);
    ini->setIniCodec("GB18030");

    QStringList WireColor = ini->value("/Conf/color", "").toString().split(",");
    for (int i=0; i < qMin(colors.size(), WireColor.size()); i++) {
        colors.at(i)->setStyleSheet(QString("background-color:%1").arg(WireColor.at(i)));
    }
    textType->setText(QString("型号:%1").arg(CurrentSettings()));
}

QString Tester::CurrentSettings()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", "Base_File").toString();
    return n.remove(".ini");
}

int Tester::row()
{
    int t = 0;
    for (int i=0; i < mView->rowCount(); i++) {
        if (!mView->item(i, 2)->text().isEmpty()) {
            t = i;
        }
    }
    return t+1;
}

void Tester::showBarcode()
{
    scanner->stop();
    textNumb->setText(QString("编码:%1").arg(barcode));
    barcode.clear();
}

void Tester::keyPressEvent(QKeyEvent *e)
{
    this->setFocus();
    scanner->stop();
    barcode.append(e->text());
    e->accept();
}

void Tester::keyReleaseEvent(QKeyEvent *e)
{
    scanner->start(10);
    e->accept();
}

void Tester::clickButton()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    QJsonObject msg;
    if (btn->text() == tr("开始测试L")) {
        msg.insert("txCommand", CMD_START);
        msg.insert("txMessage", 0x13);
        emit sendAppMsg(msg);
    } else if (btn->text() == tr("开始测试R")) {
        msg.insert("txCommand", CMD_START);
        msg.insert("txMessage", 0x14);
        emit sendAppMsg(msg);
    } else if (btn->text() == tr("停止测试")) {
        msg.insert("txCommand", CMD_STOP);
        emit sendAppMsg(msg);
    } else {
        emit sendAppShow(QObject::sender()->objectName().toUtf8());
    }
}

void Tester::clickView()
{
    int row = view->currentIndex().row();
    if (row < 0)
        return;
    if (mView->item(row, 0)->text().contains(tr("空载"))) {
        //        box->setText(textLoad);
        //        box->show();
    } else if (mView->item(row, 0)->text().contains(tr("霍尔"))) {
        //        box->setText(textHall);
        //        box->show();
    } else if (mView->item(row, 0)->text().contains(tr("反嵌"))) {
        clearWave();
        for (int i=row; i < row+3; i++) {
            if (waveMAG.keys().contains(i)) {
                labels[mag_count] = mView->item(i, 0)->text();
                QVector<double> ticks;
                ticks << 200 << 600 << 1000;
                QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
                textTicker->addTicks(ticks, labels);
                wave->xAxis->setTicker(textTicker);
                QString w = waveMAG[i];
                drawMAG(w.remove(0, 2));

            }
        }
    }  else if (mView->item(row, 0)->text().contains(tr("匝间"))) {
        clearWave();
        for (int i=row; i < row+3; i++) {
            if (waveIMP.keys().contains(i)) {
                labels[imp_count] = mView->item(i, 0)->text();
                QVector<double> ticks;
                ticks << 200 << 600 << 1000;
                QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
                textTicker->addTicks(ticks, labels);
                wave->xAxis->setTicker(textTicker);
                QString w = waveIMP[i];
                drawIMP(w.remove(0, 2));
                QString s = waveStd[i];
                drawStd(s.remove(0, 2));
            }
        }
    } else {
    }
}

void Tester::clearWave()
{
    QVector<double> x(1), y(1);
    x[0] = -1;
    y[0] = -1;
    for (int i=0; i < 6; i++) {
        graphs.at(i)->setData(x, y);
    }
    mag_count = 0;
    imp_count = 0;

    QVector<double> ticks;
    ticks << 200 << 600 << 1000;
    labels.clear();
    labels << tr("项目1") << tr("项目2") << tr("项目3");
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    wave->xAxis->setTicker(textTicker);

    wave->replot();
}

void Tester::DrawHistogram()
{
    QVector<double> x1(1), y1(1);
    QVector<double> x2(1), y2(1);
    QVector<double> x3(1), y3(1);

    x1[0] = 1;
    x2[0] = 2;
    x3[0] = 3;
    if (countAll == 0) {
        y1[0] = 100;
        y2[0] = 100;
        y3[0] = 1;
        histogram->yAxis->setRange(0, 101);
    } else {
        y1[0] = countAll;
        y2[0] = countOk;
        y3[0] = countAll - countOk;
        histogram->yAxis->setRange(0, countAll+1);
    }
    bars.at(0)->setData(x1, y1);
    bars.at(1)->setData(x2, y2);
    bars.at(2)->setData(x3, y3);

    histogram->replot();
}

void Tester::recvAppShow(QString win)
{
    if (win == this->objectName()) {
        initSettings();
    }
}


