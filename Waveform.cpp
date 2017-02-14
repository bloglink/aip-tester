#include "Waveform.h"

Waveform::Waveform(QWidget *parent) : QLabel(parent)
{
    isTest = false;
    isItem = false;
    isBlock = false;
}

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
        else
            WaveByteY.append(this->height()-WaveByteH.at(t)*this->height()/256);
    }
    this->update();
}

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
        else
            WaveTestY.append(this->height()-WaveTestH.at(t)*this->height()/65536);
    }
    isTest = true;
    this->update();
}

void Waveform::WaveItemShow(QByteArray msg)
{
    WaveItem = msg;
    isItem = true;
}

void Waveform::WaveBlock(int b1, int b2)
{
    Block0 = b1;
    Block1 = b2;
    isBlock = true;
    this->update();
}

void Waveform::InitWaveByte()
{
    WaveByteH.clear();
    for (int i=0; i<WaveByte.size()/2; i++) {
        WaveByteH.append(quint16(WaveByte.at(i*2)*256) + quint8(WaveByte.at(i*2+1)));
    }
}

void Waveform::InitWaveTest()
{
    WaveTestH.clear();
    for (int i=0; i<WaveTest.size()/2; i++) {
        WaveTestH.append(quint16(WaveTest.at(i*2)*256) + quint8(WaveTest.at(i*2+1)));
    }
}

void Waveform::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);
    painter->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    //横线
    painter->drawLine(QPoint(0,height()*3/4),QPoint(width(),height()*3/4));
    painter->drawLine(QPoint(0,height()*2/4),QPoint(width(),height()*2/4));
    painter->drawLine(QPoint(0,height()*1/4),QPoint(width(),height()*1/4));
    //坚线
    painter->drawLine(QPoint(width()*1/4,0),QPoint(width()*1/4,height()));
    painter->drawLine(QPoint(width()*2/4,0),QPoint(width()*2/4,height()));
    painter->drawLine(QPoint(width()*3/4,0),QPoint(width()*3/4,height()));

    painter->setPen(QPen(Qt::green, 1, Qt::SolidLine));
    for (int i=0; i<WaveByteY.size()-1; i++) {
        painter->drawLine(QPoint(i,WaveByteY.at(i)),QPoint(i+1,WaveByteY.at(i+1)));
    }
    if (isTest) {
        painter->setPen(QPen(Qt::white, 1, Qt::SolidLine));
        for (int i=0; i<WaveTestY.size()-1; i++) {
            painter->drawLine(QPoint(i,WaveTestY.at(i)),QPoint(i+1,WaveTestY.at(i+1)));
        }
    }
    if (isItem) {
        painter->drawText(QPoint(width()*3/4,height()*1/4-5),WaveItem);
    }
    if (isBlock) {
        int b0 = Block0*this->width()/400;
        int b1 = Block1*this->width()/400;
        painter->setPen(QPen(Qt::yellow, 1, Qt::SolidLine));
        painter->drawLine(QPoint(b0,0),QPoint(b0,height()));
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine));
        painter->drawLine(QPoint(b1,0),QPoint(b1,height()));
    }
    painter->end();
}

void Waveform::resizeEvent(QResizeEvent *)
{
    WaveByteShow(WaveByte);
    if (isTest)
        WaveTestShow(WaveTest);
    if (isBlock)
        WaveBlock(Block0,Block1);
}

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
