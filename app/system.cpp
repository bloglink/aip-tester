/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       系统界面
*******************************************************************************/
#include "system.h"

System::System(QWidget *parent)
    : QWidget(parent)
{
    initUI();
}

System::~System()
{
}

void System::setIobrd(QString s)
{
    iobrd->recvNetCommand(s);
}

void System::setMasterName(QString name)
{
    masterName = name;
}

void System::initUI()
{
    initLayout();
    initGlobal();
    initIobrd();
    initMaster();
    initSource();
    initAction();
    initBackup();
    initLogger();
    initLogout();
}

void System::initLayout()
{
    stack = new QStackedWidget(this);
    btnLayout = new QVBoxLayout;
    btnLayout->setMargin(0);
    btnLayout->setSpacing(0);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(btnLayout);
    leftLayout->addStretch();
    leftLayout->setMargin(0);

    QWidget *leftWidget = new QWidget(this);
    leftWidget->setObjectName("leftWidget");
    leftWidget->setLayout(leftLayout);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(leftWidget);
    layout->addWidget(stack);

    this->setLayout(layout);
    this->setObjectName("system");
}

void System::initGlobal()
{
    global = new Global(this);
    global->setObjectName("global");
    connect(this, SIGNAL(sendAppShow(QString)), global, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), global, SLOT(recvSqlOpen(QString)));
    stack->addWidget(global);

    QPushButton *btnGlobal = new QPushButton(tr("全局设置"), this);
    btnGlobal->setFlat(true);
    btnGlobal->setCheckable(true);
    btnGlobal->setFixedSize(196, 60);
    btnGlobal->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnGlobal);
    btnLayout->addWidget(btnGlobal);
    btnGlobal->setObjectName("global");
    connect(btnGlobal, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
}

void System::initIobrd()
{
    iobrd = new Iobrd(this);
    iobrd->setObjectName("iobrd");
    connect(iobrd, SIGNAL(sendAppMsg(QJsonObject)), this, SIGNAL(sendAppMsg(QJsonObject)));
    connect(this, SIGNAL(sendAppShow(QString)), iobrd, SLOT(recvAppShow(QString)));
//    connect(this, SIGNAL(sendSqlOpen(QString)), global, SLOT(recvSqlOpen(QString)));
    stack->addWidget(iobrd);

    QPushButton *btnGlobal = new QPushButton(tr("动作设置"), this);
    btnGlobal->setFlat(true);
    btnGlobal->setCheckable(true);
    btnGlobal->setFixedSize(196, 60);
    btnGlobal->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnGlobal);
    btnLayout->addWidget(btnGlobal);
    btnGlobal->setObjectName("iobrd");
    connect(btnGlobal, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
}

void System::initMaster()
{
    master = new Master(this);
    master->setObjectName("master");
    connect(this, SIGNAL(sendAppShow(QString)), master, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), master, SLOT(recvSqlOpen(QString)));
    stack->addWidget(master);

    QPushButton *btnMaster = new QPushButton(tr("用户管理"), this);
    btnMaster->setFlat(true);
    btnMaster->setCheckable(true);
    btnMaster->setFixedSize(196, 60);
    btnMaster->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnMaster);
    btnLayout->addWidget(btnMaster);
    btnMaster->setObjectName("master");
    connect(btnMaster, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
}

void System::initSource()
{
    source = new Source(this);
    source->setObjectName("source");
    connect(this, SIGNAL(sendAppShow(QString)), source, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), source, SLOT(recvSqlOpen(QString)));
    stack->addWidget(source);

    QPushButton *btnSource = new QPushButton(tr("资源管理"), this);
    btnSource->setFlat(true);
    btnSource->setCheckable(true);
    btnSource->setFixedSize(196, 60);
    btnSource->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnSource);
    btnLayout->addWidget(btnSource);
    btnSource->setObjectName("source");
    connect(btnSource, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
}

void System::initAction()
{
    action = new Action(this);
    action->setObjectName("action");
    connect(this, SIGNAL(sendAppShow(QString)), action, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), action, SLOT(recvSqlOpen(QString)));
    stack->addWidget(action);

    QPushButton *btnAction = new QPushButton(tr("权限管理"), this);
    btnAction->setFlat(true);
    btnAction->setCheckable(true);
    btnAction->setFixedSize(196, 60);
    btnAction->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnAction);
    btnLayout->addWidget(btnAction);
    btnAction->setObjectName("action");
    connect(btnAction, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
}

