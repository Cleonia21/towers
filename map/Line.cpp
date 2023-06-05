//
// Created by Cleonia on 05.06.2023.
//

#include "Line.hpp"

bool Line::empty() {
    if (this->second.x < 0)
        return true;
    return false;
}

bool Line::hasIntersection() {
    if (this->intersectionPoint == Point{-1, -1})
        return false;
    return true;
}

void Line::removeIntersection() {
    this->intersectionPoint = {-1,-1};
    this->intersectionTower = {-1,-1};
    this->intersectionTowerLen = 0;
}