#include "addadmindialog.h"
#include "ui_addadmindialog.h"

AddAdminDialog::AddAdminDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddAdminDialog)
{
	ui->setupUi(this);
}

AddAdminDialog::~AddAdminDialog()
{
	delete ui;
}
