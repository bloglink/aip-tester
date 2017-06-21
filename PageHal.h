#ifndef PAGEHAL_H
#define PAGEHAL_H

#include <QWidget>
#include "delegate.h"
#define HAL_MAX 8
namespace Ui {
class PageHal;
}

class PageHal : public QWidget
{
    Q_OBJECT

public:
    explicit PageHal(QWidget *parent = 0);
    ~PageHal();

private slots:
    void InitWin();
private:
    Ui::PageHal *ui;
    StandardItemModel *m;
};

#endif // PAGEHAL_H
