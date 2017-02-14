#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QDebug>
#include <QWidget>
#include <QSettings>
#include <QDateTime>
#include <QButtonGroup>

#include "main.h"

namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = 0);
    ~HomePage();

private:
    Ui::HomePage *ui;
signals:
    void TransformCmd(quint16 addr,quint16 cmd,QByteArray data);
public slots:
    void ExcuteCmd(QByteArray msg);
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    bool TimeJudge(void);
    void TimeError(void);
    virtual void hideEvent(QHideEvent *);

private:
    QSettings *set;
};

#endif // HOMEPAGE_H
