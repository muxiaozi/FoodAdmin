#ifndef ADDFOOD_H
#define ADDFOOD_H

#include <QDialog>

namespace Ui {
	class AddFood;
}

class AddFood : public QDialog
{
	Q_OBJECT

public:
	explicit AddFood(QWidget *parent = 0);
	~AddFood();

private:
	Ui::AddFood *ui;
};

#endif // ADDFOOD_H
