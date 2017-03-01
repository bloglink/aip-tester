/**
  ******************************************************************************
  * @file    HomePage.cpp
  * @author  link
  * @version 2.0.1.0
  * @date    2017-02-28
  * @brief   Home page of the motor comprehensive tester
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "HomePage.h"
#include "ui_HomePage.h"
/**
  * @brief  Initializes
  * @param  parent:parent widget
  * @retval None
  */
HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);
    initShow();
    initButton();

}
/**
  * @brief  Destruct the window
  * @param  None
  * @retval None
  */
HomePage::~HomePage()
{
    delete ui;
}
/**
  * @brief  Initializes title version and style
  * @param  None
  * @retval None
  */
void HomePage::initShow()
{
    this->setWindowTitle(tr("电机综合测试仪V-2.0.1.0"));
    ui->titleVn->setText("V-2.0.1.0");
    ui->titleVn->hide();

    //设置界面风格
    QFile file;
    QString qss;
    file.setFileName(":/source/SoftRainBlue.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);

    Testing = false;
    isCheckOk = false;
}
/**
  * @brief  Initializes the buttons
  * @param  None
  * @retval None
  */
void HomePage::initButton()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->btnSystem,Qt::Key_1);
    btnGroup->addButton(ui->btnType,Qt::Key_2);
    btnGroup->addButton(ui->btnData,Qt::Key_3);
    btnGroup->addButton(ui->btnTest,Qt::Key_4);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(judgeButton(int)));
}
/**
  * @brief  Button functions
  * @param  id:button id
  * @retval None
  */
void HomePage::judgeButton(int id)
{
    switch (id) {
    case Qt::Key_1:
        jumpToWindow("系统设置");
        break;
    case Qt::Key_2:
        jumpToWindow("型号管理");
        break;
    case Qt::Key_3:
        jumpToWindow("数据管理");
        break;
    case Qt::Key_4:
        jumpToWindow("进入测试");
        break;
    case Qt::Key_5:
        QApplication::closeAllWindows();
        break;
    }
}
/**
  * @brief  Jump to another window
  * @param  msg:window name
  * @retval None
  */
void HomePage::jumpToWindow(QByteArray win)
{
    int WinCurrent = ui->desktop->currentIndex();
    if (win.isNull()) { //空代表返回
        ui->desktop->setCurrentIndex(previous_window.last());
        previous_window.removeLast();
        return;
    }
    for (int i=0; i<ui->desktop->count(); i++) {
        if (ui->desktop->widget(i)->objectName() == win) {
            ui->desktop->setCurrentIndex(i);
            break;
        }
    }
    previous_window.append(WinCurrent);
    if (previous_window.size()>10) { //最大嵌套10层
        previous_window.removeFirst();
    }
}
/**
  * @brief  Initializes all window used
  * @param  None
  * @retval None
  */
