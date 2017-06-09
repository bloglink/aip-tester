/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       波形处理模块
*******************************************************************************/
#include "WaveBox.h"

WaveBox::WaveBox(QWidget *parent) : QLabel(parent)
{
}

void WaveBox::ShowWave(QVariantHash hash)
{
    WaveInit = hash;
    WaveItem.clear();
    WaveByte.clear();
    WaveTest.clear();
    WaveItem = hash.value("WaveItem").toByteArray();
    QByteArray Byte = QByteArray::fromBase64(hash.value("WaveByte").toByteArray());
    QByteArray Test = QByteArray::fromBase64(hash.value("WaveTest").toByteArray());
    if (Byte.size() <= 400 && Byte.size() >= 395) {
        for (int i=0; i < this->width(); i++) {
            int t = qMin(i*Byte.size()/this->width(), Byte.size());
            int h = Byte.at(t);
            WaveByte.append(this->height()-h*this->height()/256);
        }
    }
    if (Byte.size() <= 800 && Byte.size() >= 790) {
        for (int i=0; i < this->width(); i++) {
            int t = qMin(i*(Byte.size()/2)/this->width(), Byte.size()/2);
            int h = quint16(Byte.at(2*t))*256 + quint8(Byte.at(2*t+1));
            WaveByte.append(this->height()-h*this->height()/1024);
        }
    }
    if (Test.size() <= 400 && Test.size() >= 395) {
        for (int i=0; i < this->width(); i++) {
            int t = qMin(i*Test.size()/this->width(), Test.size());
            int h = Test.at(t);
            WaveTest.append(this->height() - h*this->height()/256);
        }
    }
    if (Test.size() <= 800 && Test.size() >= 790) {
        for (int i=0; i < this->width(); i++) {
            int t = qMin(i*(Test.size()/2)/this->width(), Test.size()/2);
            int h = quint16(Test.at(2*t))*256 + quint8(Test.at(2*t+1));
            WaveTest.append(this->height() - h*this->height()/1024);
        }
    }
    this->update();
}

void WaveBox::paintEvent(QPaintEvent *e)
{
    QPainter *painter = new QPainter(this);
    painter->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    //横线
    painter->drawLine(QPoint(0, height()*3/4), QPoint(width(), height()*3/4));
    painter->drawLine(QPoint(0, height()*2/4), QPoint(width(), height()*2/4));
    painter->drawLine(QPoint(0, height()*1/4), QPoint(width(), height()*1/4));
    //坚线
    painter->drawLine(QPoint(width()*1/4, 0), QPoint(width()*1/4, height()));
    painter->drawLine(QPoint(width()*2/4, 0), QPoint(width()*2/4, height()));
    painter->drawLine(QPoint(width()*3/4, 0), QPoint(width()*3/4, height()));

    if (!WaveByte.isEmpty()) {
        painter->setPen(QPen(Qt::white, 1, Qt::SolidLine));
        for (int i=0; i < WaveByte.size()-1; i++) {
            painter->drawLine(QPoint(i, WaveByte.at(i)), QPoint(i+1, WaveByte.at(i+1)));
        }
    }
    if (!WaveTest.isEmpty()) {
        painter->setPen(QPen(Qt::green, 1, Qt::SolidLine));
        for (int i=0; i < WaveTest.size()-1; i++) {
            painter->drawLine(QPoint(i, WaveTest.at(i)), QPoint(i+1, WaveTest.at(i+1)));
        }
    }
    if (!WaveInit.value("Block0").toString().isEmpty()) {
        int b0 = WaveInit.value("Block0").toString().toInt()*this->width()/400;
        painter->setPen(QPen(Qt::yellow, 1, Qt::SolidLine));
        painter->drawLine(QPoint(b0, 0), QPoint(b0, height()));
    }
    if (!WaveInit.value("Block1").toString().isEmpty()) {
        int b1 = WaveInit.value("Block1").toString().toInt()*this->width()/400;
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine));
        painter->drawLine(QPoint(b1, 0), QPoint(b1, height()));
    }
    if (!WaveItem.isEmpty())
        painter->setPen(QPen(Qt::white, 1, Qt::SolidLine));
    painter->drawText(QPoint(width()*3/4, height()*1/4-5), WaveItem);
    painter->end();
    e->accept();
}

void WaveBox::resizeEvent(QResizeEvent *e)
{
    ShowWave(WaveInit);
    e->accept();
}

void WaveBox::mousePressEvent(QMouseEvent *e)
{
    int x = e->x()*400/this->width();
    if (!WaveInit.value("Block0").toString().isEmpty() && x <= 199)
        WaveInit.insert("Block0", QString::number(x));
    if (!WaveInit.value("Block1").toString().isEmpty() && x >= 200)
        WaveInit.insert("Block1", QString::number(x));
    this->update();
    emit SendVariant(WaveInit);
    QLabel::mousePressEvent(e);
}
/*********************************END OF FILE**********************************/
