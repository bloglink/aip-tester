/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       系统设置模块
*******************************************************************************/
#ifndef WINSYST_H
#define WINSYST_H

#include <QFile>
#include <QTime>
#include <QDebug>
#include <QWidget>
#include <QShowEvent>
#include <QListView>
#include <QSettings>
#include <QProcess>
#include <QButtonGroup>
#include <QTextStream>
#include <QMessageBox>
#include <QHostAddress>
#include <QNetworkInterface>

#include "define.h"

namespace Ui {
class WinSyst;
}

class WinSyst : public QWidget
{
    Q_OBJECT

public:
    explicit WinSyst(QWidget *parent = 0);
    ~WinSyst();

private:
    Ui::WinSyst *ui;

signals:
    void SendCommand(quint16 addr, quint16 cmd, QByteArray data);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int id);
    void InitSettings(void);
    void SaveSettings(void);
    void SetDateTime(void);
    void SetPassword(void);
    void ReadMessage(quint16 addr, quint16 cmd, QByteArray msg);
    void SendWinCmdStartMode(void);
    void WriteLog(QByteArray msg);
    QString GetLocalHostIP(void);
    virtual void showEvent(QShowEvent *);
private:
    QString password;
    QFile *file;
    QDateTime dateTime;
};

#endif // WINSYST_H
