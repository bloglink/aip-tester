#ifndef INPUT_H
#define INPUT_H

#include <QTime>
#include <QWidget>
#include <QMutex>
#include <QDebug>
#include <QLineEdit>
#include <QMouseEvent>
#include <QButtonGroup>

#ifdef __arm__
#include <QWSServer>
#endif


namespace Ui {
class Input;
}

class Input : public QWidget
{
    Q_OBJECT

public:
    explicit Input(QWidget *parent = 0);
    ~Input();

private:
    Ui::Input *ui;
public:
    static Input *Instance() {
        static QMutex mutex;
        if (!_instance) {
            QMutexLocker locker(&mutex);
            if (!_instance) {
                _instance = new Input;
            }
        }
        return _instance;
    }
private slots:
    void initShow();
    void initButton();
    void judgeButton(int);
    void focusChanged(QWidget *oldWidget, QWidget *nowWidget);
    void showEvent(QShowEvent *);
private:
    static Input *_instance;
    bool upper;
    QWidget *currentWidget;
};

#endif // INPUT_H
