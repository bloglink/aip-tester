#include "iobrd.h"

Iobrd::Iobrd(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Iobrd::initUI()
{
    QHBoxLayout *com_layout = new QHBoxLayout;
    QGridLayout *btn_layout = new QGridLayout;

    QStringList btnNames;
    btnNames << "右工位" << "右抽气" << "右回气" << "左抽气" << "左回气"  << "右下" << "右上" << "NULL"
             << "左下" << "左上"
             << "NULL" << "真空泵" << "NULL" << "NULL"
             << "NULL" << "NULL" << "NULL" << "NULL" << "NULL";

    for (int i=0; i < 16; i++) {
        btns.append(new QToolButton(this));
        btns.at(i)->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btns.at(i)->setCheckable(true);
        btns.at(i)->setText(btnNames.at(i));
        btns.at(i)->setStyleSheet("border-image:url(./image/circle_128_1.png)");
        btn_layout->addWidget(btns.at(i), i/8, i%8);
        connect(btns.at(i), SIGNAL(clicked(bool)), this, SLOT(readButton()));
    }

    QGridLayout *led_layout = new QGridLayout;
    QStringList names;
    names << "右启动" << "右停止" << "右光幕" << "左启动"
          << "左停止" << "左光幕" << "右气压吸到位" << "右气压放到位"
          << "右气缸下位" << "左压力到位" << "左气压放到位" << "左气缸下位"
          << "相序错误" << "热继电器信号" << "NULL" << "NULL" << "NULL";
    for (int i=0; i < 16; i++) {
        leds.append(new QLabel(this));
        leds.at(i)->setText(names.at(i));
        leds.at(i)->setAlignment(Qt::AlignCenter);
        leds.at(i)->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        leds.at(i)->setStyleSheet("border-image:url(./image/circle_128_1.png)");
        led_layout->addWidget(leds.at(i), i/8, i%8);
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(com_layout);
    layout->addLayout(btn_layout);
    layout->addLayout(led_layout);
    this->setLayout(layout);
}

void Iobrd::readButton()
{
    QToolButton *btn = qobject_cast<QToolButton*>(sender());
    if (btn->isChecked()) {
        btn->setStyleSheet("border-image:url(./image/circle_128_2.png)");
    } else {
        btn->setStyleSheet("border-image:url(./image/circle_128_1.png)");
    }
    quint16 hex = 0x00;
    for (int i=0; i < btns.size(); i++) {
        if (btns.at(i)->isChecked())
            hex += quint16(0x0001 << i);
    }
    QString cmd = QString("6036 %1").arg(hex);

    QJsonObject msg;
    msg.insert("txCommand", IOBRD);
    msg.insert("txMessage", cmd);
    emit sendAppMsg(msg);
}

void Iobrd::recvNetCommand(QString msg)
{
    int a = msg.indexOf(" ");
    int cmd = msg.mid(0, a).toInt();
    QString dat = msg.mid(a+1, msg.size());
    quint16 hex = dat.toInt();
    qDebug() << hex;
    switch (cmd) {
    case 6037:
        for (int i=0; i < 16; i++) {
            if (hex & (0x0001 << i))
                leds.at(i)->setStyleSheet("border-image:url(./image/circle_128_2.png)");
            else
                leds.at(i)->setStyleSheet("border-image:url(./image/circle_128_1.png)");
        }
        break;
    case 6059:
        for (int i=0; i < 16; i++) {
            if (hex & (0x0001 << i)) {
                btns.at(i)->setStyleSheet("border-image:url(./image/circle_128_2.png)");
                btns.at(i)->setChecked(true);
            } else {
                btns.at(i)->setStyleSheet("border-image:url(./image/circle_128_1.png)");
                btns.at(i)->setChecked(false);
            }
        }
        break;
    default:
        break;
    }
}

void Iobrd::recvAppShow(QString win)
{
    if (win == this->objectName()) {
        QJsonObject msg;
        msg.insert("txCommand", IOBRD);
        msg.insert("txMessage", "6058 1");
        emit sendAppMsg(msg);
    }
}

void Iobrd::hideEvent(QHideEvent *e)
{
    QJsonObject msg;
    msg.insert("txCommand", IOBRD);
    msg.insert("txMessage", "6058 0");
    emit sendAppMsg(msg);
    e->accept();
}

