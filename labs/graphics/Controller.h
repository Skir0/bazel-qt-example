
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

    QPointF GetLightSource();
    void SetLightSource(QPointF lightSource);

    const std::vector<Polygon>& GetPolygons();

    void AddPolygon(const Polygon&);

    void AddVertexToLastPolygon(const QPointF& new_vertex);

    void UpdateLastPolygon(const QPointF& new_vertex);

    std::vector<Ray> CastRays();

    void IntersectRays(std::vector<Ray>* rays);

    void RemoveAdjacentRays(std::vector<Ray>* rays);

    Polygon CreateLightArea();

    Ray FindNeighbourRay(QPointF vertex, double angle);






private slots:
    void ModeChanged();

    void MouseMoveEvent(QPointF pos);
    void MouseLeftClicked(QPointF pos);
    void MouseRightClicked(QPointF pos);

    double AngleBetweenPoints(const QPointF &p1, const QPointF &p2);

    bool CompareAngles(double angle1, double angle2);


private:
    QPointF light_source_;
    Mode mode_;
    bool is_right_clicked = true;

    // widgets
    QComboBox *mode_box_;
    PaintWidget *paint_widget_;
    std::vector<Polygon> polygons_;


    void CreateActions();


};
#endif //CONTROLLER_H
