/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       匝间配置
*******************************************************************************/
#include "impulse.h"

Impulse::Impulse(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Impulse::drawWave(QString w)
{
    QStringList ws = w.split(" ");
    int row = ws.at(0).toInt();
    if (station == 0x13)
        waveStringL[row] = w;
    else
        waveStringR[row] = w;
    ws.removeFirst();
    QVector<double> ux(ws.size()), uy(ws.size());
    for (int i=0; i < ws.size(); i++) {
        ux[i] = i;
        uy[i] = ws.at(i).toDouble()*100/1024;
    }
    graphs.at(row)->setData(ux, uy);
    waves.at(row)->replot();
}

void Impulse::parameters(QString w)
{
    int f = w.toInt();
    int row = f/100;
    int ff = f%100;
    if (station == 0x13) {
        freqsL[row] = QString::number(ff);
    } else if (station == 0x14) {
        freqsR[row] = QString::number(ff);
    }
}

QString Impulse::readWaveL(QString w)
{
    QStringList ws = w.split(" ");
    int row = ws.at(0).toInt();
    return waveStringL[row];
}

QString Impulse::readWaveR(QString w)
{
    QStringList ws = w.split(" ");
    int row = ws.at(0).toInt();
    return waveStringR[row];
}

void Impulse::initUI()
{
    initView();
    initWave();
    initButton();
    initLayout();
    initDelegate();
}

void Impulse::initView()
{
    QStringList headers;
    headers << tr("反嵌") << tr("端一") << tr("端二") << tr("电压") << tr("次数")
            << tr("面积") << tr("差积") << tr("电晕") << tr("相位")
            << tr("拉伸") << tr("波形") << tr("压缩");
    itemNames << "test" << "port1" << "port2" << "volt" << "time"
              << "area" << "diff" << "flut" << "phase"
              << "stretch" << "wave" << "compress";
    mView = new LQStandardItem();
    mView->setRowCount(8);
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 8; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "", Qt::DisplayRole);
        }
        mView->item(i, 0)->setCheckable(true);
        mView->setData(mView->index(i, 9), "<", Qt::DisplayRole);
        mView->setData(mView->index(i, 11), ">", Qt::DisplayRole);
    }
    view = new QTableView(this);
    view->setModel(mView);
    view->horizontalHeader()->setSectionResizeMode(10, QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    view->setColumnWidth(0, 50);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(clickView(QModelIndex)));
}

void Impulse::initWave()
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
        waveStringL[i] = QString::number(i);
        waveStringR[i] = QString::number(i);
        freqsL.append("7");
        freqsR.append("7");
    }
}

void Impulse::initButton()
{
    vacuoComboBox = new QComboBox(this);
    vacuoComboBox->addItem(tr("非真空"));
    vacuoComboBox->addItem(tr("真空"));
    vacuoComboBox->setMinimumSize(97, 35);
    vacuoComboBox->setView(new QListView);

    QPushButton *btnWaveL = new QPushButton(this);
    btns.append(btnWaveL);
    btnWaveL->setText(tr("采集左"));
    btnWaveL->setMinimumSize(97, 35);
    connect(btnWaveL, SIGNAL(clicked(bool)), this, SLOT(waveL()));

    QPushButton *btnWaveR = new QPushButton(this);
    btns.append(btnWaveR);
    btnWaveR->setText(tr("采集右"));
    btnWaveR->setMinimumSize(97, 35);
    connect(btnWaveR, SIGNAL(clicked(bool)), this, SLOT(waveR()));

    QPushButton *btnOver = new QPushButton(this);
    btns.append(btnOver);
    btnOver->setText(tr("采集完成"));
    btnOver->setMinimumSize(97, 35);
    connect(btnOver, SIGNAL(clicked(bool)), this, SLOT(waveO()));

    QPushButton *btnSave = new QPushButton(this);
    btns.append(btnSave);
    btnSave->setText(tr("保存退出"));
    btnSave->setMinimumSize(97, 35);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(readSettings()));

    QPushButton *btnExit = new QPushButton(this);
    btns.append(btnExit);
    btnExit->setText(tr("退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(back()));

    btnLayout = new QHBoxLayout;
    btnLayout->addWidget(new QLabel("真空", this));
    btnLayout->addWidget(vacuoComboBox);
    btnLayout->addStretch();
    btnLayout->addWidget(btnWaveL);
    btnLayout->addWidget(btnWaveR);
    btnLayout->addWidget(btnOver);
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnExit);
}

void Impulse::initLayout()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    layout->addLayout(btnLayout);
    layout->setStretch(0, 8);
    layout->setStretch(1, 1);
    this->setLayout(layout);
}

void Impulse::initDelegate()
{
    QStringList names;
    names << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8";
    LQComboBox *comboBox = new LQComboBox;
    comboBox->setItemNames(names);

    LQSpinBox *voltage = new LQSpinBox;
    voltage->setMininum(300);
    voltage->setMaxinum(3000);

    LQSpinBox *times = new LQSpinBox;
    times->setMaxinum(9);

    LQSpinBox *spinBox = new LQSpinBox;

    LQSpinBox *flut = new LQSpinBox;
    flut->setMaxinum(65535);

    view->setItemDelegateForColumn(1, comboBox);
    view->setItemDelegateForColumn(2, comboBox);
    view->setItemDelegateForColumn(3, voltage);
    view->setItemDelegateForColumn(4, times);
    view->setItemDelegateForColumn(5, spinBox);
    view->setItemDelegateForColumn(6, spinBox);
    view->setItemDelegateForColumn(7, flut);
    view->setItemDelegateForColumn(8, spinBox);
    view->setItemDelegateForColumn(9, new LQReadOnlyItem);
    view->setItemDelegateForColumn(11, new LQReadOnlyItem);

    for (int i=0; i < 8; i++) {
        view->setIndexWidget(mView->index(i, 10), waves.at(i));
    }
}

