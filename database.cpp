#include "database.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QDateTime>

Database* Database::instance = NULL;

Database::Database()
{
}

Database *Database::getInstance()
{
	if(instance == NULL){
		instance = new Database();
	}
	return instance;
}

bool Database::login(const QString& admin_id, const QString& admin_password)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("SELECT * FROM admin WHERE admin_id=? AND admin_password=? LIMIT 1");
	query.bindValue(0, admin_id);
	query.bindValue(1, admin_password);
	query.exec();
	return query.first();
}

bool Database::addUser(const User &user)
{
	QSqlDatabase db = QSqlDatabase::database();
	db.transaction();

	QSqlQuery query(db);
	query.prepare("INSERT INTO user(user_id,user_name,user_count) VALUES(?,?,?)");
	query.bindValue(0, user.user_id);
	query.bindValue(1, user.user_name);
	query.bindValue(2, user.user_count);
	if(!query.exec()){
		db.rollback();
		return false;
	}

	query.prepare("INSERT INTO goods(user_id,food_id,quantity)"
				  " SELECT ?,food_id,user_per_quantity*? FROM food");
	query.bindValue(0, user.user_id);
	query.bindValue(1, user.user_count);
	if(!query.exec()){
		db.rollback();
		return false;
	}

	return db.commit();
}

bool Database::deleteUser(qint64 user_id)
{
	QSqlDatabase db = QSqlDatabase::database();
	db.transaction();

	QSqlQuery query(db);
	query.prepare("DELETE FROM user WHERE user_id=?");
	query.bindValue(0, user_id);
	if(!query.exec()){
		db.rollback();
		return false;
	}

	query.prepare("DELETE FROM goods WHERE user_id=?");
	query.bindValue(0, user_id);
	if(!query.exec()){
		db.rollback();
		return false;
	}

	return db.commit();
}

bool Database::getUserList(QList<User> &userList)
{
	QSqlQuery query(QSqlDatabase::database());
	bool result = query.exec("SELECT user_id,user_name,user_count FROM user");
	while(query.next()){
		User user;
		user.user_id = query.value(0).toLongLong();
		user.user_name = query.value(1).toString();
		user.user_count = query.value(2).toDouble();
		userList.append(user);
	}
	return result;
}

QString Database::getUserNameById(qint64 user_id)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("SELECT user_name FROM user WHERE user_id=? LIMIT 1");
	query.bindValue(0, user_id);
	query.exec();
	if(query.first()){
		return query.value(0).toString();
	}else{
		return QString();
	}
}

bool Database::updateUserName(qint64 userId, const QString &userName)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("UPDATE user SET user_name=? WHERE user_id=?");
	query.bindValue(0, userName);
	query.bindValue(1, userId);
	return query.exec();
}

bool Database::updateUserCount(qint64 userId, double userCount)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("UPDATE user SET user_count=? WHERE user_id=?");
	query.bindValue(0, userCount);
	query.bindValue(1, userId);
	return query.exec();
}

bool Database::updateUserQuantity(qint64 userId, int foodId, int quantity)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("UPDATE goods SET quantity=? WHERE user_id=? AND food_id=?");
	query.bindValue(0, quantity);
	query.bindValue(1, userId);
	query.bindValue(2, foodId);
	return query.exec();
}

bool Database::getUserDetail(QList<QStringList> &userDetail, qint64 userId)
{
	//粮食-单位-剩余 / user_id food_id
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("SELECT food.food_name, food.food_unit, goods.quantity,"
				  "user.user_id, food.food_id FROM food"
				  " JOIN goods ON food.food_id=goods.food_id"
				  " JOIN user ON goods.user_id=user.user_id"
				  " WHERE user.user_id=?");
	query.bindValue(0, userId);
	if(query.exec()){
		while(query.next()){
			QStringList _list;
			_list<<query.value(0).toString();
			_list<<query.value(1).toString();
			_list<<query.value(2).toString();
			_list<<query.value(3).toString();
			_list<<query.value(4).toString();
			userDetail.append(_list);
		}
		return true;
	}else{
		return false;
	}
}

