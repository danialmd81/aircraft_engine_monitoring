#include "monitoring.h"
#include "ui_monitoring.h"

monitoring::monitoring(QWidget* parent, QSerialPort* serial)
	: QDialog(parent)
	, ui(new Ui::monitoring)
	, serial(serial)
	, buffer(new QByteArray)
	, workerThread(new QThread(this))
	, processor(new BufferProcessor(buffer))
{
	ui->setupUi(this);

	openOrCreateExcelFile();

	connect(serial, &QSerialPort::readyRead, this, &monitoring::read);
	connect(serial, &QSerialPort::errorOccurred, this, &monitoring::serialError);

	setupTables();
	setupGauges();
	setupLights();

	processor->moveToThread(workerThread);
	connect(processor, &BufferProcessor::newMessage, this, &monitoring::handleNewMessage);
	connect(processor, &BufferProcessor::msgCounterFull, this, &monitoring::msgCounterFull);

	// Ensure the processor's start method is called when the thread starts
	connect(workerThread, &QThread::started, processor, &BufferProcessor::start);

	workerThread->start();
}

monitoring::~monitoring()
{
	processor->stop();
	workerThread->quit();
	workerThread->wait();
	delete workerThread;
	delete ui;
	closeExcelFile();
	serial->close();
}

void monitoring::read()
{
	if (serial->isOpen())
	{
		QByteArray data = serial->readAll();
		if (!data.isEmpty())
		{
			buffer->append(data.toHex().toUpper());
		}
	}
	else
	{
		qDebug() << "Serial port is not open";
	}
}

void monitoring::handleNewMessage(const message& msg)
{
	updateTables(msg);
	updateGauges(msg);
	updateLights(msg);
	writeDataToExcel(msg);
}

void monitoring::msgCounterFull()
{
	qDebug() << "Message counter full";
	processor->clear();
}

void monitoring::serialError(QSerialPort::SerialPortError error)
{
	if (error == QSerialPort::SerialPortError::ResourceError)
	{
		qDebug() << "Serial port error: " << serial->errorString();
	}
}

void monitoring::setupTables()
{
	ui->tableWidget->setColumnCount(5);
	QStringList headers = { "Name", "Value", "  ", "Name", "Value" };
	ui->tableWidget->setHorizontalHeaderLabels(headers);
	ui->tableWidget->setAlternatingRowColors(true);

	// Set resize mode for each column
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);

	// Set specific column widths
	ui->tableWidget->setColumnWidth(1, 75); // Set width for the second column
	ui->tableWidget->setColumnWidth(4, 75); // Set width for the fifth column
}

void monitoring::updateTables(const message& msg)
{
	// Clear the table contents
	ui->tableWidget->clearContents();
	ui->tableWidget->setRowCount(0); // Reset the row count

	int errorRow = 0; // Row counter for error data
	int otherRow = 0; // Row counter for other data

	for (const Data& data : msg.getDataList())
	{
		// Create items for the data
		QTableWidgetItem* nameItem = new QTableWidgetItem(data.getType().name);
		QTableWidgetItem* valueItem = new QTableWidgetItem(QString::number(data.getType().value));
		valueItem->setTextAlignment(Qt::AlignCenter);
		valueItem->setBackground(data.getType().error ? Qt::red : Qt::green);
		valueItem->setForeground(Qt::black);

		// Check if the data has an error
		if (data.getType().ID >= 0x11 && data.getType().ID <= 0x1F) // error data
		{
			// Insert a new row if necessary
			if (errorRow >= ui->tableWidget->rowCount())
			{
				ui->tableWidget->insertRow(ui->tableWidget->rowCount());
			}

			ui->tableWidget->setItem(errorRow, 0, nameItem);
			ui->tableWidget->setItem(errorRow, 1, valueItem);
			errorRow++;
		}
		else if (data.getType().ID > 0x00 && data.getType().ID < 0x10) // other data
		{
			// Insert a new row if necessary
			if (otherRow >= ui->tableWidget->rowCount())
			{
				ui->tableWidget->insertRow(ui->tableWidget->rowCount());
			}
			ui->tableWidget->setItem(otherRow, 3, nameItem);
			ui->tableWidget->setItem(otherRow, 4, valueItem);
			otherRow++;
		}
		else
		{
			delete nameItem;
			delete valueItem;
			// qDebug() << "Unknown data type:" << data.getType().ID;
		}
	}
}

std::pair<Data::DataType, std::pair<QcGaugeWidget*, QcNeedleItem*>> monitoring::setGauge(Data::DataType type)
{
	QcGaugeWidget* mSpeedGauge;
	QcNeedleItem* mSpeedNeedle;
	mSpeedGauge = new QcGaugeWidget;
	mSpeedGauge->addBackground(99);
	// this will add side color to gauge
	QcBackgroundItem* bkg1 = mSpeedGauge->addBackground(92);
	bkg1->clearrColors();
	bkg1->addColor(0.1, Qt::black);
	bkg1->addColor(1.0, Qt::white);
	QcBackgroundItem* bkg2 = mSpeedGauge->addBackground(88);
	bkg2->clearrColors();
	// this two line below will set background color of gauge
	bkg2->addColor(0.1, Qt::gray);
	bkg2->addColor(1.0, Qt::darkGray);
	//
	mSpeedGauge->addArc(55);
	mSpeedGauge->addDegrees(65)->setValueRange(type.minValue, type.maxValue);
	mSpeedGauge->addColorBand(50);

	// add value range
	mSpeedGauge->addValues(80)->setValueRange(type.minValue, type.maxValue);
	// set gauge title
	mSpeedGauge->addLabel(70)->setText(type.name);
	QcLabelItem* lab = mSpeedGauge->addLabel(40);
	lab->setText("0");
	mSpeedNeedle = mSpeedGauge->addNeedle(60);
	// add lable
	mSpeedNeedle->setLabel(lab);
	// set needle color
	mSpeedNeedle->setColor(Qt::black);
	// add range for your gauge => it should be just like the gauge range
	mSpeedNeedle->setValueRange(type.minValue, type.maxValue);
	mSpeedGauge->addBackground(7);
	// add some shadow like glass
	mSpeedGauge->addGlass(88);
	//
	return std::make_pair(type, std::make_pair(mSpeedGauge, mSpeedNeedle));
}

