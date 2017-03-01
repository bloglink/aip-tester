#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QTime>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QSettings>
#include <QMessageBox>
#include <QButtonGroup>
#include <QElapsedTimer>

#include "define.h"

#include "System.h"
#include "MotorType.h"
#include "SqlPage.h"
#include "Test.h"

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
    void PutCanData(QByteArray msg);
    void WriteSql(QByteArray msg);
    void sendMessage(quint16 addr,quint16 cmd,QByteArray data);
public slots:
    void initAllWindows(void);
private slots:
    void initShow(void);
    void initButton(void);
    void judgeButton(int id);
    void jumpToWindow(QByteArray win);
    void readMessage(quint16 addr,quint16 cmd,QByteArray data);
    void initTest(void);
    void checkStatus(void);
    void startTest(QByteArray msg);
    void saveTest(void);
    void TestPause(void);
    void saveJudge(QByteArray msg);
    bool WaitTestOver(quint16 t);
    void delay(int ms);
private:
    QList<int> previous_window;
    QString motor_type;
    bool Testing;
    bool isCheckOk;
    QString ItemJudge;
    QStringList ItemToTest;
    int PauseMode;
    QStringList Items;
};

#endif // HOMEPAGE_H
