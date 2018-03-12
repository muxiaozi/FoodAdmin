#ifndef PURCHASEDIALOG_H
#define PURCHASEDIALOG_H

#include <QDialog>
class Database;
class QPrinter;

namespace Ui {
	class PurchaseDialog;
}

class PurchaseDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PurchaseDialog(QWidget *parent = 0);
	~PurchaseDialog();

	QString upperMoney(qreal x);

private slots:
	void on_btnOk_clicked();

	void on_btnCancel_clicked();

	void on_cbFoodName_currentTextChanged(const QString &foodName);

public slots:
	void printPreview(QPrinter *printer);

private:
	Ui::PurchaseDialog *ui;
	Database *db;
};

#endif // PURCHASEDIALOG_H
