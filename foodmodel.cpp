#include "foodmodel.h"
#include "database.h"
#include <QDebug>
#include <QColor>
#include "authorizedialog.h"

FoodModel::FoodModel(QObject *parent) :
	MyTableModel(parent)
{
	header_data<<"名称"<<"存量"<<"单位"<<"单价/元"<<"每年每人数量"<<"操作";
}

QVariant FoodModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid()) return QVariant();
	switch(role){
		case Qt::TextAlignmentRole:
			return Qt::AlignCenter;
			break;
		case Qt::ForegroundRole:
			if(index.column() == columnCount(index) - 1){
				return QColor(255, 0, 0);
			}
			break;
		case Qt::DisplayRole:
			return table_data.at(index.row()).at(index.column());
			break;
		default:
			return QVariant();
	}
	return QVariant();
}

bool FoodModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(role == Qt::EditRole && !value.toString().isEmpty()){
		bool ok;
		switch(index.column()){
			case 0: //粮食名
				if(db->updateFoodName(table_data[index.row()][6].toInt(), value.toString())){
					table_data[index.row()][index.column()] = value.toString();
					emit dataChanged(index, index);
					return true;
				}
				break;
			case 2: //单位
				if(db->updateFoodUnit(table_data[index.row()][6].toInt(), value.toString())){
					table_data[index.row()][index.column()] = value.toString();
					emit dataChanged(index, index);
					return true;
				}
				break;
			case 3: //单价
			{
				double price = value.toDouble(&ok);
				if(ok && db->updateFoodPrice(table_data[index.row()][6].toInt(), price))
				{
					table_data[index.row()][index.column()] = value.toString();
					emit dataChanged(index, index);
					return true;
				}
				break;
			}
			case 4: //每人每年数量
			{
				AuthorizeDialog auth;
				if(auth.exec() == QDialog::Rejected) return false;

				int userPerQuantity = value.toInt(&ok);
				if(ok && db->updateFoodUserPerQuantity(table_data[index.row()][6].toInt(), userPerQuantity))
				{
					table_data[index.row()][index.column()] = value.toString();
					emit dataChanged(index, index);
					return true;
				}
				break;
			}
		}
	}
	return false;
}

Qt::ItemFlags FoodModel::flags(const QModelIndex &index) const
{
	if(index.column() == 0
			|| index.column() == 2
			|| index.column() == 3
			|| index.column() == 4 )
	{
		return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}
	return Qt::ItemIsEnabled;
}

bool FoodModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row + count - 1);
	table_data.removeAt(row);
	endRemoveRows();
	return true;
}

void FoodModel::update()
{
	beginResetModel();
	table_data.clear();
	QList<Food> foodList;
	db->getFoodList(foodList);
	for(auto food : foodList){
		QStringList list;
		list<<food.food_name; //0
		list<<QString::number(food.food_quantity); //1
		list<<food.food_unit; //2
		list<<QString::number(food.food_price);	//3
		list<<QString::number(food.user_per_quantity); //4
		list<<"删除"; //5
		list<<QString::number(food.food_id); //6
		table_data.append(list);
	}
	endResetModel();
}

bool FoodModel::deleteFood(const QModelIndex &index)
{
	int food_id = table_data.at(index.row()).at(6).toInt();
	if(db->deleteFood(food_id)){
		return removeRow(index.row(), index.parent());
	}
	return false;
}


