#include "warnning.h"

Warnning::Warnning(QWidget  *parent, const QString &title, const QString &text,
                   QMessageBox::StandardButtons buttons,
                   QMessageBox::StandardButton defaultButton)
    : QDialog(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setStyleSheet("QDialog{border:2px solid red;}");

    setWindowIcon(QIcon(":/source/link.png"));
    setWindowTitle(title);
    setMinimumSize(300, 130);

    ButtonBox = new QDialogButtonBox(this);
    ButtonBox->setStandardButtons(QDialogButtonBox::StandardButtons(int(buttons)) |
                                  QDialogButtonBox::StandardButtons(int(defaultButton)));

    QLabel *IconLabel = new QLabel(this);
    QLabel *TextLabel = new QLabel(this);

    QPixmap pixmap(":/source/logo.png");
    IconLabel->setPixmap(pixmap);
    IconLabel->setFixedSize(113, 57);
    IconLabel->setScaledContents(true);

    TextLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    TextLabel->setStyleSheet("color:yellow;font:15pt");
    TextLabel->setWordWrap(true);
    TextLabel->setText(text);

    QGridLayout *GridLayout = new QGridLayout(this);
    GridLayout->addWidget(IconLabel, 0, 0, 2, 1, Qt::AlignTop);
    GridLayout->addWidget(TextLabel, 0, 1, 2, 1);
    GridLayout->addWidget(ButtonBox, GridLayout->rowCount(), 0, 1, GridLayout->columnCount());

    GridLayout->setSizeConstraint(QLayout::SetNoConstraint);
    GridLayout->setHorizontalSpacing(10);
    GridLayout->setVerticalSpacing(10);
    GridLayout->setContentsMargins(10, 10, 10, 10);
    this->setLayout(GridLayout);

    TranslateButtonText();

    connect(ButtonBox, SIGNAL(clicked(QAbstractButton*)), this,
            SLOT(onButtonClicked(QAbstractButton*)));
}
void Warnning::changeEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::LanguageChange:
        TranslateButtonText();
        break;
    default:
        QDialog::changeEvent(event);
        break;
    }
}

void Warnning::TranslateButtonText()
{
    QPushButton *RetryButton = ButtonBox->button(QDialogButtonBox::Retry);
    if (RetryButton != NULL)
        RetryButton->setText(tr("重测"));

    QPushButton *CancelButton = ButtonBox->button(QDialogButtonBox::Yes);
    if (CancelButton != NULL)
        CancelButton->setText(tr("确定"));
    QPushButton *OkButton = ButtonBox->button(QDialogButtonBox::Ok);
    if (OkButton != NULL)
        OkButton->setText(tr("继续"));
}

void Warnning::ReadVariant(QVariantHash s)
{
    if (s.value("TxAddress") != "WinHome")
        return;
    if (s.value("TxCommand") != "BoxButton")
        return;
    if (s.value("TxMessage") == "Retry")
        onButtonClicked(ButtonBox->button(QDialogButtonBox::Retry));
    else
        onButtonClicked(ButtonBox->button(QDialogButtonBox::Ok));
}

void Warnning::onButtonClicked(QAbstractButton *button)
{
    if (!ButtonBox->buttons().contains(button))
        return;
    done(ButtonBox->standardButton(button));
}

