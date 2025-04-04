#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include "Constants.h"

// Предварительное объявление
class GameState;

class Game {
public:
    Game();
    ~Game();

    // Инициализация игры
    bool init();

    // Запуск игрового цикла
    void run();

    // Управление состояниями
    void pushState(std::unique_ptr<GameState> state);
    void popState();
    void changeState(std::unique_ptr<GameState> state);
    GameState* getCurrentState();

    // Завершение игры
    void quit();

    // Получение окна рендеринга
    sf::RenderWindow& getWindow() { return window; }

private:
    // Обработка событий
    void processEvents();

    // Обновление игровой логики
    void update(float deltaTime);

    // Отрисовка игры
    void render();

private:
    sf::RenderWindow window;
    std::stack<std::unique_ptr<GameState>> states;
    sf::Clock clock;
    bool running;
};