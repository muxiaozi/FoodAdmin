#ifndef USERMODEL_H
#define USERMODEL_H

#include <QAbstractTableModel>

class UserModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit UserModel(QObject *parent = 0);

	// Header:
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // USERMODEL_H