#pragma once

#include "GameState.h"
#include "Player.h"
#include "Platform.h"
#include "Coin.h"
#include <vector>

class GameplayState : public GameState {
public:
    GameplayState(Game* game, int levelNum);
    virtual ~GameplayState() {}

    // Реализация абстрактных методов GameState
    virtual void init() override;
    virtual void handleInput(sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void render(sf::RenderWindow& window) override;

    // Пауза и возобновление
    virtual void pause() override;
    virtual void resume() override;

private:
    // Номер текущего уровня
    int currentLevel;

    // Основные игровые объекты
    Player player;
    std::vector<Platform> platforms;
    std::vector<Coin> coins;

    // Шрифт и тексты
    sf::Font font;
    sf::Text scoreText;
    sf::Text instructionText;
    sf::Text coinsText;
    sf::Text hitboxText;

    // Часы для анимаций
    sf::Clock clock;
    sf::Clock coinAnimClock;

    // Игровые флаги
    bool paused;
    bool showHitboxes;
    int collectedCoinsCount;

    // Инициализация уровня
    void loadLevel(int levelNum);

    // Проверка коллизий
    void checkCollisions();

    // Проверка столкновений между игроком и платформой
    bool checkPlayerPlatformCollision(const Player& player, const Platform& platform);

    // Проверка столкновений между игроком и монетой
    bool checkPlayerCoinCollision(const Player& player, const Coin& coin);

    // Инициализация текстов
    void initTexts();

    // Обновление текстов
    void updateTexts();
};