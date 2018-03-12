#include "usermodel.h"
#include "database.h"
#include <QColor>
#include "authorizedialog.h"

UserModel::UserModel(QObject *parent)
	: MyTableModel(parent)
{
	header_data<<"卡号"<<"户主"<<"人口"<<"操作"<<"操作";
}

void UserModel::update()
{
	QList<User> userList;
	db->getUserList(userList);

	beginResetModel();
	table_data.clear();
	for(auto user : userList){
		QStringList list;
		list<<QString::number(user.user_id); //0
		list<<user.user_name; //1
		list<<QString::number(user.user_count); //2
		list<<"详细"; //3
		list<<"删除"; //4
		table_data.append(list);
	}
	endResetModel();
}

qint64 UserModel::getUserId(const QModelIndex &index)
{
	return table_data.at(index.row()).at(0).toLongLong();
}

Qt::ItemFlags UserModel::flags(const QModelIndex &index) const
{
	if(index.column() == 1 ||index.column() == 2){ //姓名 人口
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	}else if(index.column() == 3 || index.column() == 4){
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}else{
		return MyTableModel::flags(index);
	}
}

QVariant UserModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
		return QVariant();

	if(role == Qt::ForegroundRole){
		if(index.column() == 3){
			return QColor(0, 0, 255);
		}else if(index.column() == 4){
			return QColor(255, 0, 0);
		}
	}

	return MyTableModel::data(index, role);
}

bool UserModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(data(index, role) == value)
		return false;

	if(!value.toString().isEmpty() && role == Qt::EditRole){
		AuthorizeDialog auth;
		if(auth.exec() == QDialog::Rejected) return false;

		switch(index.column()){
			case 1: //姓名
			{
				if(db->updateUserName(getUserId(index), value.toString()))
				{
					table_data[index.row()][index.column()] = value.toString();
					return true;
				}
				break;
			}
			case 2: //人口
			{
				bool ok;
				double userCount = value.toDouble(&ok);
				if(ok && db->updateUserCount(getUserId(index), userCount))
				{
					table_data[index.row()][index.column()] = value.toString();
					return true;
				}
				break;
			}
		}
	}

	return QAbstractTableModel::setData(index, value, role);

}

bool UserModel::deleteUser(const QModelIndex &index)
{
	qint64 userId = getUserId(index);
	if(db->deleteUser(userId)){
		return removeRow(index.row(), index.parent());
	}
	return false;
}

bool UserModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row + count - 1);
	table_data.removeAt(row);
	endRemoveRows();
	return true;
}

