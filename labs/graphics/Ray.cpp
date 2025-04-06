
#include "Ray.h"

Ray::Ray(const QPoint &begin, const QPoint &end, double angle) {
    begin_ = begin;
    end_ = end;
    angle_ = angle;
}

Ray Ray::Rotate(double angle) const {
    return Ray(this->begin_, this->end_, this->angle_ + angle);
}
