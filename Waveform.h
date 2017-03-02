#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QDebug>
#include <QLabel>
#include <QPoint>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class Waveform : public QLabel
{
    Q_OBJECT
public:
    explicit Waveform(QWidget *parent = 0);

signals:
    void BlockClick(int x);
public slots:
    void WaveByteShow(QByteArray msg);
    void WaveTestShow(QByteArray msg);
    void WaveItemShow(QByteArray msg);
    void WaveBlock(int o,int t);
    void InitWaveByte(void);
    void InitWaveTest(void);
public:
    int Block0;
    int Block1;
    int WaveLine;
    int WaveGroup;
    QByteArray WaveByte;
    QByteArray WaveTest;
    QByteArray WaveItem;
    QList<quint16> WaveByteH;
    QList<quint16> WaveTestH;
    QList<quint16> WaveByteY;
    QList<quint16> WaveTestY;
private slots:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);
    virtual void mousePressEvent(QMouseEvent *ev);
private:
    bool isTest;
    bool isItem;
    bool isBlock;
};

#endif // WAVEFORM_H
