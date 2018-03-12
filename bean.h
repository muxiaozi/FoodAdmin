#ifndef BEAN_H
#define BEAN_H

#include <QString>

struct Admin{
	qint64 admin_id;		//管理员ID
	QString admin_password;	//管理员密码
	QString admin_name;		//管理员姓名
	int admin_type;			//管理员类型 0/超级管理员 1/管理员
};

struct User{
	qint64 user_id;			//户主ID
	QString user_name;		//户主名称
	double user_count;		//每家人口数量
};

struct Food{
	int food_id;		//食物ID
	QString food_name;		//食物名称
	QString food_unit;		//食物单位
	int food_quantity;	//食物数量
	double food_price;		//食物单价
	int user_per_quantity;	//每人每年可得数量
};

struct FoodOut{
	int out_id;				//出货ID
	int food_id;			//食物ID
	qint64 user_id;			//用户ID
	int out_quantity;		//出货量
	double out_price;		//出货价格
	qint64 out_time;		//出货时间
};

struct FoodIn{
	int in_id;				//进货ID
	int food_id;			//食物ID
	int supplier_id;		//供应商ID
	int in_quantity;		//入货数量
	double in_price;		//进货单价
	qint64 in_time;			//进货时间
};

struct Supplier{
	int supplier_id;			//供应商ID
	QString supplier_name;		//供应商名字
	qint64 supplier_telphone;	//供应商联系方式
	QString supplier_info;		//供应商信息
};


#endif // BEAN_H
