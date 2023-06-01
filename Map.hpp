//
// Created by Cleonia on 30.05.2023.
//

#ifndef TOWERS_MAP_HPP
#define TOWERS_MAP_HPP

#include <SDL2/SDL.h>

#include <vector>
#include <iostream>
#include <random>

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

class Map {
public:
    Map(int width, int height);
    void init();

    std::vector<rectangle> getTowers();
    line getLine();
    void mouseDown(int x, int y);

    ~Map();
private:
    using row = std::vector<unsigned int>;
    using map = std::vector<row>;

    struct point {
        double x;
        double y;
    };

    void printMap();
    void printPoint(point p);
    void calculatingTowerSize();
    unsigned int getLength();
    void fillMap();



    std::vector<point> diagonally(point p1, point p2);
    std::vector<point> squaresOnLine(point p1, point p2);

    void sortPointsForX(point *a, point *b);
    double getY(point p1, point p2, double x);
    bool visibility(point a, point b);


    map field;
    const int width, height;
    int towerWidth;
    int towerHeight;

    int mxFirst, myFirst, mxSecond, mySecond;
};


#endif //TOWERS_MAP_HPP
