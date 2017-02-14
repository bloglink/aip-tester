#ifndef CWINDBUG_H
#define CWINDBUG_H

#include <QWidget>

namespace Ui {
class CWinDbug;
}

class CWinDbug : public QWidget
{
    Q_OBJECT

public:
    explicit CWinDbug(QWidget *parent = 0);
    ~CWinDbug();

private:
    Ui::CWinDbug *ui;
};

#endif // CWINDBUG_H
