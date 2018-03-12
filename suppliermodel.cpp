#include "suppliermodel.h"
#include "database.h"

SupplierModel::SupplierModel(QObject *parent):
	MyTableModel(parent)
{
	header_data<<"供货商名称"<<"联系方式"<<"备注信息";
}

void SupplierModel::update()
{
	QList<Supplier> supplierList;
	db->getSupplierList(supplierList);

	beginResetModel();
	table_data.clear();
	for(auto supplier: supplierList){
		QStringList list;
		list<<supplier.supplier_name;
		list<<QString::number(supplier.supplier_telphone);
		list<<supplier.supplier_info;
		table_data.append(list);
	}
	endResetModel();
}
