#include "WinBack.h"
#include "ui_WinBack.h"

WinBack::WinBack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinBack)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
}

WinBack::~WinBack()
{
    delete ui;
}

void WinBack::InitWindows()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabItems->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabItems->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabOutput->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabItems->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabOutput->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    for (int i=0; i<ui->TabItems->columnCount(); i++) {
        for (int row=0; row<ui->TabItems->rowCount(); row++) {
            ui->TabItems->item(row,i)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ui->TabItems->item(row,i)->setTextAlignment(Qt::AlignCenter);
        }
    }
    for (int i=0; i<ui->TabOutput->columnCount(); i++) {
        for (int row=0; row<ui->TabOutput->rowCount(); row++) {
            ui->TabOutput->item(row,i)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ui->TabOutput->item(row,i)->setTextAlignment(Qt::AlignCenter);
        }
    }
    connect(ui->TabItems,SIGNAL(cellClicked(int,int)),this,SLOT(ClickItem(int,int)));
    connect(ui->TabOutput,SIGNAL(cellClicked(int,int)),this,SLOT(ClickOutput(int,int)));
}

void WinBack::InitButtons()
{
    QButtonGroup *BtnGroup = new QButtonGroup;
    BtnGroup->addButton(ui->BtnExit,Qt::Key_0);
    connect(BtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}

void WinBack::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        emit SendCommand(ADDR,CMD_JUMP,NULL);
        break;
    default:
        break;
    }
}

void WinBack::InitSettings()
{
    QSettings *g_ini = new QSettings(INI_PATH,QSettings::IniFormat);
    g_ini->setIniCodec("GB18030");
    g_ini->beginGroup("GLOBAL");
    QStringList temp = g_ini->value("ItemEnable","1 2 3 4 6").toString().split(" ");
    for (int i=0; i<temp.size(); i++) {
        int row = temp.at(i).toInt();
        if (row > 0)
        ui->TabItems->item(row-1,1)->setText("Y");
    }

    temp = g_ini->value("OutEnable","0 1").toString().split(" ");
    for (int i=0; i<temp.size(); i++) {
        ui->TabOutput->item(temp.at(i).toInt(),1)->setText("Y");
    }

    QString t = g_ini->value("Number","168912000X").toString();
    ui->EditNumber->setText(t);
    ui->OppositeDir->setChecked(g_ini->value("OppositeDir",false).toBool());
    ui->HideVoltage->setChecked(g_ini->value("HideVoltage",false).toBool());
}

void WinBack::SaveSettings()
{
    QSettings *ini = new QSettings(INI_PATH,QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("GLOBAL");
    QStringList temp;
    temp.append("0");
    for (int i=0; i<ui->TabItems->rowCount(); i++) {
        if (ui->TabItems->item(i,1)->text() == "Y")
            temp.append(QString::number(i+1));
    }
    ini->setValue("ItemEnable",temp.join(" "));

    temp.clear();
    for (int i=0; i<ui->TabOutput->rowCount(); i++) {
        if (ui->TabOutput->item(i,1)->text() == "Y")
            temp.append(QString::number(i));
    }
    ini->setValue("OutEnable",temp.join(" "));

    ini->setValue("Number",ui->EditNumber->text());
    ini->setValue("OppositeDir",ui->OppositeDir->isChecked());
    ini->setValue("HideVoltage",ui->HideVoltage->isChecked());
}

void WinBack::ClickItem(int r, int c)
{
    if (c != 1)
        return;
    if (ui->TabItems->item(r,c)->text() == "N")
        ui->TabItems->item(r,c)->setText("Y");
    else
        ui->TabItems->item(r,c)->setText("N");
}

void WinBack::ClickOutput(int r, int c)
{
    if (c != 1)
        return;
    if (ui->TabOutput->item(r,c)->text() == "N")
        ui->TabOutput->item(r,c)->setText("Y");
    else
        ui->TabOutput->item(r,c)->setText("N");
}

void WinBack::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR)
        return;
    qDebug()<<addr<<cmd<<msg;
}

void WinBack::SendCanCmdVersion()
{

}

void WinBack::showEvent(QShowEvent *)
{
    InitSettings();
}

void WinBack::hideEvent(QHideEvent *)
{
    SaveSettings();
}
