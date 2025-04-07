#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QMouseEvent>

#include "Polygon.h"

class PaintWidget : public QWidget {
    Q_OBJECT

public:
    explicit PaintWidget(QWidget *parent = nullptr);

    void PaintPolygon(Polygon polygon);

    void ClearRaysBuffer();

    void PaintRays(std::vector<Ray> rays);



signals:
    void mouseMoved(QPoint pos); // Changed to QPoint
    void rightClicked(QPoint pos);
    void leftClicked(QPoint pos);

private:
    void paintEvent(QPaintEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    // buffers
    QPixmap polygons_buffer_;  // Stores all drawings
    QPixmap rays_buffer_;  // Stores all drawings

    void initializeBuffer();

};

#endif // PAINTWIDGET_H
