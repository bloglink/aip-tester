#ifndef SQLCLIENT_H
#define SQLCLIENT_H
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

class SqlClient : public QObject
{
    Q_OBJECT
public:
    explicit SqlClient(QObject *parent = 0);

private slots:
    void DeviceOpen(void);
    void DeviceQuit(void);
    void Write(QByteArray msg);
private:
    QSqlDatabase db;
};

#endif // SQLCLIENT_H
