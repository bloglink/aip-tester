#ifndef PAGENUM_H
#define PAGENUM_H

#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>

namespace Ui {
class PageNum;
}

class PageNum : public QWidget
{
    Q_OBJECT

public:
    explicit PageNum(QWidget *parent = 0);
    ~PageNum();

private:
    Ui::PageNum *ui;

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

#endif // PAGENUM_H
