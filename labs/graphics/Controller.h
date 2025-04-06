
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <qmainwindow.h>
#include "Polygon.h"
#include "PaintWidget.h"


QT_BEGIN_NAMESPACE
class QComboBox;
class QPixmap;

QT_END_NAMESPACE




class Controller: public QMainWindow {

    enum Mode: uint8_t {
        LIGHT,
        POLYGONS
    };
public:

    Controller();


    ~Controller();

    QPoint GetLightSource();
    void SetLightSource(QPoint lightSource);

    const std::vector<Polygon>& GetPolygons();

    void AddPolygon(const Polygon&);

    void AddVertexToLastPolygon(const QPoint& new_vertex);

    void UpdateLastPolygon(const QPoint& new_vertex);

    std::vector<Ray> CastRays();

    void IntersectRays(std::vector<Ray>* rays);

    void RemoveAdjacentRays(std::vector<Ray>* rays);

    Polygon CreateLightArea();

    QPoint last_vertex = QPoint(-10, -10);



private slots:
    void ModeChanged();

    void MouseMoveEvent(QPoint pos);
    void MouseLeftClicked(QPoint pos);
    void MouseRightClicked(QPoint pos);

private:
    QPoint light_source_;
    Mode mode_;
    std::vector<QPoint> current_polygon_;
    std::vector<Polygon> polygons_;
    QPoint first_vertex_;

    // widgets
    QComboBox *mode_box_;
    PaintWidget *paint_widget_;

    void CreateActions();

};
#endif //CONTROLLER_H
