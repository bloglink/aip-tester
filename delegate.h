#ifndef DELEGATE_H
#define DELEGATE_H

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QApplication>
#include <QListView>
#include <QSpinBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

#include "qcustomplot.h"


//编号列，只读委托
//这个方法我还真想不到，呵呵
class ReadOnlyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ReadOnlyDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget*, const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        return NULL;
    }
};

//ID列，只能输入1－12个数字
//利用QLineEdit委托和正则表达式对输入进行限制
class UserIDDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    UserIDDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        QLineEdit *editor = new QLineEdit(parent);
        QRegExp regExp("[0-9]{0,10}");
        editor->setValidator(new QRegExpValidator(regExp, parent));
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(text);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString text = lineEdit->text();
        model->setData(index, text, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const
    {
        editor->setGeometry(option.rect);
    }
};

//头像列，只是在单元格中央放一张小图而已
class IconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    IconDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &) const
    {
        //show.bmp是在工程目录中的一张图片（其实就是QQ的图标啦，呵呵）
        QPixmap pixmap = QPixmap("show.bmp").scaled(24, 24);
        qApp->style()->drawItemPixmap(painter, option.rect,  Qt::AlignCenter, QPixmap(pixmap));
    }
};

//利用QComboBox委托对输入进行限制,这一列的单元格只能输入Y或N
class EnableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    EnableDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        QComboBox *editor = new QComboBox(parent);
        editor->setItemDelegate(new QStyledItemDelegate());
        editor->addItem("Y");
        editor->addItem("N");
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const
    {
        editor->setGeometry(option.rect);
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->showPopup();
    }
};
//利用QSpinBox委托进行输入限制，只能输入1－8之间的数字
class PortDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    PortDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setButtonSymbols(QAbstractSpinBox::NoButtons);
        editor->setMinimum(1);
        editor->setMaximum(8);
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const
    {
        editor->setGeometry(option.rect);
    }
};
//利用QComboBox委托对输入进行限制
class MetalDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    MetalDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        QComboBox *editor = new QComboBox(parent);
        editor->setItemDelegate(new QStyledItemDelegate());
        editor->addItem(tr("铜"));
        editor->addItem(tr("铝"));
        editor->addItem(tr("铜铝"));
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const
    {
        editor->setGeometry(option.rect);
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->showPopup();
    }
};

//利用QComboBox委托对输入进行限制
class UnitDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    UnitDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        QComboBox *editor = new QComboBox(parent);
        editor->setItemDelegate(new QStyledItemDelegate());
        editor->addItem(tr("mΩ"));
        editor->addItem(tr("Ω"));
        editor->addItem(tr("kΩ"));
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const
    {
        editor->setGeometry(option.rect);
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->showPopup();
    }
};

//利用QDoubleSpinBox委托进行输入限制
class DcrDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    DcrDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor->setButtonSymbols(QAbstractSpinBox::NoButtons);
        editor->setMinimum(0);
        editor->setMaximum(9999.99);
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        spinBox->setValue(value);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        spinBox->interpretText();
        double value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const
    {
        editor->setGeometry(option.rect);
    }
};

//利用QComboBox委托对输入进行限制
class OffsetDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    OffsetDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        QComboBox *editor = new QComboBox(parent);
        editor->setItemDelegate(new QStyledItemDelegate());
        editor->addItem(tr("取消补偿"));
        editor->addItem(tr("温度补偿"));
        editor->addItem(tr("热敏电阻"));
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const
    {
        editor->setGeometry(option.rect);
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->showPopup();
    }
};

//代理类，把所有单元格中的字符居中显示
class StandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    StandardItemModel(QObject *parent=NULL) : QStandardItemModel(parent) { }
    StandardItemModel(int row, int column, QObject *parent=NULL)
        : QStandardItemModel(row, column, parent) { }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        if( Qt::TextAlignmentRole == role )
            return Qt::AlignCenter;
        return QStandardItemModel::data(index, role);
    }
};

//利用QSpinBox委托进行输入限制，只能输入0－99之间的数字
class RateDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    RateDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setButtonSymbols(QAbstractSpinBox::NoButtons);
        editor->setMinimum(0);
        editor->setMaximum(99);
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const
    {
        editor->setGeometry(option.rect);
    }
};
//利用QSpinBox委托进行输入限制，只能输入0－99之间的数字
class VoltDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    VoltDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setButtonSymbols(QAbstractSpinBox::NoButtons);
        editor->setMinimum(0);
        editor->setMaximum(3000);
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const
    {
        editor->setGeometry(option.rect);
    }
};

//利用QDoubleSpinBox委托进行输入限制
class CurrDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    CurrDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor->setButtonSymbols(QAbstractSpinBox::NoButtons);
        editor->setMinimum(0);
        editor->setMaximum(20);
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        spinBox->setValue(value);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        spinBox->interpretText();
        double value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const
    {
        editor->setGeometry(option.rect);
    }
};

//利用QComboBox委托对输入进行限制
class DirDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    DirDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        QComboBox *editor = new QComboBox(parent);
        editor->setItemDelegate(new QStyledItemDelegate());
        editor->addItem(tr("不转"));
        editor->addItem(tr("正转"));
        editor->addItem(tr("反转"));
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const
    {
        editor->setGeometry(option.rect);
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->showPopup();
    }
};

#endif // DELEGATE_H
