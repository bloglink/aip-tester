#ifndef IPMETHOD_H
#define IPMETHOD_H
#ifdef __arm__
#include <QWSInputMethod>
#include "InputKey.h"

class IPMethod : public QWSInputMethod
{
    Q_OBJECT

public:
    IPMethod();
    ~IPMethod();

public:
    void updateHandler(int);

private:
    InputKey* wKeyb;

    QString content;
    bool isCapsLock;

signals:
    void SendCaps(bool);
private slots:
    void SendString(QString);
    void Preedit(QString);
    void Confirm();
    void Backspace();
    void Escape();
    void CapsLock();

    void Upspace();
    void Downspace();
    void Tabspace();
    void BackTabspace();
};
#endif
#endif // IPMETHOD_H
