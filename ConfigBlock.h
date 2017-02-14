#ifndef CONFIGBLOCK_H
#define CONFIGBLOCK_H

#include <QDebug>
#include <QWidget>
#include <QListView>
#include <QSettings>
#include <QComboBox>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include "main.h"

namespace Ui {
class ConfigBlock;
}

class ConfigBlock : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigBlock(QWidget *parent = 0);
    ~ConfigBlock();

private:
    Ui::ConfigBlock *ui;

signals:
    void TransformCmd(quint16 addr,quint16 cmd,QByteArray data);
public slots:
    void DisplayInit(void);
    void ExcuteCmd(QByteArray msg);
    void CmdCheckState(void);
    void CmdStartTest(quint8 pos);
    void CmdSample(void);
    void CmdStopTest(void);
    void CmdConfigure(void);
    bool WaitTestOver(void);
public:
    QStringList ListItem;
    QStringList ListPara;
    QStringList ListResult;
    QStringList ListJudge;
private slots:
    void BtnInit(void);
    void DatInit(void);
    void DatSave(void);
    void SampleOver(void);

    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);

private:
    QSettings *set;
    bool isCheckOk;
    bool Testing;
    quint16 TimeOut;
};

#endif // CONFIGBLOCK_H
