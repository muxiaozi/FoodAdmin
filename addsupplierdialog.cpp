#include "addsupplierdialog.h"
#include "ui_addsupplierdialog.h"
#include "database.h"
#include <QMessageBox>

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

void AddSupplierDialog::on_btnOk_clicked()
{
	if(ui->editSupplierName->text().isEmpty()){
		QMessageBox::warning(this, "提醒", "请填写供货商名称");
		return;
	}
	if(ui->editSupplierTelephone->text().isEmpty()){
		QMessageBox::warning(this, "提醒", "请填写供货商电话号码");
		return;
	}

	bool ok;
	Supplier supplier;
	supplier.supplier_name = ui->editSupplierName->text();
	supplier.supplier_telphone = ui->editSupplierTelephone->text().toLongLong(&ok);
	if(!ok){
		QMessageBox::warning(this, "提醒", "电话号码只可以输入数字");
		return;
	}
	supplier.supplier_info = ui->editSupplierInfo->toPlainText();

	Database *db = Database::getInstance();
	if(db->addSupplier(supplier)){
		accept();
	}else{
		QMessageBox::critical(this, "添加失败", "请检查输入的合法性");
	}
}

void AddSupplierDialog::on_btnCancel_clicked()
{
	reject();
}
