#include "authorizedialog.h"
#include "ui_authorizedialog.h"
#include "database.h"
#include "mainwindow.h"
#include <QDebug>

AuthorizeDialog::AuthorizeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AuthorizeDialog)
{
	ui->setupUi(this);
	db = Database::getInstance();
}

AuthorizeDialog::~AuthorizeDialog()
{
	delete ui;
}

void AuthorizeDialog::on_editPassword_textChanged(const QString &arg1)
{
	if(arg1 == db->getAuthCode()){
		accept();
	}
}

void AuthorizeDialog::on_btnCancel_clicked()
{
	reject();
}
