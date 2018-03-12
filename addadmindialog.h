#ifndef ADDADMINDIALOG_H
#define ADDADMINDIALOG_H

#include <QDialog>

namespace Ui {
	class AddAdminDialog;
}

class AddAdminDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddAdminDialog(QWidget *parent = 0);
	~AddAdminDialog();

private slots:
	void on_btnOk_clicked();

	void on_btnCancel_clicked();

private:
	Ui::AddAdminDialog *ui;
};

#endif // ADDADMINDIALOG_H
