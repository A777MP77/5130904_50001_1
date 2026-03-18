#include "include/composite_shape.h"
#include <limits>
#include <cmath>

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    if (shape) shapes_.push_back(std::move(shape));
}

double CompositeShape::getArea() const {
    double area = 0;
    for (const auto& s : shapes_) area += s->getArea();
    return area;
}

CompositeShape::BBox CompositeShape::getBoundingBox() const {
    BBox box{
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::lowest(),
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::lowest()
    };
    for (const auto& s : shapes_) {
        Point c = s->getCenter();
        double r = std::sqrt(s->getArea() / 3.14159);
        box.minX = std::min(box.minX, c.x - r);
        box.maxX = std::max(box.maxX, c.x + r);
        box.minY = std::min(box.minY, c.y - r);
        box.maxY = std::max(box.maxY, c.y + r);
    }
    return box;
}

Point CompositeShape::getCenter() const {
    if (shapes_.empty()) return Point();
    auto box = getBoundingBox();
    return Point((box.minX + box.maxX) / 2.0, (box.minY + box.maxY) / 2.0);
}

void CompositeShape::move(double dx, double dy) {
    for (auto& s : shapes_) s->move(dx, dy);
}

void CompositeShape::scale(double k) {
    if (k <= 0 || shapes_.empty()) return;
    Point center = getCenter();
    for (auto& s : shapes_) {
        Point oldCenter = s->getCenter();
        s->scale(k);
        Point newCenter = center + (oldCenter - center) * k;
        s->move(newCenter.x - s->getCenter().x, newCenter.y - s->getCenter().y);
    }
}

std::unique_ptr<Shape> CompositeShape::clone() const {
    auto comp = std::make_unique<CompositeShape>();
    for (const auto& s : shapes_) comp->addShape(s->clone());
    return comp;
}

void CompositeShape::print(std::ostream& os) const {
    os << std::fixed << std::setprecision(2);
    os << "[COMPOSITE," << getCenter() << ", " << getArea() << ":";
    for (size_t i = 0; i < shapes_.size(); ++i) {
        if (i > 0) os << ",";
        os << " ";
        shapes_[i]->print(os);
    }
    os << " ]";
}
