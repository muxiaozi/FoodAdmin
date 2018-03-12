#include "usermodel.h"

UserModel::UserModel(QObject *parent)
	: QAbstractTableModel(parent)
{
}

QVariant UserModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// FIXME: Implement me!
}

int UserModel::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return 0;

	// FIXME: Implement me!
}

int UserModel::columnCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return 0;

	// FIXME: Implement me!
}

QVariant UserModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	// FIXME: Implement me!
	return QVariant();
}
