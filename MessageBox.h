#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QVariant>
#include <QGridLayout>
#include <QMessageBox>
#include <QDialogButtonBox>

class MessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit MessageBox(QWidget  *parent = 0, const QString &title = tr("Tip"),
                            const QString &text = "",
                            QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                            QMessageBox::StandardButton defaultButton = QMessageBox::Ok);

    ~MessageBox();
protected:
    void changeEvent(QEvent *event);
private slots:
    void onButtonClicked(QAbstractButton *button);
    void TranslateButtonText(void);
    void ReadVariant(QVariant s);
private:
    QDialogButtonBox *ButtonBox;
};

#endif // MESSAGEBOX_H
