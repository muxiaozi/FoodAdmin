#ifndef ADDSUPPLIERDIALOG_H
#define ADDSUPPLIERDIALOG_H

#include <QDialog>

namespace Ui {
	class AddSupplierDialog;
}

class AddSupplierDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddSupplierDialog(QWidget *parent = 0);
	~AddSupplierDialog();

private slots:
	void on_btnOk_clicked();

	void on_btnCancel_clicked();

private:
	Ui::AddSupplierDialog *ui;
};

#endif // ADDSUPPLIERDIALOG_H
