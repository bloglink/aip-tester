#ifndef MOTORTYPE_H
#define MOTORTYPE_H

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
class MotorType;
}

class MotorType : public QWidget
{
    Q_OBJECT

public:
    explicit MotorType(QWidget *parent = 0);
    ~MotorType();

private:
    Ui::MotorType *ui;

signals:
    void sendMessage(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void initShow(void);
    void initButton(void);
    void judgeButton(int id);
    void initTypes(void);
    void initItems(void);
    void initSettings(void);
    void saveSettings(void);
    void selectWireColor(int row,int column);
    void showWireColor(void);
    void showAvailableItem(int,int);
    void selectItemToTest(int row,int column);
    void addType(void);
    void deleteType(void);
    void readType(void);
    void queryType(void);
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

#endif // MOTORTYPE_H
