#ifndef CWINBACK_H
#define CWINBACK_H

#include <QWidget>
#include <QSettings>
#include "main.h"
namespace Ui {
class CWinBack;
}

class CWinBack : public QWidget
{
    Q_OBJECT

public:
    explicit CWinBack(QWidget *parent = 0);
    ~CWinBack();

private:
    Ui::CWinBack *ui;

signals:
    void TransformCmd(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void DatSave(void);
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
private:
    QSettings *global;
};

#endif // CWINBACK_H
