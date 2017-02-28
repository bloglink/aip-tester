#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QTime>
#include <QFile>
#include <QDebug>
#include <QWidget>
#include <QSettings>
#include <QButtonGroup>

#include "define.h"

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
    void SendMessage(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void initShow(void);
    void initButton(void);
    void judgeButton(int id);
    void jumpToWindow(QByteArray win);
private:
    QList<int> previous_window;
};

#endif // HOMEPAGE_H
