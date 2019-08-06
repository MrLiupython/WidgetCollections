#include "progressbar.h"
#include <QtWidgets>
#include <QDebug>

ProgressBar::ProgressBar(QWidget *parent) : QWidget(parent)
{
    pressFlag = false;
    border = QString("Circle");
    angle = 0;
    step = 2;
    totalNum = 100;
    value = 0;
    setWindowFlags(Qt::FramelessWindowHint);
    setToolTip(QString("进度表"));
}

void ProgressBar::setBorder(QString border)
{
    this->border = border;
    update();
}

void ProgressBar::setValue(qreal total, qreal value)
{
    if (total > 0)
            totalNum = total;
    if (value < totalNum)
        this->value = value;
    else
        this->value = totalNum;
    angle = 360 * this->value / totalNum;
    update();

}

void ProgressBar::setStep(qreal step) {
    this->step = step;
}

void ProgressBar::increasing(bool flag) {
    if (flag && value < totalNum)
        this->value += step;
    if (!flag && value > 0)
        value -= step;
    angle = 360 * this->value / totalNum;
    update();
}

qreal ProgressBar::getValue() const
{
    return value;
}

qreal ProgressBar::getTotal() const {
    return totalNum;
}

QString ProgressBar::getBorder() const {
    return border;
}

void ProgressBar::paintEvent(QPaintEvent* event) {

    Q_UNUSED(event);


    int side = qMin(width(), height());

    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    if (border == QString("Circle")) {
        drawCircle1(&painter);
        drawCircle2(&painter);
        drawCircle3(&painter);
        drawCircle4(&painter);
        drawCircleCenter(&painter);
        drawScale(&painter);
        drawPointer(&painter);
        drawCircleCenter2(&painter);
    } else {
        drawRect1(&painter);
        drawRect2(&painter);
        drawCircle3(&painter);
        drawCircle4(&painter);
        drawCircleCenter(&painter);
        drawScale(&painter);
        drawPointer(&painter);
        drawCircleCenter2(&painter);
    }

}

void ProgressBar::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {

        qreal n = qMin(width(), height()) / 200.0;
        QRegion region = QRegion(
                    width()/2 - 100*n,
                    height()/2- 100*n,
                    200*n,
                    200*n,
                    QRegion::Ellipse
                    );

        if (region.contains(event->pos())) {
            QLineF tmpLine(
                        QPoint(width() / 2, height() / 2),
                        event->pos()
                        );
            angle = tmpLine.angle();
            value = totalNum * angle / 360;
            emit angleChange(angle);
        } else {
            pressFlag = true;
            oldPos = event->windowPos().toPoint() - pos();
        }
        update();
    }
}

void ProgressBar::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton) {
        qreal n = qMin(width(), height()) / 200.0;
        QRegion region = QRegion(width()/2 - 100*n, height()/2- 100*n, 200*n, 200*n, QRegion::Ellipse);

        if (region.contains(event->pos())) {
            QLineF tmpLine(QPoint(width() / 2, height() / 2), event->pos());
            angle = tmpLine.angle();
            value = totalNum * angle / 360;
            emit angleChange(angle);
            pressFlag = false;
        } else {
            if (pressFlag)
                move(event->windowPos().toPoint() - oldPos);
        }
        update();
    }
}

void ProgressBar::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        pressFlag = false;
    }
}

void ProgressBar::drawCircle1(QPainter *painter)
{
    // 最外层圆
    painter->save();


    QLinearGradient linearG;
    linearG.setStart(0, -100);
    linearG.setFinalStop(0, 100);
    linearG.setColorAt(0, Qt::blue);
    linearG.setColorAt(1, Qt::red);

    painter->setBrush(linearG);
    painter->setPen(Qt::NoPen);

    painter->drawEllipse(-100, -100, 2*100, 2*100);

    painter->restore();
}

void ProgressBar::drawCircle2(QPainter *painter)
{
    // 第二层圆
    painter->save();

    QLinearGradient linearG;
    linearG.setStart(0, 5-100);
    linearG.setFinalStop(0, 100 - 5);
    linearG.setColorAt(0, Qt::lightGray);
    linearG.setColorAt(1, Qt::black);

    painter->setBrush(linearG);
    painter->setPen(Qt::NoPen);

    painter->drawEllipse(5-100, 5-100, 2*100-10, 2*100-10);

    painter->restore();
}

void ProgressBar::drawCircle3(QPainter *painter)
{
    // 第三层圆
    painter->save();

    QLinearGradient linearG;
    linearG.setStart(0, 25-100);
    linearG.setFinalStop(0, 100 - 25);
    linearG.setColorAt(0, Qt::blue);
    linearG.setColorAt(1, Qt::gray);

    painter->setBrush(linearG);
    painter->setPen(Qt::NoPen);

    painter->drawEllipse(25-100, 25-100, 2*100-50, 2*100-50);
    painter->restore();
}

