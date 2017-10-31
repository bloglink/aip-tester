/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       型号管理
*******************************************************************************/
#include "config.h"

Config::Config(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Config::initUI()
{
    initType();
    initTypeBar();
    initTest();
    initTestBar();
    initDisplay();
    initLayout();
}

void Config::initType()
{
    viewLayout = new QVBoxLayout;

    QStringList headers;
    headers << tr("电机型号");
    mType = new LQStandardItem;
    mType->setHorizontalHeaderLabels(headers);

    type = new QTableView(this);
    type->setModel(mType);
    type->setEditTriggers(QAbstractItemView::NoEditTriggers);
    type->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(type, SIGNAL(clicked(QModelIndex)), this, SLOT(clickType(QModelIndex)));

    viewLayout->addWidget(type);
}

void Config::initTypeBar()
{
    typeLineEdit = new QLineEdit(this);
    typeLineEdit->setMinimumSize(97, 35);
    typeLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QRegExp rx1;
    rx1.setPattern("^[-|0-9|A-Z|a-z|^\s]{1,50}$"); // 限制接受1至50个字符,减号、数字和英文字母
    QValidator *validator_16c = new QRegExpValidator(rx1, this);
    typeLineEdit->setValidator(validator_16c);

    QPushButton *btnAppend = new QPushButton(this);
    btnAppend->setText(tr("添加型号"));
    btnAppend->setMinimumSize(97, 35);
    connect(btnAppend, SIGNAL(clicked(bool)), this, SLOT(appendType()));
    QPushButton *btnRemove = new QPushButton(this);
    btnRemove->setText(tr("删除型号"));
    btnRemove->setMinimumSize(97, 35);
    connect(btnRemove, SIGNAL(clicked(bool)), this, SLOT(deleteType()));
    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(back()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(new QLabel("型号名称", this));
    btnLayout->addWidget(typeLineEdit);
    btnLayout->addWidget(btnAppend);
    btnLayout->addWidget(btnRemove);
    btnLayout->addStretch();
    btnLayout->addWidget(btnExit);

    viewLayout->addLayout(btnLayout);
}

void Config::initTest()
{
    testLayout = new QHBoxLayout;

    QStringList headers;
    headers << tr("测试项目");
    mTest = new LQStandardItem;
    mTest->setHorizontalHeaderLabels(headers);
    mTest->appendRow(new QStandardItem(""));

    test = new QTableView(this);
    test->setModel(mTest);
    test->setEditTriggers(QAbstractItemView::NoEditTriggers);
    test->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(test, SIGNAL(clicked(QModelIndex)), this, SLOT(clickTest(QModelIndex)));

    testLayout->addWidget(test);
}

void Config::initTestBar()
{
    QVBoxLayout *btnsLayout = new QVBoxLayout;

    item = new QTableView(this);
    item->verticalHeader()->hide();
    item->setItemDelegate(new LQReadOnlyItem(this));
    item->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(item, SIGNAL(clicked(QModelIndex)), this, SLOT(clickItem(QModelIndex)));

    btnsLayout->addWidget(item);
    QPushButton *btnDelete = new QPushButton("删除项目", this);
    btnDelete->setMinimumSize(97, 35);
    connect(btnDelete, SIGNAL(clicked(bool)), this, SLOT(deleteTest()));
    btnsLayout->addWidget(btnDelete);
    QPushButton *btnConfig = new QPushButton("配置项目", this);
    btnConfig->setMinimumSize(97, 35);
    connect(btnConfig, SIGNAL(clicked(bool)), this, SLOT(configTest()));
    btnsLayout->addWidget(btnConfig);
    btnsLayout->addStretch();
    btnsLayout->setMargin(0);

    testLayout->addLayout(btnsLayout);
}

void Config::initDisplay()
{
    QDir dir("./types");
    QStringList typeNames =  dir.entryList(QDir::Files);
    for (int i=0; i < typeNames.size(); i++) {
        typeNames[i].remove(".jpg");
    }
    if (typeNames.isEmpty())
        typeNames.append("None");
    typeComboBox = new QComboBox(this);
    typeComboBox->addItems(typeNames);
    typeComboBox->setMinimumSize(97, 35);
    typeComboBox->setView(new QListView);
    connect(typeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(autoPixmap(QString)));
    typePixmap = new QLabel(this);
    autoPixmap(typeNames.at(0));

    QStringList testNames;
    testNames << tr("继续测试") << tr("暂停询问");
    testComboBox = new QComboBox(this);
    testComboBox->addItems(testNames);
    testComboBox->setMinimumSize(97, 35);
    testComboBox->setView(new QListView);

    QGridLayout *cLayout = new QGridLayout;
    for (int i=0; i < 8; i++) {
        QPushButton *btn = new QPushButton(QString::number(i+1));
        colors.append(btn);
        cLayout->addWidget(btn, i/2, i%2);
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(selectColor()));
    }

    tLayout = new QGridLayout;
    tLayout->addWidget(new QLabel("电机类型", this), 0, 0);
    tLayout->addWidget(typeComboBox, 0, 1);
    tLayout->addWidget(typePixmap, 1, 0, 1, 2);
    tLayout->addWidget(new QLabel("不合格处理", this), 2, 0);
    tLayout->addWidget(testComboBox, 2, 1);
    tLayout->addWidget(new QLabel("线夹颜色", this), 3, 0);
    tLayout->addLayout(cLayout, 4, 0, 1, 2);
    tLayout->setColumnStretch(1, 1);
    tLayout->setRowStretch(5, 1);
}

void Config::initLayout()
{
    QGroupBox *viewGroup = new QGroupBox(this);
    viewGroup->setLayout(viewLayout);

    QGroupBox *typeGroup = new QGroupBox(this);
    typeGroup->setLayout(testLayout);

    QGroupBox *tGroup = new QGroupBox(this);
    tGroup->setLayout(tLayout);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(tGroup);
    layout->addWidget(typeGroup);
    layout->addWidget(viewGroup);
    this->setLayout(layout);
    layout->setStretch(2, 1);
}


void Config::readSettings()
{
    QJsonObject obj;
    QStringList temp;
    for (int i=0; i < colors.size(); i++) {
        QPalette palette = colors.at(i)->palette();
        temp.append(palette.color(QPalette::Background).name());
    }
    obj.insert("color", temp.join(","));

    temp.clear();
    temp.append(typeComboBox->currentText());
    obj.insert("type", temp.join(","));

    temp.clear();
    temp.append(QString::number(testComboBox->currentIndex()));
    obj.insert("test", temp.join(","));

    QJsonObject array;
    array.insert("Conf", obj);
    emit sendAppMsg(array);
}

void Config::readSysItems()
{
    QJsonObject obj;
    QStringList temp;
    for (int i=0; i < mTest->rowCount(); i++) {
        QString name = mTest->item(i, 0)->text();
        for (int j=0; j < mItem->rowCount(); j++) {
            if (name == mItem->index(j, 1).data().toString())
                temp.append(mItem->index(j, 0).data().toString());
        }
    }
    if (temp.isEmpty())
        temp.append("1");
    testItem = temp;
    obj.insert("Test_Item", temp.join(","));

    QJsonObject array;
    array.insert("Sys", obj);
    emit sendAppMsg(array);
}

void Config::clickType(QModelIndex index)
{
    int row = index.row();
    QString name = mType->item(row, 0)->text();

    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("GLOBAL");
    ini->setValue("FileInUse", name);

    updateType();
    emit updateSettings();
}

void Config::clickItem(QModelIndex index)
{
    QString itemName = mItem->data(index, Qt::DisplayRole).toString();
    for (int i=0; i < mTest->rowCount(); i++) {
        if (mTest->item(i, 0)->text() == itemName)
            return;
    }
    if (test->currentIndex().row() < 0 || (test->currentIndex().row()+1 == mTest->rowCount())) {
        mTest->appendRow(new QStandardItem(""));
        if (mTest->rowCount() > 1)
            mTest->item(mTest->rowCount()-2, 0)->setText(itemName);
    } else {
        mTest->item(test->currentIndex().row(), 0)->setText(itemName);
    }
    readSysItems();
    test->setCurrentIndex(mTest->index(test->currentIndex().row()+1, 0));
}

void Config::clickTest(QModelIndex index)
{
    QString temp = index.data().toString();
    for (int i=0; i < mItem->rowCount(); i++) {
        if (mItem->index(i, 1).data(Qt::DisplayRole).toString() == temp) {
            item->setCurrentIndex(mItem->index(i, 1));
        }
    }
}

void Config::deleteTest()
{
    if (mTest->rowCount() <= 2) {
        QMessageBox::warning(this, "", "请至少保留一项", QMessageBox::Ok);
        return;
    }
    int row = test->currentIndex().row();
    if (row < 0)
        return;
    if (row+1 == mTest->rowCount())
        return;
    mTest->removeRow(row);
    readSysItems();
}

void Config::configTest()
{
    int row = item->currentIndex().row();
    if (row < 0)
        return;
    int t = mItem->index(row, 0).data().toInt();
    QStringList items;
    items << "x" << "dcr" << "mag" << "ir" << "acw" << "x"
          << "imp" << "x" << "ConfInductance" << "x" << "x" << "ConfHall"
          << "ConfLoadTesting" << "ConfNoLoadTest"  << "ConfBackForce";
    emit sendAppShow(items.at(t));
}

void Config::selectColor()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    QColor color = QColorDialog::getColor(Qt::white, this);
    if (color.isValid())
        btn->setStyleSheet(QString("background-color:%1").arg(color.name()));
}

