/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       资源管理界面
*******************************************************************************/
#include "source.h"

Source::Source(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Source::initUI()
{
    initView();
    initButton();
    initLayout();
}

void Source::initView()
{
    view = new QTableView(this);
    view->setItemDelegate(new LQReadOnlyItem(this));
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Source::initButton()
{
    btnLayout = new QHBoxLayout;
    btnLayout->addStretch();

    QPushButton *append = new QPushButton(tr("添加资源"), this);
    btnLayout->addWidget(append);
    append->setFixedSize(97, 44);
    connect(append, SIGNAL(clicked(bool)), this, SLOT(appendSqlite()));

    QPushButton *remove = new QPushButton(tr("删除资源"), this);
    btnLayout->addWidget(remove);
    remove->setFixedSize(97, 44);
    connect(remove, SIGNAL(clicked(bool)), this, SLOT(deleteSqlite()));

    QPushButton *update = new QPushButton(tr("修改资源"), this);
    btnLayout->addWidget(update);
    update->setFixedSize(97, 44);
    connect(update, SIGNAL(clicked(bool)), this, SLOT(changeSqlite()));
}

void Source::initLayout()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    layout->addLayout(btnLayout);
    this->setLayout(layout);
}

void Source::createSqlite()
{
    QSqlQuery query(QSqlDatabase::database(sqlName));
    QString cmd;
    cmd = "create table if not exists source (";
    cmd += "id integer primary key, name text, explanation text)";
    if (!query.exec(cmd)) {
        qDebug() << "create table source Error";
    }
}

void Source::updateSqlite()
{
    mView = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mView->setTable("source");
    mView->select();
    if (mView->rowCount() == 0) {
        insertSqlite();
        mView->select();
    }
    view->setModel(mView);
    view->hideColumn(0);
}

void Source::insertSqlite()
{
    QSqlQuery query(QSqlDatabase::database(sqlName));
    query.prepare("insert into source values(?,?,?)");
    query.bindValue(0, snow.getId());
    query.bindValue(1, "winSyst");
    query.bindValue(2, tr("系统设置"));
    query.exec();
}

void Source::appendSqlite()
{
    QString name = getText(tr("资源"), tr("请输入资源名:"), "name");
    if (isNameExists(name)) {
        QMessageBox::warning(this, "资源", tr("资源已存在"), QMessageBox::Ok);
    } else {
        QSqlQuery query(QSqlDatabase::database(sqlName));
        query.prepare("insert into source values(?,?,?)");
        query.bindValue(0, snow.getId());
        query.bindValue(1, name);
        query.bindValue(2, getText(tr("资源"), tr("请输入说明:"), "pass"));
        query.exec();
        mView->select();
    }
}

void Source::deleteSqlite()
{
    int row = view->currentIndex().row();
    if (row >= 0) {
        double id = mView->index(row, 0).data().toDouble();
        QSqlQuery query(QSqlDatabase::database(sqlName));
        query.prepare("delete from source where id = :id");
        query.bindValue(":id", id);
        query.exec();

        query.prepare("delete from action where source = :id");
        query.bindValue(":id", id);
        query.exec();
    } else {
        QMessageBox::warning(this, tr("警告"), tr("请选择一项数据"), QMessageBox::Ok);
    }
    mView->select();
}

void Source::changeSqlite()
{
    int row = view->currentIndex().row();
    int column = view->currentIndex().column();
    if (row >= 0) {
        double id = mView->index(row, 0).data().toDouble();
        QString name = mView->index(row, 1).data().toString();
        QString pass = mView->index(row, 2).data().toString();
        switch (column) {
        case 0:
            break;
        case 1:
            name = getText(tr("资源"), tr("请输入资源名:"), name);
            break;
        case 2:
            pass = getText(tr("资源"), tr("请输入说明:"), pass);
            break;
        default:
            break;
        }
        if ((column == 1) && isNameExists(name)) {
            QMessageBox::warning(this, "资源", tr("资源已存在"), QMessageBox::Ok);
        } else {
            QSqlQuery query(QSqlDatabase::database(sqlName));
            query.prepare("replace into source values(?,?,?)");
            query.bindValue(0, id);
            query.bindValue(1, name);
            query.bindValue(2, pass);
            query.exec();
        }
    } else {
        QMessageBox::warning(this, tr("警告"), tr("请选择一项数据"), QMessageBox::Ok);
    }
    mView->select();
}

bool Source::isNameExists(QString name)
{
    QSqlQuery query(QSqlDatabase::database(sqlName));
    query.prepare("select id from source where name = :name");
    query.bindValue(":name", name);
    query.exec();
    if (query.next()) {
        return true;
    } else {
        return false;
    }
}

QString Source::getText(QString title, QString text, QString s)
{
    bool ok;
    QString input = QInputDialog::getText(this, title, text, QLineEdit::Normal, s, &ok);
    if (ok)
        return input;
    else
        return s;
}

void Source::recvAppShow(QString win)
{
    if (win == this->objectName()) {
        mView->select();
    }
}

void Source::recvSqlOpen(QString sql)
{
    sqlName = sql;
    createSqlite();
    updateSqlite();
}

