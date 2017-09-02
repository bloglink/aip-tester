#ifndef LOGIN_H
#define LOGIN_H

#include <QTimer>
#include <QLabel>
#include <QDialog>
#include <QLayout>
#include <QComboBox>
#include <QListView>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>
#include <QSettings>
#include <QJsonObject>

#define SAVE_MAX 5

class Login : public QDialog
{
    Q_OBJECT
public:
    explicit Login(QDialog *parent = 0);
    ~Login();
signals:
    void sendJson(QJsonObject obj);
private slots:
    void initUI();
    void initData();
    void saveData();
    void login();
    void loginTimeOut();
    void recvNetMsg(QString msg);
private:
    QComboBox *svr;
    QComboBox *prt;
    QComboBox *usr;
    QLineEdit *pwd;
    QSettings *ini;
};

#endif // LOGIN_H
