#ifndef CONFIGLVS_H
#define CONFIGLVS_H

#include <cmath>
#include <QDebug>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSettings>
#include <QListView>
#include <QMessageBox>
#include <QHeaderView>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>
#include "main.h"

namespace Ui {
class ConfigLvs;
}

class ConfigLvs : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigLvs(QWidget *parent = 0);
    ~ConfigLvs();

private:
    Ui::ConfigLvs *ui;
signals:
    void TransformCmd(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void DatSave(void);
private:
    QSettings *set;
    bool isCheckOk;
    bool Testing;
    quint16 TimeOut;
};

#endif // CONFIGLVS_H