bool Database::addAdmin(const Admin &admin)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("INSERT INTO admin(admin_id,admin_password,admin_name,admin_type) VALUES(?,?,?,1)");
	query.bindValue(0, admin.admin_id);
	query.bindValue(1, admin.admin_password);
	query.bindValue(2, admin.admin_name);
	return query.exec();
}

bool Database::getAdminList(QList<Admin> &adminList)
{
	QSqlQuery query(QSqlDatabase::database());
	bool result = query.exec("SELECT admin_id,admin_name,admin_type FROM admin");
	while(query.next()){
		Admin admin;
		admin.admin_id = query.value(0).toLongLong();
		admin.admin_name = query.value(1).toString();
		admin.admin_type = query.value(2).toInt();
		adminList.append(admin);
	}
	return result;
}

QString Database::getAdminNameById(qint64 adminId)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("SELECT admin_name FROM admin WHERE admin_id=? LIMIT 1");
	query.bindValue(0, adminId);
	query.exec();
	if(query.first()){
		return query.value(0).toString();
	}else{
		return QString();
	}
}

int Database::getAdminType(qint64 adminId)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("SELECT admin_type FROM admin WHERE admin_id=? LIMIT 1");
	query.bindValue(0, adminId);
	query.exec();
	if(query.first()){
		return query.value(0).toInt();
	}else{
		return 1;
	}
}

QString Database::getAuthCode()
{
	QSqlQuery query(QSqlDatabase::database());
	query.exec("SELECT admin_auth FROM admin WHERE admin_type=0 LIMIT 1");
	if(query.first()){
		return query.value(0).toString();
	}else{
		return QString();
	}
}

bool Database::setAuthCode(const QString &authCode)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("UPDATE admin SET admin_auth=? WHERE admin_type=0");
	query.bindValue(0, authCode);
	return query.exec();
}

bool Database::addFood(const Food &food)
{
	QSqlDatabase db = QSqlDatabase::database();
	db.transaction();

	QSqlQuery query(db);
	query.prepare("INSERT INTO food(food_name,food_unit,food_quantity,food_price,user_per_quantity) VALUES(?,?,0,?,?)");
	query.bindValue(0, food.food_name);
	query.bindValue(1, food.food_unit);
	query.bindValue(2, food.food_price);
	query.bindValue(3, food.user_per_quantity);
	if(!query.exec()){
		db.rollback();
		return false;
	}
	int food_id = query.lastInsertId().toInt();

	query.prepare("INSERT INTO goods(user_id,food_id,quantity)"
				  " SELECT user_id,?,?*user_count FROM user");
	query.bindValue(0, food_id);
	query.bindValue(1, food.user_per_quantity);
	if(!query.exec()){
		db.rollback();
		return false;
	}

	return db.commit();
}

bool Database::deleteFood(int food_id)
{
	QSqlDatabase db = QSqlDatabase::database();
	db.transaction();

	QSqlQuery query(db);
	query.prepare("DELETE FROM food WHERE food_id=?");
	query.bindValue(0, food_id);
	if(!query.exec()){
		db.rollback();
		return false;
	}

	query.prepare("DELETE FROM goods WHERE food_id=?");
	query.bindValue(0, food_id);
	if(!query.exec()){
		db.rollback();
		return false;
	}

	return db.commit();
}

bool Database::updateFoodName(int food_id, const QString &food_name)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("UPDATE food SET food_name=? WHERE food_id=?");
	query.bindValue(0, food_name);
	query.bindValue(1, food_id);
	return query.exec();
}

bool Database::updateFoodUnit(int food_id, const QString &food_unit)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("UPDATE food SET food_unit=? WHERE food_id=?");
	query.bindValue(0, food_unit);
	query.bindValue(1, food_id);
	return query.exec();
}

