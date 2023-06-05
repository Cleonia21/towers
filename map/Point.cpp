//
// Created by Cleonia on 05.06.2023.
//

#include "Point.hpp"

Point::Point() {
    this->x = -1;
    this->y = -1;
}

Point::Point(double x, double y)
        : x(x), y(y)
{}

Point::Point(int x, int y) {
    this->x = double(x);
    this->y = double(y);
}

void Point::round() {
    this->x = int(this->x);
    this->y = int(this->y);
}

void Point::round(Point p) {
    this->x = int(p.x);
    this->y = int(p.y);
}

void Point::print() const {
    std::cout << "(" << this->x << " " << this->y << ")" << std::endl;
}

void Point::centering() {
    int buf = int(this->x);
    this->x = buf + 0.5;
    buf = int(this->y);
    this->y = buf + 0.5;
}

bool operator==(const Point &p1, const Point &p2)
{
    return (int(p1.x) == int(p2.x) && int(p1.y) == int(p2.y));
}

bool operator!=(const Point &p1, const Point &p2)
{
    return !(int(p1.x) == int(p2.x) && int(p1.y) == int(p2.y));
}

Point operator+=(Point &p1, Point &p2) {
    p1.x += p2.x;
    p1.y += p2.y;
    return p1;
}
