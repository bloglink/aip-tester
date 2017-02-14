#ifndef CONFIGPOWER_H
#define CONFIGPOWER_H

#include <QWidget>

namespace Ui {
class ConfigPower;
}

class ConfigPower : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigPower(QWidget *parent = 0);
    ~ConfigPower();

private:
    Ui::ConfigPower *ui;
};

#endif // CONFIGPOWER_H
