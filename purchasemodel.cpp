#include "purchasemodel.h"
#include "database.h"
#include <QDateTime>

PurchaseModel::PurchaseModel(QObject *parent)
	: MyTableModel(parent)
{
	header_data<<"供货商"<<"粮食"<<"单位"<<"价格/元"<<"数量"<<"金额/元"<<"时间";
}

void PurchaseModel::update(const QString &supplierName, const QString &foodName)
{
	beginResetModel();
	table_data.clear();
	db->getPurchaseList(table_data, supplierName, foodName);
	for(int i = 0; i < table_data.length(); ++i){
		uint _time = table_data.at(i).at(6).toUInt();
		table_data[i][6] = QDateTime::fromTime_t(_time).toString("yyyy-MM-dd HH:mm");
	}
	endResetModel();
}