void ProgressBar::drawCircle4(QPainter *painter)
{
    // 第四层圆
    painter->save();

    QLinearGradient linearG;
    linearG.setStart(0, 30-100);
    linearG.setFinalStop(0, 100 - 30);
    linearG.setColorAt(0, Qt::black);
    linearG.setColorAt(1, Qt::blue);

    painter->setBrush(linearG);
    painter->setPen(Qt::NoPen);

    painter->drawEllipse(30-100, 30-100, 2*100-60, 2*100-60);
    painter->restore();
}

void ProgressBar::drawCircleCenter(QPainter *painter)
{
    // 外圆心
    painter->save();

    QLinearGradient linearG;
    linearG.setStart(0, -12);
    linearG.setFinalStop(0, 12);
    linearG.setColorAt(0, Qt::gray);
    linearG.setColorAt(1, Qt::blue);

    painter->setBrush(linearG);
    painter->setPen(Qt::NoPen);

    painter->drawEllipse(-12, -12, 24, 24);
    painter->restore();
}

void ProgressBar::drawScale(QPainter *painter)
{
    // scale
    painter->save();
    painter->setPen(Qt::white);

    painter->drawText(-3, 15 - 100, QString("25%"));
    painter->drawText(100-15, 3, QString("0%"));
    painter->drawText(-3, 100-8, QString("75%"));
    painter->drawText(8-100, 3, QString("50%"));

    // 刻度
    painter->setPen(Qt::white);

    for (int k = 0; k < 180; k++) {
        painter->rotate(-2);
        if (2*k < angle) {
            painter->setPen(Qt::red);
            painter->drawLine(100-19, 0, 100-15, 0);
        } else {
            painter->setPen(Qt::white);
            painter->drawLine(100-15, 0, 100-15, 0);
        }
    }
    painter->restore();
}

void ProgressBar::drawPointer(QPainter *painter)
{
    // 指针
    painter->save();
    painter->rotate(-angle);
    QPainterPath path;

    path.addEllipse(80, -2.5, 5, 5);
    path.moveTo(80, 0);
    path.lineTo(0, 8);
    path.lineTo(-60, 0);
    path.lineTo(0, -8);
    path.lineTo(80, 0);

    path.lineTo(80, 0);

    QLinearGradient linearG;
    linearG.setStart(-75, 0);
    linearG.setFinalStop(85, 0);
    linearG.setColorAt(0, Qt::gray);
    linearG.setColorAt(1, Qt::red);

    painter->setBrush(linearG);
    painter->setPen(Qt::NoPen);
    painter->drawPath(path);
    painter->restore();
}

void ProgressBar::drawCircleCenter2(QPainter *painter)
{
    // 圆心
    painter->save();

    painter->setBrush(Qt::gray);
    painter->setPen(Qt::black);

    painter->drawEllipse(-10, -10, 20, 20);
    painter->restore();

    // 进度
    painter->save();
    QFont angleFont(QString("Times"), 5, Qt::AlignCenter);
    painter->setFont(angleFont);
    painter->setPen(Qt::yellow);

    QString text = QString::number(value*100/totalNum, 'f', 1) + QString("%");
    qreal x = painter->fontMetrics().width(text);
    qreal y = painter->fontMetrics().height();
    painter->drawText(QRectF(-x/2, -y/2, x, y), text);
    painter->restore();
}

void ProgressBar::drawRect1(QPainter *painter) {
    // 第一矩形
    painter->save();
//    painter->rotate(45);
    QLinearGradient linearG(-100, -100, 200, 200);
    linearG.setColorAt(0, Qt::blue);
    linearG.setColorAt(1, Qt::red);
    painter->setBrush(linearG);
    painter->setPen(Qt::NoPen);
    painter->drawRect(-100, -100, 200, 200);
    painter->restore();

}

void ProgressBar::drawRect2(QPainter *painter)
{
    // 第二矩形
    painter->save();
//    painter->rotate(45);
    QLinearGradient linearG(-95, -95, 190, 190);
    linearG.setColorAt(0, Qt::lightGray);
    linearG.setColorAt(1, Qt::black);

    painter->setBrush(linearG);
    painter->setPen(Qt::NoPen);

    painter->drawRect(-95, -95, 190, 190);
    painter->restore();
}

void ProgressBar::drawRect3(QPainter *painter)
{
    // 第三矩形
    painter->save();

    QLinearGradient linearG(-75, -75, 150, 150);
    linearG.setColorAt(0, Qt::blue);
    linearG.setColorAt(1, Qt::gray);

    painter->setBrush(linearG);
    painter->setPen(Qt::NoPen);

    painter->drawRect(-75, -75, 150, 150);
    painter->restore();
}

void ProgressBar::drawRect4(QPainter *painter)
{
    // 第四矩形
    painter->save();

    QLinearGradient linearG(-70, -70, 140, 140);
    linearG.setColorAt(0, Qt::black);
    linearG.setColorAt(1, Qt::blue);

    painter->setBrush(linearG);
    painter->setPen(Qt::NoPen);

    painter->drawRect(-70, -70, 140, 140);
    painter->restore();
}