void Config::autoPixmap(QString name)
{
    QString pixmap = QString("./types/%1.jpg").arg(name);
    typePixmap->setPixmap(QPixmap(pixmap));
}


void Config::appendType()
{
    //    if (CurrentUser() == "0") {
    //        QMessageBox::warning(this, "", "权限不足", QMessageBox::Ok);
    //        return;
    //    }
    QString name = typeLineEdit->text();
    if (name.isEmpty())
        return;
    QString c = mType->item(0, 0)->text();
    QString Source = QString("./config/%1.ini").arg(c);
    QString Target = QString("./config/%1.ini").arg(name);
    QFile *s = new QFile(Source);
    s->copy(Target);
    s->close();
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("GLOBAL");
    ini->setValue("FileInUse", name);

    updateType();
    emit updateSettings();
}

void Config::deleteType()
{
    //    if (CurrentUser() == "0") {
    //        QMessageBox::warning(this, "", "权限不足", QMessageBox::Ok);
    //        return;
    //    }
    QString name = mType->item(0, 0)->text();
    if (mType->rowCount() == 1)
        return;

    QString Target = QString("./config/%1.ini").arg(name);
    QFile::remove(Target);

    name = mType->item(1, 0)->text();
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("GLOBAL");
    ini->setValue("FileInUse", name);

    updateType();
    emit updateSettings();
}

