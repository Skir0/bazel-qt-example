#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QMouseEvent>

class PaintWidget : public QWidget {
    Q_OBJECT

public:
    explicit PaintWidget(QWidget *parent = nullptr);

    void SetPoint(const QPoint &point, bool isLastPoint);


signals:
    void mouseMoved(QPoint pos); // Changed to QPoint
    void rightClicked(QPoint pos);
    void leftClicked(QPoint pos);

private:
    void paintEvent(QPaintEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    // stores the point position
    QPoint last_point_ = QPoint(-10, -10);

    QPixmap m_buffer_;  // Stores all drawings
    void initializeBuffer();


};

#endif // PAINTWIDGET_H
