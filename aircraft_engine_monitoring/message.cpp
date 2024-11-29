#include "message.h"
#include <QDebug>

Data::DataType::DataType(int ID)
	: ID(ID)
{
	switch (ID)
	{
	case 0x01:
		name = "OIL_PRESSURE";
		minValue = 0;
		maxValue = 1000;
		break;
	case 0x02:
		name = "OIL_TEMPERATURE";
		minValue = 0;
		maxValue = 400;
		break;
	case 0x03:
		name = "FUEL_FLOW";
		minValue = 0;
		maxValue = 800;
		break;
	case 0x04:
		name = "FUEL";
		minValue = 0;
		maxValue = 800;
		break;
	case 0x05:
		name = "EGT";
		minValue = 0;
		maxValue = 400;
		break;
	case 0x06:
		name = "TORQUE";
		minValue = 0;
		maxValue = 400;
		break;
	case 0x07:
		name = "INDICATED_POWER";
		minValue = 0;
		maxValue = 400;
		break;
	case 0x08:
		name = "FRICTIONAL_POWER";
		minValue = 0;
		maxValue = 400;
		break;
	case 0x09:
		name = "THERMAL_EFFICIENCY";
		minValue = 0;
		maxValue = 100;
		break;
	case 0x0A:
		name = "AIR_FUEL_RATIO";
		minValue = 0;
		maxValue = 20;
		break;
	case 0x0B:
		name = "MOTOR_SPEED";
		minValue = 0;
		maxValue = 1000;
		break;
	case 0x0C:
		name = "OUTPUT_AIR_SPEED";
		minValue = 0;
		maxValue = 1000;
		break;
	case 0x0D:
		name = "VIBRATION";
		minValue = 0;
		maxValue = 100;
		break;
	case 0x0E:
		name = "BODY_TEMP";
		minValue = 0;
		maxValue = 400;
		break;
	case 0x0F:
		name = "AIR_TEMP";
		minValue = 0;
		maxValue = 400;
		break;
	case 0x11:
		name = "OIL_PRESSURE_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x12:
		name = "OIL_TEMPERATURE_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x13:
		name = "FUEL_FLOW_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x14:
		name = "FUEL_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x15:
		name = "EGT_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x16:
		name = "TORQUE_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x17:
		name = "INDICATED_POWER_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x18:
		name = "FRICTIONAL_POWER_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x19:
		name = "THERMAL_EFFICIENCY_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x1A:
		name = "AIR_FUEL_RATIO_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x1B:
		name = "MOTOR_SPEED_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x1C:
		name = "OUTPUT_AIR_SPEED_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x1D:
		name = "VIBRATION_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x1E:
		name = "BODY_TEMP_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x1F:
		name = "AIR_TEMP_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	default:
		name = "UNKNOWN";
		minValue = 0;
		maxValue = 0;
		break;
	}
	value = 0;
	error = false;
}

