#include "Histogram.h"

Histogram::Histogram(QWidget *parent) : QLabel(parent)
{
    Colors.append(QColor(Qt::red));
    Colors.append(QColor(Qt::green));
    Colors.append(QColor(Qt::blue));
    Colors.append(QColor(Qt::cyan));
    Colors.append(QColor(Qt::magenta));
    Colors.append(QColor(Qt::yellow));
    Colors.append(QColor(Qt::darkRed));
    Colors.append(QColor(Qt::darkGreen));
    Colors.append(QColor(Qt::darkBlue));
    Colors.append(QColor(Qt::darkCyan));
    Colors.append(QColor(Qt::darkMagenta));
    Colors.append(QColor(Qt::darkYellow));
}

void Histogram::SetNames(QStringList msg)
{
    Names = msg;
}

void Histogram::SetValues(QStringList msg)
{
    Values = msg;
}

void Histogram::paintEvent(QPaintEvent *)
{

}

void Histogram::resizeEvent(QResizeEvent *)
{

}

void Histogram::mousePressEvent(QMouseEvent *ev)
{
    qDebug()<<ev->x();
}

