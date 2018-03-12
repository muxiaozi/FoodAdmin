#include "addfooddialog.h"
#include "ui_addfooddialog.h"
#include "database.h"
#include <QMessageBox>

AddFoodDialog::AddFoodDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddFoodDialog)
{
	Qt::WindowFlags flags=Qt::Dialog;
	flags |=Qt::WindowCloseButtonHint;
	setWindowFlags(flags);
	ui->setupUi(this);
}

AddFoodDialog::~AddFoodDialog()
{
	delete ui;
}

void AddFoodDialog::on_btnOk_clicked()
{
	if(ui->editFoodName->text().isEmpty()){
		QMessageBox::warning(this, "提示", "请输入粮食的名称");
		return;
	}
	if(ui->editFoodUnit->text().isEmpty()){
		QMessageBox::warning(this, "提示", "请输入粮食的计量单位");
		return;
	}
	if(ui->sbUserPerQuantity->value() == 0){
		QMessageBox::warning(this, "提示", "每人每年数量需大于0");
		return;
	}

	Food food;
	food.food_name = ui->editFoodName->text();
	food.food_unit = ui->editFoodUnit->text();
	food.food_price = ui->dsbFoodPrice->value();
	food.user_per_quantity = ui->sbUserPerQuantity->value();

	Database *db = Database::getInstance();
	if(db->addFood(food)){
		accept();
	}else{
		QMessageBox::critical(this, "添加失败", "请检查输入合法性");
	}
}

void AddFoodDialog::on_btnCancel_clicked()
{
	reject();
}
