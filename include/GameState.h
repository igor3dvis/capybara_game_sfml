#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

// Предварительное объявление класса Game для избежания циклических зависимостей
class Game;

// Абстрактный базовый класс для всех состояний игры
class GameState {
public:
    GameState(Game* game) : game(game) {}
    virtual ~GameState() {}

    // Инициализация состояния
    virtual void init() = 0;

    // Обработка ввода
    virtual void handleInput(sf::Event& event) = 0;

    // Обновление логики состояния
    virtual void update(float deltaTime) = 0;

    // Отрисовка состояния
    virtual void render(sf::RenderWindow& window) = 0;

    // Методы для пауза и возобновления (необязательная реализация)
    virtual void pause() {}
    virtual void resume() {}

protected:
    Game* game; // Указатель на основной класс игры
};