#ifndef SQLPAGE_H
#define SQLPAGE_H

#include <QTime>
#include <QFile>
#include <QDebug>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlTableModel>

#include "define.h"

namespace Ui {
class SqlPage;
}

class SqlPage : public QWidget
{
    Q_OBJECT

public:
    explicit SqlPage(QWidget *parent = 0);
    ~SqlPage();

private:
    Ui::SqlPage *ui;

signals:
    void sendMessage(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void initShow(void);
    void initButton(void);
    void judgeButton(int id);
    void initSql(void);
    void readSql(void);
    void queryItem(QString item);
    void clearSql(void);
    virtual void showEvent(QShowEvent *);

private:
    QSqlDatabase db;
    QSqlTableModel *model;
};

#endif // SQLPAGE_H
