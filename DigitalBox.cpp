#include "DigitalBox.h"

DigitalBox::DigitalBox(QWidget *parent) : QDoubleSpinBox(parent)
{

}

void DigitalBox::focusInEvent(QFocusEvent *e)
{
    this->clear();
    e->accept();
}

void DigitalBox::focusOutEvent(QFocusEvent *e)
{
    this->setValue(this->value());
    e->accept();
}

