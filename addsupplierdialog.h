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

private:
	Ui::AddSupplierDialog *ui;
};

#endif // ADDSUPPLIERDIALOG_H
