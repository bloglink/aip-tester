#ifndef WINSYST_H
#define WINSYST_H

#include <QTime>
#include <QDebug>
#include <QWidget>
#include <QListView>
#include <QSettings>
#include <QProcess>
#include <QButtonGroup>

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
    void SendMessage(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void SetInit(void);
    void SetSave(void);
    void SetDateTime(void);
    void Password(void);
    void ReadMessage(quint16 addr,quint16 cmd,QByteArray msg);
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
private:
    QString password;
};

#endif // WINSYST_H
