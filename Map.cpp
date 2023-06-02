//
// Created by Cleonia on 30.05.2023.
//

#include "Map.hpp"

void point::round() {
    this->x = int(this->x);
    this->y = int(this->y);
}

point point::round(point p) {
    x = int(p.x);
    y = int(p.y);
    return {x, y};
}

void point::print() const {
    std::cout << "(" << this->x << " " << this->y << ")" << std::endl;
}

bool operator==(const point &p1, const point &p2)
{
    return (int(p1.x) == int(p2.x) && int(p1.y) == int(p2.y));
}

bool operator!=(const point &p1, const point &p2)
{
    return !(int(p1.x) == int(p2.x) && int(p1.y) == int(p2.y));
}

point operator+=(point &p1, point &p2) {
    p1.x += p2.x;
    p1.y += p2.y;
    return p1;
}

/*
 * Cat operator+(Cat b) {
    return Cat(this->value + b.value);
}
 */



Map::Map(int width, int height)
    : width(width), height(height)
    {
        mxFirst = -1;
        mxSecond = -1;
    }

Map::~Map() {}

void Map::init() {
    fillMap();
    printMap();
    calculatingTowerSize();
}

unsigned int Map::fieldLength(point p) {
    return field[int(p.y)][int(p.x)];
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

    if (mxSecond != -1) {
        if (visibility({double(mxFirst), double(myFirst)}, { double(mxSecond), double(mySecond)}))
            std::cout << "visibility true" << std::endl;
//        else
//            std::cout << "visibility false" << std::endl;
    }
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

line Map::getLine() const {
    if (mxSecond == -1)
        return {0,0,0,0};

    return {towerHeight * mxFirst + towerHeight / 2,
        towerWidth * myFirst + towerWidth / 2,
        towerHeight * mxSecond + towerHeight / 2,
        towerWidth * mySecond + towerWidth / 2};
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

point Map::intersection(segment s1, segment s2) {
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
        return {-1, -1}; // intersection point is outside either segment
    }

    return {x1 + ua * (x2 - x1), y1 + ua * (y2 - y1)};
}

point* Map::intersectionPoints(segment primary, point squarePoint) {
    auto* points = new point[4];

    if (primary.p2.x - primary.p1.x < 0)
        squarePoint.x -= 0.00001;
    else
        squarePoint.x += 0.00001;
    if (primary.p2.y - primary.p1.y < 0)
        squarePoint.y -= 0.00001;
    else
        squarePoint.y += 0.00001;

    squarePoint.round();
    point left_up = squarePoint;
    point left_low = squarePoint;
    point right_up = squarePoint;
    point right_low = squarePoint;

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

point Map::outputOfSegmentFromSquare(segment primary, point squarePoint) {
    auto* points = intersectionPoints(primary, squarePoint);
//    std::cout << "square intersectioning in these points\n";
    for (int i = 0; i < 4; i++) {
//        points[i].print();
        if (points[i] != point{-1,-1} && (points[i].x != squarePoint.x || points[i].y != squarePoint.y)) {
//            points[i].print();
            return points[i];
        }
    }
    return point{-1,-1};
}

std::vector<point> Map::squaresOnLine(point p1, point p2) {
    std::vector<point> points;
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
        if (p1 == point{-1,-1})
            return points;
        // в этой же точке отрезок входит в следующий квадрат, сохраняем точку
        points.push_back(p1);
    }
    return points;
}

void Map::centering(point *a, point *b) {
    a->x += 0.5;
    a->y += 0.5;
    b->x += 0.5;
    b->y += 0.5;
}

// тангенс угла между двумя точками
double Map::angleTG(point p1, point p2) {
    // прилежащий / противолежащий катет
    double adjacentCathet, oppositeCathet;
    adjacentCathet = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    oppositeCathet = fieldLength(p2) - fieldLength(p1);
    return oppositeCathet / adjacentCathet;
}

bool Map::calculateVisibility(point p1, point p2, std::vector<point> points) {
    // положим самую низкую башню в p1, что бы идти в направлении от низкой к высокой
    if (fieldLength(p1) > fieldLength(p2)) {
        point buf = p1;
        p1 = p2;
        p2 = buf;

        // тогда самая первая координата будет символизировать точку пересечения с p2
        // ее проверка не нужна, удаляем
        points.erase(points.begin());
    } else {
        // тогда самая последняя координата будет символизировать точку пересечения с p2
        // ее проверка не нужна, удаляем
        points.erase(points.end() - 1);
    }

    // проверяя взгляд от одной точки к другой мы имеем дробную координату пересечения,
    // чтобы проверить следующий за координатой квадрат(получить его высоту)
    // нужно немного сместить точку по направлению вектора, тогда при переводе
    // координат из дробных в целые, мы получим искомый квадрат
    point displacementCoefficient = {0.00001, 0.00001};
    if (p2.x - p1.x < 0)
        displacementCoefficient.x = -0.00001;
    if (p2.y - p1.y < 0)
        displacementCoefficient.y = -0.00001;

    double baseTG = angleTG(p1,p2);
    for (auto p : points) {
        if (fieldLength(p) < fieldLength(p1))
            continue;
        p += displacementCoefficient;
        if (angleTG(p1, p) > baseTG) {
            std::cout << "visibilyty false by tower\n";
            point buf = p;
            buf.round();
            std::cout << "length " << fieldLength(buf) << " y/x ";
            buf.print();
            std::cout << "at the point\n";
            p.print();

            return false;
        }
    }
    return true;
}

bool Map::visibility(point a, point b) {
    std::cout << "!!! visibility get two points: !!!" << std::endl;
    a.print();
    b.print();

    // получены координаты левой верхней стороны двух квадратов(которые символизируют башню)
    // приведем эти координаты к нужному нам формату переместив
    // координаты к центру квадратов
    centering(&a, &b);

    // получим все точки, в которых линия пересекает квадраты(башни на линии взгляда)
    auto points = squaresOnLine(a, b);

    std::cout << "!!! all intersection points !!!\n";
    for (auto p : points) {
        p.print();
    }
    return calculateVisibility(a, b, points);
}
