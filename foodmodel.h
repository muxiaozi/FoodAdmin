#ifndef FOODMODEL_H
#define FOODMODEL_H

#include "mytablemodel.h"

class FoodModel : public MyTableModel
{
	Q_OBJECT
public:
	explicit FoodModel(QObject *parent = 0);

	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
	bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
	bool removeRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;

	void update();
	bool deleteFood(const QModelIndex& index);
};

#endif // FOODMODEL_H
