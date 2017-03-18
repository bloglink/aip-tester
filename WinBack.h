#ifndef WINBACK_H
#define WINBACK_H

#include <QDebug>
#include <QWidget>
#include <QSettings>
#include "define.h"

namespace Ui {
class WinBack;
}

class WinBack : public QWidget
{
    Q_OBJECT

public:
    explicit WinBack(QWidget *parent = 0);
    ~WinBack();

private:
    Ui::WinBack *ui;
signals:
    void SendCommand(quint16 addr,quint16 cmd,QByteArray msg);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void BtnJudge(int id);
    void InitSettings(void);
    void SaveSettings(void);
    void ClickItem(int r,int c);
    void ClickOutput(int r,int c);
    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    void ExcuteCanCmd(quint16 addr,QByteArray msg);
    void SendCanCmdVersion(quint16 id);
    void SendCanCmdParam(quint16 id);
    void ReadCanCmdDcr(QByteArray msg);
    void ReadCanCmdInr(QByteArray msg);
    void ReadCanCmdImp(QByteArray msg);
    void ReadCanCmdInd(QByteArray msg);
    void ReadCanCmdPwr(QByteArray msg);
    void ReadCanCmdOut13(QByteArray msg);
    void ReadCanCmdOut14(QByteArray msg);
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
private:
    bool Testing;
};

#endif // WINBACK_H