void System::initBackup()
{
    backup = new Backup(this);
    backup->setObjectName("backup");
    connect(this, SIGNAL(sendAppShow(QString)), backup, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), backup, SLOT(recvSqlOpen(QString)));
    stack->addWidget(backup);

    QPushButton *btnBackup = new QPushButton(tr("后台管理"), this);
    btnBackup->setFlat(true);
    btnBackup->setCheckable(true);
    btnBackup->setFixedSize(196, 60);
    btnBackup->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnBackup);
    btnLayout->addWidget(btnBackup);
    btnBackup->setObjectName("backup");
    connect(btnBackup, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
}

void System::initLogger()
{
    logger = Logger::instance();
    logger->setObjectName("logger");
    connect(this, SIGNAL(sendAppShow(QString)), logger, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), logger, SLOT(recvSqlOpen(QString)));
    stack->addWidget(logger);

    QPushButton *btnLogger = new QPushButton(tr("调试信息"), this);
    btnLogger->setFlat(true);
    btnLogger->setCheckable(true);
    btnLogger->setFixedSize(196, 60);
    btnLogger->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnLogger);
    btnLayout->addWidget(btnLogger);
    btnLogger->setObjectName("logger");
    connect(btnLogger, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
}

void System::initLogout()
{
    QPushButton *btnLogout = new QPushButton(tr("退出设置"), this);
    btnLogout->setFlat(true);
    btnLogout->setCheckable(true);
    btnLogout->setFixedSize(196, 60);
    btnLogout->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnLogout);
    btnLayout->addWidget(btnLogout);
    btnLogout->setObjectName("author");
    connect(btnLogout, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
}

void System::clickButton()
{
    QString sourceName = QObject::sender()->objectName();
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    double masterId = getMasterId(masterName);
    double sourceId = getSourceId(sourceName);
    double actionValue = getAction(masterId, sourceId);

    if (actionValue == Qt::Checked) {
        for (int i=0; i < buttons.size(); i++) {
            if (buttons.at(i)->objectName() != sourceName) {
                buttons.at(i)->setChecked(false);
            }
        }
        for (int i=0; i < stack->count(); i++) {
            if (stack->widget(i)->objectName() == sourceName) {
                stack->setCurrentIndex(i);
                break;
            }
        }
        emit sendAppShow(sourceName);
    } else {
        QMessageBox::warning(this, "", "您无权限进行此操作", QMessageBox::Ok);
        btn->setChecked(false);
    }
}

double System::getMasterId(QString masterName)
{
    double id = 0;
    QSqlQuery query(QSqlDatabase::database(sqlName));
    query.prepare("select id from master where name = :name");
    query.bindValue(":name", masterName);
    query.exec();
    if (query.next()) {
        id = query.value(0).toDouble();
    } else {
        qDebug() << "get master id fail" << masterName;
    }
    return id;
}

double System::getSourceId(QString sourceName)
{
    double id = 0;
    QSqlQuery query(QSqlDatabase::database(sqlName));
    query.prepare("select id from source where name = :name");
    query.bindValue(":name", sourceName);
    query.exec();
    if (query.next()) {
        id = query.value(0).toDouble();
    } else {
        qDebug() << "get source id fail" << sourceName;
    }
    return id;
}

double System::getAction(double masterId, double sourceId)
{
    double id = 0;
    QSqlQuery query(QSqlDatabase::database(sqlName));
    query.prepare("select action from action where master = :master and source = :source");
    query.bindValue(":master", masterId);
    query.bindValue(":source", sourceId);
    query.exec();
    if (query.next()) {
        id = query.value(0).toDouble();
    } else {
        qDebug() << "get action fail";
    }
    return id;
}

void System::recvAppShow(QString win)
{
    if (win == this->objectName()) {
        buttons.at(0)->click();
    }
}

void System::recvSqlOpen(QString sql)
{
    sqlName = sql;
    emit sendSqlOpen(sql);
}

