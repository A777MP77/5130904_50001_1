#include "Rectangle.hpp"

Rectangle::Rectangle(Point bl, Point tr) {
    bottomLeft_ = bl;
    topRight_ = tr;
}

double Rectangle::getArea() const {
    double width = topRight_.x - bottomLeft_.x;
    double height = topRight_.y - bottomLeft_.y;
    return width * height;
}

Point Rectangle::getCenter() const {
    Point center;
    center.x = (bottomLeft_.x + topRight_.x) / 2.0;
    center.y = (bottomLeft_.y + topRight_.y) / 2.0;
    return center;
}

void Rectangle::move(double dx, double dy) {
    bottomLeft_.x += dx;
    bottomLeft_.y += dy;
    topRight_.x += dx;
    topRight_.y += dy;
}

void Rectangle::scale(double factor) {
    Point c = getCenter();
    bottomLeft_.x = c.x + (bottomLeft_.x - c.x) * factor;
    bottomLeft_.y = c.y + (bottomLeft_.y - c.y) * factor;
    topRight_.x = c.x + (topRight_.x - c.x) * factor;
    topRight_.y = c.y + (topRight_.y - c.y) * factor;
}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}
