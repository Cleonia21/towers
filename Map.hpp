//
// Created by Cleonia on 30.05.2023.
//

#ifndef TOWERS_MAP_HPP
#define TOWERS_MAP_HPP

#include <vector>
#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>

#include "towers.h"
#include "map/Point.hpp"
#include "map/Line.hpp"

struct rectangle {
    int y;
    int x;
    int width;
    int height;

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

class Map {
public:
    using row = std::vector<unsigned int>;
    using map = std::vector<row>;

    Map(int width, int height);
    void init();

    std::vector<rectangle> getTowers();
    Line getLine() const;
    void mouseDown(int x, int y);

    unsigned int fieldLength(Point p);

    ~Map();
private:
    struct segment {
        Point p1;
        Point p2;
    };



    void printMap();
    void calculatingTowerSize();
    static unsigned int getLength();
    void fillMap();


    static Point intersection(segment s1, segment s2);
    Point* intersectionPoints(segment primary, Point squarePoint);
    Point outputOfSegmentFromSquare(segment primary, Point squarePoint);
    std::vector<Point> squaresOnLine(Point p1, Point p2);

    double angleTG(Point p1, Point p2);
    bool calculateVisibility(Point p1, Point p2, std::vector<Point>);
    bool visibility();

    map field;
    const int width, height;
    int towerWidth;
    int towerHeight;
    Line line;
};


#endif //TOWERS_MAP_HPP