bool Database::updateFoodPrice(int food_id, double food_price)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("UPDATE food SET food_price=? WHERE food_id=?");
	query.bindValue(0, food_price);
	query.bindValue(1, food_id);
	return query.exec();
}

bool Database::updateFoodUserPerQuantity(int food_id, int user_per_quantity)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("UPDATE food SET user_per_quantity=? WHERE food_id=?");
	query.bindValue(0, user_per_quantity);
	query.bindValue(1, food_id);
	return query.exec();
}

bool Database::updateFoodQuantity(int food_id, int food_quantity)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("UPDATE food SET food_quantity=? WHERE food_id=?");
	query.bindValue(0, food_quantity);
	query.bindValue(1, food_id);
	return query.exec();
}

bool Database::getFoodNameList(QStringList &foodNameList)
{
	QSqlQuery query(QSqlDatabase::database());
	bool ok = query.exec("SELECT food_name FROM food");
	while(query.next()){
		foodNameList.append(query.value(0).toString());
	}
	return ok;
}

bool Database::getFoodList(QList<Food> &foodList)
{
	QSqlQuery query(QSqlDatabase::database());
	bool result = query.exec("SELECT food_id,food_name,food_unit,food_quantity,food_price,user_per_quantity FROM food");
	while(query.next()){
		Food food;
		food.food_id = query.value(0).toInt();
		food.food_name = query.value(1).toString();
		food.food_unit = query.value(2).toString();
		food.food_quantity = query.value(3).toInt();
		food.food_price = query.value(4).toDouble();
		food.user_per_quantity = query.value(5).toInt();
		foodList.append(food);
	}
	return result;
}

int Database::getFoodIdByName(const QString &foodName)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("SELECT food_id FROM food WHERE food_name=? LIMIT 1");
	query.bindValue(0, foodName);
	query.exec();
	if(query.first()){
		return query.value(0).toInt();
	}else{
		return -1;
	}
}

double Database::getFoodPrice(const QString &foodName)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("SELECT food_price FROM food WHERE food_name=? LIMIT 1");
	query.bindValue(0, foodName);
	query.exec();
	if(query.first()){
		return query.value(0).toDouble();
	}else{
		return 0;
	}
}

bool Database::existsFood(int food_id)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("SELECT food_id FROM food WHERE food_id=? LIMIT 1");
	query.bindValue(0, food_id);
	query.exec();
	return query.first();
}

bool Database::addSupplier(const Supplier &supplier)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("INSERT INTO supplier(supplier_name,supplier_telphone,supplier_info) VALUES(?,?,?)");
	query.bindValue(0, supplier.supplier_name);
	query.bindValue(1, supplier.supplier_telphone);
	query.bindValue(2, supplier.supplier_info);
	return query.exec();
}

bool Database::deleteSupplier(int supplier_id)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("DELETE FROM supplier WHERE supplier_id=?");
	query.bindValue(0, supplier_id);
	return query.exec();
}

bool Database::getSupplierNameList(QStringList &supplierNameList)
{
	QSqlQuery query(QSqlDatabase::database());
	bool ok = query.exec("SELECT supplier_name FROM supplier");
	while(query.next()){
		supplierNameList.append(query.value(0).toString());
	}
	return ok;
}

bool Database::getSupplierList(QList<Supplier> &supplierList)
{
	QSqlQuery query(QSqlDatabase::database());
	bool result = query.exec("SELECT supplier_id,supplier_name,supplier_telphone,supplier_info FROM supplier");
	while(query.next()){
		Supplier supplier;
		supplier.supplier_id = query.value(0).toInt();
		supplier.supplier_name = query.value(1).toString();
		supplier.supplier_telphone = query.value(2).toLongLong();
		supplier.supplier_info = query.value(3).toString();
		supplierList.append(supplier);
	}
	return result;
}

