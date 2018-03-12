#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bean.h"
#include "database.h"

#include "foodmodel.h"
#include "purchasemodel.h"
#include "shipmentmodel.h"
#include "usermodel.h"
#include "adminmodel.h"
#include "suppliermodel.h"

#include "addfooddialog.h"
#include "purchasedialog.h"
#include "addadmindialog.h"
#include "adduserdialog.h"
#include "addsupplierdialog.h"
#include "shipmentdialog.h"
#include "userdetaildialog.h"
#include "authorizedialog.h"

#include <QMessageBox>
#include <QDateTime>
#include <QDate>
#include <QList>
#include <QDebug>
#include <QInputDialog>
#include <QSettings>

qint64 MainWindow::adminId = 0;
qint64 MainWindow::adminType = 1;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	db = Database::getInstance();

	ui->setupUi(this);
	ui->mainToolBar->setVisible(false);
	ui->menuBar->setEnabled(false);
	ui->pages->setCurrentIndex(0);

	foodModel = new FoodModel(this);
	ui->table1->setModel(foodModel);
	ui->table1->setAlternatingRowColors(true);
	ui->table1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	purchaseModel = new PurchaseModel(this);
	ui->table2->setModel(purchaseModel);
	ui->table2->setAlternatingRowColors(true);
	ui->table2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	shipmentModel = new ShipmentModel(this);
	ui->table3->setModel(shipmentModel);
	ui->table3->setAlternatingRowColors(true);
	ui->table3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	adminModel = new AdminModel(this);
	ui->table4->setModel(adminModel);
	ui->table4->setAlternatingRowColors(true);
	ui->table4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	userModel = new UserModel(this);
	ui->table5->setModel(userModel);
	ui->table5->setAlternatingRowColors(true);
	ui->table5->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	supplierModel = new SupplierModel(this);
	ui->table6->setModel(supplierModel);
	ui->table6->setAlternatingRowColors(true);
	ui->table6->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
	delete ui;
	delete foodModel;
	delete userModel;
	delete adminModel;
	delete purchaseModel;
	delete shipmentModel;
	delete supplierModel;
	delete db;
}

/**
 * 登陆
 */
void MainWindow::on_btnLogin_clicked()
{
	if(ui->cbAccount->currentText().isEmpty()){
		QMessageBox::warning(this, "提示", "请输入用户名");
		return;
	}
	if(ui->editPassword->text().isEmpty()){
		QMessageBox::warning(this, "提示", "请输入密码");
		return;
	}
	if(db->login(ui->cbAccount->currentText(), ui->editPassword->text())){
		ui->mainToolBar->setVisible(true);
		ui->menuBar->setEnabled(true);
		on_actionWarehouse_triggered();
		adminId = ui->cbAccount->currentText().toLongLong();
		adminType = db->getAdminType(adminId);
		if(adminType != 0){
			ui->btnUpdateAuthPassword->setVisible(false);
			ui->btnAddAdmin4->setVisible(false);
		}
	}else{
		QMessageBox::critical(this, "登录失败", "账号不存在或密码错误");
	}
}

/**
 * 进货
 */
void MainWindow::on_actionPurchase_triggered()
{
	ui->pages->setCurrentIndex(2);
	fillFoodName(ui->cbFood2);
	fillSupplierName(ui->cbSupplier2);
	purchaseModel->update(ui->cbSupplier2->currentText(), ui->cbFood2->currentText());
}

/**
 * 出货
 */
void MainWindow::on_actionShipment_triggered()
{
	ui->pages->setCurrentIndex(3);
	shipmentModel->update();
}

/**
 * 仓库
 */
void MainWindow::on_actionWarehouse_triggered()
{
	ui->pages->setCurrentIndex(1);
	foodModel->update();
}

void MainWindow::fillFoodName(QComboBox *comboBox)
{
	comboBox->clear();
	comboBox->addItem("粮食");
	QStringList foodNameList;
	db->getFoodNameList(foodNameList);
	comboBox->addItems(foodNameList);
}

void MainWindow::fillSupplierName(QComboBox *comboBox)
{
	comboBox->clear();
	comboBox->addItem("供货商");
	QStringList supplierNameList;
	db->getSupplierNameList(supplierNameList);
	comboBox->addItems(supplierNameList);
}

void MainWindow::on_btnAddFood1_clicked()
{
	AddFoodDialog addFoodDialog(this);
	if(addFoodDialog.exec()){
		foodModel->update();
	}
}

