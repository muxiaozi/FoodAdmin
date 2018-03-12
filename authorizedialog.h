#ifndef AUTHORIZEDIALOG_H
#define AUTHORIZEDIALOG_H

#include <QDialog>

namespace Ui {
	class AuthorizeDialog;
}

class AuthorizeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AuthorizeDialog(QWidget *parent = 0);
	~AuthorizeDialog();

private:
	Ui::AuthorizeDialog *ui;
};

#endif // AUTHORIZEDIALOG_H
