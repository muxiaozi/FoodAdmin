#ifndef SHIPMENTDIALOG_H
#define SHIPMENTDIALOG_H

#include <QDialog>

namespace Ui {
	class ShipmentDialog;
}

class ShipmentDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ShipmentDialog(QWidget *parent = 0);
	~ShipmentDialog();

private:
	Ui::ShipmentDialog *ui;
};

#endif // SHIPMENTDIALOG_H
