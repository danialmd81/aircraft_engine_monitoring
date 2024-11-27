#ifndef MONITORING_H
#define MONITORING_H

#include "bufferprocessor.h"
#include "circulargauge.h"
#include "message.h"
//
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStandardItemModel>
#include <QThread>
#include <QVBoxLayout>

// [0] include QXlsx headers
#include "xlsxchart.h"
#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxworkbook.h"
#include "xlsxworksheet.h"

// [1] include QcGauge headers
#include "qgaugewidget.h"

namespace Ui
{
	class monitoring;
}

class monitoring : public QDialog
{
	Q_OBJECT

public:
	explicit monitoring(QWidget* parent = nullptr, QSerialPort* serial = nullptr);
	~monitoring();

	void read();
	void handleNewMessage(const message& msg);

private slots:
	void serialError(QSerialPort::SerialPortError error);

private:
	// UI Components
	Ui::monitoring* ui;

	// Serial Communication
	QSerialPort* serial;
	QByteArray* buffer;

	// Worker Thread
	QThread* workerThread;
	BufferProcessor* processor;

	// Models
	QStandardItemModel* errorModel;
	QStandardItemModel* otherModel;

	// Gauges
	CircularGauge* greenLight;
	CircularGauge* redLight;
	QList<std::pair<Data::DataType, std::pair<QcGaugeWidget*, QcNeedleItem*>>> speedGauges;
	//

	// Excel Handling
	QXlsx::Document* xlsxW;
	QXlsx::Worksheet* sheet;
	QXlsx::Format headerFormat;
	QXlsx::Format dataFormat;

	// Setup Methods
	void setupTables();
	std::pair<Data::DataType, std::pair<QcGaugeWidget*, QcNeedleItem*>> setGauge(Data::DataType type);
	void setupGauges();
	void setupLights();

	// Update Methods
	void updateTables(const message& msg);
	void updateGauges(const message& msg);
	void updateLights(const message& msg);

	// Excel Methods
	void openOrCreateExcelFile();
	void setupHeader();
	void setupDataHeader();
	void writeDataToExcel(const message& msg);
	void closeExcelFile();
	int currentRow;
};

#endif // MONITORING_H
