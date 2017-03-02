#include "WinBack.h"
#include "ui_WinBack.h"

WinBack::WinBack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinBack)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
}

WinBack::~WinBack()
{
    delete ui;
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       界面初始化
******************************************************************************/
void WinBack::WinInit()
{

}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       按钮初始化
******************************************************************************/
void WinBack::BtnInit()
{
    QButtonGroup *BtnGroup = new QButtonGroup;
    BtnGroup->addButton(ui->BtnExit,Qt::Key_0);
    connect(BtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(BtnJudge(int)));
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       按钮功能
******************************************************************************/
void WinBack::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        emit SendMessage(ADDR,CMD_JUMP,NULL);
        break;
    default:
        break;
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       数据初始化
******************************************************************************/
void WinBack::DatInit()
{
    QSettings *g_settings = new QSettings(INI_PATH,QSettings::IniFormat);
    g_settings->setIniCodec("GB18030");
    g_settings->beginGroup("GLOBAL");
    QStringList temp = g_settings->value("ItemEnable","1 2 3 4 6").toString().split(" ");
    if (temp.contains("1"))
        ui->BoxEnableDcr->setChecked(true);
    if (temp.contains("2"))
        ui->BoxEnableMag->setChecked(true);
    if (temp.contains("3"))
        ui->BoxEnableIr->setChecked(true);
    if (temp.contains("4"))
        ui->BoxEnableAcw->setChecked(true);
    if (temp.contains("5"))
        ui->BoxEnableDcw->setChecked(true);
    if (temp.contains("6"))
        ui->BoxEnableImp->setChecked(true);
    if (temp.contains("7"))
        ui->BoxEnableInd->setChecked(true);
    if (temp.contains("8"))
        ui->BoxEnablePwr->setChecked(true);
    if (temp.contains("9"))
        ui->BoxEnableLvs->setChecked(true);
    if (temp.contains("10"))
        ui->BoxEnableLck->setChecked(true);

    temp = g_settings->value("OutEnable","0 1").toString().split(" ");
    if (temp.contains("0"))
        ui->BoxEnableOut13->setChecked(true);
    if (temp.contains("1"))
        ui->BoxEnableOut14->setChecked(true);
    if (temp.contains("2"))
        ui->BoxEnableOut15->setChecked(true);
    if (temp.contains("3"))
        ui->BoxEnableOut16->setChecked(true);
    if (temp.contains("4"))
        ui->BoxEnableOut17->setChecked(true);

    QString t = g_settings->value("Number","V20170106").toString();
    ui->EditNumber->setText(t);

}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.06
 * brief:       数据保存
******************************************************************************/
void WinBack::DatSave()
{
    QSettings *g_settings = new QSettings(INI_PATH,QSettings::IniFormat);
    g_settings->setIniCodec("GB18030");
    g_settings->beginGroup("GLOBAL");
    QStringList temp;
    temp.append("0");
    if (ui->BoxEnableDcr->isChecked())
        temp.append("1");
    if (ui->BoxEnableMag->isChecked())
        temp.append("2");
    if (ui->BoxEnableIr->isChecked())
        temp.append("3");
    if (ui->BoxEnableAcw->isChecked())
        temp.append("4");
    if (ui->BoxEnableDcw->isChecked())
        temp.append("5");
    if (ui->BoxEnableImp->isChecked())
        temp.append("6");
    if (ui->BoxEnableInd->isChecked())
        temp.append("7");
    if (ui->BoxEnablePwr->isChecked())
        temp.append("8");
    if (ui->BoxEnableLvs->isChecked())
        temp.append("9");
    if (ui->BoxEnableLck->isChecked())
        temp.append("10");
    g_settings->setValue("ItemEnable",temp.join(" "));

    temp.clear();
    if (ui->BoxEnableOut13->isChecked())
        temp.append("0");
    if (ui->BoxEnableOut14->isChecked())
        temp.append("1");
    if (ui->BoxEnableOut15->isChecked())
        temp.append("2");
    if (ui->BoxEnableOut16->isChecked())
        temp.append("3");
    if (ui->BoxEnableOut17->isChecked())
        temp.append("4");
    g_settings->setValue("OutEnable",temp.join(" "));

    g_settings->setValue("Number",ui->EditNumber->text());
}

void WinBack::showEvent(QShowEvent *)
{
    DatInit();
}

void WinBack::hideEvent(QHideEvent *)
{
    DatSave();
}
