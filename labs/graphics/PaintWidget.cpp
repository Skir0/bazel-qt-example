#include "PaintWidget.h"

#include <iostream>
#include <qstatusbar.h>
#include <QtGui/qpainter.h>
#include <QtGui/qpainterpath.h>

#include "Controller.h"

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true); // Enable mouse move events without button press
    initializeBuffer();
}

void PaintWidget::initializeBuffer() {
    // TODO fix size
    polygons_buffer_ = QPixmap(QSize(800, 600));
    polygons_buffer_.fill(Qt::white); // Clear with background color
    rays_buffer_ = QPixmap(size());  // Initialize rays_buffer_
    rays_buffer_.fill(Qt::transparent);
}


void PaintWidget::PaintPolygon(Polygon polygon) {
    QPainter bufferPainter(&polygons_buffer_);

    QPen pen(Qt::black);
    pen.setWidth(2);
    bufferPainter.setPen(pen);
    bufferPainter.setBrush(Qt::black);

    std::vector<QPointF> vertices = polygon.GetVertices();

    if (vertices.size() < 2) {
        return;

    }
    // Create a path to define the polygon
    QPainterPath path;
    path.moveTo(vertices[0]);

    for (size_t i = 1; i < vertices.size(); ++i) {
        path.lineTo(vertices[i]);
    }

    path.closeSubpath();

    bufferPainter.drawPath(path);

    update();
}

void PaintWidget::ClearRaysBuffer() {
    rays_buffer_.fill(Qt::transparent);  // Clear previous rays
}
void PaintWidget::PaintRays(std::vector<Ray> rays) {
    if (rays_buffer_.isNull() || rays_buffer_.size() != size()) {
        rays_buffer_ = QPixmap(size());  // Recreate if invalid or wrong size
    }
    ClearRaysBuffer();
    QPainter bufferPainter(&rays_buffer_);
    if (!bufferPainter.isActive()) {
        qWarning() << "Failed to begin painting on rays_buffer_";
        return;
    }

    bufferPainter.setPen(Qt::red);  // Make rays visible (e.g., red)
    for (const auto& ray : rays) {
        bufferPainter.drawLine(ray.GetBegin(), ray.GetEnd());
    }
    update();
}


void PaintWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    // draw the buffer content
    painter.drawPixmap(0, 0, polygons_buffer_);

    painter.drawPixmap(0, 0, rays_buffer_);

}

void PaintWidget::mouseMoveEvent(QMouseEvent *event) {
    emit mouseMoved(event->pos()); // Emit the position
    QWidget::mouseMoveEvent(event);
}

void PaintWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(event->pos());
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(event->pos());
    }
    QWidget::mousePressEvent(event);
}
