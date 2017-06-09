#ifndef WARNNING_H
#define WARNNING_H

#include <QIcon>
#include <QLabel>
#include <QEvent>
#include <QVariant>
#include <QGridLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>

class Warnning : public QDialog
{
    Q_OBJECT
public:
    explicit Warnning(QWidget  *parent = 0, const QString &title = tr("Tip"),
                      const QString &text = "",
                      QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                      QMessageBox::StandardButton defaultButton = QMessageBox::Ok);

protected:
    void changeEvent(QEvent *event);
private slots:
    void onButtonClicked(QAbstractButton *button);
    void TranslateButtonText(void);
    void ReadVariant(QVariantHash s);
private:
    QDialogButtonBox *ButtonBox;
};

#endif // WARNNING_H
