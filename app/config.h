/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       型号管理
*******************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

#include <QFile>
#include <QDir>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDebug>
#include <QLabel>
#include <QGroupBox>
#include <QToolButton>
#include <QColorDialog>
#include <QPalette>
#include <QColor>
#include <QSettings>
#include <QJsonObject>
#include <QMessageBox>
#include <QListView>

#include <QStandardItemModel>
#include "lqstandarditem.h"
#include "lqsqltablemodel.h"
#include "lqreadonlyitem.h"

#define CMD_CONFIG 102

class Config : public QWidget
{
    Q_OBJECT
public:
    explicit Config(QWidget *parent = 0);

signals:
    void sendAppShow(QString win);
    void sendAppMsg(QJsonObject obj);
    void updateSettings();
private slots:
    void initUI();
    void initType();
    void initTypeBar();
    void initTest();
    void initTestBar();
    void initDisplay();
    void initLayout();
    void readSettings();
    void readSysItems();
    void clickType(QModelIndex index);
    void appendType();
    void deleteType();
    void updateType();
    void clickItem(QModelIndex index);
    void clickTest(QModelIndex index);
    void deleteTest();
    void configTest();
    void selectColor();
    void autoPixmap(QString name);
    QString CurrentSettings();
    void back();

    void updateSqlite();
    void recvAppShow(QString win);
    void recvSqlOpen(QString sql);
    void recvAppCmd(QJsonObject msg);
private:
    QTableView *type;
    LQStandardItem *mType;
    QLineEdit *typeLineEdit;
    QVBoxLayout *viewLayout;

    QTableView *test;
    LQStandardItem *mTest;
    QHBoxLayout *testLayout;

    QTableView *item;
    LQSqlTableModel *mItem;

    QString sqlName;

    QComboBox *typeComboBox;
    QComboBox *testComboBox;
    QLabel *typePixmap;

    QGridLayout *tLayout;

    QList<QPushButton *> buttons;
    QList<QPushButton *> colors;

    QFrame *btnFrame;
    QStringList btnNames;
    QStringList testItem;
    QStringList FileNames;
};

#endif // CONFIG_H
