//
// Created by Cleonia on 30.05.2023.
//

#ifndef TOWERS_GAME_HPP
#define TOWERS_GAME_HPP

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <exception>

#include </usr/local/include/SDL2/SDL.h>
//#include <SDL2/SDL_image.h>

#include "towers.h"

#include "Map.hpp"
#include "map/Point.hpp"
#include "map/Line.hpp"

class Game {
public:

    //Constructor for an empty object. before using the object, you must call init() function.
    Game(int width, int height);

    //Deallocates memory
    ~Game();

    //Initializes internal variables
    void init();

    //Main game loop
    void startGame();

private:
    //Variables needed to draw and display the window
    SDL_Window* window;
    SDL_Renderer* render;

    int towerWidth;
    int towerHeight;

    Map *map;
};

#endif //TOWERS_GAME_HPP
