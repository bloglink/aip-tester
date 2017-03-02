#ifndef SQLDATA_H
#define SQLDATA_H

#include <QUrl>
#include <QDate>
#include <QTime>
#include <QObject>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QStringList>
#include <QDebug>

#include "define.h"

class SqlData : public QObject
{
    Q_OBJECT
public:
    explicit SqlData(QObject *parent = 0);

private slots:
    void DeviceOpen(void);
    void DeviceQuit(void);
    void Write(QByteArray msg);
private:
    QSqlDatabase db;
};

#endif // SQLDATA_H
