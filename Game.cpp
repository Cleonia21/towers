//
// Created by Cleonia on 30.05.2023.
//

//
// Created by Cleonia on 15.09.2022.
//

#include "Game.hpp"

Game::Game(int width, int height)
    {
        map = new Map(width, height);
        towerWidth = SCREEN_WIDTH / width;
        towerHeight = SCREEN_HEIGHT / height;
    }

Game::~Game() {
    SDL_DestroyRenderer( render );
    SDL_DestroyWindow( window );
    SDL_Quit();
}

void Game::init() {
    //Initialize SDL
    if (SDL_Init( SDL_INIT_VIDEO ) < 0)
        throw std::runtime_error("SDL could not init! SDL Error: " + std::string(SDL_GetError()));

    //Set texture filtering to linear
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

    //Create window
    window = SDL_CreateWindow( "Slot Machine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( window == nullptr )
        throw std::runtime_error("Window could not be created! SDL Error: " + std::string(SDL_GetError()));

    //Create render for window
    render = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if( render == nullptr )
        throw std::runtime_error("Renderer could not be created! SDL Error: " + std::string(SDL_GetError()));

    map->init();
}

void Game::startGame() {
    //Loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    Uint32 timer = SDL_GetTicks();
    while (!quit) {
        //Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            //User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if(e.type == SDL_MOUSEBUTTONDOWN) {
                //Get mouse position
                int x, y;
                SDL_GetMouseState(&x, &y);
                map->mouseDown(x, y);
            }
        }
        //Frame delay
        if (timer + 10 > SDL_GetTicks())
            std::this_thread::sleep_for(std::chrono::milliseconds(timer + 10 - SDL_GetTicks()));
        timer = SDL_GetTicks();

        SDL_SetRenderDrawColor(render, 0, 0, 0, 0xFF);
        SDL_RenderClear(render);

        auto towers = map->getTowers();
        for (auto t : towers) {
            SDL_SetRenderDrawColor(render, t.r, t.g, t.b, t.a);
            SDL_Rect rect = {t.x, t.y, t.width, t.height};
            SDL_RenderFillRect(render, &rect);
        }

        auto line = map->getLine();
        if (!line.empty()) {
//            std::cout << line.first.x << " " << line.first.y << " " << line.second.x << " " << line.second.y << std::endl;
            SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
            SDL_RenderDrawLine(render, line.first.x * towerHeight, line.first.y * towerWidth,
                               line.second.x * towerHeight, line.second.y * towerWidth);
        }
        if (line.hasIntersection()) {
            SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
            int x = int(line.intersectionPoint.x * towerHeight);
            int y = int(line.intersectionPoint.y * towerWidth);
            SDL_Rect rect = {x - 5, y - 5, 10, 10};
            SDL_RenderFillRect(render, &rect);

//            std::cout << "!!! visibility false !!!\n";

//            std::cout << "intersection Point:\n";
//            line.intersectionPoint.print();
//            std::cout << "intersection tower:\n";
//            line.intersectionTower.print();
//            std::cout << "intersection tower len: " << line.intersectionTowerLen << std::endl;
//            printIntersection = false;

        }

        SDL_RenderPresent(render);
    }
}
