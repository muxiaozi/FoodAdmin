#include "mytablemodel.h"
#include "database.h"

MyTableModel::MyTableModel(QObject *parent) :
	QAbstractTableModel(parent)
{
	db = Database::getInstance();
}

int MyTableModel::rowCount(const QModelIndex &/*parent*/) const
{
	return table_data.length();
}

int MyTableModel::columnCount(const QModelIndex &/*parent*/) const
{
	return header_data.length();
}

QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
		return QVariant();

	switch(role){
		case Qt::TextAlignmentRole:
			return Qt::AlignCenter;
			break;
		case Qt::DisplayRole:
			return table_data.at(index.row()).at(index.column());
			break;
		default:
			return QVariant();
	}
}

QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role != Qt::DisplayRole || orientation == Qt::Vertical) return QVariant();
	return header_data.at(section);
}

Qt::ItemFlags MyTableModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled;
}
