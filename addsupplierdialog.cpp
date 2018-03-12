#include "addsupplierdialog.h"
#include "ui_addsupplierdialog.h"

AddSupplierDialog::AddSupplierDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddSupplierDialog)
{
	ui->setupUi(this);
}

AddSupplierDialog::~AddSupplierDialog()
{
	delete ui;
}
