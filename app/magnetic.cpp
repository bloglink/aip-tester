/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       反嵌配置
*******************************************************************************/
#include "magnetic.h"

Magnetic::Magnetic(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Magnetic::drawWave(QString w)
{
    QStringList ws = w.split(" ");
    int row = ws.at(0).toInt();
    waveStrings[row] = w;
    ws.removeFirst();
    QVector<double> ux(ws.size()), uy(ws.size());
    for (int i=0; i < ws.size(); i++) {
        ux[i] = i;
        uy[i] = ws.at(i).toDouble()*100/256;
    }
    graphs.at(row)->setData(ux, uy);
    waves.at(row)->replot();
}

void Magnetic::parameters(QString w)
{
    QStringList ws = w.split(" ");
    int row = ws.at(0).toInt();
    if (station == 0x13) {
        areasL[row] = QString::number(ws.at(3).toInt()*256 + ws.at(4).toInt());
        freqsL[row] = QString::number(ws.at(5).toInt());
    } else if (station == 0x14) {
        areasR[row] = QString::number(ws.at(3).toInt()*256 + ws.at(4).toInt());
        freqsR[row] = QString::number(ws.at(5).toInt());
    }
}

void Magnetic::initUI()
{
    initView();
    initWave();
    initButton();
    initLayout();
    initDelegate();
}

void Magnetic::initView()
{
    QStringList headers;
    headers << tr("反嵌") << tr("端一") << tr("端二") << tr("上限") << tr("波形");
    itemNames << "test" << "port1" << "port2" << "max" << "wave";
    mView = new LQStandardItem();
    mView->setRowCount(8);
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 8; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "", Qt::DisplayRole);
        }
        mView->item(i, 0)->setCheckable(true);
    }
    view = new QTableView(this);
    view->setModel(mView);
    view->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    view->setColumnWidth(0, 50);
}

void Magnetic::initWave()
{
    for (int i=0; i < 8; i++) {
        waves.append(new QCustomPlot(this));
        waves.at(i)->setBackground(QBrush(QColor(25, 25, 25))); //设置背景色
        waves.at(i)->xAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
        waves.at(i)->yAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
        waves.at(i)->xAxis->setTicks(false);
        waves.at(i)->yAxis->setTicks(false);
        waves.at(i)->xAxis->setTickLabels(false);
        waves.at(i)->yAxis->setTickLabels(false);
        waves.at(i)->axisRect()->setMinimumMargins(QMargins(1, 5, 5, 5));
        waves.at(i)->xAxis->setBasePen(Qt::NoPen);
        waves.at(i)->yAxis->setBasePen(Qt::NoPen);
        waves.at(i)->xAxis2->setBasePen(Qt::NoPen);
        waves.at(i)->yAxis2->setBasePen(Qt::NoPen);
        waves.at(i)->xAxis->setRange(0, 400);
        waves.at(i)->yAxis->setRange(0, 101);
        graphs.append(waves.at(i)->addGraph());
        graphs.at(i)->setPen(QPen(Qt::green, 2));
        areasL.append("0");
        freqsL.append("0");
        areasR.append("0");
        freqsR.append("0");
        waveStrings[i] = QString::number(i);
    }
}

void Magnetic::initButton()
{
    turnComboBox = new QComboBox(this);
    turnComboBox->addItem(tr("CW"));
    turnComboBox->addItem(tr("CCW"));
    turnComboBox->addItem(tr("NULL"));
    turnComboBox->setMinimumSize(97, 35);
    turnComboBox->setView(new QListView);

    QPushButton *btnWaveL = new QPushButton(this);
    btnWaveL->setText(tr("采集左"));
    btnWaveL->setMinimumSize(97, 35);
    connect(btnWaveL, SIGNAL(clicked(bool)), this, SLOT(waveL()));

    QPushButton *btnWaveR = new QPushButton(this);
    btnWaveR->setText(tr("采集右"));
    btnWaveR->setMinimumSize(97, 35);
    connect(btnWaveR, SIGNAL(clicked(bool)), this, SLOT(waveR()));

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setMinimumSize(97, 35);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(readSettings()));

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(back()));

    btnLayout = new QHBoxLayout;
    btnLayout->addWidget(new QLabel(tr("旋向"), this));
    btnLayout->addWidget(turnComboBox);
    btnLayout->addStretch();
    btnLayout->addWidget(btnWaveL);
    btnLayout->addWidget(btnWaveR);
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnExit);
}

