/**
  ******************************************************************************
  * @file    histogram.cpp
  * @author  link
  * @version 2.0.0.0
  * @date    2017-02-27
  * @brief   histogram for sql data
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Histogram.h"
/**
  * @brief  Initializes
  * @param  parent:parent label
  * @retval None
  */
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
/**
  * @brief  Initializes histogram names
  * @param  msg:histogram names
  * @retval None
  */
void Histogram::setNames(QStringList msg)
{
    Names = msg;
}
/**
  * @brief  Initializes histogram values
  * @param  msg:histogram values
  * @retval None
  */
void Histogram::setValues(QStringList msg)
{
    Max = 1;
    Values.clear();
    for (int i=0; i<msg.size(); i++) {
        Values.append(QString(msg.at(i)).remove("%").toInt());
        if (Values.at(i) > Max)
            Max = Values.at(i);
    }
}
/**
  * @brief  draw histogram
  * @param  None
  * @retval None
  */
void Histogram::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);
    if (Names.size() == 0)
        return;

    int w = this->width()/Names.size();
    int h = this->height();
    w = qMin(w,100);

    for (int i=0; i<Names.size(); i++) {

        int xx = w-20;
        int yy = (h-50)*(Values.at(i))/Max;
        int x = i*w+20;
        int y = h-yy-25;
        painter->setPen(QPen(Colors.at(i)));
        painter->setBrush(QBrush(Colors.at(i)));
        painter->drawRect(x,y,xx,yy);

        painter->setPen(QPen(QColor(Qt::white)));
        painter->drawText(QRect(x+xx/2-15,h-25,xx,25),Names.at(i));
        painter->drawText(QRect(x+xx/2-15,h-yy-40,xx,25),QString("%1%").arg(Values.at(i)));
    }
    painter->drawLine(5,h-24,this->width(),h-24);
    painter->drawLine(5,5,5,h-24);

    painter->end();
}
/*********************************END OF FILE**********************************/

