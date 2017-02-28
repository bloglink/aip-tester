#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QPainter>

class Histogram : public QLabel
{
    Q_OBJECT
public:
    explicit Histogram(QWidget *parent = 0);

public slots:
    void setNames(QStringList msg);
    void setValues(QStringList msg);
private slots:
    virtual void paintEvent(QPaintEvent *);
private:
    QList<QColor> Colors;
    QStringList Names;
    QList<quint16> Values;
    quint16 Max;
};

#endif // HISTOGRAM_H
