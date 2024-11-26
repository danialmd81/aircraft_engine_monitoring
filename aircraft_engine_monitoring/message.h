#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QVector>

// Class to represent data
class Data
{
public:
	struct DataType
	{
		int ID;
		QString name;
		int minValue;
		int maxValue;
		int value;
		bool error;
		DataType(int ID);
		DataType(int ID, int DATA, int FACTOR);
	};

private:
	DataType type;
	int RESERVE;
	int DATA;
	int FACTOR;

public:
	Data(int ID, int Reserve, int Data, int Factor)
		: type(ID, Data, Factor)
		, RESERVE(Reserve)
		, DATA(Data)
		, FACTOR(Factor)
	{
	}
	int getReserve() const { return RESERVE; }
	int getData() const { return DATA; }
	int getFactor() const { return FACTOR; }
	DataType getType() const { return type; }
};

// Class to represent a message
class message
{
private:
	QString header;
	int msgCounter;
	int idNumber;
	QVector<Data> dataList;
	QString checksumStr;
	QString footer;

public:
	message() {};
	void parseHexMessage(const QByteArray& hexMessage);
	// Getters
	QString getHeader() const { return header; }
	int getMsgCounter() const { return msgCounter; }
	int getIdNumber() const { return idNumber; }
	QVector<Data> getDataList() const { return dataList; }
	QString getChecksum() const { return checksumStr; }
	QString getFooter() const { return footer; }

	QString checksum(const QByteArray& hexMessage);
};

#endif // MESSAGE_H
