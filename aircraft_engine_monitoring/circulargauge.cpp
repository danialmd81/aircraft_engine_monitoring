#include "circulargauge.h"
#include <QConicalGradient>
#include <QPainter>

CircularGauge::CircularGauge(QWidget* parent)
	: QWidget(parent)
	, mValue(0)
	, mMin(0)
	, mMax(100)
	, mBackgroundColor(Qt::white)
{
	setMinimumSize(150, 150);
}

void CircularGauge::setValue(int value)
{
	if (value < mMin)
		mValue = mMin;
	else if (value > mMax)
		mValue = mMax;
	else
		mValue = value;

	update();
}

void CircularGauge::setRange(int min, int max)
{
	mMin = min;
	mMax = max;
	update();
}

void CircularGauge::setBackgroundColor(const QColor& color)
{
	mBackgroundColor = color;
	update();
}

void CircularGauge::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	int side = qMin(width(), height());
	painter.setViewport((width() - side) / 2, (height() - side) / 2, side, side);
	painter.setWindow(-50, -50, 100, 100);

	// Draw background
	painter.setBrush(mBackgroundColor);
	painter.setPen(Qt::NoPen);
	painter.drawEllipse(-50, -50, 100, 100);

	// Determine color based on value
	QColor gaugeColor;
	if (mValue < (mMin + mMax) / 3)
	{
		gaugeColor = Qt::green;
	}
	else if (mValue < 2 * (mMin + mMax) / 3)
	{
		gaugeColor = Qt::yellow;
	}
	else
	{
		gaugeColor = Qt::red;
	}

	// Draw gauge
	painter.setBrush(gaugeColor);
	painter.setPen(Qt::NoPen);

	// Correct the calculation for the pie drawing
	int spanAngle = static_cast<int>((mValue - mMin) * 360.0 / (mMax - mMin) * 16);
	painter.drawPie(-50, -50, 100, 100, 90 * 16, -spanAngle);
}