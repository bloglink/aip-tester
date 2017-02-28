#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QTime>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QSettings>
#include <QButtonGroup>

#include "define.h"

#include "System.h"

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
    void sendMessage(quint16 addr,quint16 cmd,QByteArray data);
public slots:
    void initAllWindows(void);
private slots:
    void initShow(void);
    void initButton(void);
    void judgeButton(int id);
    void jumpToWindow(QByteArray win);
    void readMessage(quint16 addr,quint16 cmd,QByteArray data);
private:
    QList<int> previous_window;
};

#endif // HOMEPAGE_H
