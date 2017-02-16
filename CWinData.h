#ifndef CWINDATA_H
#define CWINDATA_H

#include <QDir>
#include <QFile>
#include <QWidget>
#include <QSettings>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include "main.h"
#include <QTableWidgetItem>
namespace Ui {
class CWinData;
}

class CWinData : public QWidget
{
    Q_OBJECT

public:
    explicit CWinData(QWidget *parent = 0);
    ~CWinData();

private:
    Ui::CWinData *ui;

signals:
    void TransformCmd(quint16 addr,quint16 cmd,QByteArray data);
public:
    QList<QTableWidgetItem*> Amount;
    QList<QTableWidgetItem*> Dcr;
    QList<QTableWidgetItem*> Mag;
    QList<QTableWidgetItem*> Dir;
    QList<QTableWidgetItem*> Inr;
    QList<QTableWidgetItem*> Acw;
    QList<QTableWidgetItem*> Imp;
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void SqlInit(void);
    void DatInit(void);
    void ItemInit(QString item);
    void DeleteAll(void);
    virtual void showEvent(QShowEvent *);
private:
    QSqlDatabase db;
    QSqlTableModel *model;
    QSettings *global;
    int MaxId;
    QString SqlName;
};

#endif // CWINDATA_H
