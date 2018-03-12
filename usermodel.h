#ifndef USERMODEL_H
#define USERMODEL_H

#include "mytablemodel.h"

class UserModel : public MyTableModel
{
	Q_OBJECT

public:
	explicit UserModel(QObject *parent = 0);

	void update();

	qint64 getUserId(const QModelIndex &index);

	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

	bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

	bool deleteUser(const QModelIndex &index);

	bool removeRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
};

#endif // USERMODEL_H
