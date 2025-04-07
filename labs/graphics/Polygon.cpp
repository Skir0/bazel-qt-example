#include "Polygon.h"

#include <iostream>
#include <ostream>

bool Polygon::IsIntersected(QPoint p1, QPoint p2, QPoint p3, QPoint p4, QPoint& intersection) {
    const double epsilon = 1e-10;

    // calculate vectors between points
    QPoint r = p2 - p1;
    QPoint s = p4 - p3;
    QPoint qp = p3 - p1;

    // calculate cross products

    // (x2-x1)(y4-y3) - (y2-y1)(x4-x3)
    double rxs = r.x() * s.y() - r.y() * s.x();
    // (x3-x1)(y2-y1) - (y3-y1)(x2-x1)
    double qpxr = qp.x() * r.y() - qp.y() * r.x();

    // collinear case
    if (std::abs(rxs) < epsilon && std::abs(qpxr) < epsilon) {
        double t0 = (qp.x() * r.x() + qp.y() * r.y()) / (r.x() * r.x() + r.y() * r.y());
        double t1 = t0 + (s.x() * r.x() + s.y() * r.y()) / (r.x() * r.x() + r.y() * r.y());

        if (t0 > t1) std::swap(t0, t1);

        if (t0 <= 1.0 + epsilon && t1 >= 0.0 - epsilon) {
            double t = std::max(0.0, t0);
            intersection.setX(p1.x() + r.x() * t);
            intersection.setY(p1.y() + r.y() * t);
            return true;
        }
        return false;
    }

    // parallel case
    if (std::abs(rxs) < epsilon) {
        return false;
    }

    // calculate intersection parameters
    double t = (qp.x() * s.y() - qp.y() * s.x()) / rxs;
    double u = qpxr / rxs;

    // check if intersection occurs within both segments
    if (t >= -epsilon && t <= 1.0 + epsilon &&
        u >= -epsilon && u <= 1.0 + epsilon) {
        intersection.setX(p1.x() + r.x() * t);
        intersection.setY(p1.y() + r.y() * t);
        std::cout << "intersection: " << intersection.x() << " " << intersection.y() << std::endl;
        return true;
    }

    return false;
}

std::optional<QPoint> Polygon::IntersectRay(const Ray& ray) {
    std::optional<QPoint> closest_intersection = std::nullopt;
    double min_distance = std::numeric_limits<double>::max();

    QPoint ray_direction = ray.GetEnd() - ray.GetBegin();
    QPoint far_point = ray.GetBegin() + ray_direction * 1000.0;

    for (size_t i = 0; i < vertices_.size(); ++i) {
        size_t j = (i + 1) % vertices_.size();
        QPoint intersection;
        if (IsIntersected(vertices_[i], vertices_[j], ray.GetBegin(), far_point, intersection)) {
            double dist = GetDistance(intersection, ray.GetBegin());
            if (dist < min_distance) {
                min_distance = dist;
                closest_intersection = intersection;
            }
        }
    }

    return closest_intersection;
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
