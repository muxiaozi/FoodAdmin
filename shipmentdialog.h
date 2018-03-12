#ifndef SHIPMENTDIALOG_H
#define SHIPMENTDIALOG_H

#include <QDialog>
#include "mytablemodel.h"
#include <QStyledItemDelegate>
#include "bean.h"

class QPrinter;

namespace Ui {
	class ShipmentDialog;
}
class SaleModel;
class CheckBoxDelegate;

class ShipmentDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ShipmentDialog(qint64 user_id, QWidget *parent = 0);
	~ShipmentDialog();

private slots:
	void on_btnOk_clicked();

	void on_btnCancel_clicked();

private:
	Ui::ShipmentDialog *ui;
	SaleModel *saleModel;
	CheckBoxDelegate *checkBoxDelegate;
	qint64 user_id;
};

/**
 * @brief 出货类
 */
class SaleModel : public MyTableModel
{
	Q_OBJECT

public:
	QString description;

	explicit SaleModel(qint64 user_id, QObject *parent = 0);
	~SaleModel();

	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
	bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

	bool getSaleList(QList<FoodOut> &saleList);
	QString upperMoney(qreal x);

public slots:
	void printPreview(QPrinter *printer);


private:
	qint64 user_id;
};

/**
 * @brief 复选框类
 */
class CheckBoxDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	explicit CheckBoxDelegate(QObject *parent = 0);
	~CheckBoxDelegate();

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) Q_DECL_OVERRIDE;
};

#endif // SHIPMENTDIALOG_H
