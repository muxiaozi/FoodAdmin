#ifndef USERDETAILDIALOG_H
#define USERDETAILDIALOG_H

#include <QDialog>
#include "mytablemodel.h"
class UserDetailModel;

namespace Ui {
	class UserDetailDialog;
}

class UserDetailDialog : public QDialog
{
	Q_OBJECT

public:
	explicit UserDetailDialog(qint64 user_id, QWidget *parent = 0);
	~UserDetailDialog();

private:
	Ui::UserDetailDialog *ui;
	UserDetailModel *userDetailModel;
};

class UserDetailModel : public MyTableModel
{
	Q_OBJECT

public:
	explicit UserDetailModel(qint64 user_id, QObject* parent = 0);
	~UserDetailModel();

	bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // USERDETAILDIALOG_H
