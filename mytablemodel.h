#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QList>

class Database;

class MyTableModel: public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit MyTableModel(QObject *parent = 0);
	int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

protected:
	QStringList header_data;
	QList<QStringList> table_data;
	Database *db;
};

#endif // MYTABLEMODEL_H
