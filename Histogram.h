#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QDebug>
#include <QDebug>
#include <QLabel>
#include <QPoint>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class Histogram : public QLabel
{
    Q_OBJECT
public:
    explicit Histogram(QWidget *parent = 0);

signals:

public slots:
    void SetNames(QStringList msg);
    void SetValues(QStringList msg);
private slots:
    virtual void paintEvent(QPaintEvent *);
private:
    QList<QColor> Colors;
    QStringList Names;
    QList<quint16> Values;
    quint16 Max;
};

#endif // HISTOGRAM_H
