//
// Created by Cleonia on 30.05.2023.
//

#include "Map.hpp"

Map::Map(int width, int height)
    : width(width), height(height)
    {
        line = Line{{-1,-1},{-1,-1},
                    {-1,-1},{-1,-1}, 0};
    }

Map::~Map() {}

void Map::init() {
    fillMap();
    printMap();
    calculatingTowerSize();
}

unsigned int Map::fieldLength(Point p) {
    return field[int(p.y)][int(p.x)];
}

Line Map::getLine() const {
    return line;

//    if (mxSecond == -1)
//        return {0,0,0,0};
//
//    return {towerHeight * mxFirst + towerHeight / 2,
//            towerWidth * myFirst + towerWidth / 2,
//            towerHeight * mxSecond + towerHeight / 2,
//            towerWidth * mySecond + towerWidth / 2};
}

std::vector<rectangle> Map::getTowers() {
    double redColor = 0, blueColor = 255;
    double colorStep = 255.0 / 100.0;
    std::vector<rectangle> towers(field.size() * field[0].size());

    for (int i = 0; i < int(field.size()); i++) {
        for (int j = 0; j < int(field[i].size()); j++) {
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

void Map::printMap() {
    for (auto & i : field) {
        for (unsigned int j : i) {
            std::cout << j << " ";
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

Point Map::intersection(segment s1, segment s2) {
//    std::cout << "intersection get segments\n";
//    s1.p1.print();
//    s1.p2.print();
//    std::cout << "---\n";
//    s2.p1.print();
//    s2.p2.print();
//    std::cout << "---------------=========\n";
    double x1 = s1.p1.x, y1 = s1.p1.y, x2 = s1.p2.x, y2 = s1.p2.y;
    double x3 = s2.p1.x, y3 = s2.p1.y, x4 = s2.p2.x, y4 = s2.p2.y;

    double denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);

    if (denom == 0) {
        return {-1, -1}; // lines are parallel
    }

    double ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
    double ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;

    if (ua < 0 || ua > 1 || ub < 0 || ub > 1) {
        return {-1, -1}; // intersection Point is outside either segment
    }

    return {x1 + ua * (x2 - x1), y1 + ua * (y2 - y1)};
}

Point* Map::intersectionPoints(segment primary, Point squarePoint) {
    auto* points = new Point[4];

    if (primary.p2.x - primary.p1.x < 0)
        squarePoint.x -= 0.00001;
    else
        squarePoint.x += 0.00001;
    if (primary.p2.y - primary.p1.y < 0)
        squarePoint.y -= 0.00001;
    else
        squarePoint.y += 0.00001;

    squarePoint.round();
    Point left_up = squarePoint;
    Point left_low = squarePoint;
    Point right_up = squarePoint;
    Point right_low = squarePoint;

    left_low.y++;
    right_up.x++;
    right_low.y++;
    right_low.x++;

    points[0] = intersection(primary, {left_up, left_low});
    points[1] = intersection(primary, {left_up, right_up});
    points[2] = intersection(primary, {right_up, right_low});
    points[3] = intersection(primary, {left_low, right_low});
    return points;
}

Point Map::outputOfSegmentFromSquare(segment primary, Point squarePoint) {
    auto* points = intersectionPoints(primary, squarePoint);
//    std::cout << "square intersectioning in these points\n";
    for (int i = 0; i < 4; i++) {
//        points[i].print();
        if (points[i] != Point{-1, -1} && (points[i].x != squarePoint.x || points[i].y != squarePoint.y)) {
//            points[i].print();
            return points[i];
        }
    }
    return Point{-1, -1};
}

std::vector<Point> Map::squaresOnLine(Point p1, Point p2) {
    std::vector<Point> points;
    segment primeSegment = {p1, p2};

    while (true) {
        // в какой точке отрезок выходит из квадрата
        p1 = outputOfSegmentFromSquare(primeSegment, p1);
        /* print
        std::cout << "outputOfSegmentFromSquare SET:" << std::endl;
        p.print();
        break;
        */
        // если мы дошли до квадрата в точке p2, то отрезок закончился, выходим из цикла
        if (p1 == Point{-1, -1})
            return points;
        // в этой же точке отрезок входит в следующий квадрат, сохраняем точку
        points.push_back(p1);
    }
    return points;
}

// тангенс угла между двумя точками
double Map::angleTG(Point p1, Point p2) {
    // прилежащий / противолежащий катет
    double adjacentCathet, oppositeCathet;
    oppositeCathet = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    adjacentCathet = abs(double(fieldLength(p2)) - double(fieldLength(p1)));


    std::cout << "-------\nfrom\n";
    p1.print();
    std::cout << "in\n";
    p2.print();
    std::cout << "prilezashi katet: " << adjacentCathet << std::endl;
    std::cout << "protivolezashi katet: " << oppositeCathet << std::endl;
    std::cout << "-------\n";


    return oppositeCathet / adjacentCathet;
}

bool Map::calculateVisibility(Point p1, Point p2, std::vector<Point> points) {
    if (points.empty())
        return true;

    if (fieldLength(p1) < fieldLength(p2)) {
        Point buf = p1;
        p1 = p2;
        p2 = buf;
        std::reverse(points.begin(), points.end());
    }
    points.erase(points.begin());

    Point displacementCoefficient = Point{0.00001, 0.00001};
    if (p1.x - p2.x < 0)
        displacementCoefficient.x = -0.00001;
    if (p1.y - p2.y < 0)
        displacementCoefficient.y = -0.00001;

    double baseTG = angleTG(p1,p2);
    for (auto p : points) {
        p += displacementCoefficient;
        if (fieldLength(p) > fieldLength(p1) || angleTG(p1, p) > baseTG) {
            line.intersectionPoint = p;
            line.intersectionTower.round(p);
            line.intersectionTowerLen = fieldLength(p);
            return false;
        }
    }
    return true;
}

void Map::mouseDown(int x, int y) {
    if (line.first != Point{-1, -1}) {
        line.second = line.first;
    }
    line.first = Point{x / towerHeight, y / towerWidth};
    line.removeIntersection();
    if (line.second != Point{-1, -1}) {
        visibility();
    }
}

bool Map::visibility() {

    // получены координаты левой верхней стороны двух квадратов(которые символизируют башню)
    // приведем эти координаты к нужному нам формату переместив
    // координаты к центру квадратов
    line.first.centering();
    line.second.centering();

    // получим все точки, в которых линия пересекает квадраты(башни на линии взгляда)
    auto points = squaresOnLine(line.first, line.second);

    return calculateVisibility(line.first, line.second, points);
}
