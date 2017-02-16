#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QDebug>
#include <QWidget>
#include <QLabel>
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
    virtual void resizeEvent(QResizeEvent *);
    virtual void mousePressEvent(QMouseEvent *ev);
private:
    QList<QColor> Colors;
    QStringList Names;
    QStringList Values;
};

#endif // HISTOGRAM_H
