#include "PageHal.h"
#include "ui_PageHal.h"

PageHal::PageHal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageHal)
{
    ui->setupUi(this);
    InitWin();
}

PageHal::~PageHal()
{
    delete ui;
}

void PageHal::InitWin()
{
    this->setFocus();
    m = new StandardItemModel(HAL_MAX, 11);

    QStringList headerList;
    headerList << tr("电阻") << tr("端一") << tr("端二") << tr("线圈材料") << tr("单位")
               << tr("电阻下限") << tr("电阻上限") << tr("标准电阻") << tr("温度补偿")
               << tr("线路补偿1") << tr("线路补偿2");
    m->setHorizontalHeaderLabels(headerList);
    ui->parameters->setModel(m);

    ui->parameters->setItemDelegateForColumn(0, new EnableDelegate(this));
    ui->parameters->setItemDelegateForColumn(1, new PortDelegate);
    ui->parameters->setItemDelegateForColumn(2, new PortDelegate);
    ui->parameters->setItemDelegateForColumn(3, new MetalDelegate);
    ui->parameters->setItemDelegateForColumn(4, new UnitDelegate);
    ui->parameters->setItemDelegateForColumn(5, new DcrDelegate);
    ui->parameters->setItemDelegateForColumn(6, new DcrDelegate);
    ui->parameters->setItemDelegateForColumn(7, new DcrDelegate);
    ui->parameters->setItemDelegateForColumn(8, new OffsetDelegate);
    ui->parameters->setItemDelegateForColumn(9, new DcrDelegate);
    ui->parameters->setItemDelegateForColumn(10, new DcrDelegate);
    ui->parameters->setEditTriggers(QAbstractItemView::CurrentChanged);
}
