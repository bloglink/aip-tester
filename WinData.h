#ifndef WINDATA_H
#define WINDATA_H

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QTime>
#include <QFile>
#include <QDebug>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlTableModel>

#include "define.h"

namespace Ui {
class WinData;
}

class WinData : public QWidget
{
    Q_OBJECT

public:
    explicit WinData(QWidget *parent = 0);
    ~WinData();

private:
    Ui::WinData *ui;
signals:
    void SendCommand(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void SqlInit(void);
    void SqlRead(void);
    void SqlQuery(QString item);
    void SqlClear(void);
    void SqlExport(void);
    QString ReadUsb(void);
    virtual void showEvent(QShowEvent *);

private:
    QSqlDatabase db;
    QSqlTableModel *model;
};

#endif // WINDATA_H
