#include "addadmindialog.h"
#include "ui_addadmindialog.h"
#include "database.h"
#include <QMessageBox>

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

void AddAdminDialog::on_btnOk_clicked()
{
	if(ui->editAdminID->text().isEmpty()){
		QMessageBox::information(this, "提示", "请输入管理员账号");
		return;
	}
	if(ui->editAdminName->text().isEmpty()){
		QMessageBox::information(this, "提示", "请输入管理员名称");
		return;
	}
	if(ui->editAdminPassword->text().isEmpty()){
		QMessageBox::information(this, "提示", "请输入管理员密码");
		return;
	}

	bool ok;
	Admin admin;
	admin.admin_id = ui->editAdminID->text().toLongLong(&ok);
	if(!ok){
		QMessageBox::information(this, "提示", "管理员账号只可以是数字");
		return;
	}
	admin.admin_name = ui->editAdminName->text();
	admin.admin_password = ui->editAdminPassword->text();

	Database *db = Database::getInstance();
	if(db->addAdmin(admin)){
		accept();
	}else{
		QMessageBox::critical(this, "提醒", "该账号已经存在");
	}
}

void AddAdminDialog::on_btnCancel_clicked()
{
	reject();
}
