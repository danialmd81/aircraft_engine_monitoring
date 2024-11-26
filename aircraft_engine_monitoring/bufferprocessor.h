#ifndef BUFFERPROCESSOR_H
#define BUFFERPROCESSOR_H

#include "message.h"
#include <QByteArray>
#include <QObject>
#include <QSet>

class BufferProcessor : public QObject
{
	Q_OBJECT
public:
	explicit BufferProcessor(QByteArray* buffer, QObject* parent = nullptr);
	void start();
	void clear();
	void stop();

signals:
	void newMessage(const message& msg);
	void msgCounterFull();

private:
	QByteArray* buffer;
	QByteArray messageBuffer;
	int messageSize;
	bool headerDetected;
	bool processing;

	QSet<int> receivedMsgCounters;

	void processBuffer();
	void showMessage(const message& message);
};

#endif // BUFFERPROCESSOR_H
