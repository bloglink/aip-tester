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
    void SendCommand(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void JudgeButtons(int id);
    void InitMotorTypes(void);
    void InitTestItems(void);
    void InitSettings(void);
    void SaveSettings(void);
    void SelectWireColor(int row,int column);
    void ShowWireColorWindow(void);
    void ShowAvailableItem(int,int);
    void SelectItemToTest(int row,int column);
    void AddSettings(void);
    void RemoveSettings(void);
    void ReadSettings(void);
    void QuerySettings(void);

    QString CurrentSettings(void);
    QStringList CurrentItems(void);
    QStringList EnableItems(void);

    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
private:
    QButtonGroup *btnGroup;
    QStringList prev;
    QStringList ItemNames;
    QStringList FileNames;
};

#endif // WINTYPE_H
