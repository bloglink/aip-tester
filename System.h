#ifndef SYSTEM_H
#define SYSTEM_H

#include <QTime>
#include <QDebug>
#include <QWidget>
#include <QListView>
#include <QSettings>
#include <QButtonGroup>

#include "define.h"

namespace Ui {
class System;
}

class System : public QWidget
{
    Q_OBJECT

public:
    explicit System(QWidget *parent = 0);
    ~System();

private:
    Ui::System *ui;

signals:
    void sendMessage(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void initShow(void);
    void initButton(void);
    void judgeButton(int id);
    void initSettings(void);
    void saveSettings(void);
    void modifyPassword(void);
    void readMessage(quint16 addr,quint16 cmd,QByteArray msg);
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
private:
    QString password;
};

#endif // SYSTEM_H
