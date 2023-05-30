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

class Map {
public:
    Map(int width, int height);
    void init(SDL_Renderer* _render);

    void draw();
    void mouseDown(int x, int y);

    ~Map();
private:
    using row = std::vector<unsigned int>;
    using map = std::vector<row>;

    struct point {
        double x;
        double y;
    };

    void drawLine();

    void printMap();
    void calculatingTowerSize();
    unsigned int getHeight();
    void fillMap();
    double getY(point p1, point p2, double x);
    std::vector<point> setOfSquares(point p1, point p2);
    bool visibility(point a, point b);


    map field;
    const int width, height;
    int towerWidth;
    int towerHeight;

    SDL_Renderer* render;

    int mxFirst, myFirst, mxSecond, mySecond;
};


#endif //TOWERS_MAP_HPP
