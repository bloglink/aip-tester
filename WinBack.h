#ifndef WINBACK_H
#define WINBACK_H

#include <QWidget>
#include <QSettings>
#include "define.h"

namespace Ui {
class WinBack;
}

class WinBack : public QWidget
{
    Q_OBJECT

public:
    explicit WinBack(QWidget *parent = 0);
    ~WinBack();

private:
    Ui::WinBack *ui;
signals:
    void SendCommand(quint16 addr,quint16 cmd,QByteArray data);
private slots:
    void WinInit(void);
    void BtnInit(void);
    void BtnJudge(int id);
    void DatInit(void);
    void DatSave(void);
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
};

#endif // WINBACK_H