void Impulse::readSettings()
{
    QJsonObject obj;
    QStringList items = itemNames;
    items << "freqsL" << "freqsR" << "waveR";
    for (int i=0; i < items.size(); i++) {
        QStringList temp;
        if (items.at(i) == "test") {
            for (int t=0; t < mView->rowCount(); t++) {
                if (mView->item(t, 0)->checkState() == Qt::Unchecked)
                    temp.append("0");
                else
                    temp.append("1");
            }
        } else if (items.at(i) == "stretch") {
        } else if (items.at(i) == "compress") {
        } else if (items.at(i) == "freqsL") {
            temp = freqsL;
        }  else if (items.at(i) == "freqsR") {
            temp = freqsR;
        } else if (items.at(i) == "wave") {
            foreach(int t, waveStringL.keys()) {
                temp.append(waveStringL[t]);
            }
        }  else if (items.at(i) == "waveR") {
            foreach(int t, waveStringR.keys()) {
                temp.append(waveStringR[t]);
            }
        } else {
            for (int t=0; t < mView->rowCount(); t++) {
                double x = mView->item(t, i)->text().toDouble();
                temp.append(QString::number(x));
            }
        }
        obj.insert(items.at(i), temp.join(","));
    }

    QStringList temp;
    temp.append(QString::number(vacuoComboBox->currentIndex()));
    obj.insert("vacuo", temp.join(","));

    QJsonObject array;
    array.insert("IMP", obj);
    emit sendAppMsg(array);
    back();
}

void Impulse::clickView(QModelIndex index)
{
    int column = index.column();
    int row = index.row();
    QJsonObject msg;
    msg.insert("txCommand", IMP_WAVE);

    switch (column) {
    case 9:
        msg.insert("txMessage", QString("6056 %1").arg(row*10+1));
        emit sendAppMsg(msg);
        break;
    case 11:
        msg.insert("txMessage", QString("6056 %1").arg(row*10+3));
        emit sendAppMsg(msg);
        break;
    default:
        break;
    }
}

void Impulse::waveL()
{
    clearWave();
    station = 0x13;
    int vacuo = vacuoComboBox->currentIndex();
    QJsonObject msg;
    msg.insert("txCommand", IMP_WAVE);
    msg.insert("txMessage", QString("6041 %1 %2").arg(0x13).arg(vacuo));
    emit sendAppMsg(msg);

    for (int i=0; i < btns.size(); i++) {
        btns.at(i)->setEnabled(false);
    }
    btns.at(2)->setEnabled(true);
}

void Impulse::waveR()
{
    clearWave();
    station = 0x14;
    int vacuo = vacuoComboBox->currentIndex();
    QJsonObject msg;
    msg.insert("txCommand", IMP_WAVE);
    msg.insert("txMessage", QString("6041 %1 %2").arg(0x14).arg(vacuo));
    emit sendAppMsg(msg);
    for (int i=0; i < btns.size(); i++) {
        btns.at(i)->setEnabled(false);
    }
    btns.at(2)->setEnabled(true);
}

void Impulse::waveO()
{
    QJsonObject msg;
    msg.insert("txCommand", IMP_WAVE);
    msg.insert("txMessage", QString("6046"));
    emit sendAppMsg(msg);
    for (int i=0; i < btns.size(); i++) {
        btns.at(i)->setEnabled(true);
    }
}

void Impulse::clearWave()
{
    QVector<double> x(1), y(1);
    x[0] = -1;
    y[0] = -1;
    for (int i=0; i < graphs.size(); i++) {
        graphs.at(i)->setData(x, y);
        waves.at(i)->replot();
    }
}

void Impulse::back()
{
    emit sendAppShow("config");
}

void Impulse::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
}

void Impulse::recvSqlOpen(QString sql)
{
    sqlName = sql;
}

void Impulse::recvAppCmd(QJsonObject msg)
{
    QJsonObject obj = msg.value("IMP").toObject();
    QStringList items = itemNames;
    items << "vacuo";
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
                waveStringL[row] = temp.at(row);
            }
            foreach(int t, waveStringL.keys()) {
                drawWave(waveStringL[t]);
            }
        }  else if (items.at(i) == "waveL") {
            for (int j=0; j < temp.size(); j++) {
                QStringList ws = temp.at(j).split(" ");
                if (ws.isEmpty()) {
                    continue;
                }
                int row = ws.at(0).toInt();
                waveStringR[row] = temp.at(row);
            }
        } else if (items.at(i) == "vacuo") {
            vacuoComboBox->setCurrentIndex(temp.at(0).toInt());
        } else if (items.at(i) == "stretch") {
        } else if (items.at(i) == "compress") {
        } else {
            for (int t=0; t < temp.size(); t++)
                mView->item(t, i)->setText(temp.at(t));
        }
    }
}
