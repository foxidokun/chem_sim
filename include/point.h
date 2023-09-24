#pragma once

#include "vector.h"

class Point {
public:
    double x;
    double y;
    double z;

    Point() = default;

    Point(double x, double y, double z):
        x(x),
        y(y),
        z(z)
        {};

    Point& operator+=(const Vector& vec);

    Point& operator-=(const Vector& vec);
};

static inline Point operator+(const Point& point, const Vector& vec) {
    return { point.x + vec.x, point.y + vec.y, point.z + vec.z };
}

static inline Point operator+(const Vector& vec, const Point& point) {
    return point + vec;
}

static inline Point operator-(const Point& point, const Vector& vec) {
    return { point.x - vec.x, point.y - vec.y, point.z - vec.z };
}

static inline Point operator-(const Vector& vec, const Point& point) {
    return point - vec;
}

static inline Vector operator-(const Point& end, const Point& start) {
    return Vector(end.x - start.x, end.y - start.y, end.z - start.z);
}

inline Point& Point::operator+=(const Vector& vec) {
    *this = *this + vec;
    return *this;
}

inline Point& Point::operator-=(const Vector& vec) {
    *this = *this - vec;
    return *this;
}