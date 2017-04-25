#ifndef MESSAGE_BOX
#define MESSAGE_BOX

#include <QMessageBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include "custom_window.h"
#endif

class QLabel;

class MessageBox :  public CustomWindow
{
    Q_OBJECT

public:
    explicit MessageBox(QWidget  *parent = 0, const QString &title = tr("Tip"), const QString &text = "",
                        QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::Ok);
    ~MessageBox();
    QAbstractButton *clickedButton() const;
    QMessageBox::StandardButton standardButton(QAbstractButton *button) const;
    // 设置默认按钮
    void setDefaultButton(QPushButton *button);
    void setDefaultButton(QMessageBox::StandardButton button);
    // 设置窗体标题
    void setTitle(const QString &title);
    // 设置提示信息
    void setText(const QString &text);
    // 设置窗体图标
    void setIcon(const QString &icon);
    // 添加控件-替换提示信息所在的QLabel
    void addWidget(QWidget  *pWidget);

protected:
    // 多语言翻译
    void changeEvent(QEvent *event);

public slots:
    void onButtonClicked(QAbstractButton *button);
    void readcnd(QByteArray);

private:
    void translateUI();
    int execReturnCode(QAbstractButton *button);

private:
    QLabel *m_pIconLabel;
    QLabel *m_pLabel;
    QGridLayout *m_pGridLayout;
    QVBoxLayout *m_pLayout;
    QDialogButtonBox *m_pButtonBox;
    QAbstractButton *m_pClickedButton;
    QAbstractButton *m_pDefaultButton;

};
