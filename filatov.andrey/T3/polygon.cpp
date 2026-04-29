#include "polygon.h"
#include <cmath>
#include <algorithm>

bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y;
}

bool Polygon::operator==(const Polygon& other) const {
    if (points.size() != other.points.size()) return false;
    for (size_t i = 0; i < points.size(); ++i) {
        if (points[i].x != other.points[i].x || points[i].y != other.points[i].y) {
            return false;
        }
    }
    return true;
}

double Polygon::getArea() const {
    if (points.size() < 3) return 0.0;

    double sum = 0.0;
    for (size_t i = 0; i < points.size(); ++i) {
        size_t j = (i + 1) % points.size();
        sum += points[i].x * points[j].y - points[j].x * points[i].y;
    }
    return std::abs(sum) / 2.0;
}

int Polygon::getVertexCount() const {
    return points.size();
}

bool Polygon::isPermutationOf(const Polygon& other) const {
    if (points.size() != other.points.size()) return false;

    std::vector<Point> p1 = points;
    std::vector<Point> p2 = other.points;

    std::sort(p1.begin(), p1.end(), [](const Point& a, const Point& b) {
        return a.x != b.x ? a.x < b.x : a.y < b.y;
    });
    std::sort(p2.begin(), p2.end(), [](const Point& a, const Point& b) {
        return a.x != b.x ? a.x < b.x : a.y < b.y;
    });

    return p1 == p2;
}

std::istream& operator>>(std::istream& is, Point& p) {
    char ch1, ch2, ch3;
    if (!(is >> ch1 >> p.x >> ch2 >> p.y >> ch3)) {
        is.setstate(std::ios::failbit);
        return is;
    }
    if (ch1 != '(' || ch2 != ';' || ch3 != ')') {
        is.setstate(std::ios::failbit);
    }
    return is;
}

std::istream& operator>>(std::istream& is, Polygon& poly) {
    int n;
    if (!(is >> n) || n < 3) {
        is.setstate(std::ios::failbit);
        return is;
    }

    poly.points.clear();
    for (int i = 0; i < n; ++i) {
        Point pt;
        if (!(is >> pt)) {
            is.setstate(std::ios::failbit);
            return is;
        }
        poly.points.push_back(pt);
    }
    return is;
}