void Config::updateType()
{
    QDir dir("./config");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    FileNames.clear();
    QFileInfoList list = dir.entryInfoList();
    for (int i=0; i < list.size(); i++)
        FileNames.append(list.at(i).fileName());

    if (FileNames.isEmpty())
        FileNames.append("Bash_File.ini");

    mType->setRowCount(0);
    mType->appendRow(new QStandardItem(CurrentSettings()));
    for (int i=0; i < FileNames.size(); i++) {
        QString type = FileNames.at(i);
        type = type.remove(".ini");
        if (type == CurrentSettings())
            continue;
        mType->appendRow(new QStandardItem(type.remove(".ini")));
    }
}

QString Config::CurrentSettings()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", "Base_File").toString();
    return n.remove(".ini");
}

void Config::back()
{
    readSettings();
    readSysItems();
    emit sendAppShow("tester");
}

void Config::updateSqlite()
{
    QStringList headers;
    headers << "ID" << tr("可选项目") << tr("状态");
    mItem = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mItem->setTable("backup");
    mItem->setFilter(QObject::tr("enable='%1'").arg(Qt::Checked));
    mItem->select();
    for (int i=0; i < headers.size(); i++) {
        mItem->setHeaderData(i, Qt::Horizontal, headers.at(i));
    }
    item->setModel(mItem);
    item->hideColumn(0);
    item->hideColumn(2);
}

void Config::recvAppShow(QString win)
{
    if (win == this->objectName()) {
        mItem->select();
        updateType();
    }
}

void Config::recvSqlOpen(QString sql)
{
    sqlName = sql;
    updateSqlite();
}

void Config::recvAppCmd(QJsonObject msg)
{
    QJsonObject conf = msg.value("Conf").toObject();
    QStringList confs;
    confs << "color" << "type" << "test";
    for (int i=0; i < confs.size(); i++) {
        QStringList temp = conf.value(confs.at(i)).toString().split(",");
        if (confs.at(i) == "color") {
            for (int t=0; t < temp.size(); t++)
                colors.at(t)->setStyleSheet(QString("background-color:%1").arg(temp.at(t)));
        } else if (confs.at(i) == "type") {
            typeComboBox->setCurrentText(temp.at(0));
        } else if (confs.at(i) == "test") {
            testComboBox->setCurrentIndex(temp.at(0).toInt());
        }
    }

    QJsonObject sys = msg.value("Sys").toObject();
    QStringList syss;
    syss << "Test_Item";
    for (int i=0; i < syss.size(); i++) {
        QStringList temp = sys.value(syss.at(i)).toString().split(",");
        if (temp.isEmpty())
            temp.append("1");
        if (syss.at(i) == "Test_Item") {
            mTest->setRowCount(0);
            testItem = temp;
            for (int k=0; k < temp.size(); k++) {
                for (int j=0; j < mItem->rowCount(); j++) {
                    if (mItem->index(j, 0).data().toString() == temp.at(k)) {
                        QString name = mItem->index(j, 1).data().toString();
                        mTest->appendRow(new QStandardItem(name));
                    }
                }
            }
            mTest->appendRow(new QStandardItem);
        }
    }
}
