#include "bufferprocessor.h"
#include <QDebug>

BufferProcessor::BufferProcessor(QByteArray* buffer, QObject* parent)
	: QObject(parent)
	, buffer(buffer)
	, headerDetected(false)
	, processing(false)
{
}

void BufferProcessor::start()
{
	processing = true;
	while (processing)
	{
		processBuffer();
	}
}

void BufferProcessor::stop()
{
	processing = false;
}

void BufferProcessor::processBuffer()
{
	// Check if the buffer has at least 2 characters to process
	if (buffer->size() >= 2)
	{
		// Append the first 2 characters from the buffer to the messageBuffer
		messageBuffer.append(buffer->first(2));
		// Remove the first 2 characters from the buffer
		buffer->remove(0, 2);

		// If the header has not been detected and messageBuffer has at least 12 characters
		if (!headerDetected && messageBuffer.size() >= 12)
		{
			// Check if the messageBuffer starts with the header "A5A5A5A5"
			if (messageBuffer.startsWith("A5A5A5A5"))
			{
				headerDetected = true; // Header detected
				// Extract the message size from the messageBuffer and convert it from hex to int
				messageSize = messageBuffer.mid(10, 2).toInt(nullptr, 16);
				// Calculate the total message size in hex characters
				messageSize = 18 + (messageSize * 20);
			}
			else
			{
				// If header is not detected, remove the first 2 characters from messageBuffer
				messageBuffer.remove(0, 2);
			}
		}
		// If the header has been detected
		else if (headerDetected)
		{
			// Check if the messageBuffer has reached the expected message size
			if (messageBuffer.size() >= messageSize)
			{
				// Check if the message ends with "55"
				if (messageBuffer.at(messageSize - 2) == '5' && messageBuffer.at(messageSize - 1) == '5')
				{
					message msg;
					// Parse the hex message into a message object
					msg.parseHexMessage(messageBuffer);

					// Verify the checksum of the message
					if (msg.checksum(messageBuffer) == msg.getChecksum())
					{
						int msgCounter = msg.getMsgCounter();
						// Check if the message counter has not been received before
						if (!receivedMsgCounters.contains(msgCounter))
						{
							// Insert the message counter into the set of received message counters
							receivedMsgCounters.insert(msgCounter);
							// Emit signals based on the number of received message counters
							if (receivedMsgCounters.size() == 256)
							{
								emit newMessage(msg);
								qDebug() << "Message counter full";
								receivedMsgCounters.clear();
							}
							else
								emit newMessage(msg);
						}
						else
						{
							qDebug() << "Message with counter" << msgCounter << "already received";
						}
					}
					else
					{
						qDebug() << "Checksum error";
					}
					// Clear the messageBuffer and reset header detection for the next message
					messageBuffer.clear();
					headerDetected = false;
				}
				// for the case when the message has fake header
				messageBuffer.remove(0, 2);
			}
		}
	}
}

void BufferProcessor::showMessage(const message& msg)
{
	qDebug() << "Header:" << msg.getHeader();
	qDebug() << "Message counter:" << msg.getMsgCounter();
	qDebug() << "ID Number:" << msg.getIdNumber();

	for (const Data& data : msg.getDataList())
	{
		qDebug() << "Data type:" << data.getType().ID;
		qDebug() << data.getType().name << "Value:" << data.getType().value;
	}
	qDebug() << "Checksum:" << msg.getChecksum();
	qDebug() << "Footer:" << msg.getFooter();
}
