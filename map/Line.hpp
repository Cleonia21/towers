//
// Created by Cleonia on 05.06.2023.
//

#ifndef TOWERS_LINE_HPP
#define TOWERS_LINE_HPP

#include <iostream>
#include "Point.hpp"

struct Line {
    Point first;
    Point second;
    Point intersectionPoint;
    Point intersectionTower;
    unsigned int intersectionTowerLen;

    bool empty();
    bool hasIntersection();
    void removeIntersection();
};

#endif //TOWERS_LINE_HPP
