#ifndef PAGEKEY_H
#define PAGEKEY_H

#include <QTimer>
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
class PageKey;
}

class PageKey : public QWidget
{
    Q_OBJECT

public:
    explicit PageKey(QWidget *parent = 0);
    ~PageKey();

private:
    Ui::PageKey *ui;

public:
    static PageKey *Instance() {
        static QMutex mutex;
        if (!_instance) {
            QMutexLocker locker(&mutex);
            if (!_instance) {
                _instance = new PageKey;
            }
        }
        return _instance;
    }
private slots:
    void InitWindows();
    void InitButtons();
    void ReadButtons(int);
    void SwitchInput(void);
    void focusChanged(QWidget *oldWidget, QWidget *nowWidget);
    void showEvent(QShowEvent *);
private:
    static PageKey *_instance;
    bool upper;
    QWidget *currentWidget;
    bool num;
};

#endif // PAGEKEY_H
