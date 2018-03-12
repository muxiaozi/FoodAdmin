#include "shipmentmodel.h"
#include "database.h"
#include <QDateTime>

ShipmentModel::ShipmentModel(QObject *parent)
	: MyTableModel(parent)
{
	header_data<<"户主"<<"粮食"<<"单价/元"<<"单位"<<"数量"<<"金额/元"<<"时间";
}

void ShipmentModel::update(qint64 user_id)
{
	beginResetModel();
	table_data.clear();
	db->getShipmentList(table_data, user_id);
	for(int i = 0; i < table_data.length(); ++i){
		uint _time = table_data.at(i).at(6).toUInt();
		table_data[i][6] = QDateTime::fromTime_t(_time).toString("yyyy-MM-dd HH:mm");
	}
	endResetModel();
}
