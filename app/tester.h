/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       测试界面
*******************************************************************************/
#ifndef TESTER_H
#define TESTER_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>

#include <QtXml/QDomDocument>

#include "lqstandarditem.h"
#include "lib/qcustomplot.h"

#define CMD_START 104
#define CMD_STOP 105

class Tester : public QWidget
{
    Q_OBJECT
public:
    explicit Tester(QWidget *parent = 0);

signals:
    void sendAppShow(QString win);
    void sendAppMsg(QJsonObject obj);
    void buttonTest(bool s, quint16 st);
public slots:
    void setMasterName(QString user);
    void initItems(quint16 s);
    void updateItems(QString dat);
    void updateItem(QString item);
    bool updateJudge(QString item);
    bool updateResult(quint16 s);
    void updateTemp(QString s);
    QString readResult();
    QString readNumb();
    void drawMAG(QString w);
    void saveMAG(QString w);
    void drawIMP(QString w);
    void saveIMP(QString w);
    void drawStd(QString w);
    void saveStd(QString w);
private slots:
    void initUI();
    void initView();
    void initWave();
    void initStatusBar();
    void initButtonBar();
    void initHistogram();
    void initWireColor();
    void initStationBar();
    void initLayout();
    void initSettings();
    QString CurrentSettings();
    int row();
    void showBarcode();
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

    void clickButton();
    void clickView();
    void clearWave();
    void DrawHistogram();

    void recvAppShow(QString win);
private:
    QTableView *view;
    LQStandardItem *mView;

    QCustomPlot *wave;
    QList<QCPGraph*> graphs;

    QLabel *textType;
    QLabel *textNumb;
    QLabel *textUser;
    QLabel *textTemp;
    QString barcode;
    QTimer *scanner;

    QVBoxLayout *btnLayout;

    QCustomPlot *histogram;
    QList<QCPBars*> bars;
    quint32 countOk;
    quint32 countAll;

    QGridLayout *colorLayout;
    QList<QLabel *> colors;

    QLabel *station;
    QLabel *judge;

    int mag_count;
    int imp_count;
    QMap<int, QString> waveMAG;
    QMap<int, QString> waveIMP;
    QMap<int, QString> waveStd;
    QVector<QString> labels;
};

#endif // TESTER_H
