/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170424
 * author:      zhaonanlin
 * brief:       数据管理模块
*******************************************************************************/
#ifndef WINDATA_H
#define WINDATA_H

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QShowEvent>
#include <QTime>
#include <QFile>
#include <QDebug>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QTextCodec>
#include <QProcess>

typedef struct {
    QString  wm_item;
    QString wm_para;
    QString wm_result;
    QString wm_judge;
    QString wm_date;
}wm_ResData;


//typedef struct
//{
//    QString wm_r1;
//    QString wm_r2;
//    QString wm_r3;
//    QString wm_r4;
//    QString wm_
//}wm_head;


typedef struct
{
    QString wm_atp;
    int wm_d;
}wm_Resatp;

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
    void SendVariant(QVariantHash s);
    void SendCommand(quint16 addr, quint16 cmd, QByteArray data);
public:
    QList<QString>m_vhead;
    QList<QString>m_head;               //标题头
    QList<QString>m_chead;
    QList<QString>mm_head;
    QList<QString>typeExlist;
    QList<wm_ResData>mm_xcode;
    QList<wm_ResData>exportdata;
    wm_ResData str_ResData;
    QList<QString> Ex_head(int b,QString qs);
    bool filecsv(int istime,QString qs,QString filepath);
    QThread *thread_and;
private slots:
    void initUI();
    void initDB();
    void initData();
    void initSql(QString sql);
    void typeSql(QString qs);
    void SendWarnning(QString s);
    void InitButtons(void);
    void ReadButtons(int id);
    void InitSqlite(void);
    void ReadSqliteAll(void);
    void ClearSqlite(void);
    void ExportToUsb(void);
    QString ReadUsb(void);
    virtual void showEvent(QShowEvent *);

    QByteArray ToGbk(const QString &inStr);
    void GoToWindow(QString w);
    QString filePath();
    void ReadView(QString item);
private:
    QSqlDatabase db;
    QSqlTableModel *model;
    QStringList items;
};

#endif // WINDATA_H
