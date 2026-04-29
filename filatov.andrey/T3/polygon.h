#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <iostream>

struct Point {
    int x, y;
};

struct Polygon {
    std::vector<Point> points;

    double getArea() const;
    int getVertexCount() const;
    bool operator==(const Polygon& other) const;
    bool isPermutationOf(const Polygon& other) const;
};

std::istream& operator>>(std::istream& is, Point& p);
std::istream& operator>>(std::istream& is, Polygon& poly);

#endif
