#include "shipmentdialog.h"
#include "ui_shipmentdialog.h"

ShipmentDialog::ShipmentDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ShipmentDialog)
{
	ui->setupUi(this);
}

ShipmentDialog::~ShipmentDialog()
{
	delete ui;
}
