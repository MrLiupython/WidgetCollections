#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QWidget>

class ProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressBar(QWidget *parent = 0);
    void setBorder(QString);
    void setValue(qreal, qreal);
    void setStep(qreal);
    void increasing(bool);
    qreal getValue() const;
    qreal getTotal() const;
    QString getBorder() const;
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
signals:
    void angleChange(int);

public slots:
private:
    int angle;
    qreal totalNum;
    qreal step;
    qreal value;
    QPoint oldPos;
    QString border;
    bool pressFlag;

    void drawCircle1(QPainter* painter);
    void drawCircle2(QPainter* painter);
    void drawCircle3(QPainter* painter);
    void drawCircle4(QPainter* painter);

    void drawRect1(QPainter* painter);
    void drawRect2(QPainter* painter);
    void drawRect3(QPainter* painter);
    void drawRect4(QPainter* painter);

    void drawCircleCenter(QPainter* painter);
    void drawScale(QPainter* painter);
    void drawPointer(QPainter* painter);
    void drawCircleCenter2(QPainter* painter);
};

#endif // PROGRESSBAR_H