void monitoring::setupGauges()
{
	// Create a gauge for each data type
	speedGauges.append(setGauge(Data::DataType(0x01)));
	speedGauges.append(setGauge(Data::DataType(0x02)));
	speedGauges.append(setGauge(Data::DataType(0x04)));
	speedGauges.append(setGauge(Data::DataType(0x06)));
	speedGauges.append(setGauge(Data::DataType(0x0B)));

	// Use a layout to add the gauge to the dialog
	ui->oilPressure->addWidget(speedGauges[0].second.first);
	ui->oilTemperature->addWidget(speedGauges[1].second.first);
	ui->fuel->addWidget(speedGauges[2].second.first);
	ui->torque->addWidget(speedGauges[3].second.first);
	ui->motorSpeed->addWidget(speedGauges[4].second.first);
}

void monitoring::updateGauges(const message& msg)
{
	for (const auto& gauge : speedGauges)
	{
		gauge.second.second->setCurrentValue(0);
	}

	for (const Data& data : msg.getDataList())
	{
		for (const auto& gauge : speedGauges)
		{
			if (data.getType().ID == gauge.first.ID)
			{
				gauge.second.second->setCurrentValue(data.getType().value);
			}
		}
	}
}

void monitoring::setupLights()
{
	redLight = new CircularGauge(this);
	redLight->setBackgroundColor(Qt::white);
	greenLight = new CircularGauge(this);
	greenLight->setBackgroundColor(Qt::white);

	ui->redLight->addWidget(redLight);
	ui->greenLight->addWidget(greenLight);
}

void monitoring::updateLights(const message& msg)
{
	bool hasError = false;

	for (const Data& data : msg.getDataList())
	{
		if (data.getType().error)
		{
			hasError = true;
			break;
		}
	}

	if (hasError)
	{
		redLight->setBackgroundColor(Qt::red);
		greenLight->setBackgroundColor(Qt::white);
	}
	else
	{
		redLight->setBackgroundColor(Qt::white);
		greenLight->setBackgroundColor(Qt::green);
	}
}

void monitoring::openOrCreateExcelFile()
{
	// Open an existing file or create a new one
	QString path = QCoreApplication::applicationDirPath() + "/DATA.xlsx";
	qDebug() << path;
	xlsxW = new (QXlsx::Document)(path);
	if (xlsxW->load()) // file already exists
	{
		int sheetCount = xlsxW->sheetNames().size();
		QString newSheetName = QString("Session %1").arg(sheetCount + 1);
		xlsxW->addSheet(newSheetName);
	}
	else // file does not exist
	{
		xlsxW->addSheet("Session 1");
	}
	xlsxW->save();

	setupHeader();
	setupDataHeader();
}

void monitoring::setupHeader()
{
	// set header format
	headerFormat.setPatternBackgroundColor(QColor(Qt::green));
	headerFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	headerFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
	headerFormat.setFontBold(true);
	headerFormat.setFontColor(QColor(Qt::black));
	headerFormat.setFontSize(16);
	// Write the header row
	int col = 1;
	QStringList headers = { "Header", "MsgCounter", "IdNumber", "Checksum", "Footer" };
	for (const QString& header : headers)
	{
		xlsxW->write(1, col++, header, headerFormat);
	}

	currentRow = 2;
}

void monitoring::setupDataHeader()
{
	// set data format
	dataFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
	dataFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
	dataFormat.setFontColor(QColor(Qt::black));
	dataFormat.setFontSize(12);
	// Write the data header row
	int col = 6;
	QStringList dataHeaders = { "ID", "Name", "Value", "Error" };
	for (int i = 0; i < 128; i++)
	{
		for (const QString& header : dataHeaders)
		{
			xlsxW->write(1, col++, header, headerFormat);
		}
	}
}

void monitoring::writeDataToExcel(const message& msg)
{
	if (!xlsxW)
	{
		qDebug() << "No active workbook to write data.";
		return;
	}

	// Write the message data to the new row
	xlsxW->write(currentRow, 1, msg.getHeader(), dataFormat);
	xlsxW->write(currentRow, 2, msg.getMsgCounter(), dataFormat);
	xlsxW->write(currentRow, 3, msg.getIdNumber(), dataFormat);
	xlsxW->write(currentRow, 4, msg.getChecksum(), dataFormat);
	xlsxW->write(currentRow, 5, msg.getFooter(), dataFormat);

	int col = 6;
	for (const Data& data : msg.getDataList())
	{
		xlsxW->write(currentRow, col++, data.getType().ID, dataFormat);
		xlsxW->write(currentRow, col++, data.getType().name, dataFormat);
		xlsxW->write(currentRow, col++, data.getType().value, dataFormat);
		xlsxW->write(currentRow, col++, data.getType().error, dataFormat);
	}
	currentRow++;
}

void monitoring::closeExcelFile()
{
	xlsxW->setColumnWidth(QXlsx::CellRange(1, 1, currentRow, 517), 40);
	// Save and close the workbook
	if (xlsxW)
	{
		xlsxW->save();
		delete xlsxW;
		xlsxW = nullptr;
	}
	else
	{
		qDebug() << "No active workbook to close.";
	}
}