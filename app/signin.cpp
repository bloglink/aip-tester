/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       登录界面
*******************************************************************************/
#include "signin.h"

SignIn::SignIn(QWidget *parent) : QWidget(parent)
{
    initUI();
    initSettings();
}

void SignIn::initUI()
{
    initLayout();
    initTitleBar();
    initContent();
    initAdvance();
    initInputBar();
}

void SignIn::initLayout()
{
    layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(layout);

    QGridLayout *lay = new QGridLayout;

    lay->addWidget(group, 1, 1);
    lay->setColumnStretch(0, 1);
    lay->setColumnStretch(2, 1);
    lay->setRowStretch(0, 1);
    lay->setRowStretch(2, 1);
    this->setLayout(lay);
}

void SignIn::initTitleBar()
{
    QLabel *titleIcon  = new QLabel(this);
    titleIcon->setPixmap(QPixmap("./image/link.ico"));
    titleIcon->setScaledContents(true);
    titleIcon->setFixedSize(32, 32);

    QLabel *titleText = new QLabel(tr("青岛艾普综合测试系统"));

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->addWidget(titleIcon);
    titleLayout->addWidget(titleText);
    titleLayout->addStretch();
    titleLayout->setMargin(0);

    QWidget *titleBar = new QWidget(this);
    titleBar->setStyleSheet("background:#005270;");
    titleBar->setLayout(titleLayout);

    layout->addWidget(titleBar);
}

void SignIn::initContent()
{
    QLabel *contentIcon = new QLabel(this);
    contentIcon->setPixmap(QPixmap("./image/aip.png"));
    contentIcon->setScaledContents(true);

    QHBoxLayout *contentLayout = new QHBoxLayout;
    contentLayout->addWidget(contentIcon);
    contentLayout->setMargin(0);

    QWidget *content = new QWidget(this);
    content->setLayout(contentLayout);
    content->setFixedSize(550, 150);
    layout->addWidget(content);
}

void SignIn::initAdvance()
{
    hostaddr = new QComboBox(this);
    hostaddr->setEditable(true);
    hostaddr->setMinimumHeight(35);
    hostaddr->setView(new QListView);

    hostport = new QComboBox(this);
    hostport->setEditable(true);
    hostport->setMinimumHeight(35);
    hostport->setView(new QListView);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("设  备:")), 1, 0);
    layout->addWidget(new QLabel(tr("端  口:")), 2, 0);
    layout->addWidget(hostaddr, 1, 1);
    layout->addWidget(hostport, 2, 1);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 2);
    layout->setColumnStretch(2, 1);
    layout->setMargin(0);

    advance = new QWidget(this);
    advance->setLayout(layout);
    advance->hide();
}

void SignIn::initInputBar()
{
    username = new QComboBox(this);
    username->setEditable(true);
    username->setMinimumHeight(35);
    username->setView(new QListView);

    password = new QLineEdit(this);
    password->setMinimumHeight(35);
    password->setEchoMode(QLineEdit::Password);

    QPushButton *btnLogin = new QPushButton(this);
    btnLogin->setText(tr("连接"));
    btnLogin->setMinimumHeight(35);
    btnLogin->setFocusPolicy(Qt::NoFocus);
    connect(btnLogin, SIGNAL(clicked(bool)), this, SLOT(sendLogin()));

    QPushButton *btnClose = new QPushButton(this);
    btnClose->setText(tr("退出"));
    btnClose->setMinimumHeight(35);
    btnClose->setFocusPolicy(Qt::NoFocus);
    connect(btnClose, SIGNAL(clicked(bool)), this, SLOT(sendClose()));

    QPushButton *btnAdvance = new QPushButton(this);
    btnAdvance->setText(tr("高级"));
    btnAdvance->setMinimumHeight(35);
    btnAdvance->setFocusPolicy(Qt::NoFocus);
    connect(btnAdvance, SIGNAL(clicked(bool)), advance, SLOT(show()));

    QGridLayout *btnsLayout = new QGridLayout;
    btnsLayout->addWidget(new QLabel(tr("用  户:")), 2, 0);
    btnsLayout->addWidget(new QLabel(tr("密  码:")), 3, 0);
    btnsLayout->addWidget(username, 2, 1);
    btnsLayout->addWidget(password, 3, 1);
    btnsLayout->addWidget(btnLogin, 2, 2);
    btnsLayout->addWidget(btnClose, 3, 2);
    btnsLayout->addWidget(btnAdvance, 4, 2);
    btnsLayout->addWidget(advance, 5, 0, 1, 3);
    btnsLayout->setColumnStretch(0, 1);
    btnsLayout->setColumnStretch(1, 2);
    btnsLayout->setColumnStretch(2, 1);

    QWidget *input = new QWidget(this);
    input->setLayout(btnsLayout);

    layout->addWidget(input);
}

