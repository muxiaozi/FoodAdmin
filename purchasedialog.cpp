#include "purchasedialog.h"
#include "ui_purchasedialog.h"
#include "database.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QDate>
#include <QPrintDialog>
#include <QDebug>

PurchaseDialog::PurchaseDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PurchaseDialog)
{
	ui->setupUi(this);
	db = Database::getInstance();

	ui->cbFoodName->clear();
	ui->cbFoodName->addItem("粮食");
	QStringList foodNameList;
	db->getFoodNameList(foodNameList);
	ui->cbFoodName->addItems(foodNameList);

	ui->cbSupplier->clear();
	ui->cbSupplier->addItem("供货商");
	QStringList supplierNameList;
	db->getSupplierNameList(supplierNameList);
	ui->cbSupplier->addItems(supplierNameList);
}

PurchaseDialog::~PurchaseDialog()
{
	delete ui;
}

void PurchaseDialog::on_btnOk_clicked()
{
	if(ui->cbSupplier->currentText() == "供货商"){
		QMessageBox::critical(this, "提示", "请选择供货商");
		return;
	}

	if(ui->cbFoodName->currentText() == "粮食"){
		QMessageBox::critical(this, "提示", "请选择粮食");
		return;
	}

	if(ui->sbQuantity->value() == 0){
		QMessageBox::critical(this, "提示", "请输入进货量");
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
			this, SLOT(printPreview(QPrinter*)));
	if(previewDialog.exec()){
		Database *db = Database::getInstance();
		FoodIn foodIn;
		foodIn.food_id = db->getFoodIdByName(ui->cbFoodName->currentText());
		foodIn.supplier_id = db->getSupplierIdByName(ui->cbSupplier->currentText());
		foodIn.in_price = ui->dsbFoodPrice->value();
		foodIn.in_quantity = ui->sbQuantity->value();
		if(!db->purchase(foodIn)){
			QMessageBox::critical(this, "提示", "请检查输入数据合法性");
			return;
		}
		accept();
	}


	//直接打印
//	QPrintDialog printDialog(&printer, this);
//	if(printDialog.exec() == QDialog::Accepted){
//		this->printPreview(&printer);
//	}
}

void PurchaseDialog::on_btnCancel_clicked()
{
	reject();
}

void PurchaseDialog::on_cbFoodName_currentTextChanged(const QString &foodName)
{
	ui->dsbFoodPrice->setValue(db->getFoodPrice(foodName));
}

void PurchaseDialog::printPreview(QPrinter *printer)
{
	QPainter painter(printer);
	qreal cellWidth = printer->width() / 9; //excel显示宽度72像素
	qreal cellHeight = cellWidth / 72 * 20; //excel显示高度20像素
	qreal hOffset = cellWidth * 0.1;

	//画分割线
	painter.setPen(QPen(QBrush(QColor(0,0,0)), 4));
	painter.drawLine(cellWidth * 1, cellHeight * 4.3, cellWidth * 8, cellHeight * 4.3);
	painter.drawLine(cellWidth * 1, cellHeight * 5.3, cellWidth * 8, cellHeight * 5.3);
	painter.drawLine(cellWidth * 1, cellHeight * 7.3, cellWidth * 8, cellHeight * 7.3);

	painter.drawLine(cellWidth * 1, cellHeight * 4.3, cellWidth * 1, cellHeight * 7.3);
	painter.drawLine(cellWidth * 3, cellHeight * 4.3, cellWidth * 3, cellHeight * 7.3);
	painter.drawLine(cellWidth * 4, cellHeight * 4.3, cellWidth * 4, cellHeight * 7.3);
	painter.drawLine(cellWidth * 5, cellHeight * 4.3, cellWidth * 5, cellHeight * 7.3);
	painter.drawLine(cellWidth * 6, cellHeight * 4.3, cellWidth * 6, cellHeight * 7.3);
	painter.drawLine(cellWidth * 7, cellHeight * 4.3, cellWidth * 7, cellHeight * 7.3);
	painter.drawLine(cellWidth * 8, cellHeight * 4.3, cellWidth * 8, cellHeight * 7.3);

	//画标题
	QFont font("宋体");
	font.setPointSize(15);
	painter.setFont(font);
	painter.drawText(cellWidth * 3.5, cellHeight * 1, "西易村福利供应站进货单");

	//画摘要
	font.setPointSize(11);
	painter.setFont(font);
	painter.drawText(cellWidth * 1, cellHeight * 3, "时间：" + QDate::currentDate().toString("yyyy-MM-dd"));
	painter.drawText(cellWidth * 3, cellHeight * 3, "供货商：" + ui->cbSupplier->currentText());
	painter.drawText(cellWidth * 6, cellHeight * 3, "操作员：" + db->getAdminNameById(MainWindow::adminId));

	//画表头
	font.setBold(true);
	painter.setFont(font);
	painter.drawText(cellWidth * 1 + hOffset, cellHeight * 5, "粮食名"); //粮食名
	painter.drawText(cellWidth * 3 + hOffset, cellHeight * 5, "单位"); //单位
	painter.drawText(cellWidth * 4 + hOffset, cellHeight * 5, "数量"); //数量
	painter.drawText(cellWidth * 5 + hOffset, cellHeight * 5, "单价"); //单价
	painter.drawText(cellWidth * 6 + hOffset, cellHeight * 5, "金额"); //金额

	//画内容
	font.setBold(false);
	painter.setFont(font);

	painter.drawText(cellWidth * 1 + hOffset, cellHeight * 6, ui->cbFoodName->currentText()); //粮食名
	painter.drawText(cellWidth * 3 + hOffset, cellHeight * 6, "斤"); //单位
	painter.drawText(cellWidth * 4 + hOffset, cellHeight * 6, QString::number(ui->sbQuantity->value())); //数量
	painter.drawText(cellWidth * 5 + hOffset, cellHeight * 6, QString::number(ui->dsbFoodPrice->value())); //单价
	double _all = ui->sbQuantity->value() * ui->dsbFoodPrice->value();
	painter.drawText(cellWidth * 6 + hOffset, cellHeight * 6, QString::number(_all)); //金额

	//合计
//	painter.drawText(cellWidth * 1, cellHeight * 6, QString("合计：%1").arg(_all));
//	painter.drawText(cellWidth * 3, cellHeight * 6, QString("大写：%1").arg(upperMoney(_all)));

	//备注
	painter.drawText(cellWidth * 1, cellHeight * 9, QString("备注：%1").arg(ui->editRemark->toPlainText()));
}

QString PurchaseDialog::upperMoney(qreal x)
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

