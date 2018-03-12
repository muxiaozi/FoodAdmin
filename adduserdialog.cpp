#include "adduserdialog.h"
#include "ui_adduserdialog.h"
#include "database.h"
#include <QMessageBox>

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

void AddUserDialog::on_btnOk_clicked()
{
	if(ui->editUserID->text().isEmpty()){
		QMessageBox::warning(this, "提醒", "请填写客户卡号");
		return;
	}
	if(ui->editUserName->text().isEmpty()){
		QMessageBox::warning(this, "提醒", "请填写户主姓名");
		return;
	}

	bool ok;
	User user;
	user.user_id = ui->editUserID->text().toLongLong(&ok);
	if(!ok){
		QMessageBox::warning(this, "提醒", "卡号只可以输入数字");
		return;
	}
	user.user_name = ui->editUserName->text();
	user.user_count = ui->dsbUserCount->value();

	Database *db = Database::getInstance();
	if(db->addUser(user)){
		accept();
	}else{
		QMessageBox::critical(this, "添加失败", "该卡号已经存在");
	}
}

void AddUserDialog::on_btnCancel_clicked()
{
	reject();
}