void HomePage::initAllWindows()
{
    qDebug()<<QTime::currentTime().toString()<<"初始化所有窗口";

    System *system = new System(this);
    ui->desktop->addWidget(system);
    system->setObjectName("系统设置");
    connect(system,SIGNAL(sendMessage(quint16,quint16,QByteArray)),this,
            SLOT(readMessage(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(sendMessage(quint16,quint16,QByteArray)),system,
            SLOT(readMessage(quint16,quint16,QByteArray)));

    MotorType *type = new MotorType(this);
    ui->desktop->addWidget(type);
    type->setObjectName("型号管理");
    connect(type,SIGNAL(sendMessage(quint16,quint16,QByteArray)),this,
            SLOT(readMessage(quint16,quint16,QByteArray)));

    SqlPage *sql = new SqlPage(this);
    ui->desktop->addWidget(sql);
    sql->setObjectName("数据管理");
    connect(sql,SIGNAL(sendMessage(quint16,quint16,QByteArray)),this,
            SLOT(readMessage(quint16,quint16,QByteArray)));

    Test *test = new Test(this);
    ui->desktop->addWidget(test);
    test->setObjectName("进入测试");
    connect(test,SIGNAL(sendMessage(quint16,quint16,QByteArray)),this,
            SLOT(readMessage(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(sendMessage(quint16,quint16,QByteArray)),test,
            SLOT(readMessage(quint16,quint16,QByteArray)));

    qDebug()<<QTime::currentTime().toString()<<"初始化所有窗口OK";

    checkStatus();

}
/**
  * @brief  Excute command
  * @param  addr:target address;cmd:command to excute;msg:command param
  * @retval None
  */
void HomePage::readMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    switch (cmd) {
    case WIN_CMD_SWITCH:
        jumpToWindow(msg);
        break;
    case WIN_CMD_INIT:
        initTest();
        break;
    case WIN_CMD_SHOW:
        Items.append(QString(msg).split("\n"));
        break;
    case WIN_CMD_JUDGE:
        saveJudge(msg);
        break;
    case WIN_CMD_ITEM:
        emit WriteSql(msg);
    case WIN_CMD_TEMP:
    case WIN_WAVE_BYTE:
    case WIN_WAVE_TEST:
    case WIN_WAVE_ITEM:
    case WIN_WAVE_HIDE:
        emit sendMessage(WIN_ID_TEST,cmd,msg);
        break;
    case WIN_CMD_DEBUG:
        emit sendMessage(ADDR,WIN_CMD_DEBUG,msg);
        break;
    case WIN_CMD_WAVE:
        emit sendMessage(ADDR,WIN_CMD_WAVE,msg);
        break;
    case CAN_DAT_GET:
        break;
    case CAN_DAT_PUT:
        emit PutCanData(msg);
        break;
    case CAN_CMD_START:
        startTest(msg);
        break;
    case CAN_CMD_STOP:
        Testing = false;
        break;
    default:
        qDebug()<<addr<<cmd<<msg;
        break;
    }
}
/**
  * @brief  Initializes before test
  * @param  None
  * @retval None
  */
void HomePage::initTest()
{
    qDebug()<<QTime::currentTime().toString()<<"初始化测试";

    Items.clear();
    //全局配置
    QSettings *settings_g = new QSettings(GLOBAL_PATH,QSettings::IniFormat);
    settings_g->setIniCodec("GB18030");
    settings_g->beginGroup("GLOBAL");
    motor_type = settings_g->value("motor_type","default.ini").toString();
    motor_type.remove(".ini");
    //当前配置
    QString n = QString("./config/%1.ini").arg(motor_type);
    QSettings *settings_c = new QSettings(n,QSettings::IniFormat);
    settings_c->setIniCodec("GB18030");

//    QStringList itemToTest = settings_c->value("/GLOBAL/ProjToTest","").toString().split(" ");

//    emit sendMessage(WIN_ID_OUT,CAN_CMD_INIT,NULL);//设定启动方式

//    for (int i=0; i<itemToTest.size(); i++) {
//        emit sendMessage(itemToTest.at(i).toInt(),CAN_CMD_INIT,NULL);
//    }
//    emit sendMessage(WIN_ID_TEST,WIN_CMD_INIT,Items.join("\n").toUtf8());//初始化测试界面

    qDebug()<<QTime::currentTime().toString()<<"初始化测试OK";
}

/**
  * @brief  Check the board status
  * @param  None
  * @retval None
  */
void HomePage::checkStatus()
{
    qDebug()<<QTime::currentTime().toString()<<"开机自检";
    if (Testing)
        return;
    Testing = true;

    //全局配置
    QSettings *settings_g = new QSettings(GLOBAL_PATH,QSettings::IniFormat);
    settings_g->setIniCodec("GB18030");
    settings_g->beginGroup("GLOBAL");

    QStringList t = (settings_g->value("ItemEnable","0 1 2 3 4 6").toString()).split(" ");
    for (int i=0; i<t.size(); i++) {
        emit sendMessage(t.at(i).toInt(),CAN_CMD_CHECK,NULL);
    }
    QStringList s = (settings_g->value("OutEnable","0").toString()).split(" ");
    for (int i=0; i<s.size(); i++) {
        emit sendMessage(WIN_ID_OUT,CAN_CMD_CHECK,s.at(i).toUtf8());
    }
    qDebug()<<QTime::currentTime().toString()<<"开机自检OK";

    Testing = false;
    if (!isCheckOk) {
        isCheckOk = true;
        delay(1000);
        jumpToWindow("进入测试");
    }
}
/**
  * @brief  Test thread
  * @param  data:station to test
  * @retval None
  */
void HomePage::startTest(QByteArray station)
{
    WaitTestOver(100);
    if (Testing)
        return;
    if (ui->desktop->currentWidget()->objectName() != "进入测试")
        return;
    Testing = true;
    ItemJudge = "OK";

    initTest();

    emit sendMessage(WIN_ID_TEST,CAN_CMD_START,station);

    QByteArray msg;
    msg.append(0x02 | 0x00);
    emit sendMessage(ADDR,CAN_CMD_ALARM,msg);

    for (int i=0; i<ItemToTest.size(); i++) {
        emit sendMessage(ItemToTest.at(i).toInt(),CAN_CMD_START,station);
        if (!Testing)
            break;
    }
    saveTest();
    if (ItemJudge == "NG") {
        msg.clear();
        msg.append(0x08 | 0x01);
        emit sendMessage(ADDR,CAN_CMD_ALARM,msg);
        delay(500);
        msg.clear();
        msg.append(0x08 | 0x00);
        emit sendMessage(ADDR,CAN_CMD_ALARM,msg);
    } else {
        msg.clear();
        msg.append(0x04 | 0x01);
        emit sendMessage(ADDR,CAN_CMD_ALARM,msg);
        delay(200);
        msg.clear();
        msg.append(0x04 | 0x00);
        emit sendMessage(ADDR,CAN_CMD_ALARM,msg);
    }
    emit sendMessage(WIN_ID_TEST,WIN_CMD_JUDGE,station);
    Testing = false;
}
/**
  * @brief  Save test data
  * @param  None
  * @retval None
  */
void HomePage::saveTest()
{
    QStringList s;
    s.append("总数");
    s.append(motor_type);
    s.append(ItemJudge);
    emit WriteSql(s.join("@").toUtf8());
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.12.30
 * brief:       不合格警告
******************************************************************************/
void HomePage::TestPause()
{
    if(QMessageBox::warning(this,"此项目不合格", "是否继续",QMessageBox::Yes,QMessageBox::No)==QMessageBox::No)
        Testing = false;
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.16
 * brief:       测试结果判定保存
******************************************************************************/
void HomePage::saveJudge(QByteArray msg)
{
    emit WriteSql(msg);
    QStringList s = QString(msg).split("@");
    if (s.size() < 3)
        return;
    if (s.at(2) == "NG")
        ItemJudge = "NG";
    if (s.at(2) == "NG" && PauseMode != 1)
        TestPause();
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2017.02.18
 * brief:      等待测试结束
*******************************************************************************/
bool HomePage::WaitTestOver(quint16 t)
{
    int TimeOut = 0;
    while (Testing) {
        delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.12.30
 * brief:      延时
*******************************************************************************/
void HomePage::delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}
/*********************************END OF FILE**********************************/
