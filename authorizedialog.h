#ifndef AUTHORIZEDIALOG_H
#define AUTHORIZEDIALOG_H

#include <QDialog>
class Database;

namespace Ui {
	class AuthorizeDialog;
}

class AuthorizeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AuthorizeDialog(QWidget *parent = 0);
	~AuthorizeDialog();

private slots:
	void on_editPassword_textChanged(const QString &arg1);

	void on_btnCancel_clicked();

private:
	Ui::AuthorizeDialog *ui;
	Database *db;

};

#endif // AUTHORIZEDIALOG_H
