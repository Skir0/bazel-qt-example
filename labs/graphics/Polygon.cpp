//
// Created by Kirill Smychok on 6.04.25.
//

#include "Polygon.h"

//std::optional<QPoint> Polygon::IntersectRay(const Ray& ray) {
//    return ray.getBegin();
//}

Polygon::Polygon(const std::vector<QPoint>& vertices) {
    vertices_ = vertices;
}

std::vector<QPoint> Polygon::GetVertices() {
    return vertices_;
}

void Polygon::AddVertex(const QPoint &vertex) {
    vertices_.push_back(vertex);
}
void Polygon::UpdateLastVertex(const QPoint &new_vertex) {
    last_vertex_ = new_vertex;
}

