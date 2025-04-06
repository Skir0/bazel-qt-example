//
// Created by Kirill Smychok on 6.04.25.
//

#include "Polygon.h"

bool Polygon::IsIntersected(QPoint p1, QPoint p2, QPoint p3, QPoint p4, QPoint &intersection) {
    double n;
    // to escape a division by zero
    if (p2.y() - p1.y() != 0) {
        // convert "line by 2 points" to "general equation of line" and check intersection
        float q = (p2.x() - p1.x()) / (p1.y() - p2.y());
        float sn = (p3.x() - p4.x()) + (p3.y() - p4.rx()) * q;
        if (!sn) {
            return 0;
        }
        float fn = (p3.x() - p1.x()) + (p3.y() - p1.y()) * q;
        n = fn / sn;
    } else {
        if (!(p3.y() - p4.y())) {
            return 0;
        }
        n = (p3.y() - p1.y()) / (p3.y() - p4.y());
    }
    intersection = QPoint(p3.x() + (p4.x() - p3.x()) * n, p3.y() + (p4.y() - p3.y()) * n);
    return 1;
}

// TODO check
std::optional<QPoint> Polygon::IntersectRay(const Ray &ray) {
    std::optional<QPoint> intersection_vertex = std::nullopt;
    double min_distance = std::numeric_limits<double>::max();

    for (int i = 1; i < vertices_.size(); ++i) {
        QPoint intersection;
        if (IsIntersected(vertices_[i - 1], vertices_[i], ray.GetBegin(), ray.GetEnd(), intersection)) {
            double current_distance = GetDistance(intersection, ray.GetBegin());
            if (current_distance < min_distance) {
                min_distance = current_distance;
                intersection_vertex = intersection;
            }
        }
    }
    return intersection_vertex;
}

Polygon::Polygon(const std::vector<QPoint> &vertices) {
    vertices_ = vertices;
}

std::vector<QPoint> Polygon::GetVertices() {
    return vertices_;
}

void Polygon::AddVertex(const QPoint &vertex) {
    vertices_.push_back(vertex);
}

void Polygon::UpdateLastVertex(const QPoint &new_vertex) {
    auto &last_vertex = vertices_.back();
    last_vertex = new_vertex;
}

bool Polygon::IsEmpty() {
    return vertices_.empty();
}

void Polygon::Clear() {
    vertices_.clear();
}

double Polygon::GetDistance(const QPoint &p1, const QPoint &p2) {
    return qSqrt(qPow(p1.x() - p2.x(), 2) + qPow(p1.y() - p2.y(), 2));
}
