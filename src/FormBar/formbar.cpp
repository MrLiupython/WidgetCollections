#include "formbar.h"
#include <QPainter>
#include <QMouseEvent>

FormBar::FormBar(QWidget* parent)
      m_min(0),m_max(10),m_cur(0),bgColor(Qt::black),
      color1(Qt::red), color2(Qt::yellow)
{

}

void FormBar::SetRangle(int min, int max)
{
    if (min < 0 || min > max)
        return;
    m_min = min;
    m_max = max;
    update();
}

void FormBar::SetCurrent(int current)
{
    if (current < m_min || current > m_max)
        return;
    m_cur = current;
    emit OnChanged(m_cur);
    update();
}

void FormBar::SetColor(QString colors)
{
    if (colors.isEmpty() || colors.split(",").size() != 3)
        return;

    QStringList cs = colors.split(",");
    bgColor = QColor(cs[0]);
    color1 = QColor(cs[1]);
    color2 = QColor(cs[2]);
    update();
}

void FormBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int w = width();
    int h = height();
    int n = m_max - m_min;
    int cn = m_cur - m_min;
    double ratio = w / n;

    painter.setBrush(bgColor);
    painter.drawRoundRect(0, 0, w, h, 3, 3);

    double x, y, w2, h2;
    x = 0;
    y = 2;
    w2 = ratio - 4.0;
    h2 = h - 4.0;

    for (int i = 0; i < n; i++) {
        if (i < cn) {
            painter.setBrush(color1);
        } else {
            painter.setBrush(color2);
        }
        painter.drawRect(x + 2.0, y, w2, h2);
        x += ratio;
    }
}

void FormBar::mousePressEvent(QMouseEvent * event)
{
    int w = width();
    int n = m_max - m_min;
    double ratio = w / n;

    m_cur = m_min + ceil(event->pos().x() / ratio);
    if (m_cur > m_max)
        m_cur = m_max;
    else if (m_cur < m_min)
        m_cur = m_min;
    emit OnChanged(m_cur);
    update();
}


