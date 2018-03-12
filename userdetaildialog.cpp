#include "userdetaildialog.h"
#include "ui_userdetaildialog.h"
#include "database.h"
#include <QDebug>
#include "authorizedialog.h"

UserDetailDialog::UserDetailDialog(qint64 user_id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UserDetailDialog)
{
	ui->setupUi(this);

	userDetailModel = new UserDetailModel(user_id, this);
	ui->table->setModel(userDetailModel);
	ui->table->setAlternatingRowColors(true);
	ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	Database *db = Database::getInstance();
	this->setWindowTitle(db->getUserNameById(user_id));
}

UserDetailDialog::~UserDetailDialog()
{
	delete ui;
}

UserDetailModel::UserDetailModel(qint64 user_id, QObject *parent):
	MyTableModel(parent)
{
	header_data<<"粮食"<<"单位"<<"余量"; //<< user_id << food_id
	db->getUserDetail(table_data, user_id);
}

UserDetailModel::~UserDetailModel()
{
}

bool UserDetailModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(data(index, role) == value)
		return false;

	if(!value.toString().isEmpty() && role == Qt::EditRole && index.column() == 2){
		AuthorizeDialog auth;
		if(auth.exec() == QDialog::Rejected) return false;

		if(db->updateUserQuantity(table_data.at(index.row()).at(3).toLongLong(),
									  table_data.at(index.row()).at(4).toInt(),
									  value.toInt()))
		{
			table_data[index.row()][index.column()] = value.toString();
			return true;
		}
	}
}

Qt::ItemFlags UserDetailModel::flags(const QModelIndex &index) const
{
	if(index.column() == 2){ //余量
		return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
	}
	return MyTableModel::flags(index);
}
