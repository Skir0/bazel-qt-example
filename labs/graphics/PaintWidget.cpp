#include "PaintWidget.h"

#include <iostream>
#include <qstatusbar.h>
#include <QtGui/qpainter.h>

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);  // Enable mouse move events without button press
    initializeBuffer();
}

void PaintWidget::initializeBuffer() {
    // TODO fix size
    m_buffer_ = QPixmap(QSize(800, 600));
    m_buffer_.fill(Qt::white);  // Clear with background color
}
void PaintWidget::SetVertex(const QPoint &vertex, bool isLastVertex) {
    // Draw to buffer first
    QPainter bufferPainter(&m_buffer_);
    bufferPainter.setPen(Qt::black);
    bufferPainter.setBrush(Qt::black);
    if (last_vertex_ != QPoint(-10, -10)) {
        bufferPainter.drawLine(last_vertex_.rx(), last_vertex_.ry(), vertex.x(), vertex.y());
    }
    // bufferPainter.drawEllipse(point, 3, 3);
    bufferPainter.end();
    update();  // Trigger widget repaint

    if (!isLastVertex) {
        last_vertex_ = vertex;
    }
    else {
        last_vertex_ = QPoint(-10, -10);
    }
}

void PaintWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    // draw the buffer content
    painter.drawPixmap(0, 0, m_buffer_);

    // draw temporary overlays
    painter.setPen(Qt::blue);
    painter.drawEllipse(last_vertex_, 5, 5);

}

void PaintWidget::mouseMoveEvent(QMouseEvent *event) {
    emit mouseMoved(event->pos());  // Emit the position
    QWidget::mouseMoveEvent(event);
}

void PaintWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(event->pos());
    }
    else if (event->button() == Qt::RightButton) {
        emit rightClicked(event->pos());
    }
    QWidget::mousePressEvent(event);
}
