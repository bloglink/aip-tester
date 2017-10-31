/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       绝缘配置
*******************************************************************************/
#include "insulation.h"

Insulation::Insulation(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Insulation::initUI()
{
    initView();
    initButton();
    initLayout();
    initDelegate();
}

void Insulation::initView()
{
    QStringList headers;
    headers << tr("交耐") << tr("高端") << tr("低端") << tr("电压")
            << tr("电阻下限") << tr("电阻上限") << tr("时间");
    itemNames << "test" << "port1" << "port2" << "volt"
              << "min" << "max" << "time";
    mView = new LQStandardItem();
    mView->setRowCount(5);
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 5; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "", Qt::DisplayRole);
        }
        mView->item(i, 0)->setCheckable(true);
    }
    view = new QTableView(this);
    view->setModel(mView);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setVisible(false);
    view->hideColumn(0);
    view->hideColumn(1);
    view->hideColumn(2);
    view->hideRow(0);
    view->hideRow(1);
    view->hideRow(2);
    view->hideRow(3);
}

void Insulation::initButton()
{
    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setMinimumSize(97, 35);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(readSettings()));

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(back()));

    btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnExit);;
}

void Insulation::initLayout()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();
    layout->addStretch();
    layout->addWidget(view);
    layout->addLayout(btnLayout);
    layout->addStretch();
    layout->addStretch();
    this->setLayout(layout);
}

void Insulation::initDelegate()
{
    QStringList items;
    items << "500" << "1000";
    LQComboBox *voltage = new LQComboBox;
    voltage->setItemNames(items);
    LQSpinBox *spinBox = new LQSpinBox;
    spinBox->setMaxinum(500);
    LQDoubleSpinBox *doubleSpinBox = new LQDoubleSpinBox;
    doubleSpinBox->setMaxinum(999.9);
    view->setItemDelegateForColumn(3, voltage);
    view->setItemDelegateForColumn(4, spinBox);
    view->setItemDelegateForColumn(5, spinBox);
    view->setItemDelegateForColumn(6, doubleSpinBox);
}


void Insulation::readSettings()
{
    QJsonObject obj;
    QStringList items = itemNames;
    for (int i=0; i < items.size(); i++) {
        QStringList temp;
        if (items.at(i) == "test") {
            for (int t=0; t < mView->rowCount(); t++) {
                if (mView->item(t, 0)->checkState() == Qt::Unchecked)
                    temp.append("0");
                else
                    temp.append("1");
            }
        } else if (items.at(i) == "volt") {
            for (int t=0; t < mView->rowCount(); t++) {
                if (mView->item(t, 3)->text() == "500")
                    temp.append("0");
                else
                    temp.append("1");
            }
            obj.insert(items.at(i), temp.join(","));
        } else {
            for (int t=0; t < mView->rowCount(); t++) {
                double x = mView->item(t, i)->text().toDouble();
                temp.append(QString::number(x));
            }
        }
        obj.insert(items.at(i), temp.join(","));
    }

    QJsonObject array;
    array.insert("IR", obj);
    emit sendAppMsg(array);
}

void Insulation::back()
{
    emit sendAppShow("config");
}

void Insulation::recvAppShow(QString win)
{
    if (win == this->objectName()) {
        // nothing
    }
}

void Insulation::recvSqlOpen(QString sql)
{
    sqlName = sql;
}

void Insulation::recvAppCmd(QJsonObject msg)
{
    QJsonObject ir = msg.value("IR").toObject();
    QStringList items = itemNames;
    for (int i=0; i < items.size(); i++) {
        QStringList temp = ir.value(items.at(i)).toString().split(",");
        if (items.at(i) == "test") {
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    mView->item(t, 0)->setCheckState(Qt::Unchecked);
                else
                    mView->item(t, 0)->setCheckState(Qt::Checked);
            }
        } else if (items.at(i) == "volt") {
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    mView->item(t, 3)->setText("500");
                else
                    mView->item(t, 3)->setText("1000");
            }
        } else {
            for (int t=0; t < temp.size(); t++)
                mView->item(t, i)->setText(temp.at(t));
        }
    }
}
