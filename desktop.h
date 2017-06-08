#ifndef DESKTOP_H
#define DESKTOP_H

#include <QFile>
#include <QWidget>

#include "sockeckcan.h"

namespace Ui {
class Desktop;
}

class Desktop : public QWidget
{
    Q_OBJECT

public:
    explicit Desktop(QWidget *parent = 0);
    ~Desktop();
private slots:
    void InitWin(void);
    void InitCan(void);
    void ReadCan(void);
private:
    Ui::Desktop *ui;
    QString TestStatus;
    SockeckCan can;
    QTimer *timer;
    quint32 count;
};

#endif // DESKTOP_H
