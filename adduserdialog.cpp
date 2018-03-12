#include "adduserdialog.h"
#include "ui_adduserdialog.h"

AddUserDialog::AddUserDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddUserDialog)
{
	ui->setupUi(this);
}

AddUserDialog::~AddUserDialog()
{
	delete ui;
}
