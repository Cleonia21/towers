//
// Created by Cleonia on 05.06.2023.
//

#ifndef TOWERS_POINT_HPP
#define TOWERS_POINT_HPP

#include <iostream>

struct Point {
    double x;
    double y;

    Point();
    Point(int x, int y);
    Point(double x, double y);
    void centering();
    void round();
    void round(Point);
    void print() const;

    friend bool operator==(const Point &p1, const Point &p2);
    friend bool operator!=(const Point &p1, const Point &p2);
    friend Point operator+=(Point &p1, Point &p2);
};


#endif //TOWERS_POINT_HPP
