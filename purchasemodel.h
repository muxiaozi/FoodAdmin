#ifndef PURCHASEMODEL_H
#define PURCHASEMODEL_H

#include "mytablemodel.h"

class PurchaseModel : public MyTableModel
{
	Q_OBJECT

public:
	explicit PurchaseModel(QObject *parent = 0);

	void update(const QString &supplierName, const QString &foodName);
};

#endif // PURCHASEMODEL_H
