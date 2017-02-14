#ifndef SETTINGS_H
#define SETTINGS_H

#include <QFile>
#include <QDebug>
#include <QWidget>
#include <QSettings>
#include <QListView>
#include <QSettings>
#include <QButtonGroup>
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
    void ExcuteCmd(QByteArray msg);
    void CmdConfigure(void);
    void SysStyle(void);
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
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
private:
    QProcess *process;
    QSettings *global;
    QString pwd;
};

#endif // SETTINGS_H
