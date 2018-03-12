#include "shipmentdialog.h"
#include "ui_shipmentdialog.h"
#include "database.h"
#include "mainwindow.h"
#include <QCheckBox>
#include <QMouseEvent>
#include <QMessageBox>
#include <QtMath>
#include <QDebug>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QPainter>
#include <QDate>
#include <QBrush>
#include <QColor>

ShipmentDialog::ShipmentDialog(qint64 user_id, QWidget *parent):
	QDialog(parent),
	ui(new Ui::ShipmentDialog),
	user_id(user_id)
{
	ui->setupUi(this);

	saleModel = new SaleModel(user_id, this);
	checkBoxDelegate = new CheckBoxDelegate(this);
	ui->table->setModel(saleModel);
	ui->table->setItemDelegate(checkBoxDelegate);
	ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

ShipmentDialog::~ShipmentDialog()
{
	delete ui;
	delete saleModel;
}

void ShipmentDialog::on_btnOk_clicked()
{
	saleModel->description = ui->editRemark->text();

	QList<FoodOut> saleList;
	saleModel->getSaleList(saleList);
	if(saleList.isEmpty()){
		QMessageBox::warning(this, "提示", "您未选择任何粮食！");
		return;
	}

	//设置打印机
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::NativeFormat);
	printer.setPageSize(QPrinter::A4);
	printer.setPageOrientation(QPageLayout::Portrait);
	QPagedPaintDevice::Margins m;
	m.top = 20;
	printer.setMargins(m);

	//生成预览
	QPrintPreviewDialog previewDialog(&printer, this);
	connect(&previewDialog, SIGNAL(paintRequested(QPrinter*)),
			saleModel, SLOT(printPreview(QPrinter*)));
	if(previewDialog.exec()){
		Database *db = Database::getInstance();
		for(auto food : saleList){
			food.user_id = this->user_id;
			db->shipment(food);
		}
		accept();
	}

	//直接打印
//	QPrintDialog printDialog(&printer, this);
//	if(printDialog.exec() == QDialog::Accepted){
//		saleModel->printPreview(&printer);
//	}
}

void ShipmentDialog::on_btnCancel_clicked()
{
	reject();
}

SaleModel::SaleModel(qint64 user_id, QObject *parent):
	MyTableModel(parent),
	user_id(user_id)
{
	header_data<<"选择"<<"粮食"<<"单位"<<"单价/元"<<"取量"<<"今年余量"<<"金额/元"<<"库存";
	db->getSaleList(table_data, user_id);
}

SaleModel::~SaleModel()
{
}

Qt::ItemFlags SaleModel::flags(const QModelIndex &index) const
{
	if(!index.isValid()) return Qt::NoItemFlags;
	if(table_data.at(index.row()).at(0) == "false"){
		return Qt::NoItemFlags;
	}else if(index.column() == 3 || index.column() == 4){ //单价 取量
		return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
	}else{
		return Qt::ItemIsEnabled;
	}
}

QVariant SaleModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
		return QVariant();

	switch(role){
		case Qt::UserRole:
			if(index.column() == 0){
				return table_data.at(index.row()).at(index.column());
			}
			break;

		case Qt::DisplayRole:
			if(index.column() != 0){
				return table_data.at(index.row()).at(index.column());
			}
			break;

		default:
			return MyTableModel::data(index, role);
	}

	return QVariant();
}

bool SaleModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(data(index, role) == value)
		return false;

	switch(role){
		case Qt::UserRole:
			if(index.column() == 0){
				beginResetModel();
				table_data[index.row()][index.column()] = value.toString();
				emit dataChanged(index, index, QVector<int>()<<Qt::UserRole);
				endResetModel();
				return true;
			}
			break;

		case Qt::EditRole:
			if(index.column() == 3){ //修改单价
				bool ok;
				double price = value.toDouble(&ok);
				if(ok){
					//单价修改
					table_data[index.row()][3] = value.toString();
					//修改价格
					table_data[index.row()][6] = QString::number(price * table_data[index.row()][4].toInt());
					emit dataChanged(index, index, QVector<int>()<<Qt::EditRole);
					return true;
				}
			}else if(index.column() == 4){ //修改数量
				bool ok;
				int count = value.toInt(&ok);
				if(ok){
					if(count > table_data[index.row()][5].toInt()){
						QMessageBox::information(0, "提示", "数量超过今年余量，已调整到最大可取");
						count = qMin(table_data[index.row()][5].toInt(), table_data[index.row()][7].toInt());
					}else if(count > table_data[index.row()][7].toInt()){
						QMessageBox::information(0, "提示", "数量超过库存，已调整到最大可取");
						count = qMin(table_data[index.row()][5].toInt(), table_data[index.row()][7].toInt());
					}
					//数量修改
					table_data[index.row()][4] = QString::number(count);
					//修改价格
					table_data[index.row()][6] = QString::number(count * table_data[index.row()][3].toDouble());
					emit dataChanged(index, index, QVector<int>()<<Qt::EditRole);
					return true;
				}
			}
			break;

		default:
			return false;
	}
	return false;
}

