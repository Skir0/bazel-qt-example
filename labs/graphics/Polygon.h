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

    std::vector<QPoint> GetVertices();

    void AddVertex(const QPoint& vertex);
    void UpdateLastVertex(const QPoint& new_vertex);
    std::optional<QPoint> IntersectRay(const Ray& ray);


private:
    std::vector<QPoint> vertices_;
    QPoint last_vertex_ = QPoint(-10, -10);
};



#endif //POLYGON_H
