#pragma once

#include <SFML/Graphics.hpp>

namespace GameConstants {
    // Настройки персонажа
    constexpr float PLAYER_SPEED = 300.0f;     // Скорость движения игрока
    constexpr float JUMP_POWER = 700.0f;       // Сила прыжка
    constexpr float GRAVITY = 1000.0f;         // Сила гравитации
    constexpr float MAX_FALL_SPEED = 1600.0f;  // Максимальная скорость падения
    constexpr float PLAYER_FRAME_WIDTH = 68.0f;  // Ширина кадра игрока
    constexpr float PLAYER_FRAME_HEIGHT = 68.0f; // Высота кадра игрока

    // Настройки платформ
    constexpr float GROUND_HEIGHT = 50.0f;
    constexpr float PLATFORM_WIDTH = 100.0f;
    constexpr float PLATFORM_HEIGHT = 20.0f;
    const sf::Color PLATFORM_COLOR = sf::Color(50, 150, 50); // Цвет платформ (зеленый)

    // Настройки монет
    constexpr float COIN_RADIUS = 10.0f;       // Радиус монеты
    const sf::Color COIN_COLOR = sf::Color(255, 215, 0); // Цвет монет (золотой)
    constexpr int COIN_VALUE = 1;             // Стоимость монеты в очках

    // Настройки игры
    constexpr float GAME_SPEED = 1.0f;         // Скорость игры
    const sf::Color BACKGROUND_COLOR = sf::Color(120, 200, 255); // Цвет фона (голубой)

    // Настройки хитбоксов
    constexpr bool SHOW_HITBOXES_DEFAULT = true; // Показывать ли хитбоксы по умолчанию
    const sf::Color PLAYER_HITBOX_COLOR = sf::Color(255, 0, 0, 128); // Красный полупрозрачный
    const sf::Color COIN_HITBOX_COLOR = sf::Color(255, 255, 0, 128); // Желтый полупрозрачный
    const sf::Color PLATFORM_HITBOX_COLOR = sf::Color(0, 255, 0, 128); // Зеленый полупрозрачный

    // Настройки анимации
    constexpr float DEFAULT_ANIMATION_FRAME_TIME = 0.1f; // Время одного кадра по умолчанию

    // Настройки окна
    constexpr int WINDOW_WIDTH = 1200;
    constexpr int WINDOW_HEIGHT = 700;
    constexpr int FRAMERATE_LIMIT = 60;
}