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

void Map::init(SDL_Renderer* _render) {
    fillMap();
    printMap();
    calculatingTowerSize();
    render = _render;
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

    std::cout << mxFirst << " " << myFirst << std::endl;

    if (mxSecond != -1) {
        if (visibility({double(myFirst), double(mxFirst)}, {double(mySecond), double(mxSecond)}))
            std::cout << "there is direct visibility between the towers" << std::endl;
        else
            std::cout << "there is no line of sight between the towers" << std::endl;
    }
}


void Map::draw() {
    double redColor = 0, blueColor = 255;
    double colorStep = 255.0 / 100.0;

    for (int i = 0; i < field.size(); i++) {
        for (int j = 0; j < field[i].size(); j++) {
            SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
            SDL_Rect rect = {j * towerHeight, i * towerWidth, towerHeight, towerWidth};
            SDL_RenderFillRect(render, &rect);

            SDL_SetRenderDrawColor(render,
                                   redColor + field[i][j] * colorStep,
                                   0,
                                   blueColor - field[i][j] * colorStep,
                                   0xFF);
            rect = {j * towerHeight + 1, i * towerWidth + 1, towerHeight - 2, towerWidth - 2};
            SDL_RenderFillRect(render, &rect);
        }
    }
    drawLine();
}

void Map::drawLine() {
    if (mxSecond == -1)
        return;

    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);

    SDL_RenderDrawLine(render,
                       towerHeight * mxFirst + towerHeight / 2,
                       towerWidth * myFirst + towerWidth / 2,
                       towerHeight * mxSecond + towerHeight / 2,
                       towerWidth * mySecond + towerWidth / 2);
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

unsigned int Map::getHeight() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,100); // distribution in range [1, 6]
    return dist(rng);
}

void Map::fillMap() {
    for (int i = 0; i < width; i++) {
        row r;
        for (int j = 0; j < height; j++) {
            r.push_back(getHeight());
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

std::vector<Map::point> Map::setOfSquares(point p1, point p2) {
    std::vector<point> points;
    // последовательно проверим все "вертикальные" стороны "квадратов"
    for (int x = p1.x + 1; x <= p2.x; x++) {
        auto y = getY(p1, p2, x);
        // если прямая пересекает левую сторону квадрата,
        // то она пересекает и соседний квадрат слева
        points.push_back({static_cast<double>(x) - 1, y});
        points.push_back({static_cast<double>(x), y});
    }
    points.erase(points.end() - 1);
    return points;
}

bool Map::visibility(point a, point b) {
    if (a.x > b.x) {
        point buf = a;
        a = b;
        b = buf;
    }
    auto points = setOfSquares(a, b);
    unsigned int minHeight = field.at(a.x).at(a.y);
    if (minHeight > field.at(b.x).at(b.y)) {
        minHeight = field.at(b.x).at(b.y);
    }
    for (auto &&p : points) {
        std::cout << "new line" << std::endl;
        std::cout << p.x << " " << p.y << std::endl;
        if (field.at(p.x).at(p.y) > minHeight) {
//            std::cout << "tower len " << field.at(p.x).at(p.y) << std::endl;
            return false;
        }
    }
    return true;
}
