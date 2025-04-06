//
// Created by Kirill Smychok on 6.04.25.
//

#ifndef POLYGON_H
#define POLYGON_H
#include <vector>
#include <QtCore/qmetatype.h>
#include <QtCore/qpoint.h>

#include "Ray.h"


class Polygon {

public:
    Polygon(const std::vector<QPoint>& vertices);
    Polygon(): vertices_(std::vector<QPoint>()) {}

    std::vector<QPoint> GetVertices();

    void AddVertex(const QPoint& vertex);
    void UpdateLastVertex(const QPoint& new_vertex);
    std::optional<QPoint> IntersectRay(const Ray& ray);
    bool IsEmpty();
    void Clear();

    static double GetDistance(const QPoint &p1, const QPoint &p2);

    bool IsIntersected(QPoint p1, QPoint p2, QPoint p3, QPoint p4, QPoint &intersection);

private:
    std::vector<QPoint> vertices_;
    QPoint last_vertex_ = QPoint(-10, -10);
};



#endif //POLYGON_H
