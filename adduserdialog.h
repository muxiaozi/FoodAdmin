﻿#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>

namespace Ui {
	class AddUserDialog;
}

class AddUserDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddUserDialog(QWidget *parent = 0);
	~AddUserDialog();

private slots:
	void on_btnOk_clicked();

	void on_btnCancel_clicked();

private:
	Ui::AddUserDialog *ui;
};

#endif // ADDUSERDIALOG_H
