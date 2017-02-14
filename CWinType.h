#ifndef CWINTYPE_H
#define CWINTYPE_H

#include <QDir>
#include <QDebug>
#include <QWidget>
#include <QString>
#include <QListView>
#include <QSettings>
#include <QHideEvent>
#include <QButtonGroup>

#include "main.h"
namespace Ui {
class CWinType;
}

class CWinType : public QWidget
{
    Q_OBJECT

public:
    explicit CWinType(QWidget *parent = 0);
    ~CWinType();

private:
    Ui::CWinType *ui;

signals:
    void TransformCmd(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void DatSave(void);
    void WireColorSelect(int row,int column);
    void WireColorShow(void);
    void ProjAvailableShow(int,int);
    void ProjToTestSelect(int row,int column);
    void TypeInsert(void);
    void TypeDelete(void);
    void TypeRead(void);
    void TypeCheck(void);
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
private:
    QButtonGroup *btnGroup;
    QSettings *set;
    QSettings *global;
    QStringList prev;
    QStringList ItemName;
    QStringList FileName;
};

#endif // CWINTYPE_H