Data::DataType::DataType(int ID, unsigned int DATA, unsigned int FACTOR)
	: ID(ID)
{

	switch (ID)
	{
	case 0x01:
		name = "OIL_PRESSURE";
		minValue = 0;
		maxValue = 1000;
		break;
	case 0x02:
		name = "OIL_TEMPERATURE";
		minValue = 0;
		maxValue = 400;
		break;
	case 0x03:
		name = "FUEL_FLOW";
		minValue = 0;
		maxValue = 800;
		break;
	case 0x04:
		name = "FUEL";
		minValue = 0;
		maxValue = 800;
		break;
	case 0x05:
		name = "EGT";
		minValue = 0;
		maxValue = 400;
		break;
	case 0x06:
		name = "TORQUE";
		minValue = 0;
		maxValue = 400;
		break;
	case 0x07:
		name = "INDICATED_POWER";
		minValue = 0;
		maxValue = 400;
		break;
	case 0x08:
		name = "FRICTIONAL_POWER";
		minValue = 0;
		maxValue = 400;
		break;
	case 0x09:
		name = "THERMAL_EFFICIENCY";
		minValue = 0;
		maxValue = 100;
		break;
	case 0x0A:
		name = "AIR_FUEL_RATIO";
		minValue = 0;
		maxValue = 20;
		break;
	case 0x0B:
		name = "MOTOR_SPEED";
		minValue = 0;
		maxValue = 1000;
		break;
	case 0x0C:
		name = "OUTPUT_AIR_SPEED";
		minValue = 0;
		maxValue = 1000;
		break;
	case 0x0D:
		name = "VIBRATION";
		minValue = 0;
		maxValue = 100;
		break;
	case 0x0E:
		name = "BODY_TEMP";
		minValue = 0;
		maxValue = 400;
		break;
	case 0x0F:
		name = "AIR_TEMP";
		minValue = 0;
		maxValue = 400;
		break;
	case 0x11:
		name = "OIL_PRESSURE_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x12:
		name = "OIL_TEMPERATURE_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x13:
		name = "FUEL_FLOW_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x14:
		name = "FUEL_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x15:
		name = "EGT_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x16:
		name = "TORQUE_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x17:
		name = "INDICATED_POWER_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x18:
		name = "FRICTIONAL_POWER_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x19:
		name = "THERMAL_EFFICIENCY_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x1A:
		name = "AIR_FUEL_RATIO_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x1B:
		name = "MOTOR_SPEED_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x1C:
		name = "OUTPUT_AIR_SPEED_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x1D:
		name = "VIBRATION_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x1E:
		name = "BODY_TEMP_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	case 0x1F:
		name = "AIR_TEMP_SENSOR_ERROR";
		minValue = 0;
		maxValue = 0;
		break;
	default:
		name = "UNKNOWN";
		minValue = 0;
		maxValue = 0;
		value = 0;
		break;
	}

	if (FACTOR == 0)
	{
		value = DATA;
	}
	else
		value = float(DATA) / FACTOR;

	if (value < minValue || value > maxValue)
		error = true;
	else
		error = false;
}

void message::parseHexMessage(const QByteArray& hexMessage)
{
	header = hexMessage.mid(0, 8).toUpper(); // 4 bytes for header 4*2=8
	msgCounter = hexMessage.mid(8, 2).toInt(nullptr, 16); // 1 byte for message counter 1*2=2
	idNumber = hexMessage.mid(10, 2).toInt(nullptr, 16); // 1 byte for ID number 1*2=2

	auto littleEndian = [](QByteArray message) -> QByteArray
	{
		char byte1[2] = { message[0], message[1] };
		char byte2[2] = { message[2], message[3] };
		message[0] = message[6];
		message[1] = message[7];
		message[2] = message[4];
		message[3] = message[5];
		message[4] = byte2[0];
		message[5] = byte2[1];
		message[6] = byte1[0];
		message[7] = byte1[1];
		return message;
	};

	for (int i = 0; i < idNumber; i++)
	{
		int idIndex = 12 + (i * 20); // 10 bytes for each data 10*2=20
		int id = hexMessage.mid(idIndex, 2).toInt(nullptr, 16); // 1 byte for ID 1*2=2
		int reserve = hexMessage.mid(idIndex + 2, 2).toInt(nullptr, 16); // 1 byte for reserve 1*2=2
		unsigned int dataValue = littleEndian(hexMessage.mid(idIndex + 4, 8)).toUInt(nullptr, 16); // 4 bytes for data 4*2=8
		unsigned int factorValue = littleEndian(hexMessage.mid(idIndex + 12, 8)).toUInt(nullptr, 16); // 4 bytes for factor 4*2=8
		dataList.push_back(Data(id, reserve, dataValue, factorValue));
	}

	int checksumIndex = 32 + (idNumber - 1) * 20; // 32=16*2
	checksumStr = hexMessage.mid(checksumIndex, 4); // 2 bytes for checksum 2*2=4
	footer = hexMessage.mid(checksumIndex + 4, 2); // 1 byte for footer 1*2=2
}

QString message::checksum(const QByteArray& hexMessage)
{
	int checksumValue = 0;

	// Iterate through the hexMessage in steps of 2 characters
	for (int i = 8; i < hexMessage.size() - 6; i += 2)
	{
		// Convert the two characters to an integer
		int value = hexMessage.mid(i, 2).toInt(nullptr, 16);
		checksumValue += value;
	}

	// Convert the checksum value to a hexadecimal string
	QString check = QString::number(checksumValue, 16).toUpper();

	// Ensure the checksum is 4 characters long (2 bytes)
	while (check.length() < 4)
	{
		check.prepend('0');
	}
	QChar byte[2] = { check[0], check[1] };
	check[0] = check[2];
	check[1] = check[3];
	check[2] = byte[0];
	check[3] = byte[1];
	// qDebug() << check;
	return check;
}