int Database::getSupplierIdByName(const QString &supplierName)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("SELECT supplier_id FROM supplier WHERE supplier_name=? LIMIT 1");
	query.bindValue(0, supplierName);
	query.exec();
	if(query.first()){
		return query.value(0).toInt();
	}else{
		return -1;
	}
}

QString Database::getSupplierNameById(int supplier_id)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("SELECT supplier_name FROM supplier WHERE supplier_id=? LIMIT 1");
	query.bindValue(0, supplier_id);
	query.exec();
	if(query.first()){
		return query.value(0).toString();
	}else{
		return QString();
	}
}

bool Database::existsSupplier(int supplier_id)
{
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("SELECT supplier_id FROM supplier WHERE supplier_id=? LIMIT 1");
	query.bindValue(0, supplier_id);
	query.exec();
	return query.first();
}

bool Database::getPurchaseList(QList<QStringList> &list, const QString &supplierName, const QString &foodName)
{
	//header_data<<"供货商"<<"粮食"<<"单位"<<"价格"<<"数量"<<"金额"<<"时间";
	QSqlQuery query(QSqlDatabase::database());
	QString sql("SELECT supplier.supplier_name,food.food_name,food.food_unit,"
				"food_in.in_price,food_in.in_quantity,"
				"food_in.in_price*food_in.in_quantity,food_in.in_time FROM food_in"
				" LEFT JOIN supplier ON food_in.supplier_id=supplier.supplier_id"
				" LEFT JOIN food ON food_in.food_id=food.food_id WHERE 1");

	if(!supplierName.isEmpty() && supplierName != "供货商"){
		sql += QString(" AND supplier.supplier_name='%1'").arg(supplierName);
	}

	if(!foodName.isEmpty() && foodName != "粮食"){
		sql += QString(" AND food.food_name='%1'").arg(foodName);
	}

	sql += " ORDER BY food_in.in_time DESC";

	if(query.exec(sql)){
		while(query.next()){
			QStringList _list;
			_list<<query.value(0).toString();
			_list<<query.value(1).toString();
			_list<<query.value(2).toString();
			_list<<query.value(3).toString();
			_list<<query.value(4).toString();
			_list<<query.value(5).toString();
			_list<<query.value(6).toString();
			list.append(_list);
		}
		return true;
	}else{
		return false;
	}
}

bool Database::getShipmentList(QList<QStringList> &list, qint64 user_id)
{
	//<<"户主"<<"粮食"<<"单价"<<"单位"<<"数量"<<"金额"<<"时间";
	QSqlQuery query(QSqlDatabase::database());
	bool ok;
	if(user_id == -1){ //查询所有
		ok = query.exec("SELECT user.user_name,food.food_name,food_out.out_price,"
						"food.food_unit,food_out.out_quantity,"
						"food_out.out_price*food_out.out_quantity,"
						"food_out.out_time FROM food_out"
						" LEFT JOIN user ON food_out.user_id=user.user_id"
						" LEFT JOIN food ON food_out.food_id=food.food_id"
						" ORDER BY food_out.out_time DESC");
	}else{
		query.prepare("SELECT user.user_name,food.food_name,food_out.out_price,"
					  "food.food_unit,food_out.out_quantity,"
					  "food_out.out_price*food_out.out_quantity,"
					  "food_out.out_time FROM food_out"
					  " LEFT JOIN user ON food_out.user_id=user.user_id"
					  " LEFT JOIN food ON food_out.food_id=food.food_id"
					  " WHERE user.user_id=? ORDER BY food_out.out_time DESC");
		query.bindValue(0, user_id);
		ok = query.exec();
	}

	if(ok){
		while(query.next()){
			QStringList _list;
			_list<<query.value(0).toString();
			_list<<query.value(1).toString();
			_list<<query.value(2).toString();
			_list<<query.value(3).toString();
			_list<<query.value(4).toString();
			_list<<query.value(5).toString();
			_list<<query.value(6).toString();
			list.append(_list);
		}
		return true;
	}else{
		return false;
	}
}

