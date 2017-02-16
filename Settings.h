#ifndef SETTINGS_H
#define SETTINGS_H

#include <QFile>
#include <QDebug>
#include <QWidget>
#include <QMessageBox>
#include <QSettings>
#include <QListView>
#include <QSettings>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QNetworkInterface>
#include <QProcess>
#include "main.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private:
    Ui::Settings *ui;
signals:
    void TransformCmd(quint16 addr,quint16 cmd,QByteArray data);
public slots:


public:
    quint8 StartMode;
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void DatSave(void);
    void ReadData();
    void Password(void);
    QString IpAddress(void);

    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray data);
    void ExcuteCanCmd(quint16 id,QByteArray msg);
    void TestStyle(void);
    void TestCheck(quint16 pos);

    void TestConfig(void);

    bool WaitTestOver(quint16 t);
    void Delay(int ms);

    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
private:
    QProcess *process;
    QSettings *global;
    QString pwd;
    bool Testing;
    bool isStop;
    quint16 TimeOut;
    quint16 Pos;
};

#endif // SETTINGS_H
