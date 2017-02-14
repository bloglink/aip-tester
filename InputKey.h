#ifndef INPUTKEY_H
#define INPUTKEY_H

#include <QWidget>
#include <QButtonGroup>
#include <QDebug>

namespace Ui {
class InputKey;
}

class InputKey : public QWidget
{
    Q_OBJECT

public:
    explicit InputKey(QWidget *parent = 0);
    ~InputKey();

private:
    Ui::InputKey *ui;

private:

    void SetKeyBoard();

    QButtonGroup *btnGroup;
    QPoint  mouse_pos;
    QPoint  widget_pos;
    bool    should_move;
public slots:
    void ButtonJudge(int);
    void updatePos();
    void Caps(bool);
signals:
    void SendString(QString str);
    void SendEnter();
    void SendBackspace();
    void SendEscape();
    void SendCapsLock();
    void SendCaps();
};

#endif // INPUTKEY_H
