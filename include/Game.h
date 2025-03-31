#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "Player.h"
#include "Platform.h"
#include "Coin.h"
#include "Constants.h"

class Game {
public:
    Game();
    ~Game();

    // Инициализация игры
    bool init();

    // Запуск игрового цикла
    void run();

private:
    // Обработка событий
    void processEvents();

    // Обновление игровой логики
    void update(float deltaTime);

    // Проверка коллизий
    void checkCollisions();

    // Отрисовка игры
    void render();

    // Инициализация игрового уровня
    void initLevel();

    // Инициализация текстов
    void initTexts();

    // Обновление текстов
    void updateTexts();

    // Проверка коллизий между игроком и платформой
    bool checkPlayerPlatformCollision(const Player& player, const Platform& platform);

    // Проверка коллизий между игроком и монетой
    bool checkPlayerCoinCollision(const Player& player, const Coin& coin);

private:
    sf::RenderWindow window;
    Player player;
    std::vector<Platform> platforms;
    std::vector<Coin> coins;
    sf::Font font;
    sf::Text instructionText;
    sf::Text scoreText;
    sf::Text speedText;
    sf::Text jumpText;
    sf::Text gravityText;
    sf::Text coinsText;
    sf::Text hitboxText;

    sf::Clock clock;
    sf::Clock coinAnimClock;

    bool showHitboxes;
    bool running;
    int collectedCoinsCount;
};