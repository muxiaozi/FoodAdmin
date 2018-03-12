#ifndef USERDETAILDIALOG_H
#define USERDETAILDIALOG_H

#include <QDialog>

namespace Ui {
	class UserDetailDialog;
}

class UserDetailDialog : public QDialog
{
	Q_OBJECT

public:
	explicit UserDetailDialog(QWidget *parent = 0);
	~UserDetailDialog();

private:
	Ui::UserDetailDialog *ui;
};

#endif // USERDETAILDIALOG_H
