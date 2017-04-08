#ifndef WINSYST_H
#define WINSYST_H

#include <QFile>
#include <QTime>
#include <QDebug>
#include <QWidget>
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
    void SendCommand(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void BtnJudge(int id);
    void InitSettings(void);
    void SaveSettings(void);
    void SetDateTime(void);
    void SetPassword(void);
    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
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
