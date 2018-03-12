#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QList>
#include "bean.h"

class Database
{
public:
	static Database *getInstance();

	bool login(const QString& admin_id, const QString& admin_password);

	bool addUser(const User &user);
	bool deleteUser(qint64 user_id);
	bool getUserList(QList<User> &userList);
	QString getUserNameById(qint64 user_id);
	bool updateUserName(qint64 userId, const QString& userName);
	bool updateUserCount(qint64 userId, double userCount);
	bool updateUserQuantity(qint64 userId, int foodId, int quantity);

	bool getUserDetail(QList<QStringList> &userDetail, qint64 userId);

	bool addAdmin(const Admin &admin);
	bool getAdminList(QList<Admin> &adminList);
	QString getAdminNameById(qint64 adminId);
	int getAdminType(qint64 adminId);
	QString getAuthCode();
	bool setAuthCode(const QString &authCode);

	bool addFood(const Food& food);
	bool deleteFood(int food_id);
	bool updateFoodName(int food_id, const QString& food_name);
	bool updateFoodUnit(int food_id, const QString& food_unit);
	bool updateFoodPrice(int food_id, double food_price);
	bool updateFoodUserPerQuantity(int food_id, int user_per_quantity);
	bool updateFoodQuantity(int food_id, int food_quantity);
	bool getFoodNameList(QStringList &foodNameList);
	bool getFoodList(QList<Food> &foodList);
	int getFoodIdByName(const QString& foodName);
	double getFoodPrice(const QString &foodName);
	bool existsFood(int food_id);

	bool addSupplier(const Supplier& supplier);
	bool deleteSupplier(int supplier_id);
	bool getSupplierNameList(QStringList &supplierNameList);
	bool getSupplierList(QList<Supplier> &supplierList);
	int getSupplierIdByName(const QString& supplierName);
	QString getSupplierNameById(int supplier_id);
	bool existsSupplier(int supplier_id);

	bool getPurchaseList(QList<QStringList> &list, const QString &supplierName, const QString &foodName);
	bool getShipmentList(QList<QStringList> &list, qint64 user_id = -1);
	bool getSaleList(QList<QStringList> &list, qint64 user_id);

	bool purchase(const FoodIn& foodIn);
	bool shipment(const FoodOut& foodOut);

	/**
	 * @brief 一键分粮
	 * @return
	 */
	bool distribute();

protected:
	Database();

private:
	static Database *instance;

};

#endif // DATABASE_H
