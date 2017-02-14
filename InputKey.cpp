/*******************************************************************************
 * Copyright (c) 2015,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2015.12.30
 * brief:       软键盘界面
*******************************************************************************/
#include "InputKey.h"
#include "ui_InputKey.h"
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.12.30
  * brief:      软键盘界面初始化
******************************************************************************/
InputKey::InputKey(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputKey)
{
    ui->setupUi(this);
    this->btnGroup = new QButtonGroup;
    this->SetKeyBoard();
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(ButtonJudge(int)));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.12.30
  * brief:      软键盘析构
******************************************************************************/
InputKey::~InputKey()
{
    delete this->btnGroup;
    delete ui;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.12.30
  * brief:      按键定义
******************************************************************************/
void InputKey::SetKeyBoard()
{
    this->btnGroup->addButton(ui->Key_0,Qt::Key_0);
    this->btnGroup->addButton(ui->Key_1,Qt::Key_1);
    this->btnGroup->addButton(ui->Key_2,Qt::Key_2);
    this->btnGroup->addButton(ui->Key_3,Qt::Key_3);
    this->btnGroup->addButton(ui->Key_4,Qt::Key_4);
    this->btnGroup->addButton(ui->Key_5,Qt::Key_5);
    this->btnGroup->addButton(ui->Key_6,Qt::Key_6);
    this->btnGroup->addButton(ui->Key_7,Qt::Key_7);
    this->btnGroup->addButton(ui->Key_8,Qt::Key_8);
    this->btnGroup->addButton(ui->Key_9,Qt::Key_9);

    this->btnGroup->addButton(ui->Key_A,Qt::Key_A);
    this->btnGroup->addButton(ui->Key_B,Qt::Key_B);
    this->btnGroup->addButton(ui->Key_C,Qt::Key_C);
    this->btnGroup->addButton(ui->Key_D,Qt::Key_D);
    this->btnGroup->addButton(ui->Key_E,Qt::Key_E);
    this->btnGroup->addButton(ui->Key_F,Qt::Key_F);
    this->btnGroup->addButton(ui->Key_G,Qt::Key_G);
    this->btnGroup->addButton(ui->Key_H,Qt::Key_H);
    this->btnGroup->addButton(ui->Key_I,Qt::Key_I);
    this->btnGroup->addButton(ui->Key_J,Qt::Key_J);
    this->btnGroup->addButton(ui->Key_K,Qt::Key_K);
    this->btnGroup->addButton(ui->Key_L,Qt::Key_L);
    this->btnGroup->addButton(ui->Key_M,Qt::Key_M);
    this->btnGroup->addButton(ui->Key_N,Qt::Key_N);
    this->btnGroup->addButton(ui->Key_O,Qt::Key_O);
    this->btnGroup->addButton(ui->Key_P,Qt::Key_P);
    this->btnGroup->addButton(ui->Key_Q,Qt::Key_Q);
    this->btnGroup->addButton(ui->Key_R,Qt::Key_R);
    this->btnGroup->addButton(ui->Key_S,Qt::Key_S);
    this->btnGroup->addButton(ui->Key_T,Qt::Key_T);
    this->btnGroup->addButton(ui->Key_U,Qt::Key_U);
    this->btnGroup->addButton(ui->Key_V,Qt::Key_V);
    this->btnGroup->addButton(ui->Key_W,Qt::Key_W);
    this->btnGroup->addButton(ui->Key_X,Qt::Key_X);
    this->btnGroup->addButton(ui->Key_Y,Qt::Key_Y);
    this->btnGroup->addButton(ui->Key_Z,Qt::Key_Z);

    this->btnGroup->addButton(ui->Key_Space,Qt::Key_Space);
    this->btnGroup->addButton(ui->Key_Enter,Qt::Key_Enter);
    // this->button_group->addButton(ui->Key_Escape,Qt::Key_Escape);
    this->btnGroup->addButton(ui->Key_Period,Qt::Key_Period);
    this->btnGroup->addButton(ui->Key_CapsLock,Qt::Key_CapsLock);
    this->btnGroup->addButton(ui->Key_Backspace,Qt::Key_Backspace);
    this->btnGroup->addButton(ui->Key_Minus,Qt::Key_Minus);

}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.12.30
  * brief:      按键功能
******************************************************************************/
void InputKey::ButtonJudge(int id)
{
    switch (id)
    {
    case Qt::Key_0 :
        emit this->SendString("0");
        break;
    case Qt::Key_1 :
        emit this->SendString("1");
        break;
    case Qt::Key_2 :
        emit this->SendString("2");
        break;
    case Qt::Key_3 :
        emit this->SendString("3");
        break;
    case Qt::Key_4 :
        emit this->SendString("4");
        break;
    case Qt::Key_5 :
        emit this->SendString("5");
        break;
    case Qt::Key_6 :
        emit this->SendString("6");
        break;
    case Qt::Key_7 :
        emit this->SendString("7");
        break;
    case Qt::Key_8 :
        emit this->SendString("8");
        break;
    case Qt::Key_9 :
        emit this->SendString("9");
        break;

    case Qt::Key_A :
        emit this->SendString("a");
        break;
    case Qt::Key_B :
        emit this->SendString("b");
        break;
    case Qt::Key_C :
        emit this->SendString("c");
        break;
    case Qt::Key_D :
        emit this->SendString("d");
        break;
    case Qt::Key_E :
        emit this->SendString("e");
        break;
    case Qt::Key_F :
        emit this->SendString("f");
        break;
    case Qt::Key_G :
        emit this->SendString("g");
        break;
    case Qt::Key_H :
        emit this->SendString("h");
        break;
    case Qt::Key_I :
        emit this->SendString("i");
        break;
    case Qt::Key_J :
        emit this->SendString("j");
        break;
    case Qt::Key_K :
        emit this->SendString("k");
        break;
    case Qt::Key_L :
        emit this->SendString("l");
        break;
    case Qt::Key_M :
        emit this->SendString("m");
        break;
    case Qt::Key_N :
        emit this->SendString("n");
        break;
    case Qt::Key_O :
        emit this->SendString("o");
        break;
    case Qt::Key_P :
        emit this->SendString("p");
        break;
    case Qt::Key_Q :
        emit this->SendString("q");
        break;
    case Qt::Key_R :
        emit this->SendString("r");
        break;
    case Qt::Key_S :
        emit this->SendString("s");
        break;
    case Qt::Key_T :
        emit this->SendString("t");
        break;
    case Qt::Key_U :
        emit this->SendString("u");
        break;
    case Qt::Key_V :
        emit this->SendString("v");
        break;
    case Qt::Key_W :
        emit this->SendString("w");
        break;
    case Qt::Key_X :
        emit this->SendString("x");
        break;
    case Qt::Key_Y :
        emit this->SendString("y");
        break;
    case Qt::Key_Z :
        emit this->SendString("z");
        break;

    case Qt::Key_Space :
        emit this->SendString(" ");
        break;
    case Qt::Key_Period:
        emit this->SendString(".");
        break;
    case Qt::Key_Minus:
        emit this->SendString("-");
        break;
    case Qt::Key_Enter :
        emit this->SendEnter();
        emit this->SendEscape();
        break;
    case Qt::Key_Escape :
        emit this->SendEscape();
        break;
    case Qt::Key_Backspace :
        emit this->SendBackspace();
        break;
    case Qt::Key_CapsLock:
        emit this->SendCapsLock();
        break;
    default:
        emit this->SendString("wrong");
        break;

    }
}
void InputKey::Caps(bool set)
{
    if(set)
        ui->Key_CapsLock->setText(tr("大写"));
    else
        ui->Key_CapsLock->setText(tr("小写"));
}
void InputKey::updatePos()
{
    /*   QWidget *widget = QApplication::focusWidget();
    int y = 0;
    while (widget->parentWidget() != NULL) {
        y += widget->y();
        widget = widget->parentWidget();
    }  */

    /*    int x = QApplication::focusWidget()->y();
    if(x>240)
    {
        this->move(41,0);
    }
    else
    {
        this->move(41,600-240);
    }*/

    int mouse_y = QCursor::pos().y();//鼠标点击处纵坐标

    if(mouse_y>300)
    {
        this->move(41,0);
    }
    else
    {
        this->move(41,600-240);
    }


    //   this->show();
}

/******************************************************************************
                                    END
******************************************************************************/
