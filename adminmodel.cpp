#include "adminmodel.h"
#include "database.h"

AdminModel::AdminModel(QObject *parent) :
	MyTableModel(parent)
{
	header_data<<"账号"<<"姓名"<<"类型";
}

void AdminModel::update()
{
	QList<Admin> adminList;
	db->getAdminList(adminList);

	beginResetModel();
	table_data.clear();
	for(auto admin : adminList){
		QStringList list;
		list<<QString::number(admin.admin_id);
		list<<admin.admin_name;
		list<<(admin.admin_type == 0 ? "超级管理员" : "管理员");

		table_data.append(list);
	}
	endResetModel();
}
