#ifndef SHIPMENTMODEL_H
#define SHIPMENTMODEL_H

#include "mytablemodel.h"

class ShipmentModel : public MyTableModel
{
	Q_OBJECT

public:
	explicit ShipmentModel(QObject *parent = 0);

	void update(qint64 user_id = -1);
};

#endif // SHIPMENTMODEL_H
