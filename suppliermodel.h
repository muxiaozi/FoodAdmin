#ifndef SUPPLIERMODEL_H
#define SUPPLIERMODEL_H

#include "mytablemodel.h"

class SupplierModel : public MyTableModel
{
	Q_OBJECT
public:
	explicit SupplierModel(QObject *parent = 0);

	void update();
};

#endif // SUPPLIERMODEL_H
