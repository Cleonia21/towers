//
// Created by Cleonia on 30.05.2023.
//

#ifndef TOWERS_MAP_HPP
#define TOWERS_MAP_HPP

#include <SDL2/SDL.h>

#include <vector>
#include <iostream>
#include <random>
#include <cmath>

#include "towers.h"

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

struct line {
    int x1;
    int y1;
    int x2;
    int y2;
};

struct point {
    double x;
    double y;

    void round();
    point round(point);
    void print() const;
};

class Map {
public:
    using row = std::vector<unsigned int>;
    using map = std::vector<row>;

    Map(int width, int height);
    void init();

    std::vector<rectangle> getTowers();
    line getLine() const;
    void mouseDown(int x, int y);

    unsigned int fieldLength(point p);

    ~Map();
private:
    struct segment {
        point p1;
        point p2;
    };

    void printMap();
    void calculatingTowerSize();
    static unsigned int getLength();
    void fillMap();


    static point intersection(segment s1, segment s2);
    point* intersectionPoints(segment primary, point squarePoint);
    point outputOfSegmentFromSquare(segment primary, point squarePoint);
    static void centering(point *a, point *b);
    std::vector<point> squaresOnLine(point p1, point p2);

    double angleTG(point p1, point p2);
    bool calculateVisibility(point p1, point p2, std::vector<point>);
    bool visibility(point a, point b);


    map field;
    const int width, height;
    int towerWidth;
    int towerHeight;

    int mxFirst, myFirst, mxSecond, mySecond;
};


#endif //TOWERS_MAP_HPP