void Magnetic::initLayout()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    layout->addLayout(btnLayout);
    layout->setStretch(0, 8);
    layout->setStretch(1, 1);
    this->setLayout(layout);
}

void Magnetic::initDelegate()
{
    QStringList names;
    names << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8";
    LQComboBox *comboBox = new LQComboBox;
    comboBox->setItemNames(names);
    LQSpinBox *spinBox = new LQSpinBox;

    view->setItemDelegateForColumn(1, comboBox);
    view->setItemDelegateForColumn(2, comboBox);
    view->setItemDelegateForColumn(3, spinBox);
    for (int i=0; i < 8; i++) {
        view->setIndexWidget(mView->index(i, 4), waves.at(i));
    }
}

void Magnetic::readSettings()
{
    QJsonObject obj;
    QStringList items = itemNames;
    items << "areal" << "arear" << "freql" << "freqr";
    for (int i=0; i < items.size(); i++) {
        QStringList temp;
        if (items.at(i) == "test") {
            for (int t=0; t < mView->rowCount(); t++) {
                if (mView->item(t, 0)->checkState() == Qt::Unchecked)
                    temp.append("0");
                else
                    temp.append("1");
            }
        } else if (items.at(i) == "wave") {
            foreach(int t, waveStrings.keys()) {
                temp.append(waveStrings[t]);
            }
        }  else if (items.at(i) == "areal") {
            temp = areasL;
        }  else if (items.at(i) == "arear") {
            temp = areasR;
        }   else if (items.at(i) == "freql") {
            temp = freqsL;
        }   else if (items.at(i) == "freqr") {
            temp = freqsR;
        } else {
            for (int t=0; t < mView->rowCount(); t++) {
                double x = mView->item(t, i)->text().toDouble();
                temp.append(QString::number(x));
            }
        }
        obj.insert(items.at(i), temp.join(","));
    }

    QStringList temp;
    temp.append(QString::number(turnComboBox->currentIndex()));
    obj.insert("dir", temp.join(","));

    QJsonObject array;
    array.insert("MAG", obj);
    emit sendAppMsg(array);
}

void Magnetic::waveL()
{
    station = 0x13;
    QJsonObject msg;
    msg.insert("txCommand", MAG_WAVE);
    msg.insert("txMessage", QString("6038 %1").arg(0x13));
    emit sendAppMsg(msg);
}

void Magnetic::waveR()
{
    station = 0x14;
    QJsonObject msg;
    msg.insert("txCommand", MAG_WAVE);
    msg.insert("txMessage", QString("6038 %1").arg(0x14));
    emit sendAppMsg(msg);
}

void Magnetic::back()
{
    emit sendAppShow("config");
}

void Magnetic::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
}

void Magnetic::recvSqlOpen(QString sql)
{
    sqlName = sql;
}

void Magnetic::recvAppCmd(QJsonObject msg)
{
    QJsonObject obj = msg.value("MAG").toObject();
    QStringList items = itemNames;
    items << "dir";
    for (int i=0; i < items.size(); i++) {
        QStringList temp = obj.value(items.at(i)).toString().split(",");
        if (items.at(i) == "test") {
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    mView->item(t, 0)->setCheckState(Qt::Unchecked);
                else
                    mView->item(t, 0)->setCheckState(Qt::Checked);
            }
        } else if (items.at(i) == "wave") {
            for (int j=0; j < temp.size(); j++) {
                QStringList ws = temp.at(j).split(" ");
                if (ws.isEmpty()) {
                    continue;
                }
                int row = ws.at(0).toInt();
                waveStrings[row] = temp.at(row);
            }
            foreach(int t, waveStrings.keys()) {
                drawWave(waveStrings[t]);
            }
        } else if (items.at(i) == "areal") {
            areasL = temp;
        } else if (items.at(i) == "arear") {
            areasR = temp;
        } else if (items.at(i) == "freql") {
            freqsL = temp;
        } else if (items.at(i) == "freqr") {
            freqsR = temp;
        } else if (items.at(i) == "dir") {
            turnComboBox->setCurrentIndex(temp.at(0).toInt());
        } else {
            for (int t=0; t < temp.size(); t++)
                mView->item(t, i)->setText(temp.at(t));
        }
    }
}

