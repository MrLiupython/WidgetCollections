#include "messagebar.h"
#include <QDebug>

MessageBar::MessageBar(QWidget *parent)
    : QWidget(parent),
      timerId(0),
      roundArea(1),
      border(Rect),
      roundPoint(0, 0),
      text(tr("MessageBar")),
      backgroundColor(Qt::white),
      textColor(),
      textFont()
{
    currentRect = rect();
    setAutoFillBackground(true);
    setWindowFlags(Qt::FramelessWindowHint);
}

void MessageBar::SetText(QString text) {
    this->text = text;
}

void MessageBar::SetFont(QString family, int size, int weight) {
    textFont = QFont(family, size, weight);
}

void MessageBar::SetBackgroundColor(QColor color) {
    backgroundColor = color;
}

void MessageBar::SetForegroundColor(QColor color) {
    textColor = color;
}

void MessageBar::SetBorder(borderType type) {
    border = type;
}

void MessageBar::SetRoundPoint(qreal x, qreal y)
{
    roundPoint = QPoint(x, y);

    if (x > width()/2) {
        if (y > height()/2)
            roundArea = 4;
        else
            roundArea = 2;
    } else {
        if (y > height()/2)
            roundArea = 3;
        else
            roundArea = 1;
    }
}

void MessageBar::SetRoundPoint(QString round) {
    int x, y;
    x = (round.contains(tr("right"), Qt::CaseInsensitive))?width():0;
    y = (round.contains(tr("bottom"), Qt::CaseInsensitive))?height():0;
    roundPoint = QPoint(x, y);

    if (x > width()/2) {
        if (y > height()/2)
            roundArea = 4;
        else
            roundArea = 2;
    } else {
        if (y > height()/2)
            roundArea = 3;
        else
            roundArea = 1;
    }
}

void MessageBar::Start(int num) {
    currentRect = QRect(0, 0, 0, 0);
    timerId = startTimer(num);
}

void MessageBar::mousePressEvent(QMouseEvent *event) {
    emit clicked();
    if (timerId) {
        killTimer(timerId);
        timerId = 0;
        currentRect = rect();
    }
    update();
    QWidget::mousePressEvent(event);
}

void MessageBar::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if (currentRect.width() >= rect().width()) {
        killTimer(timerId);
        timerId = 0;
    } else {
        qreal w = currentRect.width() + width()/5;
        qreal h = currentRect.height() + height()/5;
        qreal x = 0, y = 0;
        switch (roundArea) {
        case 1:
            x = y = 0;
            break;
        case 2:
            x = width() - w;
            y = 0;
            break;
        case 3:
            x = 0;
            y = height() - h;
            break;
        case 4:
            x = width() - w;
            y = height() - h;
        }

        currentRect = QRect(x, y, w, h);
        update();
    }

}

void MessageBar::resizeEvent(QResizeEvent *event)
{
    if (!timerId) {
        currentRect = rect();
    }
    QWidget::resizeEvent(event);
}

void MessageBar::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    DrawBackground_(&painter);
    // 文本
    DrawText_(&painter);
}


void MessageBar::DrawBackground_(QPainter* painter) {
    if (currentRect == QRect(0, 0, 0, 0))
        return;

    painter->save();

    QPainterPath path;
    switch (border) {
    case RoundedRect:
        path.addRoundedRect(
                    currentRect,
                    qMin(currentRect.width(), currentRect.height())/3.0,
                    qMin(currentRect.width(), currentRect.height())/3.0
                    );
        break;
    case Ellipse:
        path.addEllipse(currentRect);
        break;
    default:
        path.addRect(currentRect);
        break;
    }

    QPainterPath path2;
    path2.moveTo(roundPoint);
    path2.lineTo(
                currentRect.x() + currentRect.width()/2,
                currentRect.y() + currentRect.height()/4
                );
    path2.lineTo(
                currentRect.x() + currentRect.width()/2,
                currentRect.y() + currentRect.height()*3/4
                );
    path2.lineTo(roundPoint);


    painter->setBrush(QBrush(backgroundColor));
    painter->drawPath(path.united(path2));

    painter->restore();
}

void MessageBar::DrawText_(QPainter * painter) {
    painter->save();

    painter->setPen(textColor);

    QFont tmp = QFont(textFont);
    int pSize = textFont.pointSize() * currentRect.width() / width();

    if (pSize) {
        tmp.setPointSize(pSize);
        painter->setFont(tmp);

        QTextOption opt(Qt::AlignCenter);
        opt.setWrapMode(QTextOption::WordWrap);
        painter->drawText(currentRect, text, opt);
    }

    painter->restore();
}
