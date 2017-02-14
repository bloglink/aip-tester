#ifndef DIGITALBOX_H
#define DIGITALBOX_H

#include <QDebug>
#include <QWidget>
#include <QLineEdit>
#include <QFocusEvent>
#include <QDoubleSpinBox>

class DigitalBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    explicit DigitalBox(QWidget *parent = 0);

signals:

public slots:
    void focusInEvent(QFocusEvent *);
    void focusOutEvent(QFocusEvent *);
public:
    double digital;
};

#endif // DIGITALBOX_H
