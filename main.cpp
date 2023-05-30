#include <vector>
#include <iostream>
#include <random>
#include "Game.hpp"

void sizeProcessing(int argc, char** argv, int* width, int* height) {
    if (argc != 3) {
        return;
    }
    *width = std::atoi(argv[1]);
    *height = std::atoi(argv[2]);
}

int main(int argc, char** argv) {
    int width = 0, height = 0;
    sizeProcessing(argc, argv, &width, &height);
    if (width == 0 || height == 0) {
        std::cout << "incorrect command line parameters. Input format: (width) (height)" << std::endl;
        return 1;
    }

    Game game(width, height);

    try {
        game.init();
    } catch (const char *msg) {
        std::cout << msg << std::endl;
        return 1;
    }

    try {
        game.startGame();
    } catch (const char *msg) {
        std::cout << msg << std::endl;
        return 1;
    }
}


