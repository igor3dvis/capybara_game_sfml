#include "Game.h"
#include <iostream>

int main() {
    try {
        Game game;

        if (game.init()) {
            game.run();
        }
        else {
            std::cerr << "Не удалось инициализировать игру!" << std::endl;
            return -1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cerr << "Неизвестная ошибка!" << std::endl;
        return -1;
    }

    return 0;
}