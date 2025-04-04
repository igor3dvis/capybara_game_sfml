#include "Game.h"
#include <iostream>

int main() {
    try {
        Game game;

        if (game.init()) {
            game.run();
        }
        else {
            std::cerr << "The Game is not initialized!" << std::endl;
            return -1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cerr << "Unknown error!" << std::endl;
        return -1;
    }

    return 0;
}