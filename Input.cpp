/**
  ******************************************************************************
  * @file    input.cpp
  * @author  link
  * @version 2.0.1.1
  * @date    2017-02-28
  * @brief   Input method for arm use QWSServer
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Input.h"
#include "ui_Input.h"
Input *Input::_instance = 0;
/**
  * @brief  Initializes
  * @param  *parent:parent widget
  * @retval None
  */
Input::Input(QWidget *parent) :
    QWidget(parent,Qt::Tool| Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint),
    ui(new Ui::Input)
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
Input::~Input()
{
    delete ui;
}
/**
  * @brief  Initializes the window
  * @param  None
  * @retval None
  */
void Input::initShow()
{
    //绑定全局改变焦点信号槽
    connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)),
            this, SLOT(focusChanged(QWidget *, QWidget *)));
    //绑定按键事件过滤器
    qApp->installEventFilter(this);
    upper = true;
    currentWidget = NULL;
}
/**
  * @brief  Initializes the buttons
  * @param  None
  * @retval None
  */
void Input::initButton()
{
    QButtonGroup *btn_group = new QButtonGroup;
    btn_group->addButton(ui->Key_0,Qt::Key_0);
    btn_group->addButton(ui->Key_1,Qt::Key_1);
    btn_group->addButton(ui->Key_2,Qt::Key_2);
    btn_group->addButton(ui->Key_3,Qt::Key_3);
    btn_group->addButton(ui->Key_4,Qt::Key_4);
    btn_group->addButton(ui->Key_5,Qt::Key_5);
    btn_group->addButton(ui->Key_6,Qt::Key_6);
    btn_group->addButton(ui->Key_7,Qt::Key_7);
    btn_group->addButton(ui->Key_8,Qt::Key_8);
    btn_group->addButton(ui->Key_9,Qt::Key_9);

    btn_group->addButton(ui->Key_A,Qt::Key_A);
    btn_group->addButton(ui->Key_B,Qt::Key_B);
    btn_group->addButton(ui->Key_C,Qt::Key_C);
    btn_group->addButton(ui->Key_D,Qt::Key_D);
    btn_group->addButton(ui->Key_E,Qt::Key_E);
    btn_group->addButton(ui->Key_F,Qt::Key_F);
    btn_group->addButton(ui->Key_G,Qt::Key_G);
    btn_group->addButton(ui->Key_H,Qt::Key_H);
    btn_group->addButton(ui->Key_I,Qt::Key_I);
    btn_group->addButton(ui->Key_J,Qt::Key_J);
    btn_group->addButton(ui->Key_K,Qt::Key_K);
    btn_group->addButton(ui->Key_L,Qt::Key_L);
    btn_group->addButton(ui->Key_M,Qt::Key_M);
    btn_group->addButton(ui->Key_N,Qt::Key_N);
    btn_group->addButton(ui->Key_O,Qt::Key_O);
    btn_group->addButton(ui->Key_P,Qt::Key_P);
    btn_group->addButton(ui->Key_Q,Qt::Key_Q);
    btn_group->addButton(ui->Key_R,Qt::Key_R);
    btn_group->addButton(ui->Key_S,Qt::Key_S);
    btn_group->addButton(ui->Key_T,Qt::Key_T);
    btn_group->addButton(ui->Key_U,Qt::Key_U);
    btn_group->addButton(ui->Key_V,Qt::Key_V);
    btn_group->addButton(ui->Key_W,Qt::Key_W);
    btn_group->addButton(ui->Key_X,Qt::Key_X);
    btn_group->addButton(ui->Key_Y,Qt::Key_Y);
    btn_group->addButton(ui->Key_Z,Qt::Key_Z);

    btn_group->addButton(ui->Key_Space,Qt::Key_Space);
    btn_group->addButton(ui->Key_Enter,Qt::Key_Enter);
    // this->button_group->addButton(ui->Key_Escape,Qt::Key_Escape);
    btn_group->addButton(ui->Key_Period,Qt::Key_Period);
    btn_group->addButton(ui->Key_CapsLock,Qt::Key_CapsLock);
    btn_group->addButton(ui->Key_Backspace,Qt::Key_Backspace);
    btn_group->addButton(ui->Key_Minus,Qt::Key_Minus);

    connect(btn_group,SIGNAL(buttonClicked(int)),this,SLOT(judgeButton(int)));
}
/**
  * @brief  Button functions
  * @param  id:button id
  * @retval None
  */
void Input::judgeButton(int id)
{
    switch (id) {
    case Qt::Key_Enter :
        this->setVisible(false);
        QApplication::focusWidget()->clearFocus();
        break;
    case Qt::Key_CapsLock:
        upper = !upper;
        if (upper)
            ui->Key_CapsLock->setText(tr("大写"));
        else
            ui->Key_CapsLock->setText(tr("小写"));
        break;
    default:
#ifdef __arm__
        if (id>=Qt::Key_A && id<=Qt::Key_Z && !upper)
            id += 32;
        QWSServer::sendKeyEvent(id,id,Qt::NoModifier,true,false);
#endif
        break;
    }
}
/**
  * @brief  focus change event
  * @param  nowWidget:focus widget
  * @retval None
  */
void Input::focusChanged(QWidget *, QWidget *nowWidget)
{
    if (nowWidget != 0 && !this->isAncestorOf(nowWidget)) {
        bool visible = false;
        if (nowWidget->inherits("QLineEdit"))
            visible = true;
        if (nowWidget->inherits("QSpinBox"))
            visible = true;
        if (nowWidget->inherits("QDoubleSpinBox"))
            visible = true;
        this->setVisible(visible);
    }
}
/**
  * @brief  Initializes position when show
  * @param  None
  * @retval None
  */
void Input::showEvent(QShowEvent *)
{
    int mouse_y = QCursor::pos().y();//鼠标点击处纵坐标

    if(mouse_y>300) {
        this->move(70,0);
    } else {
        this->move(70,600-240);
    }
}
/*********************************END OF FILE**********************************/
