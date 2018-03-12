#include "authorizedialog.h"
#include "ui_authorizedialog.h"

AuthorizeDialog::AuthorizeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AuthorizeDialog)
{
	ui->setupUi(this);
}

AuthorizeDialog::~AuthorizeDialog()
{
	delete ui;
}
