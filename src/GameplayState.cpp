#include "GameplayState.h"
#include "Game.h"
#include "MenuState.h"
#include "Constants.h"
#include <iostream>

GameplayState::GameplayState(Game* game, int levelNum)
    : GameState(game),
    currentLevel(levelNum),
    paused(false),
    showHitboxes(GameConstants::SHOW_HITBOXES_DEFAULT),
    collectedCoinsCount(0) {
}

void GameplayState::init() {
    // Загрузка шрифта
    if (!font.loadFromFile("assets/fonts/arial.ttf") &&
        !font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf") &&
        !font.loadFromFile("fonts\\arial.ttf")) {
        std::cout << "Fonts is not founded." << std::endl;
    }

    // Загрузка текстуры игрока
    if (!player.loadTexture("assets/textures/capy_anim.png")) {
        std::cout << "Texture for Player don't upload!" << std::endl;
    }

    // Загрузка текстуры монет
    if (!Coin::loadTexture("assets/textures/coin_anim.png")) {
        std::cout << "Texture for Coin don't upload." << std::endl;
    }

    // Устанавливаем начальную позицию игрока
    player.setPosition(100, 500 - GameConstants::PLAYER_FRAME_HEIGHT);

    // Инициализируем тексты
    initTexts();

    // Загружаем уровень
    loadLevel(currentLevel);
}

void GameplayState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            // Escape возвращает в меню
            game->changeState(std::make_unique<MenuState>(game));
        }
        else if (event.key.code == sf::Keyboard::Space) {
            // Пробел для прыжка
            player.jump();
        }
        else if (event.key.code == sf::Keyboard::H) {
            // H для переключения хитбоксов
            showHitboxes = !showHitboxes;
            hitboxText.setString(L"Хитбоксы: " + std::wstring(showHitboxes ? L"Включены (H - выкл)" : L"Выключены (H - вкл)"));
        }
        else if (event.key.code == sf::Keyboard::P) {
            // P для паузы
            paused = !paused;
        }
    }
}

void GameplayState::update(float deltaTime) {
    if (paused) return;

    // Обновляем игрока
    player.update(deltaTime);

    // Время для анимации монет
    float coinAnimTime = coinAnimClock.getElapsedTime().asSeconds();

    // Обновляем монеты
    for (auto& coin : coins) {
        coin.update(coinAnimTime);
    }

    // Проверяем коллизии
    checkCollisions();

    // Обновляем тексты
    updateTexts();
}

void GameplayState::render(sf::RenderWindow& window) {
    // Очищаем окно
    window.clear(GameConstants::BACKGROUND_COLOR);

    // Рисуем платформы
    for (const auto& platform : platforms) {
        platform.draw(window, showHitboxes);
    }

    // Рисуем монеты
    for (auto& coin : coins) {
        coin.draw(window, showHitboxes);
    }

    // Рисуем игрока
    player.draw(window, showHitboxes);

    // Рисуем текст
    window.draw(instructionText);
    window.draw(scoreText);
    window.draw(coinsText);
    window.draw(hitboxText);

    // Если игра на паузе, показываем сообщение
    if (paused) {
        sf::Text pauseText;
        pauseText.setFont(font);
        pauseText.setString(L"ПАУЗА");
        pauseText.setCharacterSize(60);
        pauseText.setFillColor(sf::Color::White);
        pauseText.setOutlineColor(sf::Color::Black);
        pauseText.setOutlineThickness(3);

        // Центрируем текст паузы
        sf::FloatRect pauseBounds = pauseText.getLocalBounds();
        pauseText.setOrigin(pauseBounds.left + pauseBounds.width / 2.0f, pauseBounds.top + pauseBounds.height / 2.0f);
        pauseText.setPosition(GameConstants::WINDOW_WIDTH / 2.0f, GameConstants::WINDOW_HEIGHT / 2.0f);

        window.draw(pauseText);
    }
}

void GameplayState::pause() {
    paused = true;
}

void GameplayState::resume() {
    paused = false;
}

