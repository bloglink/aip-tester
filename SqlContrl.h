#ifndef SQLCONTRL_H
#define SQLCONTRL_H

#include <QUrl>
#include <QTime>
#include <QObject>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>

#define SQL "./data/aip.db"

class SqlContrl : public QObject
{
    Q_OBJECT
public:
    explicit SqlContrl(QObject *parent = 0);

private slots:
    void DeviceOpen(void);
    void DeviceQuit(void);
    void PutItem(QByteArray msg);
private:
    QSqlDatabase db;
    QString SqlName;
};

#endif // SQLCONTRL_H
