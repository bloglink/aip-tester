#ifndef WINTYPE_H
#define WINTYPE_H

#include <QDir>
#include <QTime>
#include <QDebug>
#include <QWidget>
#include <QString>
#include <QListView>
#include <QSettings>
#include <QHideEvent>
#include <QButtonGroup>

#include "define.h"

namespace Ui {
class WinType;
}

class WinType : public QWidget
{
    Q_OBJECT

public:
    explicit WinType(QWidget *parent = 0);
    ~WinType();

private:
    Ui::WinType *ui;

signals:
    void SendMessage(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void WinInit(void);
    void BtnInit(void);
    void judgeButton(int id);
    void TypesInit(void);
    void ItemsInit(void);
    void SetInit(void);
    void SetSave(void);
    void selectWireColor(int row,int column);
    void showWireColor(void);
    void showAvailableItem(int,int);
    void selectItemToTest(int row,int column);
    void TypeAdd(void);
    void TypeDelete(void);
    void TypeRead(void);
    void TypeQuery(void);
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
private:
    QButtonGroup *btnGroup;
    QSettings *c_settings;
    QSettings *g_settings;
    QStringList prev;
    QStringList ItemNames;
    QStringList FileNames;
    QString FileInUse;
};

#endif // WINTYPE_H