bool Database::getSaleList(QList<QStringList> &list, qint64 user_id)
{
	//<<"选择"<<"粮食"<<"单位"<<"单价"<<"取量"<<"今年余量"<<"金额"<<"库存"
	QSqlQuery query(QSqlDatabase::database());
	query.prepare("SELECT 'true',food.food_name,food.food_unit,food.food_price,"
				  "0,goods.quantity,0,food.food_quantity,food.food_id FROM food"
				  " JOIN goods ON food.food_id=goods.food_id"
				  " JOIN user ON user.user_id=goods.user_id"
				  " WHERE user.user_id=?");
	query.bindValue(0, user_id);

	if(query.exec()){
		while(query.next()){
			QStringList _list;
			_list<<query.value(0).toString();
			_list<<query.value(1).toString();
			_list<<query.value(2).toString();
			_list<<query.value(3).toString();
			_list<<query.value(4).toString();
			_list<<query.value(5).toString();
			_list<<query.value(6).toString();
			_list<<query.value(7).toString();
			_list<<query.value(8).toString();
			list.append(_list);
		}
		return true;
	}else{
		return false;
	}
}

bool Database::purchase(const FoodIn &foodIn)
{
	if(!existsFood(foodIn.food_id) || !existsSupplier(foodIn.supplier_id))
		return false;

	QSqlDatabase db = QSqlDatabase::database();
	db.transaction();

	QSqlQuery query(db);
	query.prepare("INSERT INTO food_in(food_id,supplier_id,in_quantity,in_price,in_time)"
				  " VALUES(?,?,?,?,?)");
	query.bindValue(0, foodIn.food_id);
	query.bindValue(1, foodIn.supplier_id);
	query.bindValue(2, foodIn.in_quantity);
	query.bindValue(3, foodIn.in_price);
	query.bindValue(4, QDateTime::currentDateTime().toTime_t());
	if(!query.exec()){
		db.rollback();
		return false;
	}

	query.prepare("UPDATE food SET food_quantity=food_quantity+? WHERE food_id=?");
	query.bindValue(0, foodIn.in_quantity);
	query.bindValue(1, foodIn.food_id);
	if(!query.exec()){
		db.rollback();
		return false;
	}

	return db.commit();
}

bool Database::shipment(const FoodOut &foodOut)
{
	QSqlDatabase db = QSqlDatabase::database();
	db.transaction();

	QSqlQuery query(db);
	query.prepare("INSERT INTO food_out(food_id,user_id,out_quantity,out_price,out_time)"
				  " VALUES(?,?,?,?,?)");
	query.bindValue(0, foodOut.food_id);
	query.bindValue(1, foodOut.user_id);
	query.bindValue(2, foodOut.out_quantity);
	query.bindValue(3, foodOut.out_price);
	query.bindValue(4, QDateTime::currentDateTime().toTime_t());
	if(!query.exec()){
		db.rollback();
		return false;
	}

	query.prepare("UPDATE food SET food_quantity=food_quantity-? WHERE food_id=?");
	query.bindValue(0, foodOut.out_quantity);
	query.bindValue(1, foodOut.food_id);
	if(!query.exec()){
		db.rollback();
		return false;
	}

	query.prepare("UPDATE goods SET quantity=quantity-? WHERE food_id=? AND user_id=?");
	query.bindValue(0, foodOut.out_quantity);
	query.bindValue(1, foodOut.food_id);
	query.bindValue(2, foodOut.user_id);
	if(!query.exec()){
		db.rollback();
		return false;
	}

	return db.commit();
}

bool Database::distribute()
{
	QSqlQuery query(QSqlDatabase::database());
	return query.exec("UPDATE goods SET quantity=quantity+"
				  "(SELECT user.user_count*food.user_per_quantity FROM user,food WHERE"
				  " goods.user_id=user.user_id AND goods.food_id=food.food_id)");

}
