#include "CompositeShade.hpp"

CompositeShape::CompositeShape() {
    shapes_ = new Shape*[10];
    count_ = 0;
}

CompositeShape::~CompositeShape() {
    for (int i = 0; i < count_; i++) {
        delete shapes_[i];
    }
    delete[] shapes_;
}

void CompositeShape::addShape(Shape* shape) {
    shapes_[count_] = shape;
    count_++;
}

int CompositeShape::getCount() const {
    return count_;
}

Shape* CompositeShape::getShape(int index) const {
    return shapes_[index];
}

double CompositeShape::getArea() const {
    double total = 0;
    for (int i = 0; i < count_; i++) {
        total += shapes_[i]->getArea();
    }
    return total;
}

Point CompositeShape::getCenter() const {
    double minX = shapes_[0]->getCenter().x;
    double minY = shapes_[0]->getCenter().y;
    double maxX = shapes_[0]->getCenter().x;
    double maxY = shapes_[0]->getCenter().y;
    for (int i = 1; i < count_; i++) {
        Point c = shapes_[i]->getCenter();
        if (c.x < minX) minX = c.x;
        if (c.y < minY) minY = c.y;
        if (c.x > maxX) maxX = c.x;
        if (c.y > maxY) maxY = c.y;
    }
    Point center;
    center.x = (minX + maxX) / 2.0;
    center.y = (minY + maxY) / 2.0;
    return center;
}

void CompositeShape::move(double dx, double dy) {
    for (int i = 0; i < count_; i++) {
        shapes_[i]->move(dx, dy);
    }
}

void CompositeShape::scale(double factor) {
    Point c = getCenter();
    for (int i = 0; i < count_; i++) {
        Point sc = shapes_[i]->getCenter();
        double newCx = c.x + (sc.x - c.x) * factor;
        double newCy = c.y + (sc.y - c.y) * factor;
        shapes_[i]->move(newCx - sc.x, newCy - sc.y);
        shapes_[i]->scale(factor);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}
