#include "PageNum.h"
#include "ui_PageNum.h"

PageNum::PageNum(QWidget *parent) :
    QWidget(parent,Qt::Popup),
    ui(new Ui::PageNum)
{
    ui->setupUi(this);
}

PageNum::~PageNum()
{
    delete ui;
}
void PageNum::BtnInit(QStringList t)
{
    QGridLayout *layout = new QGridLayout;
    btnGroup = new QButtonGroup;
    for (int i=0; i<t.size(); i++) {
        btnGroup->addButton(new QPushButton(this),i);
        btnGroup->buttons().at(i)->setText(t.at(i));
        layout->addWidget(btnGroup->buttons().at(i),i/4,i%4);
        btnGroup->buttons().at(i)->setMinimumSize(97,44);
    }
    btnGroup->addButton(new QPushButton(this),t.size());
    btnGroup->buttons().last()->setText(tr("返回"));
    btnGroup->buttons().last()->setMinimumSize(97,44);
    layout->addWidget(btnGroup->buttons().last(),t.size()/4+1,3);
    ui->BoxButton->setLayout(layout);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/*******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.12.20
  * brief:      按键功能
*******************************************************************************/
void PageNum::BtnJudge(int id)
{
    if (id != btnGroup->buttons().size()-1)
        emit ItemChange(btnGroup->button(id)->text());
    this->hide();
}

void PageNum::showEvent(QShowEvent *)
{
    QWidget *widget = this;
    while (widget->parentWidget() != NULL) {
        widget = widget->parentWidget();
    }
    int w = widget->width();
    int h = widget->height();
    int x = (w-480)/2;
    int y = (h-240)/2;
    this->move(x,y);
}
