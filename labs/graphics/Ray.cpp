
#include "Ray.h"

Ray::Ray(const QPoint &begin, const QPoint &end, double angle) {
    begin_ = begin;
    end_ = end;
    angle_ = angle;
}

QPoint Ray::GetBegin() const {
    return begin_;
}
QPoint Ray::GetEnd() const {
    return end_;
}
double Ray::GetAngle() const {
    return angle_;
}

void Ray::SetBegin(const QPoint &begin) {
    begin_ = begin;
}
void Ray::SetEnd(const QPoint &end) {
    end_ = end;
}
void Ray::SetAngle(double angle) {
    angle_ = angle;
}

Ray Ray::Rotate(double angle) const {
    return Ray(this->begin_, this->end_, this->angle_ + angle);
}
