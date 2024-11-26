#include "mainwindow.h"
#include "monitoring.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, serial(new QSerialPort(this))
{
	ui->setupUi(this);

	ui->serialPortAddress->setText("/home/danial/5");

	for (const auto i : QSerialPortInfo::standardBaudRates())
	{
		if (i >= 9600 && i <= 1000000)
			ui->baud_rate->addItem(QString::number(i));
	}
	ui->baud_rate->setCurrentIndex(4);
}

MainWindow::~MainWindow()
{
	if (serial->isOpen())
	{
		serial->close();
	}

	delete serial;
	delete ui;
}

void MainWindow::on_start_clicked()
{
	// Configure serial port
	QString serialPortName = ui->serialPortAddress->text();
	if (!serialPortName.isEmpty())
	{
		serial->setPortName(serialPortName);

		if (serial->open(QIODevice::ReadWrite))
		{

			serial->setBaudRate(ui->baud_rate->currentText().toInt());

			QString parityText = ui->parity->currentText();
			QSerialPort::Parity parity = QSerialPort::OddParity;
			if (parityText == "None")
			{
				parity = QSerialPort::NoParity;
			}
			else if (parityText == "Even")
			{
				parity = QSerialPort::EvenParity;
			}
			else if (parityText == "Odd")
			{
				parity = QSerialPort::OddParity;
			}
			else if (parityText == "Mark")
			{
				parity = QSerialPort::MarkParity;
			}
			else if (parityText == "Space")
			{
				parity = QSerialPort::SpaceParity;
			}

			serial->setParity(parity);

			QString stopBitsText = ui->stop_bit->currentText();
			QSerialPort::StopBits stopBits = QSerialPort::OneStop;
			if (stopBitsText == "1")
			{
				stopBits = QSerialPort::OneStop;
			}
			else if (stopBitsText == "1.5")
			{
				stopBits = QSerialPort::OneAndHalfStop;
			}
			else if (stopBitsText == "2")
			{
				stopBits = QSerialPort::TwoStop;
			}

			serial->setStopBits(stopBits);

			serial->setDataBits(QSerialPort::Data8);
			serial->setFlowControl(QSerialPort::NoFlowControl);

			// Open serial port
			if (serial->isOpen())
			{
				monitoring monitoringWindow(this, serial);
				monitoringWindow.setWindowFlags(monitoringWindow.windowFlags() | Qt::WindowMinimizeButtonHint);
				monitoringWindow.setModal(true);
				monitoringWindow.showMaximized();
				monitoringWindow.exec();
			}
			else
			{
				qDebug() << "Failed to open serial port." << serial->errorString();
			}
		}
		else
		{
			qDebug() << serial->errorString();
		}
	}
}
