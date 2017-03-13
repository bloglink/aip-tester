#include "WinBack.h"
#include "ui_WinBack.h"

WinBack::WinBack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinBack)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
}

WinBack::~WinBack()
{
    delete ui;
}

void WinBack::WinInit()
{
}

void WinBack::BtnInit()
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

void WinBack::DatInit()
{
    QSettings *g_ini = new QSettings(INI_PATH,QSettings::IniFormat);
    g_ini->setIniCodec("GB18030");
    g_ini->beginGroup("GLOBAL");
    QStringList temp = g_ini->value("ItemEnable","1 2 3 4 6").toString().split(" ");
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

    temp = g_ini->value("OutEnable","0 1").toString().split(" ");
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

    QString t = g_ini->value("Number","168912000X").toString();
    ui->EditNumber->setText(t);
    ui->OppositeDir->setChecked(g_ini->value("OppositeDir",false).toBool());
    ui->HideVoltage->setChecked(g_ini->value("HideVoltage",false).toBool());
}

void WinBack::DatSave()
{
    QSettings *g_ini = new QSettings(INI_PATH,QSettings::IniFormat);
    g_ini->setIniCodec("GB18030");
    g_ini->beginGroup("GLOBAL");
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
    g_ini->setValue("ItemEnable",temp.join(" "));

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
    g_ini->setValue("OutEnable",temp.join(" "));

    g_ini->setValue("Number",ui->EditNumber->text());
    g_ini->setValue("OppositeDir",ui->OppositeDir->isChecked());
    g_ini->setValue("HideVoltage",ui->HideVoltage->isChecked());
}

void WinBack::showEvent(QShowEvent *)
{
    DatInit();
}

void WinBack::hideEvent(QHideEvent *)
{
    DatSave();
}
