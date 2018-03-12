#include "userdetaildialog.h"
#include "ui_userdetaildialog.h"

UserDetailDialog::UserDetailDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UserDetailDialog)
{
	ui->setupUi(this);
}

UserDetailDialog::~UserDetailDialog()
{
	delete ui;
}
