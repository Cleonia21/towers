//
// Created by Cleonia on 30.05.2023.
//

#include "Map.hpp"

//map* m = fillMap(width, height);
//    std::cout << "map created:" << std::endl;
//    printMap(m);
//    visibility(m, {0,5}, {7,0});

Map::Map(int width, int height)
    : width(width), height(height)
    {
        mxFirst = -1;
        mxSecond = -1;
    }

void Map::init() {
    fillMap();
    printMap();
    calculatingTowerSize();
}

void Map::mouseDown(int x, int y) {
    if (mxFirst == -1) {
        mxFirst = x;
        myFirst = y;
    } else {
        mxSecond = mxFirst;
        mySecond = myFirst;
        mxFirst = x;
        myFirst = y;
    }
    mxFirst = mxFirst / towerHeight;
    myFirst = myFirst / towerWidth;

//    std::cout << mxFirst << " " << myFirst << std::endl;

    if (mxSecond != -1) {
        visibility({double(mxFirst), double(myFirst)}, { double(mxSecond), double(mySecond)});
//        if (visibility({double(mxFirst), double(myFirst)}, { double(mxSecond), double(mySecond)}))
//            std::cout << "there is direct visibility between the towers" << std::endl;
//        else
//            std::cout << "there is no line of sight between the towers" << std::endl;
    }
}

std::vector<rectangle> Map::getTowers() {
    double redColor = 0, blueColor = 255;
    double colorStep = 255.0 / 100.0;
    std::vector<rectangle> towers(field.size() * field[0].size());

    for (int i = 0; i < field.size(); i++) {
        for (int j = 0; j < field[i].size(); j++) {
            rectangle rect = {i * towerWidth,
                              j * towerHeight,
                              towerWidth,
                              towerHeight,
                              uint8_t(redColor + field[i][j] * colorStep),
                              0,
                              uint8_t(blueColor - field[i][j] * colorStep),
                              0xFF};
            towers.push_back(rect);
        }
    }
    return towers;
}

line Map::getLine() {
    if (mxSecond == -1)
        return {0,0,0,0};

    return {towerHeight * mxFirst + towerHeight / 2,
        towerWidth * myFirst + towerWidth / 2,
        towerHeight * mxSecond + towerHeight / 2,
        towerWidth * mySecond + towerWidth / 2};
}

void Map::printMap() {
    for (int i = 0; i < field.size(); i++) {
        for (int j = 0; j < field[i].size(); j++) {
            std::cout << field[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Map::calculatingTowerSize() {
    towerWidth = SCREEN_WIDTH / width;
    towerHeight = SCREEN_HEIGHT / height;
}

unsigned int Map::getLength() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,100); // distribution in range [1, 6]
    return dist(rng);
}

void Map::fillMap() {
    for (int i = 0; i < width; i++) {
        row r;
        for (int j = 0; j < height; j++) {
            r.push_back(getLength());
        }
        field.push_back(r);
    }
}

// getY возвращает Y соответствующий X,
// принадлежащему прямой заданной двумя точками p1 и p2
double Map::getY(point p1, point p2, double x) {
    // уровнение прямой
    double y = ((x - p1.x) * (p2.y - p1.y) + p1.y) / (p2.x - p1.x);
    return y;
}

std::vector<Map::point> Map::diagonally(point p1, point p2) {
    std::vector<point> points;
    p1.x++;
    p1.y++;
    while (p1.x < p2.x) {
        points.push_back({p1.x, p1.y});
        p1.x++;
        p1.y++;
    }
    return points;
}

std::vector<Map::point> Map::squaresOnLine(point p1, point p2) {
    if (p1.x - p2.x == p1.y - p2.y) {
        return diagonally(p1, p2);
    }
    std::vector<point> points;
    // последовательно проверим все "вертикальные" стороны "квадратов"
    for (int x = p1.x + 1; x <= p2.x; x++) {
        auto y = getY(p1, p2, x);
        // если прямая пересекает левую сторону квадрата,
        // то она пересекает и соседний квадрат слева
        points.push_back({static_cast<double>(x) - 1, y});
        points.push_back({static_cast<double>(x), y});
    }
    return points;
}

void Map::printPoint(point p) {
    std::cout << "(" << p.x << " " << p.y << ")" << std::endl;
}

void Map::sortPointsForX(point *a, point *b) {
    if (a->x > b->x) {
        point buf = *a;
        *a = *b;
        *b = buf;
    } else if (a->y > b->y) {
        point buf = *a;
        *a = *b;
        *b = buf;
    }
}

/*
struct Point {
    double x;
    double y;
};

struct Segment {
    Point p1;
    Point p2;
};

Point intersection(Segment s1, Segment s2) {
    double x1 = s1.p1.x, y1 = s1.p1.y, x2 = s1.p2.x, y2 = s1.p2.y;
    double x3 = s2.p1.x, y3 = s2.p1.y, x4 = s2.p2.x, y4 = s2.p2.y;

    double denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);

    if (denom == 0) {
        return {0.0, 0.0}; // lines are parallel
    }

    double ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
    double ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;

    if (ua < 0 || ua > 1 || ub < 0 || ub > 1) {
        return {0.0, 0.0}; // intersection point is outside of either segment
    }

    return {x1 + ua * (x2 - x1), y1 + ua * (y2 - y1)};
}
 */
bool Map::visibility(point a, point b) {
    sortPointsForX(&a, &b);

    std::cout << "!!! visibility get two points: !!!" << std::endl;
    printPoint(a);
    printPoint(b);

    auto points = squaresOnLine(a, b);

    std::cout << "towers on line:" << std::endl;
    for (int i = 0; i < points.size(); i++) {
        std::cout << "(" << points[i].x << " " << points[i].y << ")"
            << " " << field[points[i].y][points[i].x] << std::endl;
    }
//
//    unsigned int minHeight = field.at(a.x).at(a.y);
//    if (minHeight > field.at(b.x).at(b.y)) {
//        minHeight = field.at(b.x).at(b.y);
//    }
//    for (auto &&p : points) {
//        std::cout << "new line" << std::endl;
//        std::cout << p.x << " " << p.y << std::endl;
//        if (field.at(p.x).at(p.y) > minHeight) {
//            std::cout << "tower len " << field.at(p.x).at(p.y) << std::endl;
//            return false;
//        }
//    }
    return true;
}
