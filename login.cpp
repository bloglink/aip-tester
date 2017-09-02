#include "login.h"

Login::Login(QDialog *parent) : QDialog(parent)
{
    initUI();
    initData();
}

Login::~Login()
{

}


void Login::initUI()
{
    this->setObjectName("Login");
    this->setWindowFlags(Qt::FramelessWindowHint);

    QLabel *icon  = new QLabel(this);
    icon->setPixmap(QPixmap(":/source/link.ico"));
    icon->setScaledContents(true);
    icon->setMaximumSize(25, 25);
    QLabel *text = new QLabel(tr("青岛艾普综合测试系统"));
    QToolButton *ext = new QToolButton(this);
    ext->setIcon(QIcon(":/source/radio_2.png"));
    ext->setFocusPolicy(Qt::NoFocus);
    ext->setIconSize(QSize(25, 25));
    ext->resize(25, 25);
    connect(ext, SIGNAL(clicked(bool)), this, SLOT(close()));

    QHBoxLayout *title = new QHBoxLayout;
    title->addWidget(icon);
    title->addWidget(text);
    title->addStretch();
    title->addWidget(ext);
    title->setMargin(0);
    title->setSpacing(5);

    QLabel *logo = new QLabel(this);
    logo->setPixmap(QPixmap(":/source/aip.png"));
    logo->setScaledContents(true);
    logo->setMaximumSize(480, 130);

    svr = new QComboBox(this);
    svr->setEditable(true);
    svr->setMinimumHeight(35);
    svr->setView(new QListView);

    prt = new QComboBox(this);
    prt->setEditable(true);
    prt->setMinimumHeight(35);
    prt->setView(new QListView);

    usr = new QComboBox(this);
    usr->setEditable(true);
    usr->setMinimumHeight(35);
    usr->setView(new QListView);

    pwd = new QLineEdit(this);
    pwd->setMinimumHeight(35);
    pwd->setEchoMode(QLineEdit::Password);

    QPushButton *btn_login = new QPushButton(this);
    btn_login->setText(tr("连接"));
    btn_login->setMinimumHeight(35);
    btn_login->setFocusPolicy(Qt::NoFocus);
    connect(btn_login, SIGNAL(clicked(bool)), this, SLOT(login()));

    QPushButton *btn_close = new QPushButton(this);
    btn_close->setText(tr("退出"));
    btn_close->setMinimumHeight(35);
    btn_close->setFocusPolicy(Qt::NoFocus);
    connect(btn_close, SIGNAL(clicked(bool)), this, SLOT(close()));

    QGridLayout *btnsLayout = new QGridLayout;
    btnsLayout->addWidget(new QLabel(tr("设  备:")), 0, 0);
    btnsLayout->addWidget(new QLabel(tr("端  口:")), 1, 0);
    btnsLayout->addWidget(new QLabel(tr("用  户:")), 2, 0);
    btnsLayout->addWidget(new QLabel(tr("密  码:")), 3, 0);
    btnsLayout->addWidget(svr, 0, 1);
    btnsLayout->addWidget(prt, 1, 1);
    btnsLayout->addWidget(usr, 2, 1);
    btnsLayout->addWidget(pwd, 3, 1);
    btnsLayout->addWidget(btn_login, 2, 2);
    btnsLayout->addWidget(btn_close, 3, 2);
    btnsLayout->setColumnStretch(0, 1);
    btnsLayout->setColumnStretch(1, 2);
    btnsLayout->setColumnStretch(2, 1);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(btnsLayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(title);
    layout->addWidget(logo);
    layout->addWidget(group);
    layout->setMargin(0);
    layout->setSpacing(0);
    this->setLayout(layout);
    this->resize(500, 360);
}

void Login::initData()
{
    ini = new QSettings("conf.ini",  QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("LOGIN");
    QStringList items;
    QByteArray byte_svr = "192.168.1.51";
    QByteArray save_svr = ini->value("svr",  byte_svr.toBase64()).toByteArray();
    QByteArray byte_prt = "6000";
    QByteArray save_prt = ini->value("prt",  byte_prt.toBase64()).toByteArray();
    QByteArray byte_usr = "admin";
    QByteArray save_usr = ini->value("usr",  byte_usr.toBase64()).toByteArray();
    items = QString(QByteArray::fromBase64(save_svr)).split("@");
    svr->addItems(items);
    items = QString(QByteArray::fromBase64(save_prt)).split("@");
    prt->addItems(items);
    items = QString(QByteArray::fromBase64(save_usr)).split("@");
    usr->addItems(items);
}

void Login::saveData()
{
    QStringList items;
    items.append(svr->currentText());
    for (int i=0; i < svr->count(); i++) {
        if (svr->itemText(i) == svr->currentText())
            continue;
        items.append(svr->itemText(i));
    }
    if (items.size() > SAVE_MAX)  // 最多存储5条
        items.removeLast();
    QByteArray save_svr = QString(items.join("@")).toUtf8();
    ini->setValue("svr",  save_svr.toBase64());
    items.clear();

    for (int i=0; i < prt->count(); i++)
        items.append(prt->itemText(i));
    if (!items.contains(prt->currentText()))
        items.append(prt->currentText());
    if (items.size() > SAVE_MAX)  // 最多存储5条
        items.removeFirst();
    QByteArray save_prt = QString(items.join("@")).toUtf8();
    ini->setValue("prt",  save_prt.toBase64());
    items.clear();

    for (int i=0; i < usr->count(); i++)
        items.append(usr->itemText(i));
    if (!items.contains(usr->currentText()))
        items.append(usr->currentText());
    if (items.size() > SAVE_MAX)  // 最多存储5条
        items.removeFirst();
    QByteArray save_usr = QString(items.join("@")).toUtf8();
    ini->setValue("usr",  save_usr.toBase64());
    items.clear();
}

void Login::login()
{
    saveData();
    QJsonObject obj;
    obj.insert("host_addr", svr->currentText());
    obj.insert("host_port", prt->currentText());
    obj.insert("TxMessage","6000");
    emit sendJson(obj);

    QTimer *timer = new QTimer(this);
    timer->singleShot(1000, this, SLOT(loginTimeOut()));

    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    btn->setText("连接中...");
    btn->setEnabled(false);
}

void Login::loginTimeOut()
{
    QMessageBox::warning(this, tr("警告"), tr("连接超时:请检查设置后重新连接"));
    this->accept();
}

void Login::recvNetMsg(QString msg)
{
    QString TxMessage = msg.split(" ").at(0);
    if (TxMessage.toInt() == 6001)
        this->accept();
}

/*********************************END OF FILE**********************************/
