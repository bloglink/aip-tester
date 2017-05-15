/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       波形处理模块
*******************************************************************************/
#ifndef WAVEBOX_H
#define WAVEBOX_H

#include <QDebug>
#include <QLabel>
#include <QPoint>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#define MAX_WAVE 5

class WaveBox : public QLabel
{
    Q_OBJECT
public:
    explicit WaveBox(QWidget *parent = 0);

signals:
    void BlockClick(QVariant msg);
public slots:
    void ShowWave(QVariant msg);
private slots:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);
    virtual void mousePressEvent(QMouseEvent *ev);
private:
    QVariant WaveInit;
    QByteArray WaveItem;
    QList<quint16> WaveByte;
    QList<quint16> WaveTest;

};

#endif // WAVEBOX_H
