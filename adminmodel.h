#ifndef ADMINMODEL_H
#define ADMINMODEL_H

#include "mytablemodel.h"

class AdminModel : public MyTableModel
{
public:
	explicit AdminModel(QObject *parent = 0);

	void update();
};

#endif // ADMINMODEL_H
