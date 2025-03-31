#include "Game.h"
#include <iostream>

Game::Game() :
    window(sf::VideoMode(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT), L"Платформеррррррр со сбором монет"),
    showHitboxes(GameConstants::SHOW_HITBOXES_DEFAULT),
    running(false),
    collectedCoinsCount(0)
{
}

Game::~Game() {
}

bool Game::init() {
    // Настраиваем окно
    window.setFramerateLimit(GameConstants::FRAMERATE_LIMIT);

    // Загружаем текстуру игрока
    if (!player.loadTexture("assets/textures/capy_anim.png")) {
        return false;
    }

    // Загружаем текстуру монет
    if (!Coin::loadTexture("assets/textures/coin_anim.png")) {
        std::cout << "Не удалось загрузить текстуру для монет. Будет использоваться цветной круг." << std::endl;
        // Продолжаем работу без текстуры монет, используя цветную форму
    }

    // Устанавливаем начальную позицию игрока
    player.setPosition(100, 500 - GameConstants::PLAYER_FRAME_HEIGHT);

    // Загружаем шрифт
    if (!font.loadFromFile("assets/fonts/arial.ttf") &&
        !font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf") &&
        !font.loadFromFile("fonts\\arial.ttf")) {
        std::cout << L"Шрифт не загружен! Текст может не отображаться." << std::endl;
    }

    // Инициализируем тексты
    initTexts();

    // Инициализируем уровень
    initLevel();

    running = true;
    return true;
}

void Game::run() {
    while (running && window.isOpen()) {
        // Измеряем прошедшее время
        float deltaTime = clock.restart().asSeconds() * GameConstants::GAME_SPEED;

        // Обрабатываем события
        processEvents();

        // Обновляем игровую логику
        update(deltaTime);

        // Проверяем коллизии
        checkCollisions();

        // Отрисовываем игру
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Обработка нажатия клавиш
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                player.jump();
            }
            // Переключение отображения хитбоксов по клавише H
            else if (event.key.code == sf::Keyboard::H) {
                showHitboxes = !showHitboxes;
                hitboxText.setString(L"Хитбоксы: " + std::wstring(showHitboxes ? L"Включены (H - выкл)" : L"Выключены (H - вкл)"));
            }
            else if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
    }
}

void Game::update(float deltaTime) {
    // Обновляем игрока
    player.update(deltaTime);

    // Время для анимации монет
    float coinAnimTime = coinAnimClock.getElapsedTime().asSeconds();

    // Обновляем монеты
    for (auto& coin : coins) {
        coin.update(coinAnimTime);
    }

    // Обновляем тексты
    updateTexts();
}

void Game::checkCollisions() {
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

void Game::render() {
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
    window.draw(speedText);
    window.draw(jumpText);
    window.draw(gravityText);
    window.draw(coinsText);
    window.draw(hitboxText);

    // Отображаем содержимое окна
    window.display();
}

void Game::initLevel() {
    // Создаем землю
    {
        Platform ground;
        ground.init(0, 568, GameConstants::WINDOW_WIDTH, GameConstants::GROUND_HEIGHT);
        platforms.push_back(ground);
    }

    // Создаем платформы
    {
        Platform platform;

        platform.init(250, 245, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT);
        platforms.push_back(platform);

        platform.init(450, 305, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT);
        platforms.push_back(platform);

        platform.init(645, 190, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT);
        platforms.push_back(platform);

        platform.init(685, 385, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT);
        platforms.push_back(platform);
    }

    // Создаем монеты с анимацией (при наличии текстуры)
    bool useTexture = Coin::isTextureLoaded();

    {
        Coin coin;

        coin.init(280, 215, useTexture);
        coins.push_back(coin);

        coin.init(315, 215, useTexture);
        coins.push_back(coin);

        coin.init(350, 215, useTexture);
        coins.push_back(coin);

        coin.init(515, 85, useTexture);
        coins.push_back(coin);

        coin.init(515, 115, useTexture);
        coins.push_back(coin);

        coin.init(515, 145, useTexture);
        coins.push_back(coin);

        coin.init(515, 175, useTexture);
        coins.push_back(coin);

        coin.init(515, 205, useTexture);
        coins.push_back(coin);

        coin.init(710, 95, useTexture);
        coins.push_back(coin);

        coin.init(670, 160, useTexture);
        coins.push_back(coin);

        coin.init(710, 160, useTexture);
        coins.push_back(coin);

        coin.init(750, 160, useTexture);
        coins.push_back(coin);

        coin.init(745, 360, useTexture);
        coins.push_back(coin);
    }
}

void Game::initTexts() {
    // Текст с инструкциями
    instructionText.setFont(font);
    instructionText.setCharacterSize(16);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(10, 10);
    instructionText.setString(L"Управление: стрелки влево/вправо для движения, пробел для прыжка");

    // Текст для отображения счета
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(650, 10);
    scoreText.setString(L"Счет: 0");

    // Тексты для отображения настроек
    speedText.setFont(font);
    speedText.setCharacterSize(14);
    speedText.setFillColor(sf::Color::White);
    speedText.setPosition(10, 30);
    speedText.setString(L"Скорость: " + std::to_wstring(int(GameConstants::PLAYER_SPEED)));

    jumpText.setFont(font);
    jumpText.setCharacterSize(14);
    jumpText.setFillColor(sf::Color::White);
    jumpText.setPosition(10, 50);
    jumpText.setString(L"Сила прыжка: " + std::to_wstring(int(GameConstants::JUMP_POWER)));

    gravityText.setFont(font);
    gravityText.setCharacterSize(14);
    gravityText.setFillColor(sf::Color::White);
    gravityText.setPosition(10, 70);
    gravityText.setString(L"Гравитация: " + std::to_wstring(int(GameConstants::GRAVITY)));

    // Текст для отображения количества собранных монет
    coinsText.setFont(font);
    coinsText.setCharacterSize(14);
    coinsText.setFillColor(sf::Color::White);
    coinsText.setPosition(10, 90);
    coinsText.setString(L"Собрано монет: 0/0");

    // Текст для отображения информации о хитбоксах
    hitboxText.setFont(font);
    hitboxText.setCharacterSize(14);
    hitboxText.setFillColor(sf::Color::White);
    hitboxText.setPosition(10, 110);
    hitboxText.setString(L"Хитбоксы: " + std::wstring(showHitboxes ? L"Включены (H - выкл)" : L"Выключены (H - вкл)"));
}

void Game::updateTexts() {
    coinsText.setString(L"Собрано монет: " + std::to_wstring(collectedCoinsCount) +
        L"/" + std::to_wstring(coins.size()));
}

bool Game::checkPlayerPlatformCollision(const Player& player, const Platform& platform) {
    return player.getCollisionBounds().intersects(platform.getBounds());
}

bool Game::checkPlayerCoinCollision(const Player& player, const Coin& coin) {
    return player.getCollisionBounds().intersects(coin.getBounds());
}