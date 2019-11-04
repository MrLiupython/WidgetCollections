#pragma once

#include <QWidget>
#include <QString>

class FormBar : public QWidget {
	Q_OBJECT
public:
	FormBar(QWidget* parent = nullptr);

public:
	void SetRange(int min, int max);
	void SetCurrent(int n);
	void SetColor(QString colors);

protected:
	void paintEvent(QPaintEvent* ) override;
	void mousePressEvent(QMouseEvent* ) override;

signals:
	void OnChanged(int);
private:
	int m_min;
	int m_max;
	int m_cur;
	QColor bgColor;
	QColor color1;
	QColor color2;

};
