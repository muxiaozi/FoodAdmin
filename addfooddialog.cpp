#include "addfood.h"
#include "ui_addfood.h"

AddFood::AddFood(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddFood)
{
	ui->setupUi(this);
}

AddFood::~AddFood()
{
	delete ui;
}