bool SaleModel::getSaleList(QList<FoodOut> &saleList)
{
	for(QStringList food : table_data){
		if(food.at(0) == "true" && food.at(4).toInt() != 0){ //选中该粮食且数量不为0
			FoodOut out;
			out.food_id = food.at(8).toInt();
			out.out_price = food.at(3).toDouble();
			out.out_quantity = food.at(4).toInt();
			saleList.append(out);
		}
	}
}

QString SaleModel::upperMoney(qreal x)
{
	QStringList NUM_C;
	NUM_C<<"零"<<"壹"<<"贰"<<"叁"<<"肆"<<"伍"<<"陆"<<"柒"<<"捌"<<
		 "玖"<<"拾"<<"佰"<<"仟"<<"万"<<"亿";
	QString strNum = QString("%1").arg(x);
	int j;
	int length = strNum.size();

	int dotPos = strNum.indexOf('.');
	if(dotPos != -1){
		length = dotPos;
	}

	QString result;
	if(length == 1 && strNum[0] == '0')
		result.append(NUM_C[0]);
	else {
		for(int i = 0; i < length; i++) {
			if(strNum.at(i) == '0') {
				int offset = (length - i) % 4 == 0 ? 4 : (length - i) % 4;
				for(j = i + 1; j < i + offset; j++) {
					if(strNum[j] != '0')
						break;
				}
				if(j < i + offset && (strNum[i - 1] != '0' || (length - i) % 4 == 0))
					result.append(NUM_C[strNum.at(i).toLatin1() - '0']);

			} else if(strNum[i] == '1') {
				if(length % 4 != 2 || i != 0)
					result.append(NUM_C[strNum.at(i).toLatin1() - '0']);
			} else {
				result.append(NUM_C[strNum.at(i).toLatin1() - '0']);
			}

			switch((length - i) % 4) {
			case 0://千
				if(strNum[i] != '0')
					result.append(NUM_C[12]);
				break;
			case 1: {
				switch((length - i) / 4) {
				case 1:
					if(length > 8 && strNum[i] == '0' && strNum[i - 1] == '0' && strNum[i - 2] == '0' && strNum[i - 3] == '0');
					else
						result.append(NUM_C[13]);//万
					break;
				case 2:
					result.append(NUM_C[14]);//亿
					break;
				}
				break;
			}
			case 2: { //十
				if(strNum[i] != '0')
					result.append(NUM_C[10]);
				break;
			}
			case 3://百
				if(strNum[i] != '0')
					result.append(NUM_C[11]);
				break;
			}
		}

		if(dotPos == -1){
			result.append("元整");
		}else{
			result.append("元");
			if(strNum.size() - length > 1){
				result.append(NUM_C[strNum.at(length + 1).toLatin1() - '0']).append("角");
			}
			if(strNum.size() - length > 2){
				result.append(NUM_C[strNum.at(length + 2).toLatin1() - '0']).append("分");
			}
		}
	}
	return result;
}