void SignIn::initSettings()
{
    userConf = new QSettings("./nandflash/global.ini",  QSettings::IniFormat);
    userConf->setIniCodec("GB18030");
    userConf->beginGroup("LOGIN");
    QStringList items;
    QByteArray hostaddrbyte = "192.168.1.51";
    QByteArray hostaddrsave = userConf->value("hostaddr", hostaddrbyte.toBase64()).toByteArray();
    QByteArray hostportbyte = "6000";
    QByteArray hostportsave = userConf->value("hostport", hostportbyte.toBase64()).toByteArray();
    QByteArray usernamebyte = "admin";
    QByteArray usernamesave = userConf->value("username", usernamebyte.toBase64()).toByteArray();
    items = QString(QByteArray::fromBase64(hostaddrsave)).split("@");
    hostaddr->addItems(items);
    items = QString(QByteArray::fromBase64(hostportsave)).split("@");
    hostport->addItems(items);
    items = QString(QByteArray::fromBase64(usernamesave)).split("@");
    username->addItems(items);
}

void SignIn::saveSettings()
{
    saveHostAddr();
    saveHostPort();
    saveUserName();
}

void SignIn::saveHostAddr()
{
    QStringList items;
    items.append(hostaddr->currentText());
    for (int i=0; i < hostaddr->count(); i++) {
        if (hostaddr->itemText(i) == hostaddr->currentText())
            continue;
        items.append(hostaddr->itemText(i));
    }
    if (items.size() > MAX_RECORD)  // 最多存储5条
        items.removeLast();
    QByteArray msg = QString(items.join("@")).toUtf8();
    userConf->setValue("hostaddr",  msg.toBase64());
}

void SignIn::saveHostPort()
{
    QStringList items;
    items.append(hostport->currentText());
    for (int i=0; i < hostport->count(); i++) {
        if (hostport->itemText(i) == hostport->currentText())
            continue;
        items.append(hostport->itemText(i));
    }
    if (items.size() > MAX_RECORD)  // 最多存储5条
        items.removeFirst();
    QByteArray msg = QString(items.join("@")).toUtf8();
    userConf->setValue("hostport",  msg.toBase64());
}

void SignIn::saveUserName()
{
    QStringList items;
    items.append(username->currentText());
    for (int i=0; i < username->count(); i++) {
        if (username->itemText(i) == username->currentText())
            continue;
        items.append(username->itemText(i));
    }
    if (items.size() > MAX_RECORD)  // 最多存储5条
        items.removeLast();
    QByteArray msg = QString(items.join("@")).toUtf8();
    userConf->setValue("username",  msg.toBase64());
}

void SignIn::updateSqlite()
{
    mMaster = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mMaster->setTable("master");
    mMaster->select();
}

void SignIn::sendLogin()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    mMaster->select();
    QStringList usernames;
    QStringList passwords;
    for (int i=0; i < mMaster->rowCount(); i++) {
        usernames.append(mMaster->data(mMaster->index(i, 1), Qt::DisplayRole).toString());
        passwords.append(mMaster->data(mMaster->index(i, 2), Qt::DisplayRole).toString());
    }
    if (usernames.contains(username->currentText())) {
        int index = usernames.indexOf(username->currentText());
        if (passwords.at(index) == password->text()) {
            QJsonObject obj;
            obj.insert("hostaddr", hostaddr->currentText());
            obj.insert("hostport", hostport->currentText());
            obj.insert("username", username->currentText());
            obj.insert("password", password->text());
            obj.insert("txCommand", CMD_SIGNIN);
            emit sendAppMsg(obj);
            saveSettings();
            btn->setText(tr("连接中..."));
        } else {
            QMessageBox::warning(this, tr("登录"), tr("密码不正确"), QMessageBox::Ok);
        }
    } else {
        QMessageBox::warning(this, tr("登录"), tr("用户不存在"), QMessageBox::Ok);
    }
}

void SignIn::sendClose()
{
    QApplication::closeAllWindows();
}

void SignIn::recvAppShow(QString win)
{
    if (win == this->objectName()) {
        mMaster->select();
    }
}

void SignIn::recvSqlOpen(QString sql)
{
    sqlName = sql;
    updateSqlite();
}

