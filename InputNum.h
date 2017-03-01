#ifndef INPUTNUM_H
#define INPUTNUM_H

#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>

namespace Ui {
class InputNum;
}

class InputNum : public QWidget
{
    Q_OBJECT

public:
    explicit InputNum(QWidget *parent = 0);
    ~InputNum();

private:
    Ui::InputNum *ui;
signals:
    void ItemChange(QString msg);
public slots:
    void BtnInit(QStringList t);
public:
    QString Output;
    QButtonGroup *btnGroup;
private slots:
    void BtnJudge(int id);
    virtual void showEvent(QShowEvent *);
};

#endif // INPUTNUM_H
