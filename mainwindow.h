#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>

class Database;
class FoodModel;
class PurchaseModel;
class ShipmentModel;
class UserModel;
class AdminModel;
class SupplierModel;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	static qint64 adminId;
	static qint64 adminType;
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_btnLogin_clicked();

	void on_actionPurchase_triggered();

	void on_actionShipment_triggered();

	void on_actionWarehouse_triggered();

	void on_btnAddFood1_clicked();

	void on_cbSupplier2_currentTextChanged(const QString &supplierName);

	void on_cbFood2_currentTextChanged(const QString &foodName);

	void on_btnPurchase2_clicked();

	void on_editUserId3_textChanged(const QString &user_id);

	void on_btnSale3_clicked();

	void on_actionAdministrator_triggered();

	void on_actionCustomer_triggered();

	void on_actionSupplier_triggered();

	void on_table1_clicked(const QModelIndex &index);

	void on_table2_clicked(const QModelIndex &index);

	void on_table3_clicked(const QModelIndex &index);

	void on_table4_clicked(const QModelIndex &index);

	void on_table5_clicked(const QModelIndex &index);

	void on_table6_clicked(const QModelIndex &index);

	void on_btnAddAdmin4_clicked();

	void on_btnAddUser5_clicked();

	void on_btnAddSupplier6_clicked();

	void on_btnUpdateAuthPassword_clicked();

	void on_btnDistribte_clicked();

private:
	void fillFoodName(QComboBox *comboBox);
	void fillSupplierName(QComboBox *comboBox);

	bool authorize();

private:
	Ui::MainWindow *ui;
	Database *db;

	FoodModel *foodModel;
	PurchaseModel *purchaseModel;
	ShipmentModel *shipmentModel;
	UserModel *userModel;
	AdminModel *adminModel;
	SupplierModel *supplierModel;
};

#endif // MAINWINDOW_H
