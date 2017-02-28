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
  * @brief  Excute command
  * @param  addr:target address;cmd:command to excute;msg:command param
  * @retval None
  */
void HomePage::readMessage(quint16 addr, quint16 cmd, QByteArray data)
{
    switch (cmd) {
    case WIN_CMD_SWITCH:
        jumpToWindow(data);
        break;
    default:
        qDebug()<<addr<<cmd<<data;
        break;
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

    qDebug()<<QTime::currentTime().toString()<<"初始化所有窗口OK";

}
/*********************************END OF FILE**********************************/
