/**
  ******************************************************************************
  * @file    waveform.cpp
  * @author  link
  * @version 2.0.0.0
  * @date    2017-02-27
  * @brief   Waveform of magnetic and impulse
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Waveform.h"
/**
  * @brief  Initializes
  * @param  parent:parent label
  * @retval None
  */
Waveform::Waveform(QWidget *parent) : QLabel(parent)
{
    isTest = false;
    isItem = false;
    isBlock = false;
    Block0 = 5;
    Block1 = 395;
    for (int i=0; i<MAX_WAVE; i++) {
        WaveBytes.append("0000");
        WaveTests.append("0000");
    }
}
/**
  * @brief  Initializes standard wave data
  * @param  msg:Standart wave data
  * @retval None
  */
void Waveform::WaveByteShow(QByteArray msg)
{
    WaveByte = msg;
    WaveByteH.clear();
    WaveByteY.clear();
    if (WaveByte.size() == 400) {
        for (int i=0; i<WaveByte.size(); i++)
            WaveByteH.append(quint8(WaveByte.at(i)));
    }
    if (WaveByte.size() == 800) {
        for (int i=0; i<WaveByte.size()/2; i++)
            WaveByteH.append(quint16(WaveByte.at(i*2))*256+quint8(WaveByte.at(i*2+1)));
    }
    for (int i=0; i<this->width(); i++) {
        int t = i*WaveByteH.size()/this->width();
        if (WaveByte.size() == 800)
            WaveByteY.append(this->height()-WaveByteH.at(t)*this->height()/1024);
        if (WaveByte.size() == 400)
            WaveByteY.append(this->height()-WaveByteH.at(t)*this->height()/256);
    }
    this->update();
}
/**
  * @brief  Initializes test wave data
  * @param  msg:Test wave data
  * @retval None
  */
void Waveform::WaveTestShow(QByteArray msg)
{
    WaveTest = msg;
    WaveTestH.clear();
    WaveTestY.clear();
    for (int i=0; i<WaveTest.size()/2; i++) {
        WaveTestH.append(quint16(msg.at(i*2)*256) + quint8(msg.at(i*2+1)));
    }
    for (int i=0; i<this->width(); i++) {
        int t = i*WaveTestH.size()/this->width();
        if (WaveTestH.size() == 400)
            WaveTestY.append(this->height()-WaveTestH.at(t)*this->height()/1024);
        if (WaveTestH.size() == 800)
            WaveTestY.append(this->height()-WaveTestH.at(t)*this->height()/65536);
    }
    isTest = true;
    this->update();
}
/**
  * @brief  Initializes wave item
  * @param  msg:wave item
  * @retval None
  */
void Waveform::WaveItemShow(QByteArray msg)
{
    WaveItem = msg;
    isItem = true;
}
/**
  * @brief  Initializes wave block
  * @param  b1:startup line postion;b2:end line postion.
  * @retval None
  */
void Waveform::WaveBlock(int b1,  int b2)
{
    Block0 = b1;
    Block1 = b2;
    isBlock = true;
    this->update();
}
/**
  * @brief  Calulate the hight of the standard wave
  * @param  None
  * @retval None
  */
void Waveform::InitWaveByte(quint8 s)
{
    QByteArray w = WaveBytes.at(s);
    WaveByteH.clear();
    for (int i=0; i<w.size()/2; i++) {
        WaveByteH.append(quint16(w.at(i*2)*256) + quint8(w.at(i*2+1)));
    }
}
/**
  * @brief  Calulate the hight of the test wave
  * @param  None
  * @retval None
  */
void Waveform::InitWaveTest(quint8 s)
{
    QByteArray w = WaveTests.at(s);
    WaveTestH.clear();
    for (int i=0; i<w.size()/2; i++) {
        WaveTestH.append(quint16(w.at(i*2)*256) + quint8(w.at(i*2+1)));
    }
}
/**
  * @brief  Draw waves
  * @param  None
  * @retval None
  */
void Waveform::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);
    painter->setPen(QPen(Qt::darkGreen,  1,  Qt::DotLine));
    //横线
    painter->drawLine(QPoint(0, height()*3/4), QPoint(width(), height()*3/4));
    painter->drawLine(QPoint(0, height()*2/4), QPoint(width(), height()*2/4));
    painter->drawLine(QPoint(0, height()*1/4), QPoint(width(), height()*1/4));
    //坚线
    painter->drawLine(QPoint(width()*1/4, 0), QPoint(width()*1/4, height()));
    painter->drawLine(QPoint(width()*2/4, 0), QPoint(width()*2/4, height()));
    painter->drawLine(QPoint(width()*3/4, 0), QPoint(width()*3/4, height()));

    painter->setPen(QPen(Qt::green,  1,  Qt::SolidLine));
    for (int i=0; i<WaveByteY.size()-1; i++) {
        painter->drawLine(QPoint(i, WaveByteY.at(i)), QPoint(i+1, WaveByteY.at(i+1)));
    }
    if (isTest) {
        painter->setPen(QPen(Qt::white,  1,  Qt::SolidLine));
        for (int i=0; i<WaveTestY.size()-1; i++) {
            painter->drawLine(QPoint(i, WaveTestY.at(i)), QPoint(i+1, WaveTestY.at(i+1)));
        }
    }
    if (isItem) {
        painter->drawText(QPoint(width()*3/4, height()*1/4-5), WaveItem);
    }
    if (isBlock) {
        int b0 = Block0*this->width()/400;
        int b1 = Block1*this->width()/400;
        painter->setPen(QPen(Qt::yellow,  1,  Qt::SolidLine));
        painter->drawLine(QPoint(b0, 0), QPoint(b0, height()));
        painter->setPen(QPen(Qt::red,  1,  Qt::SolidLine));
        painter->drawLine(QPoint(b1, 0), QPoint(b1, height()));
    }
    painter->end();
}
/**
  * @brief  Update wave when resize
  * @param  None
  * @retval None
  */
void Waveform::resizeEvent(QResizeEvent *)
{
    WaveByteShow(WaveByte);
    if (isTest)
        WaveTestShow(WaveTest);
    if (isBlock)
        WaveBlock(Block0, Block1);
}
/**
  * @brief  Click to change block
  * @param  e:Mouse event
  * @retval None
  */
void Waveform::mousePressEvent(QMouseEvent *e)
{
    if (!isBlock) {
        QLabel::mousePressEvent(e);
        return;
    }
    int x = e->x()*400/this->width();
    if (x >= 200)
        Block1 = x;
    else
        Block0 = x;
    this->update();
    emit BlockClick(x);
}
/*********************************END OF FILE**********************************/
