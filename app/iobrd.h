#ifndef IOBRD_H
#define IOBRD_H

#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QComboBox>
#include <QToolButton>
#include <QPushButton>

#include <QJsonObject>
#include <QJsonDocument>

#include <QHideEvent>

#define IOBRD 300

class Iobrd : public QWidget
{
    Q_OBJECT
public:
    explicit Iobrd(QWidget *parent = 0);

signals:
    void sendAppMsg(QJsonObject obj);
public slots:
    void initUI();
    void readButton();
    void recvNetCommand(QString cmd);
    void recvAppShow(QString win);
    void hideEvent(QHideEvent *e);
private:
    QList<QToolButton*> btns;
    QList<QLabel*> leds;
    QComboBox *udps;
    QTimer *timer;
};

#endif // IOBRD_H