void GameplayState::loadLevel(int levelNum) {
    // Очищаем существующие объекты
    platforms.clear();
    coins.clear();
    collectedCoinsCount = 0;

    // В будущем здесь будет загрузка уровня из файла
    // Пока простая инициализация уровня

    // Проверяем номер уровня
    if (levelNum == 1) {
        // Создаем землю
        Platform ground;
        ground.init(0, 568, GameConstants::WINDOW_WIDTH, GameConstants::GROUND_HEIGHT);
        platforms.push_back(ground);

        // Создаем платформы
        Platform platform1;
        platform1.init(250, 245, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT);
        platforms.push_back(platform1);

        Platform platform2;
        platform2.init(450, 305, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT);
        platforms.push_back(platform2);

        Platform platform3;
        platform3.init(645, 190, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT);
        platforms.push_back(platform3);

        Platform platform4;
        platform4.init(685, 385, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT);
        platforms.push_back(platform4);

        // Создаем монеты с анимацией (при наличии текстуры)
        bool useTexture = Coin::isTextureLoaded();

        Coin coin1;
        coin1.init(280, 215, useTexture);
        coins.push_back(coin1);

        Coin coin2;
        coin2.init(315, 215, useTexture);
        coins.push_back(coin2);

        Coin coin3;
        coin3.init(350, 215, useTexture);
        coins.push_back(coin3);

        Coin coin4;
        coin4.init(515, 85, useTexture);
        coins.push_back(coin4);

        Coin coin5;
        coin5.init(515, 115, useTexture);
        coins.push_back(coin5);

        Coin coin6;
        coin6.init(515, 145, useTexture);
        coins.push_back(coin6);

        Coin coin7;
        coin7.init(515, 175, useTexture);
        coins.push_back(coin7);

        Coin coin8;
        coin8.init(515, 205, useTexture);
        coins.push_back(coin8);

        Coin coin9;
        coin9.init(710, 95, useTexture);
        coins.push_back(coin9);

        Coin coin10;
        coin10.init(670, 160, useTexture);
        coins.push_back(coin10);

        Coin coin11;
        coin11.init(710, 160, useTexture);
        coins.push_back(coin11);

        Coin coin12;
        coin12.init(750, 160, useTexture);
        coins.push_back(coin12);

        Coin coin13;
        coin13.init(745, 360, useTexture);
        coins.push_back(coin13);
    }
    else if (levelNum == 2) {
        // Уровень 2: другая компоновка платформ и монет
        // Создаем землю
        Platform ground;
        ground.init(0, 568, GameConstants::WINDOW_WIDTH, GameConstants::GROUND_HEIGHT);
        platforms.push_back(ground);

        // Пример другой компоновки платформ
        Platform platform1;
        platform1.init(200, 500, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT);
        platforms.push_back(platform1);

        Platform platform2;
        platform2.init(350, 420, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT);
        platforms.push_back(platform2);

        Platform platform3;
        platform3.init(500, 340, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT);
        platforms.push_back(platform3);

        Platform platform4;
        platform4.init(650, 260, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT);
        platforms.push_back(platform4);

        Platform platform5;
        platform5.init(800, 180, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT);
        platforms.push_back(platform5);

        // Несколько монет
        bool useTexture = Coin::isTextureLoaded();

        // Делаем дорожку из монет вдоль лестницы из платформ
        for (int i = 0; i < 5; i++) {
            Coin coin;
            coin.init(250 + i * 150, 470 - i * 80, useTexture);
            coins.push_back(coin);
        }

        // Добавим монеты на верхней платформе
        for (int i = 0; i < 3; i++) {
            Coin coin;
            coin.init(800 + i * 30, 150, useTexture);
            coins.push_back(coin);
        }
    }
    // Можно добавить больше уровней здесь
}

void GameplayState::checkCollisions() {
    // Сначала предполагаем, что игрок в воздухе
    player.setOnGround(false);

    // Проверяем столкновения с платформами
    for (const auto& platform : platforms) {
        if (checkPlayerPlatformCollision(player, platform)) {
            // Получаем позицию игрока
            sf::Vector2f pos = player.getPosition();

            // Получаем границы платформы
            sf::FloatRect platformBounds = platform.getBounds();

            // Если игрок падал сверху на платформу
            if (player.getVelocity().y > 0) {
                // Проверка столкновения сверху
                float penetration = pos.y + GameConstants::PLAYER_FRAME_HEIGHT - platformBounds.top;
                if (penetration >= 0 && penetration < 30) {
                    player.setPosition(pos.x, platformBounds.top - GameConstants::PLAYER_FRAME_HEIGHT);
                    player.setVelocity(player.getVelocity().x, 0);
                    player.setOnGround(true);
                }
            }
        }
    }

    // Проверяем столкновения с монетами
    for (auto& coin : coins) {
        // Если монета уже собрана, пропускаем её
        if (coin.isCollected()) {
            continue;
        }

        // Проверяем, собрал ли игрок монету
        if (checkPlayerCoinCollision(player, coin)) {
            coin.collect();
            player.addScore(coin.getValue());
            collectedCoinsCount++;

            // Обновляем текст счета и монет
            scoreText.setString(L"Счет: " + std::to_wstring(player.getScore()));
            coinsText.setString(L"Собрано монет: " + std::to_wstring(collectedCoinsCount) +
                L"/" + std::to_wstring(coins.size()));
        }
    }
}

bool GameplayState::checkPlayerPlatformCollision(const Player& player, const Platform& platform) {
    return player.getCollisionBounds().intersects(platform.getBounds());
}

bool GameplayState::checkPlayerCoinCollision(const Player& player, const Coin& coin) {
    return player.getCollisionBounds().intersects(coin.getBounds());
}

void GameplayState::initTexts() {
    // Текст с инструкциями
    instructionText.setFont(font);
    instructionText.setCharacterSize(16);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(10, 10);
    instructionText.setString(L"Управление: стрелки влево/вправо для движения, пробел для прыжка, Esc - в меню");

    // Текст для отображения счета
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(650, 10);
    scoreText.setString(L"Счет: 0");

    // Текст для отображения количества собранных монет
    coinsText.setFont(font);
    coinsText.setCharacterSize(14);
    coinsText.setFillColor(sf::Color::White);
    coinsText.setPosition(10, 40);
    coinsText.setString(L"Собрано монет: 0/0");

    // Текст для отображения информации о хитбоксах
    hitboxText.setFont(font);
    hitboxText.setCharacterSize(14);
    hitboxText.setFillColor(sf::Color::White);
    hitboxText.setPosition(10, 60);
    hitboxText.setString(L"Хитбоксы: " + std::wstring(showHitboxes ? L"Включены (H - выкл)" : L"Выключены (H - вкл)"));
}

void GameplayState::updateTexts() {
    coinsText.setString(L"Собрано монет: " + std::to_wstring(collectedCoinsCount) +
        L"/" + std::to_wstring(coins.size()));
}