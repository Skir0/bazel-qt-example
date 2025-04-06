//
// Created by Kirill Smychok on 6.04.25.
//

#ifndef RAY_H
#define RAY_H
#include <QtCore/qpoint.h>


class Ray {
public:
    Ray(const QPoint& begin, const QPoint& end, double angle);

    QPoint GetBegin() const;
    QPoint GetEnd() const;
    double GetAngle() const;

    void SetBegin(const QPoint& begin);
    void SetEnd(const QPoint& end);
    void SetAngle(double angle);

    Ray Rotate(double angle) const;

private:
    QPoint begin_;
    QPoint end_;
    double angle_;
};



#endif //RAY_H