void MainWindow::on_cbSupplier2_currentTextChanged(const QString &supplierName)
{
	purchaseModel->update(supplierName, ui->cbFood2->currentText());
}

void MainWindow::on_cbFood2_currentTextChanged(const QString &foodName)
{
	purchaseModel->update(ui->cbSupplier2->currentText(), foodName);
}

void MainWindow::on_btnPurchase2_clicked()
{
	AuthorizeDialog authDlg;
	if(authDlg.exec() == QDialog::Rejected) return;

	PurchaseDialog purchaseDialog(this);
	if(purchaseDialog.exec()){
		purchaseModel->update(ui->cbSupplier2->currentText(), ui->cbFood2->currentText());
	}
}

void MainWindow::on_editUserId3_textChanged(const QString &user_id)
{
	//出货查询表
	if(user_id.isEmpty()){
		shipmentModel->update(-1);
	}else{
		bool ok;
		qint64 _user_id = user_id.toLongLong(&ok);
		if(ok){
			shipmentModel->update(_user_id);
		}
	}
}

void MainWindow::on_btnSale3_clicked()
{
	if(ui->editUserId3->text().isEmpty()){
		QMessageBox::warning(this, "提示", "请输入用户卡号");
		return;
	}
	bool ok;
	qint64 user_id = ui->editUserId3->text().toLongLong(&ok);
	if(ok){
		ShipmentDialog dialog(user_id, this);
		if(dialog.exec()){
			shipmentModel->update(user_id);
		}
	}
}

void MainWindow::on_actionAdministrator_triggered()
{
	ui->pages->setCurrentIndex(4);
	adminModel->update();
}

void MainWindow::on_actionCustomer_triggered()
{
	ui->pages->setCurrentIndex(5);
	userModel->update();
}

void MainWindow::on_actionSupplier_triggered()
{
	ui->pages->setCurrentIndex(6);
	supplierModel->update();
}

/**
 * 仓库表格点击事件
 */
void MainWindow::on_table1_clicked(const QModelIndex &index)
{
	if(index.column() == foodModel->columnCount(index) - 1){
		AuthorizeDialog auth;
		if(auth.exec() == QDialog::Rejected) return;
		foodModel->deleteFood(index);
	}
}

void MainWindow::on_table2_clicked(const QModelIndex &index)
{

}

void MainWindow::on_table3_clicked(const QModelIndex &index)
{

}

void MainWindow::on_table4_clicked(const QModelIndex &index)
{

}

void MainWindow::on_table5_clicked(const QModelIndex &index)
{
	if(index.column() == 3){ //详细
		qint64 user_id = userModel->getUserId(index);
		UserDetailDialog dialog(user_id, this);
		dialog.exec();
	}else if(index.column() == 4){ //删除
		AuthorizeDialog auth;
		if(auth.exec() == QDialog::Rejected) return;
		userModel->deleteUser(index);
	}
}

void MainWindow::on_table6_clicked(const QModelIndex &index)
{

}

void MainWindow::on_btnAddAdmin4_clicked()
{
	AddAdminDialog dialog(this);
	if(dialog.exec()){
		adminModel->update();
	}
}

void MainWindow::on_btnAddUser5_clicked()
{
	AddUserDialog dialog(this);
	if(dialog.exec()){
		userModel->update();
	}
}

void MainWindow::on_btnAddSupplier6_clicked()
{
	AddSupplierDialog dialog(this);
	if(dialog.exec()){
		supplierModel->update();
	}
}

void MainWindow::on_btnUpdateAuthPassword_clicked()
{
	AuthorizeDialog auth;
	if(auth.exec() == QDialog::Rejected) return;
	QString code = QInputDialog::getText(this, "提示", "请输入新的授权码", QLineEdit::NoEcho);
	if(!code.isEmpty()){
		db->setAuthCode(code);
	}
}

void MainWindow::on_btnDistribte_clicked()
{
	AuthorizeDialog auth;
	if(auth.exec() == QDialog::Rejected) return;

	QSettings config("FoodAdmin.ini", QSettings::IniFormat);

	uint lastDistribute = config.value("last_distribute", 0).toUInt();
	QMessageBox::StandardButton result =
			QMessageBox::information(this, "提示", "您上次于【"
									 + QDateTime::fromTime_t(lastDistribute).toString("yyyy-MM-dd hh:mm")
									 + "】分配，确认要分配吗?", QMessageBox::Yes | QMessageBox::No,
									 QMessageBox::No);
	if(result == QMessageBox::Yes){
		db->distribute();
		config.setValue("last_distribute", QDateTime::currentDateTime().toTime_t());
	}
}
