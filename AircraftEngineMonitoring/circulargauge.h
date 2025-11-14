#ifndef CIRCULARGAUGE_H
#define CIRCULARGAUGE_H

#include <QColor>
#include <QWidget>

class CircularGauge : public QWidget
{
	Q_OBJECT

public:
	explicit CircularGauge(QWidget* parent = nullptr);
	void setValue(int value);
	void setRange(int min, int max);
	void setBackgroundColor(const QColor& color); // New method to set background color

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	int mValue;
	int mMin;
	int mMax;
	QColor mBackgroundColor; // Member to store background color
};

#endif // CIRCULARGAUGE_H