void SaleModel::printPreview(QPrinter *printer)
{
	//"选择"<<"粮食"<<"单位"<<"单价/元"<<"取量"<<"今年余量"<<"金额/元"<<"库存"
	QPainter painter(printer);
	qreal cellWidth = printer->width() / 9; //excel显示宽度72像素
	qreal cellHeight = cellWidth / 72 * 20; //excel显示高度20像素
	qreal hOffset = cellWidth * 0.1;

	//画分割线
	painter.setPen(QPen(QBrush(QColor(0,0,0)), 4));
	painter.drawLine(cellWidth * 1, cellHeight * 4.3, cellWidth * 8, cellHeight * 4.3);

	//画标题
	QFont font("宋体");
	font.setPointSize(15);
	painter.setFont(font);
	painter.drawText(cellWidth * 3.5, cellHeight * 1, "西易村福利供应站销售单");

	//画摘要
	font.setPointSize(11);
	painter.setFont(font);
	painter.drawText(cellWidth * 1, cellHeight * 3, "时间：" + QDate::currentDate().toString("yyyy-MM-dd"));
	painter.drawText(cellWidth * 3, cellHeight * 3, "客户：" + db->getUserNameById(user_id));
	painter.drawText(cellWidth * 6, cellHeight * 3, "操作员：" + db->getAdminNameById(MainWindow::adminId));

	//画表头
	font.setBold(true);
	painter.setFont(font);
	painter.drawText(cellWidth * 1 + hOffset, cellHeight * 5, "粮食名"); //粮食名
	painter.drawText(cellWidth * 3 + hOffset, cellHeight * 5, "单位"); //单位
	painter.drawText(cellWidth * 4 + hOffset, cellHeight * 5, "数量"); //数量
	painter.drawText(cellWidth * 5 + hOffset, cellHeight * 5, "单价"); //单价
	painter.drawText(cellWidth * 6 + hOffset, cellHeight * 5, "金额"); //金额
	painter.drawText(cellWidth * 7 + hOffset, cellHeight * 5, "今年余量"); //金额

	//画内容
	font.setBold(false);
	painter.setFont(font);
	double all = 0;
	int row = 0;
	for(int i = 0; i < table_data.length(); ++i){
		const QStringList &food = table_data.at(i);
		if(food.at(0) == "true" && food.at(4).toInt() != 0){ //选中该粮食且数量不为0
			painter.drawText(cellWidth * 1 + hOffset, cellHeight * (row + 6), food.at(1)); //粮食名
			painter.drawText(cellWidth * 3 + hOffset, cellHeight * (row + 6), food.at(2)); //单位
			painter.drawText(cellWidth * 4 + hOffset, cellHeight * (row + 6), food.at(4)); //数量
			painter.drawText(cellWidth * 5 + hOffset, cellHeight * (row + 6), food.at(3)); //单价
			painter.drawText(cellWidth * 6 + hOffset, cellHeight * (row + 6), food.at(6)); //金额
			int _quantity = food.at(5).toInt() - food.at(4).toInt();
			painter.drawText(cellWidth * 7 + hOffset, cellHeight * (row + 6), QString::number(_quantity)); //今年余量
			all += food.at(6).toDouble();

			//分割线
			painter.drawLine(cellWidth * 1, cellHeight * (row + 5.3), cellWidth * 8, cellHeight * (row + 5.3));
			++row;
		}
	}

	//竖线
	painter.drawLine(cellWidth * 1, cellHeight * 4.3, cellWidth * 1, cellHeight * (row + 5.3));
	painter.drawLine(cellWidth * 3, cellHeight * 4.3, cellWidth * 3, cellHeight * (row + 5.3));
	painter.drawLine(cellWidth * 4, cellHeight * 4.3, cellWidth * 4, cellHeight * (row + 5.3));
	painter.drawLine(cellWidth * 5, cellHeight * 4.3, cellWidth * 5, cellHeight * (row + 5.3));
	painter.drawLine(cellWidth * 6, cellHeight * 4.3, cellWidth * 6, cellHeight * (row + 5.3));
	painter.drawLine(cellWidth * 7, cellHeight * 4.3, cellWidth * 7, cellHeight * (row + 5.3));
	painter.drawLine(cellWidth * 8, cellHeight * 4.3, cellWidth * 8, cellHeight * (row + 5.3));

	//画最后一条线
	painter.drawLine(cellWidth * 1, cellHeight * (row + 5.3), cellWidth * 8, cellHeight * (row + 5.3));

	//合计
	painter.drawText(cellWidth * 1, cellHeight * (row + 7), QString("合计：%1").arg(all)); //粮食名
	painter.drawText(cellWidth * 3, cellHeight * (row + 7), QString("大写：%1").arg(upperMoney(all))); //金额

	//备注
	painter.drawText(cellWidth * 1, cellHeight * (row + 8), "备注：" + description);
}

CheckBoxDelegate::CheckBoxDelegate(QObject *parent):
	QStyledItemDelegate(parent)
{
}

CheckBoxDelegate::~CheckBoxDelegate()
{
}

// 绘制复选框
void CheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItem viewOption(option);
	initStyleOption(&viewOption, index);
	if (option.state.testFlag(QStyle::State_HasFocus))
		viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

	QStyledItemDelegate::paint(painter, viewOption, index);

	if (index.column() == 0)
	{
		bool data = index.model()->data(index, Qt::UserRole).toBool();

		QStyleOptionButton checkBoxStyle;
		checkBoxStyle.state = data ? QStyle::State_On : QStyle::State_Off;
		checkBoxStyle.state |= QStyle::State_Enabled;
		checkBoxStyle.iconSize = QSize(20, 20);
		checkBoxStyle.rect = option.rect;

		QCheckBox checkBox;
		QApplication::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
	}
}

// 响应鼠标事件，更新数据
bool CheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	QRect decorationRect = option.rect;

	QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
	if ((event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick)
			&& decorationRect.contains(mouseEvent->pos()))
	{
		if (index.column() == 0)
		{
			bool data = model->data(index, Qt::UserRole).toBool();
			model->setData(index, !data, Qt::UserRole);
		}
	}

	return QStyledItemDelegate::editorEvent(event, model, option, index);
}
