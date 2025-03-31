#pragma once

#include <SFML/Graphics.hpp>
#include "Constants.h"

class Platform {
public:
    Platform();
    Platform(float x, float y, float width, float height, const sf::Color& color = GameConstants::PLATFORM_COLOR);

    // Инициализация платформы
    void init(float x, float y, float width, float height, const sf::Color& color = GameConstants::PLATFORM_COLOR);

    // Отрисовка платформы - добавляем const для совместимости с константными объектами
    void draw(sf::RenderWindow& window, bool showHitbox = false) const;

    // Обновление хитбокса
    void updateHitbox();

    // Получение границ платформы
    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape shape;
    sf::RectangleShape hitbox;
};